//
// can_serial.cpp
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2015-2021  R. Stange <rsta2@o2online.de>
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

#include "can_serial.h"
#include "can.h"
#include "dash.h"

static uint8_t buffer[12 + 8];


static bool validate_id(uint32_t id) {
    return (id >= 0x153 && id <= 0xA00);
}

static bool filter_id(uint32_t id) {
   switch (id) {
        case 0x1F0:
        // case 0x1F8:
            return true;
    }
    return false;
}

uint32_t get_id(uint8_t * data) {
    uint32_t id = 0;
    
    for(int i=0; i<4; i++) {
        id <<= 8;
        id += data[i];
    }
    return id;
}

void CCANSerialDevice::Process(void) {
    while (true) {
        int avail = this->AvailableForRead();
        if (avail >= 12) {
            this->Read(buffer, 12);
            uint32_t id = 0;
            uint8_t offset = 0;

            while(!validate_id(get_id(buffer + offset)) && offset < 8)
                offset++;
            
            if (offset) {
                while (this->AvailableForRead() < offset);                    
                this->Read(buffer + 12, offset);
            }

            if (validate_id(get_id(buffer + offset))) {     
                id = get_id(buffer + offset);
                if (filter_id(id)) {
                    continue;
                }       
                process_packet(buffer + offset);
            } else {
                get_changed()->activity |= ACTIVITY_ERROR;
                // sd_card_logf("%08.3f CER %08X %02X %02X %02X %02X %02X %02X %02X %02X\n", 
                //     xTaskGetTickCount() / 1000.0, get_id(buffer + offset),
                //     buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11]);
            }
        } 
    }
}