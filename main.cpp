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
PwmOut  motorleft_B(p25); //
PwmOut  motorright_A(p24); //Motor right
DigitalOut  motorright_B(p23); //
DigitalOut  Relay_3A(p22); //Motor skill Q (may bao)
DigitalOut  Relay_3B(p21); //
DigitalOut  kichdien(p9);  //relay ac inverter 12DC -220AC
DigitalOut  kichdienB(p16); //
//DigitalOut  kichdien(p17);  //
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
uint8_t chieu_maybao=0; //xac dinh chieu may bao
uint8_t status_maybao=1;
uint8_t reversion=0;  //state of robot( reverse or not reverse)
////////////////////////////////////////////////////////////////////
void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    ble.startAdvertising();  //gửi gói tin quảng bá
    pc.printf("\n\r# disconnected\n\r");
    led1 = 0;  //led off when disconnected
    ////xoay tron va bat vu khi khi mat ket noi
//    motorleft_A = 1;  //Motor left go up
//    motorleft_B = 0;  //
//    motorright_A = 0;  //Motor right go down
//    motorright_B = 1;  //
//    Relay_3A = 0;
//    Relay_3B = 1;
    
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
void up(void)       //su dung motor 1 & 2
{
    motorleft_A = 0; //motor left go down
    motorleft_B.write (0.7); //
    motorright_A.write(0); //motor right do down
    motorright_B = 1; //
    pc.printf("\n\r# go up\n\r");
}
void down(void)     //su dung motor 1 & 2
{
    motorleft_A = 1; //motor left go up
    motorleft_B.write(0.3); //
    motorright_A.write(1); //motor right go up
    motorright_B = 0; //
    pc.printf("\n\r# go down\n\r");
}
void left(void)     //su dung motor 1 & 2
{
    motorleft_A = 1;  //Motor left go down
    motorleft_B = 0;  //
    motorright_A = 0;  //Motor right go up
    motorright_B = 1;  //
    pc.printf("\n\r# turn left\n\r");
}
void right(void)    //su dung motor 1 & 2
{
    motorleft_A = 0;  //Motor left go up
    motorleft_B = 1;  //
    motorright_A = 1;  //Motor right go down
    motorright_B = 0;  //
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
void skillQ(void)  //turn round down
{
    status_maybao = !status_maybao;  //dao trang thai may bao
    if(status_maybao == 0)    //may bao on
    {
         if(chieu_maybao == 1)
         {
            Relay_3A = 0;  //turn round down maybao
            Relay_3B = 1;
            pc.printf("\n\r# turn round down maybao\n\r");
         }  
         else if(chieu_maybao == 0)
         {
            Relay_3A = 1;  //turn round down maybao
            Relay_3B = 0;
            pc.printf("\n\r# turn round up maybao\n\r");
         }
    }
    else if(status_maybao == 1)  //may bao off
    {
        Relay_3A = 1;  //turn round down maybao
        Relay_3B = 1;
        pc.printf("\n\r# turn off maybao\n\r");
    }
}
void skillE(void)   //quay tron+turn on may bao
{
    motorleft_A = 1;  //Motor left go up
    motorleft_B = 0;  //
    motorright_A = 0;  //Motor right go down
    motorright_B = 1;  //
    Relay_3A = 0;  //turn round down maybao
    Relay_3B = 1;  
    pc.printf("\n\r# turn on skill E\n\r");
}
void offskillE(void)
{
    motorleft_A = 1;  //Motor left go up
    motorleft_B = 1;  //
    motorright_A = 1;  //Motor right go down
    motorright_B = 1;  //
    Relay_3A = 1;  //turn round down maybao
    Relay_3B = 1;  
    pc.printf("\n\r turn off skill E\n\r");
}
void skillR(void)
{
   pc.printf("\n\r#turn on skill R\n\r"); 
   motorleft_A = 0;
    motorleft_B = 1;
    motorright_A = 1;
    motorright_B = 0;
    wait(0.9);
    motorleft_A = 1;
    motorleft_B = 0;
    motorright_A = 1;
    motorright_B = 0;
    wait(1.2);
    motorleft_A = 1;
    motorleft_B = 1;
    motorright_A = 1;
    motorright_B = 1;
    wait(0.1);
    motorleft_A = 0;
    motorleft_B = 1;
    motorright_A = 0;
    motorright_B = 1;
}
void offskillR(void)
{
    pc.printf("\n\r#turn off skill R\n\r");
    
}

//////////////////////////////////////////////////////////////////////
int main(void)
{
    //Init hardware
    led1 = 0;
    motorleft_A = 1;
    motorleft_B.period(0.01);
    motorright_A.period(0.01);
    motorleft_B.write(1);
    motorright_A.write(1);
    motorright_B = 1;
    Relay_3A = 1;
    Relay_3B = 1;
    kichdien = 1;
    kichdienB = 1;
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
                                     (const uint8_t *)"GAREN", sizeof("TAO LA DUNG") - 1);      //
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                                     (const uint8_t *)UARTServiceUUID_reversed, sizeof(UARTServiceUUID_reversed)); //UUID service
    ble.setAdvertisingInterval(200); /* 1000ms; in multiples of 0.625ms. */
    ble.startAdvertising();
    UARTService uartService(ble);
    uartServicePtr = &uartService;

while (true) {
        ble.waitForEvent();
////////////////motion////////////////////////
        if     (g_cmd == 1) {
            if(reversion == 0)  //if bot isn't reversed
                up();           
            else down();      }  //go down when bot is reversed
        else if(g_cmd == 2) {
            if(reversion == 0)
                left();
            else right();    }
        else if(g_cmd == 3) {
            if(reversion == 0)
                down();
            else up();    }
        else if(g_cmd == 4) {
            if(reversion == 0)
                right();
            else left();   }
        else if(g_cmd == 5) {stop();    }
        
/////////////////SKILL Q///////////////////


        else if(g_cmd == 6) {chieu_maybao = !chieu_maybao;  }  //thiet lap chieu may bao
        else if(g_cmd == 7) {skillQ(); }  // on/off skill Q
        
////////////////SKILL W///////////////////
        else if(g_cmd == 9) {kichdien = !kichdien;
        if(kichdien == 0 ) pc.printf("\n\r# turn on kich dien\n\r");
        else pc.printf("\n\r# turn off kich dien\n\r");
         }  //on skill W    
//        else if(g_cmd ==11) {kichdien = 1; pc.printf("\n\r# turn off kich dien \n\r");}  //off skill W
        
////////////////SKILL E///////////////////

        else if(g_cmd == 12) {skillE();    }  //on skill E
       // else if(g_cmd == 13) {offskillE(); }  //off skill E
        
/////////////////SKILL R///////////////////
        else if(g_cmd == 16) {skillR();    }  //on skill R
 //       else if(g_cmd == 17) {offskillR(); }
 ///////////Check! are bot reversed? //////////////////////
        else if(g_cmd == 18) {reversion = !reversion; }  

        g_cmd = 0;
}
}
