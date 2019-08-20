#ifndef DIGITALOUTPUT_H
#define DIGITALOUTPUT_H

#include "lcDefinitions.h"

#ifdef DEBUG_ALL
#define DEBUG_DIGITALOUTPUT
#endif

class LC_DigitalOutput{
    public:
        LC_DigitalOutput(uint8_t *pinsPointer, uint8_t pinsQuantity);

        void SetMode(uint8_t mode);
        void SetActiveState(bool actvState);

        void AttachModes(uint8_t *modesPtr); // TBI
        void AttachActiveStates(uint8_t *actvStatesPtr); // TBI
        void AttachNames(const char *namesPtr, uint8_t length);

        void AttachDebugPort(Stream *ptr);
        void SetDebugLevel(uint8_t level);
        void SetDebugLevel(DebugLevel level);
        void Init();

        void Drive(uint8_t pinIndex, bool state);

    private:        
        Stream *portDbgPtr = nullptr;
        DebugLevel debugLvl=DebugLevel::none;

        uint8_t *pinsTblPtr = nullptr;
        uint8_t pinsQty=0;

        uint8_t pinMOde = OUTPUT;
        uint8_t *pinModesPtr = nullptr;

        bool pinActvState = HIGH; 
        bool *pinActvStatesPtr = nullptr;

        const char *pinNamesPtr = nullptr;
        uint8_t pinNameLen = 0;

        bool isEnabled = false;
        #ifdef DEBUG_DIGITALOUTPUT
        static char modeNames[2][10+1];
        #endif
};


#endif