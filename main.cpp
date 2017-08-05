 /* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "ble/BLE.h"
#include "ble/services/UARTService.h"


DigitalOut  led1(p7);
DigitalOut  motorleft_A(p28); //Motor left
DigitalOut  motorleft_B(p25); //
DigitalOut  motorright_A(p24); //Motor right
DigitalOut  motorright_B(p23); //
DigitalOut  Relay_3A(p22); //Motor skill Q (may bao)
DigitalOut  Relay_3B(p21); //
//DigitalOut  Relay_4A(p9);  //
//DigitalOut  Relay_4B(p16); //
DigitalOut  kichdien(p17);  //relay ac inverter 12DC -220AC
//DigitalOut  Relay_2(p18);  //
//DigitalOut  Relay_3(p19);  //
//DigitalOut  Relay_4(p20);  //

Serial      pc(p10, p11);
BLEDevice   ble;
UARTService *uartServicePtr;

//// Other
#define     ON          0
#define     OFF         1
uint8_t g_cmd;
////////////////////////////////////////////////////////////////////
void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    ble.startAdvertising();  //gửi gói tin quảng bá
    pc.printf("\n\r# disconnected\n\r");
    led1 = 0;  //led off when disconnected
    ////xoay tron va bat vu khi khi mat ket noi
    motorleft_A = 1;  //Motor left go up
    motorleft_B = 0;  //
    motorright_A = 0;  //Motor right go down
    motorright_B = 1;  //
    Relay_3A = 0;
    Relay_3B = 1;
    
}

void connectionCallback( const Gap::ConnectionCallbackParams_t *params ) {
    pc.printf("\n\r# connected\n\r");
    led1 = 1;  //led on when connected
}
void onDataWritten(const GattWriteCallbackParams *params)
{
    if ((uartServicePtr != NULL) && (params->handle == uartServicePtr->getTXCharacteristicHandle())) {
        uint16_t bytesRead = params->len;
        g_cmd = params->data[0];
        ble.updateCharacteristicValue(uartServicePtr->getRXCharacteristicHandle(), params->data, bytesRead);  //update status to server
    }
}

/////////////////////////////////////////////////////////////////////
void periodicCallback(void)
{
   // led1 = !led1;
}
//////////////////////////////////////////////////////////////////////
void down(void)       //su dung motor 1 & 2
{
    motorleft_A = 0; //motor left go down
    motorleft_B = 1; //
    motorright_A = 0; //motor right do down
    motorright_B = 1; //
    pc.printf("\n\r# go down\n\r");
}
void up(void)     //su dung motor 1 & 2
{
    motorleft_A = 1; //motor left go up
    motorleft_B = 0; //
    motorright_A = 1; //motor right go up
    motorright_B = 0; //
    pc.printf("\n\r# go up\n\r");
}
void left(void)     //su dung motor 1 & 2
{
    motorleft_A = 0;  //Motor left go down
    motorleft_B = 1;  //
    motorright_A = 1;  //Motor right go up
    motorright_B = 0;  //
    pc.printf("\n\r# turn left\n\r");
}
void right(void)    //su dung motor 1 & 2
{
    motorleft_A = 1;  //Motor left go up
    motorleft_B = 0;  //
    motorright_A = 0;  //Motor right go down
    motorright_B = 1;  //
    pc.printf("\n\r# turn right\n\r");
}
void stop(void)    //su dung motor 1 & 2
{
    motorleft_A = 1;
    motorleft_B = 1;
    motorright_A = 1;
    motorright_B = 1;
    pc.printf("\n\r# stop\n\r");
}
/////////////////////////////////////////
void maybao_down(void)  //turn round down
{
    Relay_3A = 0;
    Relay_3B = 1;
    pc.printf("\n\r# may bao down\n\r");
}
void maybao_up(void)   //turn round up
{
    Relay_3A = 1;
    Relay_3B = 0;
    pc.printf("\n\r# may bao up\n\r");
}
void maybao_stop(void)
{
    Relay_3A = 1;
    Relay_3B = 1;
    pc.printf("\n\r# may bao down\n\r");
}
void skillE(void)   //quay tro
{
    motorleft_A = 1;  //Motor left go up
    motorleft_B = 0;  //
    motorright_A = 0;  //Motor right go down
    motorright_B = 1;  //
    Relay_3A = 0;  //turn round down maybao
    Relay_3B = 1;  
}
void skillR(void)
{
    
}

//////////////////////////////////////////////////////////////////////
int main(void)
{
    //Init hardware
    led1 = 0;
    motorleft_A = 1;
    motorleft_B = 1;
    motorright_A = 1;
    motorright_B = 1;
    Relay_3A = 1;
    Relay_3B = 1;
    kichdien = 1;
    //Init UART
    pc.baud(115200);
    pc.printf("\n\r# BOT_BATTLE\n\r");
    //Init timer
    Ticker ticker;
    ticker.attach(periodicCallback, 0.1); //sec
    //Init BLE
    ble.init();  //start ble driver
    ble.onConnection(connectionCallback);   //callback when connected
    ble.onDisconnection(disconnectionCallback);   //callback when disconnected
    ble.onDataWritten(onDataWritten);   //callback when receive data
    /* setup advertising */
    ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED);     //chế độ hoạt động BLE (only le peripheral)
    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);      // có khả năng kêt nối vô hướng
    ble.accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME,                      //tên thiết bị ble peripheral 
                                     (const uint8_t *)"BOT_BATTLE", sizeof("BOT_BATTLE") - 1);      //
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                                     (const uint8_t *)UARTServiceUUID_reversed, sizeof(UARTServiceUUID_reversed)); //UUID service
    ble.setAdvertisingInterval(200); /* 1000ms; in multiples of 0.625ms. */
    ble.startAdvertising();
    UARTService uartService(ble);
    uartServicePtr = &uartService;

while (true) {
        ble.waitForEvent();
        //test_relay();

        if     (g_cmd == 1) {up();      }
        else if(g_cmd == 2) {left();    }
        else if(g_cmd == 3) {down();    }
        else if(g_cmd == 4) {right();   }
        else if(g_cmd == 5) {stop();    }
        else if(g_cmd == 6) {maybao_up();    }  //
        else if(g_cmd == 7) {maybao_down();  }  // Skill Q  
        else if(g_cmd == 8) {maybao_stop();  }  //
        else if(g_cmd == 12) {
            skillE();
            pc.printf("\n\r# turn on skill E - turn round \n\r");  } //Skill E
        else if(g_cmd == 14) {
            skillR();
            pc.printf("\n\r# turn on skill R - crazy dance \n\r");} //Skill R
        else if(g_cmd == 16) {
            kichdien = ON;
            pc.printf("\n\r# bat kich dien\n\r");    }  //
        else if(g_cmd == 17) {                          //
            kichdien = OFF;                             //skill W
            pc.printf("\n\r# tat kich dien\n\r");    }  //

        g_cmd = 0;
}
}
