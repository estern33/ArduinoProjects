// This Arduino sketch reads DS18B20 "1-Wire" digital
// temperature sensors.
// Tutorial:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-tutorial.html

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 3 on the Arduino
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html

DeviceAddress mat1Thermometer = { 0x28, 0x58, 0xC2, 0x84, 0x02, 0x00, 0x00, 0xA8 };

int relay1Pin = 7;
bool relayHigh = true;
bool relayLow = false;
float highSetPoint = 80.0f;
float lowSetPoint = 78.0f;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  pinMode(relay1Pin, OUTPUT);
  digitalWrite(relay1Pin, relayLow);
  
  // Start up the library
  sensors.begin();

  sensors.setResolution(mat1Thermometer, 12);
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00) {
    Serial.print("Error getting temperature");
  } else {
    Serial.print("C: ");
    Serial.print(tempC);
    Serial.print(" F: ");
    Serial.print(DallasTemperature::toFahrenheit(tempC));
  }
}

void loop(void)
{ 
  delay(2000);
  Serial.print("Getting temperatures...\n\r");
  sensors.requestTemperatures();
  
  Serial.print("Mat 1 temperature is: ");
  printTemperature(mat1Thermometer);
  
  float tempMat1C = sensors.getTempC(deviceAddress);
  if (tempC != -127.00) {
    if (DallasTemperature::toFahrenheit(tempMat1C) > highSetPoint)
    {
      Serial.print("Turing mat 1 power off, temp exceeded high setpoint of ");
      Serial.print(highSetPoint);
      Serial.print("\n\r");
      digitalWrite(relay1Pin, relayLow);
    }
    if (DallasTemperature::toFahrenheit(tempMat1C) < lowSetPoint)
    {
      Serial.print("Turing mat 1 power on, temp exceeded low setpoint of ");
      Serial.print(lowSetPoint);
      Serial.print("\n\r");
      digitalWrite(relay1Pin, relayHigh);
    }
  }

  Serial.print("\n\r\n\r");
  
  
}

