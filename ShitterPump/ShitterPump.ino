int relay1Pin = 7;
int floatSwitchPin = 12;
int floatSwitchLed = 2;
int problemLed = 10;

void turnPumpOn() {
    digitalWrite(relay1Pin, LOW);
}

void turnPumpOff() {
    digitalWrite(relay1Pin, HIGH);
}

void clearLEDs() {
    digitalWrite(floatSwitchLed, LOW);
    digitalWrite(problemLed, LOW);
}

void turnProblemLightOn() {
    digitalWrite(problemLed, HIGH);
}

void turnProblemLightOff() {
    digitalWrite(problemLed, LOW);
}

bool isFloatSwitchOn() {
    return digitalRead(floatSwitchPin) == LOW;
}

void setup(void) {
    // start serial port
    Serial.begin(9600);
    pinMode(relay1Pin, OUTPUT);
    pinMode(floatSwitchPin, INPUT_PULLUP);
    pinMode(floatSwitchLed, OUTPUT);
    pinMode(problemLed, OUTPUT);
    turnPumpOff();
    clearLEDs();
}

void loop(void) {
    bool floatSwitchState = isFloatSwitchOn();
    //turn the float switch LED on if applicable
    digitalWrite(floatSwitchLed, floatSwitchState);
    turnProblemLightOff();

    if (floatSwitchState) {
        Serial.print("Float switch is on\r\n");
        turnPumpOn();
        delay(10000);
        //10 seconds of runtime and still high is not right, sleep a second, check again, but then wait 30 seconds to prevent running the pump constantly
        //also turn the problem pin on while sleeping to notify that something bad happened
        if (isFloatSwitchOn()) {
            Serial.print("In problem state, still high after running\r\n");
            turnPumpOff();
            turnProblemLightOn();
            delay(30000);
        } else {
            Serial.print("Float switch is off, cleared\r\n");
            clearLEDs();
        }
    }
    turnPumpOff();

    delay(100);
}
