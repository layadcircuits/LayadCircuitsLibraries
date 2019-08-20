#include "LayadCircuits.h"

enum OutputPins{
  redLed, // A0
  greenLed, // A1
  yellowLed, // A2
  builtinLed // 13
};

#define QUANTITY_PINS_OUTPUT 4
uint8_t outputPins[QUANTITY_PINS_OUTPUT] ={A0,A1,A2,13};

#define LENGTH_MAX_PINAME 15
const char outPinNames[QUANTITY_PINS_OUTPUT][LENGTH_MAX_PINAME+1] PROGMEM = {
    "redLed",
    "greenLed", 
    "yellowLed",
    "builtinLed"
};


LC_DigitalOutput output(outputPins,QUANTITY_PINS_OUTPUT);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  output.AttachDebugPort(&Serial);
  output.AttachNames((const char*)outPinNames,LENGTH_MAX_PINAME+1);
  output.SetDebugLevel(DebugLevel::useful);
  // output.SetMode(0);
  output.Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  static uint32_t tRef = millis();
  static bool pinState = false;
  static bool runFlag = false;

  if(millis()-tRef > 1000){
  // if(millis()-tRef > 1000 && !runFlag){
    output.Drive(builtinLed, pinState);
    output.Drive(redLed, pinState);
    output.Drive(greenLed, pinState);
    output.Drive(yellowLed, pinState);
    pinState = !pinState;
    tRef=millis();
    runFlag = true;
  }

}
