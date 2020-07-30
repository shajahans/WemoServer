
/**********************************************************************************
* This is for controlling the devices directly by Alexa without using a Public Skill
  *********************************************************************************/
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3  //RX
#define D10 1 //TX
#define D11 9
#define D12 10

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

/* Network credentials */
#define WIFI_SSID "Shajahan"
#define WIFI_PASS "!Mumthas@123#"    

#define SERIAL_BAUDRATE 115200

/* Belkin WeMo emulation */
fauxmoESP fauxmo;

/* Set Relay Pins */
#define RELAY_1 D5
#define RELAY_2 D6
#define RELAY_3 D7
#define RELAY_4 D8

void setup() 
{
   Serial.begin(SERIAL_BAUDRATE);
   //setup and wifi connection
   wifiSetup();
 
   //Set relay pins to outputs
   pinMode(RELAY_1, OUTPUT);
   pinMode(RELAY_2, OUTPUT);
   pinMode(RELAY_3, OUTPUT);
   pinMode(RELAY_4, OUTPUT);

   //Set each relay pin to HIGH ====== NOTE THAT THE RELAYS USE INVERSE LOGIC =====
   digitalWrite(RELAY_1, HIGH);   
   delay(500);
   digitalWrite(RELAY_2, HIGH);  
   delay(500);
   digitalWrite(RELAY_3, HIGH);  
   delay(500);
   digitalWrite(RELAY_4, HIGH);   
   delay(500);
    
   // Device Names for Simulated Wemo switches
   fauxmo.addDevice("Fancy Light");
   //fauxmo.addDevice("Light Two");
   //fauxmo.addDevice("Outlet One");
   //fauxmo.addDevice("Outlet Two");
   //fauxmo.addDevice("Bed Room");
   //fauxmo.addDevice("Living Room");
   //fauxmo.addDevice("All Devices");
   fauxmo.setPort(80); // required for gen3 devices
   fauxmo.enable(true);
   fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
        callback(device_id, device_name, state);
    });
}

void loop() 
{
  fauxmo.handle();
}

/* ---------------------------------------------------------------------------
 Device Callback
 ----------------------------------------------------------------------------*/
void callback(uint8_t device_id, const char * device_name, bool state) 
{
  Serial.print("Device "); Serial.print(device_name); 
  Serial.print(" state: ");
  if (state) 
  {
    Serial.println("ON");
  } 
  else 
  {
    Serial.println("OFF");
  }
  
  //Switching action on detection of device name
  
  if ( (strcmp(device_name, "Light One") == 0) ) 
  {
    if (!state) 
    {
      digitalWrite(RELAY_1, HIGH);
    } 
    else 
    {
      digitalWrite(RELAY_1, LOW);
    }
  }

  if ( (strcmp(device_name, "Light Two") == 0) ) 
  {
    if (!state) 
    {
      digitalWrite(RELAY_3, HIGH);
    } 
    else 
    {
      digitalWrite(RELAY_3, LOW);
    }
  }
  
  if ( (strcmp(device_name, "Outlet One") == 0) ) 
  {
    if (!state) 
    {
      digitalWrite(RELAY_2, HIGH);
    } 
    else 
    {
      digitalWrite(RELAY_2, LOW);
    }
  }
  
  if ( (strcmp(device_name, "Outlet Two") == 0) ) 
  {
    if (!state) 
    {
      digitalWrite(RELAY_4, HIGH);
    } 
    else 
    {
      digitalWrite(RELAY_4, LOW);
    }
  }
  
  if ( (strcmp(device_name, "Bed Room") == 0) ) 
  {
    if (!state) 
    {
      digitalWrite(RELAY_3, HIGH);
      digitalWrite(RELAY_4, HIGH);
    } 
    else 
    {
      digitalWrite(RELAY_3, LOW);
      digitalWrite(RELAY_4, LOW);
    }
  }
  
  if ( (strcmp(device_name, "Living Room") == 0) ) 
  {
    if (!state) 
    {
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, HIGH);
    } 
    else 
    {
      digitalWrite(RELAY_1, LOW);
      digitalWrite(RELAY_2, LOW);
    }
  }
 
  if ( (strcmp(device_name, "All Devices") == 0) ) 
  {
    if (!state) 
    {
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, HIGH);
      digitalWrite(RELAY_3, HIGH);
      digitalWrite(RELAY_4, HIGH);
    } 
    else 
    {
      digitalWrite(RELAY_1, LOW);
      digitalWrite(RELAY_2, LOW);
      digitalWrite(RELAY_3, LOW);
      digitalWrite(RELAY_4, LOW);
    }
  }
}
    
/* -----------------------------------------------------------------------------
 Wifi Setup
 -----------------------------------------------------------------------------*/
void wifiSetup() 
{
   // Set WIFI module to STA mode
   WiFi.mode(WIFI_STA);

   // Connect
   Serial.println ();
   Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
   Serial.println();
   WiFi.begin(WIFI_SSID, WIFI_PASS);

   // Wait
   while (WiFi.status() != WL_CONNECTED) 
   {
      Serial.print(".");
      delay(100);
   }
   Serial.print(" ==> CONNECTED!" );
   Serial.println();

   // Connected!
   Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
   Serial.println();
}
