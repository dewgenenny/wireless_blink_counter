#include <MemoryFree.h>

/**
 *
 * Solar panel pulse counter using an arduino pro mini, esp8266 and a LDR
 * Based on the demo sketch provided with the ITEAD ESP8266 library
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#define INTERRUPT_INPUT 3
#include "ESP8266.h"

#define SSID        "xxxx"
#define PASSWORD    "xxxx"
#include <SoftwareSerial.h>
#include <math.h>

SoftwareSerial mySerial(8, 9); /* RX:D3, TX:D2 */



ESP8266 wifi(mySerial);




void setup(void)
{
    Serial.begin(9600);
    Serial.print(F("setup begin\r\n"));
    
      // For noise suppression, enable pullup on interrupt pin
  digitalWrite(INTERRUPT_INPUT, HIGH);
  attachInterrupt(INTERRUPT_INPUT - 2,
                  interrupt_handler,
                  RISING);
    initialize_esp();

}
 int pulse_counter = 0;

         int count_blinks = 0;
         int every4th = 0;
         int every1000th = 0;
         int iscounted = 0;
         int restarts = 0;
         int freemem = 0;
         unsigned long nolove = millis();

void loop(void)
{
    uint8_t buffer[128] = {0};
    uint8_t mux_id;
        uint8_t sens[4] = {0};
/* superceded through interrupt handling        
         int blah = analogRead(A0);
       // sens[0] = blah;
   
  //   Serial.println(blah);
    
    //figure out if there has been a blink
    
    
    if(blah > 30)
    {
      if(iscounted == 0)
      {
        
        Serial.print("incrementing blinks, blah is: ");
        Serial.println(blah);
        count_blinks++;
    //    sens[0] = count_blinks;
        iscounted = 1;
      } 
    }
    
  if (blah < 30)
  {
    if(iscounted == 1)
    {
      iscounted = 0;
    }

  }  
 
 */
 
 
 
   if(millis()- nolove > 120000)
{

          Serial.println("No love..... going to restart the wifi module and reinitialize");
          restarts++;
        wifi.restart();
        initialize_esp();
        nolove = millis();
          Serial.print("Nolove = ");
          Serial.print(nolove);
          Serial.print(" Millis = ");
          Serial.println(millis());
}

    
    
    
    
    if(every4th = 3){
    uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
    
    
    if (len > 0) {
      nolove=millis();
 //       Serial.print("Status:[");
//        Serial.print(wifi.getIPStatus().c_str());
//        Serial.println("]");
    // int sensorValue[5] = {analogRead(A0)};    
    
        //memcpy(sens, sensorValue, 4);
        //Serial.print("value of sens = ");
        //Serial.print(sensorValue[0]);
        
        
//        Serial.print("Received from :");
 //       Serial.print(mux_id);
 //       Serial.print("[");
 //       for(uint32_t i = 0; i < len; i++) {
//            Serial.print((char)buffer[i]);
//        }
 //       Serial.print("]\r\n");
        sens[0] = pulse_counter;
 //       sens[1] = 5;
 //       freemem = freeMemory()/10;
 //       sens[2] = freemem;
        if(wifi.send(mux_id, sens, len)) {
 //           Serial.print("send back ok\r\n");
            // Serial.print("Content of sens = ");
            // Serial.println(sens);
        } else {
            Serial.print(F("send back err\r\n"));
        }
        
        if (wifi.releaseTCP(mux_id)) {
   //         Serial.print("release tcp ");
   //         Serial.print(mux_id);
 //           Serial.println(" ok");
        } else {
            Serial.print(F("release tcp"));
            Serial.print(mux_id);
            Serial.println(F(" err"));
        }
        
//        Serial.print("Status:[");
//        Serial.print(wifi.getIPStatus().c_str());
 //       Serial.println("]");
                pulse_counter = 0;
    
    }
    every4th = 0;
    }
    else
    {
      every4th++;
    }
    if (every1000th = 999)
    {
  //  Serial.print("freeMemory()=");
 //   Serial.println(freeMemory());
  //    Serial.println(freemem);

      if(wifi.kick())
      {
 //       Serial.println("WIFI is Alive and kicking!!!");
      }
      else
      {
       Serial.println("Oops WIFI is dead!!!, restarting...");
        restarts++;
        wifi.restart();
        initialize_esp();
      }
      
      
     every1000th=0; 
    }
    else
    {
      every1000th++;
    }
    
    
    
}

void initialize_esp(void)
{
    Serial.print(F("FW Version:"));
    Serial.println(wifi.getVersion().c_str());
      
    if (wifi.setOprToStationSoftAP()) {
        Serial.print(F("to station + softap ok\r\n"));
    } else {
        Serial.print(F("to station + softap err\r\n"));
    }
 
    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print(F("Join AP success\r\n"));
        Serial.print(F("IP: "));
        Serial.println(wifi.getLocalIP().c_str());    
    } else {
        Serial.print(F("Join AP failure\r\n"));
    }
    
    if (wifi.enableMUX()) {
        Serial.print(F("multiple ok\r\n"));
    } else {
        Serial.print(F("multiple err\r\n"));
    }
    
    if (wifi.startTCPServer(8090)) {
        Serial.print(F("start tcp server ok\r\n"));
    } else {
        Serial.print(F("start tcp server err\r\n"));
    }
    
    if (wifi.setTCPServerTimeout(10)) { 
        Serial.print(F("set tcp server timout 10 seconds\r\n"));
    } else {
        Serial.print(F("set tcp server timout err\r\n"));
    }

    Serial.print(F("setup end\r\n"));  
  
  nolove=millis();
  
}

void interrupt_handler()
{
  pulse_counter = pulse_counter + 1;
}
        
