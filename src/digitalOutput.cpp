#include "digitalOutput.h"

#ifdef DEBUG_DIGITALOUTPUT
char LC_DigitalOutput::modeNames[2][10+1] = {
    "OUTPUT",
    "OUTPUT_ETC"
};
#endif

LC_DigitalOutput::LC_DigitalOutput(uint8_t *pinsPointer, uint8_t pinsQuantity) {
    pinsTblPtr = (uint8_t*)pinsPointer;
    pinsQty = pinsQuantity;
}

void LC_DigitalOutput::SetMode(uint8_t mode){    
    pinMOde = mode;
    // pinMOde = (mode == OUTPUT) ? OUTPUT : OUTPUT;
    #ifdef DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){        
        if(portDbgPtr!=nullptr) {
            uint8_t offst;
            offst = (pinMOde == OUTPUT) ? 0:1;
            portDbgPtr->print(F("LC Digital Outputs: Mode set to: "));
            // portDbgPtr->println(mode);
            portDbgPtr->println(modeNames[offst]);
        }
    }
    #endif
}

void LC_DigitalOutput::SetActiveState(bool actvState){
    pinActvState = actvState;
    #ifdef DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr) {
            portDbgPtr->print(F("LC Digital Outputs: Active state set to: "));
            portDbgPtr->println(actvState);
        }
    }
    #endif
}

void LC_DigitalOutput::AttachNames(const char *namesPtr, uint8_t length) {
    pinNamesPtr = namesPtr;
    pinNameLen=length;
    #ifdef DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr) {
            portDbgPtr->println(F("LC Digital Outputs: Pin names attached!"));
            portDbgPtr->print(F("  Name length set to: "));
            portDbgPtr->println(length);
        }
    }
    #endif
}


void LC_DigitalOutput::AttachDebugPort(Stream *ptr) {
    portDbgPtr = ptr;
    #ifdef DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr) portDbgPtr->println(F("LC Digital Outputs: Debug port attached!"));
    }
    #endif
}

void LC_DigitalOutput::SetDebugLevel(DebugLevel level){
    debugLvl = level;
    #ifdef DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr) {
            portDbgPtr->print(F("LC Digital Outputs: Debug level set to: "));
            portDbgPtr->println((uint8_t)debugLvl);
        }
    }
    #endif
}

void LC_DigitalOutput::SetDebugLevel(uint8_t level){
    SetDebugLevel((DebugLevel)level);
}

void LC_DigitalOutput::Init() {
    #ifdef DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr){
            portDbgPtr->print(F("LC Digital Outputs: Initializing "));
            portDbgPtr->print(pinsQty);
            portDbgPtr->println(F(" output pins..."));
        }    
    }
    #endif

    if(pinsTblPtr == nullptr) return;
    if(pinsQty==0) return;

    uint8_t *pinPtr=nullptr;
    uint8_t *modePtr=nullptr;
    bool *activeStatePtr=nullptr;
    const char *namePtr=nullptr;
    
    for(uint8_t i = 0; i < pinsQty; i++){
        pinPtr = pinsTblPtr+i;
        
        // initialize the output pins to their given mode and idle state
        if(pinModesPtr!=nullptr){ // modes table is prioritized
            modePtr = pinModesPtr+i;
            pinMode(*pinPtr,*modePtr);
        }
        else{
            pinMode(*pinPtr, pinMOde);
        }

        if(pinActvStatesPtr!=nullptr){ // activate states table is prioritized
            activeStatePtr = pinActvStatesPtr+i;
            digitalWrite(*pinPtr,!*(activeStatePtr));
        }
        else{
            digitalWrite(*pinPtr,!pinActvState);
        }        
        #ifdef DEBUG_DIGITALOUTPUT
        if(debugLvl>=DebugLevel::useful){
            if(pinNamesPtr==nullptr) return;
            char* bfrPtr = (char*)malloc(pinNameLen);
            if(bfrPtr==nullptr) {               
                if(portDbgPtr!=nullptr) portDbgPtr->println(F("LC Digital Outputs: couldn't allocate string buffer."));    
                return;
            }    
            memset(bfrPtr,0,pinNameLen);   
            namePtr = pinNamesPtr+i*pinNameLen;
            memcpy_P(bfrPtr,namePtr,pinNameLen);
            if(portDbgPtr!=nullptr){
                uint8_t offst = 0;
                portDbgPtr->print(bfrPtr);                
                portDbgPtr->println(F(" pin:"));                

                portDbgPtr->print(F("    mode set to: "));
                if(pinModesPtr!=nullptr){
                    // portDbgPtr->println(*modePtr);
                    offst = *modePtr==OUTPUT ? 0:1;                    
                    portDbgPtr->println(modeNames[offst]);
                } 
                else{
                    // portDbgPtr->println(pinMOde);
                    offst = pinMOde==OUTPUT ? 0:1;                    
                    portDbgPtr->println(modeNames[offst]);
                }                 

                portDbgPtr->print(F("    active state: "));
                if(pinActvStatesPtr!=nullptr) portDbgPtr->println(*activeStatePtr);
                else portDbgPtr->println(pinActvState);

                portDbgPtr->print(F("    state init'd to: "));
                if(pinActvStatesPtr!=nullptr) portDbgPtr->println(!*activeStatePtr);
                else portDbgPtr->println(!pinActvState);
                portDbgPtr->println();
            }
            free(bfrPtr);
        }        
        #endif
    }

    #ifdef DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr){
            portDbgPtr->println(F("LC Digital Outputs: Digital output pins init'd!"));
            portDbgPtr->println();
        }
    }
    #endif
}

void LC_DigitalOutput::Drive(uint8_t pinIndex, bool state){
    uint8_t *pinPtr = pinsTblPtr + pinIndex;
    bool *activeStatePtr = nullptr;
    bool finalState;
    const char *namePtr=nullptr;

    if(pinActvStatesPtr!=nullptr){ // activate states table is prioritized
        activeStatePtr = pinActvStatesPtr + pinIndex;
        finalState = state ? *(activeStatePtr + pinIndex) : !*(activeStatePtr + pinIndex);
        digitalWrite(*pinPtr,finalState);
    }
    else{
        finalState = state ? pinActvState : !pinActvState;
        digitalWrite(*pinPtr,finalState);
    }        
    #ifdef DEBUG_DIGITALOUTPUT
    // if(debugLvl>=DebugLevel::useful){
    if(debugLvl>=DebugLevel::devt1){
        if(pinNamesPtr==nullptr) return;
        char* bfrPtr = (char*)malloc(pinNameLen);
        if(bfrPtr==nullptr) {        
            if(portDbgPtr!=nullptr) portDbgPtr->println(F("LC Digital Outputs: couldn't allocate string buffer."));
            return;
        }         
        memset(bfrPtr,0,pinNameLen);   
        namePtr = pinNamesPtr + pinIndex*pinNameLen;
        memcpy_P(bfrPtr,namePtr,pinNameLen);
        if(portDbgPtr!=nullptr){
            portDbgPtr->print(bfrPtr);            
            portDbgPtr->print(F(" pin state set to: ")); 
            portDbgPtr->println(finalState);
        }
        free(bfrPtr);
    }
    #endif
}