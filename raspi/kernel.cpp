//
// kernel.cpp
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2019-2022  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "kernel.h"
#include "../lib/circle/addon/lvgl/lvgl/demos/lv_demos.h"
#include "dash.h"
#include "can.h"
#include <circle/util.h>
#include "messages.h"
#include "font.h"

LOGMODULE ("kernel");

CKernel *CKernel::s_pThis = nullptr;

CKernel::CKernel (void)
:	m_Screen (m_Options.GetWidth (), m_Options.GetHeight ()),
	m_Timer (&m_Interrupt),
	m_Logger (m_Options.GetLogLevel (), &m_Timer),
	m_USBHCI (&m_Interrupt, &m_Timer, TRUE),
	m_pKeyboard (0),
	m_SerialCDC (&m_Interrupt),
	m_GUI (&m_Screen, &m_Interrupt),
	m_Dash(&m_Serial, CMemorySystem::Get ()),
	m_ShutdownMode (ShutdownNone)
{
	s_pThis = this;
	// m_ActLED.Blink (5);	// show we are alive
}

CKernel::~CKernel (void)
{
	s_pThis = nullptr;
}

boolean CKernel::Initialize (void)
{
	boolean bOK = TRUE;

	if (bOK)
	{
		bOK = m_Screen.Initialize ();
	}

	if (bOK)
	{
		bOK = m_Serial.Initialize (115200);
	}

	if (bOK)
	{
		CDevice *pTarget = m_DeviceNameService.GetDevice (m_Options.GetLogDevice (), FALSE);
		if (pTarget == 0)
		{
			pTarget = &m_Screen;
		}

		bOK = m_Logger.Initialize (pTarget);
	}

	if (bOK)
	{
		bOK = m_Interrupt.Initialize ();
	}

	if (bOK)
	{
		bOK = m_Timer.Initialize ();
	}

	if (bOK)
	{
		bOK = m_USBHCI.Initialize ();
	}

	if (bOK)
	{
		bOK = m_SerialCDC.Initialize ();
	}

	if (bOK)	
	{
		m_RPiTouchScreen.Initialize ();

		bOK = m_GUI.Initialize ();
	}

	// if (bOK)
	// {
	// 	bOK = m_Dash.Initialize ();
	// }

	return bOK;
}

static lv_obj_t * textarea;

TShutdownMode CKernel::Run (void)
{
	if (m_SerialCDC.UpdatePlugAndPlay ())
	{
		m_Logger.SetNewTarget (&m_SerialCDC);
	}
	LOGNOTE ("Compile time: " __DATE__ " " __TIME__);

	m_SerialCDC.RegisterReceiveHandler (usbCDCReceiveHandler);

	textarea = lv_textarea_create(lv_scr_act());
	// lv_obj_set_style_pad_all(lv_scr_act(), 10, 0);
	lv_obj_set_size(textarea, 800, 480);
	lv_obj_align(textarea, LV_ALIGN_TOP_MID, 0, 10);
	lv_textarea_set_text_selection(textarea, true);
	DASH_FONT(RAJDHANI_REGULAR, 24);
	lv_obj_set_style_text_font(textarea, RAJDHANI_REGULAR_24, 0);

	// lv_demo_widgets ();
	// dash_create(NULL);

	// unsigned nLastTime = 0;


	// for (unsigned nCount = 0; m_ShutdownMode == ShutdownNone; nCount++)
	// {
	while (true) {
		boolean bUpdated = m_USBHCI.UpdatePlugAndPlay ();
		if (bUpdated && m_pKeyboard == 0) {
			m_pKeyboard = (CUSBKeyboardDevice *) m_DeviceNameService.GetDevice ("ukbd1", FALSE);
			if (m_pKeyboard != 0) {
				m_pKeyboard->RegisterRemovedHandler (KeyboardRemovedHandler);

#if 1	// set to 0 to test raw mode
				m_pKeyboard->RegisterShutdownHandler (ShutdownHandler);
				m_pKeyboard->RegisterKeyPressedHandler (KeyPressedHandler);
#else
				m_pKeyboard->RegisterKeyStatusHandlerRaw (KeyStatusHandlerRaw);
#endif

				LOGNOTE("Just type something!");
			}
		}

		if (m_pKeyboard != 0) {
			// CUSBKeyboardDevice::UpdateLEDs() must not be called in interrupt context,
			// that's why this must be done here. This does nothing in raw mode.
			m_pKeyboard->UpdateLEDs ();
		}
		// dash_loop();
		m_GUI.Update (bUpdated);

		if (m_SerialCDC.UpdatePlugAndPlay ()) {
			m_Logger.SetNewTarget (&m_SerialCDC);
		}

	}

		// unsigned nNow = m_Timer.GetTime ();
		// if (nLastTime != nNow)
		// {
		// 	LOGNOTE ("Time is %u", nNow);

		// 	nLastTime = nNow;
		// }

	return ShutdownHalt;
}

static void process_standard(char *pBuffer) {
    char c_id[4];
    char c_length[2];
    char c_data[17];
    dbcc_time_stamp_t ts = {0};

    memcpy(c_id, pBuffer, 3);
    uint32_t id = strtoul(c_id, NULL, 16);

    memcpy(c_length, pBuffer + 3, 1);
    uint8_t length = strtoul(c_length, NULL, 10);

    memcpy(c_data, pBuffer + 4, length * 2);
    uint64_t data = strtoull(c_data, NULL, 16);
    // uint8_t * p_data = (uint8_t*)&data;
	uint8_t p_data[8];
	p_data[0] = data >> 56;
	p_data[1] = data >> 48;
	p_data[2] = data >> 40;
	p_data[3] = data >> 32;
	p_data[4] = data >> 24;
	p_data[5] = data >> 16;
	p_data[6] = data >>  8;
	p_data[7] = data >>  0;
	// u64_to_can_msg(data, p_data);

    // LOGNOTE("%04X %02X %02X %02X %02X %02X %02X %02X %02X\n", id, p_data[0], p_data[1], p_data[2], p_data[3], p_data[4], p_data[5], p_data[6], p_data[7]);
	// add_message_fmt("%04X %02X %02X %02X %02X %02X %02X %02X %02X\n", id, p_data[0], p_data[1], p_data[2], p_data[3], p_data[4], p_data[5], p_data[6], p_data[7]);

    if (decode_can_message(ts, id, p_data) < 0) {
        get_changed()->activity |= ACTIVITY_ERROR;
    } else {
        get_changed()->activity |= ACTIVITY_SUCCESS;
    }
}

void CKernel::usbCDCReceiveHandler(void *pBuffer, unsigned nLength)
{
	char * cBuffer = (char*)pBuffer;
	unsigned pos = 0;
	while (pos < nLength) {
		switch (cBuffer[pos]) {
			case 't':
				pos++;
				process_standard(cBuffer + pos);
				pos += 20;
				break;
			default:
				pos++;
		}
	}
	// assert (s_pThis);
	// s_pThis->m_Screen.Write (pBuffer, nLength);
	// s_pThis->m_Serial.Write (pBuffer, nLength);
}


void CKernel::KeyPressedHandler (const char *pString)
{
	assert (s_pThis != 0);
#ifdef EXPAND_CHARACTERS
	while (*pString)
          {
	  CString s;
	  s.Format ("'%c' %d %02X\n", *pString, *pString, *pString);
          pString++;
	  s_pThis->m_Screen.Write (s, strlen (s));
          }
#else
	// s_pThis->m_Screen.Write (pString, strlen (pString));
	if (*pString == 0x7F) {
		lv_textarea_del_char(textarea);
	} else {
		if (pString[0] == 0x1B) {
			if (pString[2] == 'A') {
				lv_textarea_cursor_up(textarea);
			} else if (pString[2] == 'B') {
				lv_textarea_cursor_down(textarea);
			} else if (pString[2] == 'C') {
				lv_textarea_cursor_right(textarea);
			} else if (pString[2] == 'D') {
				lv_textarea_cursor_left(textarea);
			} else if (pString[2] == '3' && pString[3] == '~') {
				lv_textarea_del_char_forward(textarea);
			}
		} else {
			lv_textarea_add_text(textarea, pString);
		}
	}
#endif
}

void CKernel::ShutdownHandler (void)
{
	assert (s_pThis != 0);
	s_pThis->m_ShutdownMode = ShutdownReboot;
}

void CKernel::KeyStatusHandlerRaw (unsigned char ucModifiers, const unsigned char RawKeys[6])
{
	assert (s_pThis != 0);

	CString Message;
	Message.Format ("Key status (modifiers %02X)", (unsigned) ucModifiers);

	for (unsigned i = 0; i < 6; i++)
	{
		if (RawKeys[i] != 0)
		{
			CString KeyCode;
			KeyCode.Format (" %02X", (unsigned) RawKeys[i]);

			Message.Append (KeyCode);
		}
	}

	LOGNOTE(Message);
}

void CKernel::KeyboardRemovedHandler (CDevice *pDevice, void *pContext)
{
	assert (s_pThis != 0);

	LOGNOTE("Keyboard removed");

	s_pThis->m_pKeyboard = 0;
}