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

int incomingByte = 0;

DeviceAddress thermometer1Address = { 
  0x28, 0x58, 0xC2, 0x84, 0x02, 0x00, 0x00, 0xA8 };
String thermometer1Name = "Mat 1";
int relay1Pin = 7;

bool relayOn = false;
bool relayOff = true;

float highSetPoint = 80.0f;
float lowSetPoint = 78.0f;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  pinMode(relay1Pin, OUTPUT);
  digitalWrite(relay1Pin, relayOff);

  Serial.print("To set new high and low temperatures, enter high temp as 10xx and/or low temp as xx via serial.");

  // Start up the library
  sensors.begin();

  sensors.setResolution(thermometer1Address, 12);
}

void printTemperature(DeviceAddress deviceAddress, String deviceName)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00) {
    Serial.print("Error getting temperature of ");
    Serial.print(deviceName);
  } 
  else {
    Serial.print(deviceName);
    Serial.print(" temperature is: ");
    Serial.print("C: ");
    Serial.print(tempC);
    Serial.print(" F: ");
    Serial.print(DallasTemperature::toFahrenheit(tempC));
  }
}

void computeRelayState(DeviceAddress deviceAddress, String deviceName)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC != -127.00) 
  {
    if (DallasTemperature::toFahrenheit(tempC) > highSetPoint)
    {
      Serial.print("Turing ");
      Serial.print(deviceName);
      Serial.print(" power off, temp exceeded high setpoint of ");
      Serial.print(highSetPoint);
      Serial.print(" deg F.\n\r");
      digitalWrite(relay1Pin, relayOff);
    }
    if (DallasTemperature::toFahrenheit(tempC) < lowSetPoint)
    {
      Serial.print("Turing ");
      Serial.print(deviceName);
      Serial.print(" power on, temp dropped below low setpoint of ");
      Serial.print(lowSetPoint);
      Serial.print(" deg F.\n\r");
      digitalWrite(relay1Pin, relayOn);
    }
  }
}

void readSerialInput()
{
  String incoming="";
  while (Serial.available() > 0) 
  {
    // read the incoming byte:
    incomingByte = Serial.parseInt();  //if I didn't want to convert from ascii to integer, I would use .read()

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
    incoming += incomingByte;
    Serial.print("That makes string: ");
    Serial.println(incoming);
  }

  if (incoming != "")
  {
    int setPoint = atoi(incoming.c_str());
    if (setPoint > 1000)
    {
      Serial.print("Setting high temp threshold to ");
      Serial.print(setPoint - 1000);
      Serial.print("\n\r");
      highSetPoint = setPoint - 1000;
    }
    else 
    {
      Serial.print("Setting low temp threshold to ");
      Serial.print(setPoint);
      Serial.print("\n\r");
      lowSetPoint = setPoint;
    }
  }
}

void loop(void)
{ 
  delay(2000);

  readSerialInput();

  Serial.print("Getting temperatures...\n\r");
  sensors.requestTemperatures();

  printTemperature(thermometer1Address, thermometer1Name);

  computeRelayState(thermometer1Address, thermometer1Name);


  Serial.print("\n\r\n\r");


}


