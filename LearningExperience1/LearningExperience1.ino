/*
  Blink
 Turns on an LED on for one second, then off for one second, repeatedly.
 
 This example code is in the public domain.
 */

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int delayMs = 1000;
int incomingByte=0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  Serial.begin(9600);
  pinMode(led, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
  String incoming="";
  while (Serial.available() > 0) {
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
    delayMs=atoi(incoming.c_str());

  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(delayMs);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(delayMs);  // wait for a second

  Serial.println(delayMs);
}


