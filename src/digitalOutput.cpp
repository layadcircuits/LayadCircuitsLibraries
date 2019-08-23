#include "digitalOutput.h"

#ifdef LC_DEBUG_DIGITALOUTPUT
char LC_DigitalOutput::modeNames[2][10+1] = {
    "OUTPUT",
    "OUTPUT_ETC"
};
#endif

uint32_t * LC_DigitalOutput::timeRefsTblPtr = nullptr;
uint32_t * LC_DigitalOutput::blinkIntervlsTblPtr = nullptr;
uint8_t * LC_DigitalOutput::blinkRepsTblPtr = nullptr;
uint8_t * LC_DigitalOutput::blinkCntrsTblPtr = nullptr;

// uint32_t timeRefsTbl[4];

// uint32_t * LC_DigitalOutput::timeRefsTblPtr = nullptr;
// uint32_t * LC_DigitalOutput::blinkIntervlsTblPtr = nullptr;
// uint8_t * LC_DigitalOutput::blinkRepsTblPtr = nullptr;
// uint8_t * LC_DigitalOutput::blinkCntrsTblPtr = nullptr;

LC_DigitalOutput::LC_DigitalOutput(uint8_t *pinsPointer, uint8_t pinsQuantity) {
    pinsTblPtr = (uint8_t*)pinsPointer;
    pinsQty = pinsQuantity;

    #ifdef LC_DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){        
        if(portDbgPtr!=nullptr) portDbgPtr->println(F("LC Dgtl Output: Output pins table pointer attached!"));     
    }
    #endif
}

void LC_DigitalOutput::SetMode(uint8_t mode){
    pinMOde = mode;
    // pinMOde = (mode == OUTPUT) ? OUTPUT : OUTPUT;
    #ifdef LC_DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){        
        if(portDbgPtr!=nullptr) {
            uint8_t offst;
            offst = (pinMOde == OUTPUT) ? 0:1;
            portDbgPtr->print(F("LC Dgtl Output: Mode set to: "));
            // portDbgPtr->println(mode);
            portDbgPtr->println(modeNames[offst]);
        }
    }
    #endif
}

void LC_DigitalOutput::SetActiveState(bool actvState){
    pinActvState = actvState;
    #ifdef LC_DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr) {
            portDbgPtr->print(F("LC Dgtl Output: Active state set to: "));
            portDbgPtr->println(actvState);
        }
    }
    #endif
}

void LC_DigitalOutput::AttachNames(const char *namesPtr, uint8_t length) {
    pinNamesPtr = namesPtr;
    pinNameLen=length;
    #ifdef LC_DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr) {
            portDbgPtr->println(F("LC Dgtl Output: Pin names attached!"));
            portDbgPtr->print(F("  Name length set to: "));
            portDbgPtr->println(length);
        }
    }
    #endif
}

void LC_DigitalOutput::AttachActiveStates(bool *actvStatesPtr) {
    pinActvStatesTblPtr = actvStatesPtr;
    #ifdef LC_DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr) portDbgPtr->println(F("LC Dgtl Output: Actv states tbl atchd!"));
    }
    #endif
}

void LC_DigitalOutput::AttachDebugPort(Stream *ptr) {
    portDbgPtr = ptr;
    #ifdef LC_DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr) portDbgPtr->println(F("LC Dgtl Output: Debug port attached!"));
    }
    #endif
}


void LC_DigitalOutput::SetDebugLevel(DebugLevel level){
    debugLvl = level;
    #ifdef LC_DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr) {
            portDbgPtr->print(F("LC Dgtl Output: Debug level set to: "));
            portDbgPtr->println((uint8_t)debugLvl);
        }
    }
    #endif
}

void LC_DigitalOutput::SetDebugLevel(uint8_t level){
    SetDebugLevel((DebugLevel)level);
}

void LC_DigitalOutput::Init() {
    #ifdef LC_DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr){
            portDbgPtr->print(F("LC Dgtl Output: Initializing "));
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

        if(pinActvStatesTblPtr!=nullptr){ // activate states table is prioritized
            activeStatePtr = pinActvStatesTblPtr+i;
            digitalWrite(*pinPtr,!*(activeStatePtr));
        }
        else{
            digitalWrite(*pinPtr,!pinActvState);
        }        
        #ifdef LC_DEBUG_DIGITALOUTPUT
        if(debugLvl>=DebugLevel::useful){
            if(portDbgPtr==nullptr) return;
            if(pinNamesPtr==nullptr) return;
            char* bfrPtr = (char*)malloc(pinNameLen);
            const char *namePtr=nullptr;
            if(bfrPtr==nullptr) {               
                portDbgPtr->println(F("LC Dgtl Output: String buffer malloc failed!"));    
                return;
            }    
            memset(bfrPtr,0,pinNameLen);   
            namePtr = pinNamesPtr+i*pinNameLen;
            memcpy_P(bfrPtr,namePtr,pinNameLen);
            
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
            if(pinActvStatesTblPtr!=nullptr) portDbgPtr->println(*activeStatePtr);
            else portDbgPtr->println(pinActvState);

            portDbgPtr->print(F("    state init'd to: "));
            if(pinActvStatesTblPtr!=nullptr) portDbgPtr->println(!*activeStatePtr);
            else portDbgPtr->println(!pinActvState);
            portDbgPtr->println();
            
            free(bfrPtr);
        }        
        #endif
    }

    #ifdef LC_DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr){
            portDbgPtr->println(F("LC Dgtl Output: Digital output pins init'd!"));
            portDbgPtr->println();
        }
    }
    #endif

    timeRefsTblPtr = (uint32_t*)malloc(pinsQty*sizeof(uint32_t));
    if(timeRefsTblPtr!=nullptr)
        memset(timeRefsTblPtr,0,pinsQty*sizeof(uint32_t)); 

    blinkIntervlsTblPtr = (uint32_t*)malloc(pinsQty*sizeof(uint32_t));
    if(blinkIntervlsTblPtr!=nullptr)
        memset(blinkIntervlsTblPtr,0,pinsQty*sizeof(uint32_t)); 

    blinkRepsTblPtr = (uint8_t*)malloc(pinsQty*sizeof(uint8_t));
    if(blinkRepsTblPtr!=nullptr)
        memset(blinkRepsTblPtr,0,pinsQty*sizeof(uint8_t)); 

    blinkCntrsTblPtr = (uint8_t*)malloc(pinsQty*sizeof(uint8_t));
    if(blinkCntrsTblPtr!=nullptr)
        memset(blinkCntrsTblPtr,0,pinsQty*sizeof(uint8_t)); 

    #ifdef LC_DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr) {
            if  (   timeRefsTblPtr==nullptr || blinkIntervlsTblPtr==nullptr ||
                    blinkRepsTblPtr==nullptr || blinkCntrsTblPtr==nullptr )
                {
                    portDbgPtr->println(F("LC Dgtl Output: Failed to malloc impt tables!"));
                }
            else
                portDbgPtr->println(F("LC Dgtl Output: Impt tables malloc and init success!"));
        }
    }
    #endif
}

void LC_DigitalOutput::Enable(){
    isEnabled = true;
    #ifdef LC_DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr) portDbgPtr->println(F("LC Dgtl Output: Proc enabled!"));        
    }
    #endif
}

void LC_DigitalOutput::Disable(){
    isEnabled = false;
    #ifdef LC_DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::important){
        if(portDbgPtr!=nullptr) portDbgPtr->println(F("LC Dgtl Output: Proc disabled!"));        
    }
    #endif
}

void LC_DigitalOutput::Drive(uint8_t pinIndex, bool state){
    #ifdef LC_ASSERT_STRICT
    lcAssert(pinIndex < pinsQty);
    #elif defined LC_ASSERT_RELAXED
    if(pinIndex > pinsQty) pinIndex = 0;
    #endif 

    uint8_t *pinPtr = pinsTblPtr + pinIndex;
    bool *activeStatePtr = nullptr;
    bool finalState;

    if(pinActvStatesTblPtr!=nullptr){ // activate states table is prioritized
        activeStatePtr = pinActvStatesTblPtr + pinIndex;
        finalState = state ? *activeStatePtr : !*activeStatePtr;
        digitalWrite(*pinPtr,finalState);
    }
    else{
        finalState = state ? pinActvState : !pinActvState;
        digitalWrite(*pinPtr,finalState);
    }        
    #ifdef LC_DEBUG_DIGITALOUTPUT
    // if(debugLvl>=DebugLevel::useful){
    if(debugLvl>=DebugLevel::devt1){
        if(pinNamesPtr==nullptr) return;
        const char *namePtr=nullptr;
        char* bfrPtr = (char*)malloc(pinNameLen);        
        if(bfrPtr==nullptr) {        
            if(portDbgPtr!=nullptr) portDbgPtr->println(F("LC Dgtl Output: String buffer malloc failed!")); 
            return;
        }         
        memset(bfrPtr,0,pinNameLen);   
        namePtr = pinNamesPtr + pinIndex*pinNameLen;
        memcpy_P(bfrPtr,namePtr,pinNameLen);
        if(portDbgPtr!=nullptr){
            portDbgPtr->println(F("LC Dgtl Output:"));
            portDbgPtr->print(F("  "));
            portDbgPtr->print(bfrPtr);            
            portDbgPtr->print(F(" pin state set to: ")); 
            portDbgPtr->println(finalState);
        }
        free(bfrPtr);
    }
    #endif
}

// initiate blinking of an output pin
// ideally, interval should not be lower than the interval for maintaining the out_Do() function (OUT_INTERVAL_PROCESS)
// blinking for a pin is disabled if interval is set to zero
// when its interval is set to zero, the pin will also be immediately turned off
// the number of blink repetitions "count" should be between 1 and 255
// when "count" is zero, blinking will be infinitely continuous until stopped
// note that out_Do() will perform the actual blinking

void LC_DigitalOutput::Blink(uint8_t pinIndex, uint32_t interval, uint8_t count){
    #ifdef LC_ASSERT_STRICT
    lcAssert(pinIndex < pinsQty);
    lcAssert(pinsTblPtr != nullptr);
    lcAssert(timeRefsTblPtr != nullptr);
    lcAssert(blinkIntervlsTblPtr != nullptr);
    lcAssert(blinkRepsTblPtr != nullptr);
    #elif defined LC_ASSERT_RELAXED
    if(pinIndex > pinsQty) return;
    if(pinsTblPtr == nullptr) return;
    if(timeRefsTblPtr == nullptr) return;
    if(blinkIntervlsTblPtr == nullptr) return;
    if(blinkRepsTblPtr == nullptr) return;
    #endif

    uint32_t *timeRefPtr = timeRefsTblPtr;
    uint32_t *intervalPtr = blinkIntervlsTblPtr;
    uint8_t *countLimitPtr = blinkRepsTblPtr;
    *(intervalPtr+pinIndex) = interval;
    
    #ifdef LC_DEBUG_DIGITALOUTPUT
    if(debugLvl>=DebugLevel::useful){
        if(portDbgPtr!=nullptr) portDbgPtr->println();
    }
    #endif

    if(interval == 0){ // if the interval is zero (disabled)    
        Drive(pinIndex, false); // turn the output pin off  
    }

    *(countLimitPtr+pinIndex) = count;
    *(timeRefPtr+pinIndex) = millis(); 
    
    #ifdef LC_DEBUG_DIGITALOUTPUT
    if(portDbgPtr==nullptr) return;
    if(debugLvl>=DebugLevel::useful){
    // if(debugLvl>=DebugLevel::devt1){        
        if(pinNamesPtr==nullptr) return;
        const char *namePtr=nullptr;
        char* bfrPtr = (char*)malloc(pinNameLen);
        if(bfrPtr==nullptr) {        
            portDbgPtr->println(F("LC Dgtl Output: String buffer malloc failed!")); 
            return;
        }
        memset(bfrPtr,0,pinNameLen);   
        namePtr = pinNamesPtr + pinIndex*pinNameLen;
        memcpy_P(bfrPtr,namePtr,pinNameLen);       
        portDbgPtr->println(F("LC Dgtl Output:"));
        portDbgPtr->println(F("  Blink settings changed!"));
        portDbgPtr->print(F("  "));
        portDbgPtr->print(bfrPtr);            
        portDbgPtr->println(F(" pin:"));
        portDbgPtr->print(F("    time reference set to: "));
        portDbgPtr->println(*(timeRefPtr+pinIndex));
        portDbgPtr->print(F("    interval set to: "));
        portDbgPtr->println(*(intervalPtr+pinIndex));
        portDbgPtr->print(F("    blink repetition limit set to: "));
        portDbgPtr->println(*(countLimitPtr+pinIndex));   
        portDbgPtr->println();      
        free(bfrPtr);
    }
    #endif
}

void LC_DigitalOutput::Loop(){
    static uint32_t tProc = millis();
    static uint8_t state = 0;

    static uint8_t *pinPtr = pinsTblPtr;
    static uint32_t *timeRefPtr = timeRefsTblPtr;
    static uint32_t *intervalPtr = blinkIntervlsTblPtr;
    static uint8_t *countLimitPtr = blinkRepsTblPtr;
    static bool *activeStatePtr = nullptr;

    // static uint8_t blinkCounters[pinsQty];

    if(!isEnabled) return;
    if(millis() - tProc < INTERVAL_PROCESS_DIGITALOUTPUTS) return;

    switch(state){
        case 0: // first run
            // memset(blinkCounters,0,sizeof(blinkCounters)); // initialize counters    
            
            if(pinsTblPtr == nullptr) return;
            if(timeRefsTblPtr == nullptr) return;
            if(blinkIntervlsTblPtr == nullptr) return;
            if(blinkRepsTblPtr == nullptr) return; 
            if(blinkCntrsTblPtr == nullptr) return;

            state = 1;
            break;
        case 1: // subsequent runs
            for(uint8_t i = 0; i < pinsQty; i++){
                if(*(intervalPtr+i)){ // enabled (interval is not zero)
                    if(millis() - *(timeRefPtr+i) > *(intervalPtr+i)){

                        if(*(countLimitPtr+i) == 0){ // infinite number of blinks (repetitions = 0)
                            digitalWrite(*(pinPtr+i),!digitalRead(*(pinPtr+i))); // toggle current state

                            *(timeRefPtr+i) = millis();

                            #ifdef LC_DEBUG_DIGITALOUTPUT
                            if(portDbgPtr==nullptr) return;
                            if(debugLvl>=DebugLevel::devt1){
                                const char *namePtr=nullptr;
                                char* bfrPtr = (char*)malloc(pinNameLen);
                                if(bfrPtr==nullptr) {        
                                    portDbgPtr->println(F("LC Dgtl Output: String buffer malloc failed!")); 
                                    return;
                                }
                                memset(bfrPtr,0,pinNameLen);   
                                namePtr = pinNamesPtr + i*pinNameLen;
                                memcpy_P(bfrPtr,namePtr,pinNameLen);       
                                portDbgPtr->println(F("LC Dgtl Output:"));                                
                                portDbgPtr->print(F("  "));
                                portDbgPtr->print(bfrPtr);            
                                portDbgPtr->println(F(" pin state toggled!"));
                                free(bfrPtr);
                            }
                            #endif             
                        }

                        else{ // finite number of blinks (repetitions != 0)
                            // if(blinkCounters[i] < *(countLimitPtr+i)){
                            if(*(blinkCntrsTblPtr+i) < *(countLimitPtr+i)){
                                digitalWrite(*(pinPtr+i),!digitalRead(*(pinPtr+i))); // toggle current state  

                                if(pinActvStatesTblPtr!=nullptr){ // activate states table is prioritized
                                    activeStatePtr = pinActvStatesTblPtr+i;
                                    if(digitalRead(*(pinPtr+i)) != *(activeStatePtr)){                                        
                                        *(blinkCntrsTblPtr+i) += 1;
                                    }
                                }
                                else{
                                    if(digitalRead(*(pinPtr+i)) != pinActvState){
                                        *(blinkCntrsTblPtr+i) += 1;
                                    }
                                }

                                *(timeRefPtr+i) = millis();

                                #ifdef LC_DEBUG_DIGITALOUTPUT
                                if(portDbgPtr==nullptr) return;
                                if(debugLvl>=DebugLevel::devt1){
                                    const char *namePtr=nullptr;
                                    char* bfrPtr = (char*)malloc(pinNameLen);
                                    if(bfrPtr==nullptr) {        
                                        portDbgPtr->println(F("LC Dgtl Output: String buffer malloc failed!")); 
                                        return;
                                    }
                                    memset(bfrPtr,0,pinNameLen);   
                                    namePtr = pinNamesPtr + i*pinNameLen;
                                    memcpy_P(bfrPtr,namePtr,pinNameLen);       
                                    portDbgPtr->println(F("LC Dgtl Output:"));                                
                                    portDbgPtr->print(F("  "));
                                    portDbgPtr->print(bfrPtr);            
                                    portDbgPtr->println(F(" pin state toggled!"));
                                    free(bfrPtr);
                                }
                                #endif 
                            }

                            else{ // counter has exceeded repetition limit
                                // blinkCounters[i] = 0; // reset counter
                                *(blinkCntrsTblPtr+i) = 0;
                                *(intervalPtr+i) = 0; // disable by setting interval to zero

                                if(pinActvStatesTblPtr!=nullptr){ // activate states table is prioritized
                                    activeStatePtr = pinActvStatesTblPtr+i;
                                    digitalWrite(*pinPtr,!*(activeStatePtr)); // turn off
                                }
                                else{
                                    digitalWrite(*pinPtr,!pinActvState); // turn off
                                }                                                          

                                #ifdef LC_DEBUG_DIGITALOUTPUT
                                if(portDbgPtr==nullptr) return;
                                if(debugLvl>=DebugLevel::devt1){
                                    const char *namePtr=nullptr;
                                    char* bfrPtr = (char*)malloc(pinNameLen);
                                    if(bfrPtr==nullptr) {        
                                        portDbgPtr->println(F("LC Dgtl Output: String buffer malloc failed!")); 
                                        return;
                                    }
                                    memset(bfrPtr,0,pinNameLen);   
                                    namePtr = pinNamesPtr + i*pinNameLen;
                                    memcpy_P(bfrPtr,namePtr,pinNameLen);       
                                    portDbgPtr->println(F("LC Dgtl Output:"));                                
                                    portDbgPtr->print(F("  "));
                                    portDbgPtr->print(bfrPtr);            
                                    portDbgPtr->println(F(" pin turned off!"));
                                    free(bfrPtr);
                                }
                                #endif                                 
                            }
                        }
                        
                    }
                }
            }
            break;
        default:
            break;
    }

    tProc = millis();

}