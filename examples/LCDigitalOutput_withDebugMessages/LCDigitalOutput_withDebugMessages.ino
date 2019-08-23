#include "LayadCircuits.h"

enum OutputPins{
  redLed, // red LED connected to A0
  greenLed, // green LED connected to A1
  yellowLed, // yellow LED connected to A2
  builtinLed // builtin LED conntected to 13
};

#define QUANTITY_PINS_OUTPUT 4
uint8_t outputPins[QUANTITY_PINS_OUTPUT] ={A0,A1,A2,13}; // specify the pins in order

#define LENGTH_MAX_PINAME 15 // required for debugging
const char outPinNames[QUANTITY_PINS_OUTPUT][LENGTH_MAX_PINAME+1] PROGMEM = {
    "redLed",
    "greenLed", 
    "yellowLed",
    "builtinLed"
};

LC_DigitalOutput output(outputPins,QUANTITY_PINS_OUTPUT); // pass output pins array and size to constructor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  output.AttachDebugPort(&Serial);
  output.AttachNames((const char*)outPinNames,LENGTH_MAX_PINAME+1); 
  // output.SetDebugLevel(DebugLevel::important); // print important debugging messages only
  // output.SetDebugLevel(DebugLevel::useful); // print important + extra debugging messages
  output.SetDebugLevel(DebugLevel::devt1); // print all debugging messages 

  output.Init(); // initializes all output pins to OUTPUT mode
  output.Drive(greenLed, true); // drive pin to active state

  output.Blink(builtinLed, 500, 0); // blink pin indefinitely at an interval of 500 ms
  // output.Blink(builtinLed, 500, 10); // blink pin for 10 times at an interval of 500 ms
  output.Blink(redLed, 2000, 5); // blink pin for 5 times at an interval of 2000 ms
  output.Enable(); // enable the LC_DigitalOutput library loop method to run in the main loop
}

void loop() {
  // put your main code here, to run repeatedly:
  static bool ledState = false;
  static uint32_t timeRef = millis();
  if(millis() - timeRef > 1000){    
    output.Drive(yellowLed, ledState); // drive pin
    ledState = !ledState;
    timeRef = millis();
  }

  output.Loop();
}
