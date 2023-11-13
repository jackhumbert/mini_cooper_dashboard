// ID3 ID2 ID1 ID0 EXT RTR DTA0 DTA1 DTA2 DTA3 DTA4 DTA5 DTA6 DTA7

#include "serial_can.h"
// #include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include "sd_card.h"
#include "activity.h"

#define TIMEOUT 20

// void Serial_CAN::begin(int can_tx, int can_rx, unsigned long baud)
// {
//     softwareSerial = new SoftwareSerial(can_tx, can_rx);
//     softwareSerial->begin(baud);
//     canSerial = softwareSerial;
// }

// void Serial_CAN::begin(SoftwareSerial &serial, unsigned long baud)
// {
//     serial.begin(baud);
//     softwareSerial = &serial;
//     canSerial = &serial;
// }

void Serial_CAN::begin(HardwareSerial &serial, unsigned long baud)
{
    serial.begin(baud);
    hardwareSerial = &serial;
    canSerial = &serial;
}

unsigned char Serial_CAN::send(unsigned long id, uchar ext, uchar rtrBit, uchar len, const uchar *buf)
{
    unsigned char dta[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    dta[0] = id>>24;        // id3
    dta[1] = id>>16&0xff;   // id2
    dta[2] = id>>8&0xff;    // id1
    dta[3] = id&0xff;       // id0
    
    dta[4] = ext;
    dta[5] = rtrBit;
    
    for(int i=0; i<len; i++)
    {
        dta[6+i] = buf[i];
    }
    
    for(int i=0; i<14; i++)
    {
        canSerial->write(dta[i]);
    }
}

uint16_t last_ts = 0xFFFF;

bool validate_id(uint32_t id) {
    return (id >= 0x153 && id <= 0x9FF);
}

bool filter_id(uint32_t id) {
    // uint16_t ts = id >> 8;
    // if (ts - last_ts > 64 && last_ts != 0xFFFF)
    //     return false;
    // last_ts = ts;
    switch (id) {
        case 0x153: return true;
        // case 0x1F0: return true;
        // case 0x1F3: return true;
        // case 0x1F5: return true; // same as 0x610
        // case 0x1F8: return true; // never changes?
        case 0x316: return true;
        case 0x329: return true;
        // case 0x336: return true;
        // case 0x501: return true;
        // case 0x545: return true;
        // case 0x565: return true;
        // case 0x610: return true;
        case 0x613: return true;
        case 0x615: return true;
        // case 0x618: return true;
        // case 0x61A: return true;
        case 0x61F: return true;
        case 0x630: return true;
    }
    return false;
}


// 0: no data
// 1: get data
unsigned char Serial_CAN::recv(unsigned long *id, uchar *buf) {
    if (canSerial->available()) {
        unsigned long timer_s = millis();
        uchar dta[13] = { 0 };

        if (canSerial->available() >= 12) {
            canSerial->readBytes(dta, 12);
            unsigned int __id = 0;
            
            for(int i=0; i<4; i++) // Store the id of the sender
            {
                __id <<= 8;
                __id += dta[i];
            }
            // len = 12;
            bool skipped = false;
            while (!validate_id(__id)) {
                if (!skipped) {
                    activity_update(false);
                    sd_card_logf("%08.3f CXX DISCARDING ", xTaskGetTickCount() / 1000.0);
                }
                skipped = true;
                unsigned long timeout = millis();
                while (!canSerial->available()) {
                    if((millis()-timeout) > 2) {
                        activity_update(false);
                        for (int i = 0; i < 12; i++) {
                            sd_card_logf("%02X ", dta[i]);
                        }
                        sd_card_logf("\n");
                        return 0;
                    }
                }
                sd_card_logf("%02X ", dta[0]);
                dta[0] = dta[1];
                dta[1] = dta[2];
                dta[2] = dta[3];
                dta[3] = dta[4];
                dta[4] = dta[5];
                dta[5] = dta[6];
                dta[6] = dta[7];
                dta[7] = dta[8];
                dta[8] = dta[9];
                dta[9] = dta[10];
                dta[10] = dta[11];
                dta[11] = canSerial->read();

                for(int i=0; i<4; i++) {
                    __id <<= 8;
                    __id += dta[i];
                }
            }
            if (skipped) {
                sd_card_logf("\n");
            }

            // if (len == 0) {
            //     File * file = sd_card_get_log_file();
            //     if (file && *file) {
            //         for (int i = 0; i < 12; i++) {
            //             sd_card_logf("%02X ", dta[i]);
            //         }
            //         sd_card_logf("\n");
            //         file->flush();
            //     }
            //     return 0;
            // }
            
            *id = __id;
            
            for(int i=0; i<8; i++) // Store the message in the buffer
            {
                buf[i] = dta[i+4];
            }
            return 1;
        }
        return 0;
    }
// {
//     if(canSerial->available())
//     {
//         unsigned long timer_s = millis();
        
//         int len = 0;
//         uchar dta[20];
        
//         while(1)
//         {
//             while(canSerial->available())
//             {
//                 dta[len++] = canSerial->read();

//                 if(len > 4) {
//                     // make sure timestamp is 0
//                     if ((dta[0] != 0 || dta[1] != 0 || dta[2] == 0 || dta[3] == 0) && len) {
//                         dta[0] = dta[1];
//                         dta[1] = dta[2];
//                         dta[2] = dta[3];
//                         dta[3] = dta[4];
//                         len--;
//                         continue;
//                     }
//                 }

//                 if(len == 12)
//                     break;
 
//             	if((millis()-timer_s) > TIMEOUT) {
//                     for(int i=0; i<len; i++) // Store the message in the buffer
//                     {
//                         buf[i] = dta[i];
//                     }
//                     buf[12] = len;
                    
//                     // canSerial->flush();
//                     return 0; // Reading 12 bytes should be faster than 10ms, abort if it takes longer, we loose the partial message in this case
//                 }
//             }
            
//             if(len == 12) // Just to be sure, must be 12 here
//             {
//                 unsigned long __id = 0;
                
//                 for(int i=0; i<4; i++) // Store the id of the sender
//                 {
//                     __id <<= 8;
//                     __id += dta[i];
//                 }
                
//                 *id = __id;
                
//                 for(int i=0; i<8; i++) // Store the message in the buffer
//                 {
//                     buf[i] = dta[i+4];
//                 }
//                 return 1;
//             }
            
//             if((millis()-timer_s) > TIMEOUT)
//             {
//                 for(int i=0; i<len; i++) // Store the message in the buffer
//                 {
//                     buf[i] = dta[i];
//                 }
//                 buf[12] = len;
//                 // canSerial->flush();
//                 return 0; // Reading 12 bytes should be faster than 10ms, abort if it takes longer, we loose the partial message in this case
//             }
            
//         }
//     }
    
    return 0;
}

unsigned char Serial_CAN::cmdOk(char *cmd)
{
    
    unsigned long timer_s = millis();
    unsigned char len = 0;

    canSerial->println(cmd);
    while(1)
    {
        if(millis()-timer_s > 500)
        {
            return 0;
        }
        
        while(canSerial->available())
        {

            str_tmp[len++] = canSerial->read();
            timer_s = millis();
        }

        if(len >= 4 && str_tmp[len-1] == '\n' && str_tmp[len-2] == '\r' && str_tmp[len-3] == 'K' && str_tmp[len-4] == 'O')
        {
            clear();
            return 1;        
        }
        
    }
}

/*
value	    01	02	03	04	05	    06	07	08	09	10	    11	12	13	14	15	16	17	18
rate(kb/s)	5	10	20	25	31.2	33	40	50	80	83.3	95	100	125	200	250	500	666	1000
*/
unsigned char Serial_CAN::canRate(unsigned char rate)
{
    enterSettingMode();
    if(rate < 10)
        sprintf(str_tmp, "AT+C=0%d\r\n", rate);
    else 
        sprintf(str_tmp, "AT+C=%d\r\n", rate);
    
    int ret = cmdOk(str_tmp);
    
    exitSettingMode();
    return ret;
}

/*
value	        0	    1	    2	    3   	4
baud rate(b/s)	9600	19200	38400	/	115200
*/

unsigned char Serial_CAN::baudRate(unsigned char rate)
{
    unsigned long baud[5] = {9600, 19200, 38400, 9600, 115200};
    int baudNow = 0;
    
    if(rate == 3)return 0;
    
    for(int i=0; i<5; i++)
    {
        selfBaudRate(baud[i]);
        canSerial->print("+++");
        delay(100);
        
        if(cmdOk("AT\r\n"))
        {
            sd_card_logf("SERIAL BAUD RATE IS: ");
            sd_card_logf("%d", baud[i]);
            baudNow = i;
            break;     
        }
    }
    
    sprintf(str_tmp, "AT+S=%d\r\n", rate);
    cmdOk(str_tmp);
    
    selfBaudRate(baud[rate]);
    
    int ret = cmdOk("AT\r\n");
    
    if(ret)
    {
        sd_card_logf("Serial baudrate set to ");
        sd_card_logf("%d", baud[rate]);
    }
    
    exitSettingMode();
    return ret;
}

void Serial_CAN::selfBaudRate(unsigned long baud)
{
    // if(softwareSerial)
    // {
    //     softwareSerial->begin(baud);
    // }
    // else
     if(hardwareSerial)
    {
        hardwareSerial->begin(baud);
    }
}

void Serial_CAN::clear()
{
    unsigned long timer_s = millis();
    while(1)
    {
        if(millis()-timer_s > 50)return;
        while(canSerial->available())
        {
            canSerial->read();
            timer_s = millis();
        }
    }
}

unsigned char Serial_CAN::enterSettingMode()
{
    canSerial->print("+++");
    clear();
    return 1;
}

unsigned char Serial_CAN::exitSettingMode()
{
    clear();
    int ret = cmdOk((char*)"AT+Q\r\n");
    clear();
    return ret;
}

void make8zerochar(int n, char *str, unsigned long num)
{
    for(int i=0; i<n; i++)
    {
        str[n-1-i] = num%0x10;
        if(str[n-1-i]<10)str[n-1-i]+='0';
        else str[n-1-i] = str[n-1-i]-10+'A';
        num >>= 4;
    }
    str[n] = '\0';    
}

/*
+++	                    Switch from Normal mode to Config mode
AT+S=[value]	        Set serial baud rate
AT+C=[value]	        Set CAN Bus baud rate
AT+M=[N][EXT][value]    Set mask,AT+M=[1][0][000003DF]
AT+F=[N][EXT][value]    Set filter,AT+F=[1][0][000003DF]
AT+Q	            Switch to Normal Mode
*/
unsigned char Serial_CAN::setMask(unsigned long *dta)
{
    enterSettingMode();
    char __str[10];
    
    
    for(int i=0; i<2; i++)
    {
        make8zerochar(8, __str, dta[1+2*i]);
        //log(__str);
        sprintf(str_tmp, "AT+M=[%d][%d][", i, dta[2*i]);
        for(int i=0; i<8; i++)
        {
            str_tmp[12+i] = __str[i];
        }
        str_tmp[20] = ']';
        str_tmp[21] = '\r';
        str_tmp[22] = '\n';
        str_tmp[23] = '\0';
        
        //log(str_tmp);
        
        if(!cmdOk(str_tmp))
        {
            sd_card_logf("mask fail - %d", i);
            exitSettingMode();
            return 0;
        }
        clear();
        delay(10);
        //
    }
    exitSettingMode();
    return 1;

}

unsigned char Serial_CAN::setFilt(unsigned long *dta)
{
    enterSettingMode();
    
    char __str[10];
    
    for(int i=0; i<6; i++)
    {
        make8zerochar(8, __str, dta[1+2*i]);
        //log(__str);
        sprintf(str_tmp, "AT+F=[%d][%d][", i, dta[2*i]);
        for(int i=0; i<8; i++)
        {
            str_tmp[12+i] = __str[i];
        }
        str_tmp[20] = ']';
        str_tmp[21] = '\r';
        str_tmp[22] = '\n';
        str_tmp[23] = '\0';
        
        //log(str_tmp);
        
        clear();
        if(!cmdOk(str_tmp))
        {
            //log("filt fail at - ");
            //log(i);
            exitSettingMode();
            return 0;
        }
        clear();
        delay(10);
        //
    }
    exitSettingMode();
    return 1;
}

/*
value	        0	    1	    2	    3   	4
baud rate(b/s)	9600	19200	38400	57600	115200
*/
unsigned char Serial_CAN::factorySetting()
{
    // check baudrate
    unsigned long baud[5] = {9600, 19200, 38400, 57600, 115200};
    
    for(int i=0; i<5; i++)
    {
        selfBaudRate(baud[i]);
        canSerial->print("+++");
        delay(100);
        
        if(cmdOk("AT\r\n"))
        {
            sd_card_logf("SERIAL BAUD RATE IS: %d", baud[i]);
            baudRate(0);                // set serial baudrate to 9600
            sd_card_logf("SET SERIAL BAUD RATE TO: 9600 OK");
            selfBaudRate(9600);
            break;            
        }
    }
    
    if(canRate(CAN_RATE_500))
    {
        sd_card_logf("SET CAN BUS BAUD RATE TO 500Kb/s OK");
    }
    else
    {
        sd_card_logf("SET CAN BUS BAUD RATE TO 500Kb/s FAIL");
        return 0;
    }
    
    unsigned long mask[4] = {0, 0, 0, 0,};
    unsigned long filt[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
    
    if(setFilt(filt))
    {
        sd_card_logf("FACTORY SETTING FILTS OK");
    }
    else 
    {
        sd_card_logf("FACTORY SETTING FILTS FAIL");
        return 0;        
    }
    
    if(setMask(mask))
    {
        sd_card_logf("FACTORY SETTING MASKS OK");
    }
    else
    {
        sd_card_logf("FACTORY SETTING MASKS FAIL");
        return 0;
    }
    
    return 1;
}

void Serial_CAN::debugMode()
{
    while(Serial.available())
    {
        canSerial->write(Serial.read());
    }
    
    while(canSerial->available())
    {
        Serial.write(canSerial->read());
    }
}

// END FILE