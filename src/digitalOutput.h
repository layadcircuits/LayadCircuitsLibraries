#ifndef DIGITALOUTPUT_H
#define DIGITALOUTPUT_H

#include "lcDefinitions.h"

#ifdef LC_DEBUG_ALL
#define LC_DEBUG_DIGITALOUTPUT
#endif

class LC_DigitalOutput{
    public:
        LC_DigitalOutput(uint8_t *pinsPointer, uint8_t pinsQuantity);

        void SetMode(uint8_t mode);
        void SetActiveState(bool actvState);

        void AttachModes(uint8_t *modesPtr); // TBI
        void AttachActiveStates(bool *actvStatesPtr);
        void AttachNames(const char *namesPtr, uint8_t length);

        void AttachDebugPort(Stream *ptr);
        void SetDebugLevel(uint8_t level);
        void SetDebugLevel(DebugLevel level);
        void Init();

        void Enable();
        void Disable();

        void Drive(uint8_t pinIndex, bool state);
        void Blink(uint8_t ix, uint32_t interval, uint8_t count);

        void Loop();

    private:        
        Stream *portDbgPtr = nullptr;
        DebugLevel debugLvl=DebugLevel::none;

        uint8_t *pinsTblPtr = nullptr;
        uint8_t pinsQty=0;

        uint8_t pinMOde = OUTPUT;
        uint8_t *pinModesPtr = nullptr;

        bool pinActvState = HIGH; 
        bool *pinActvStatesTblPtr = nullptr;

        const char *pinNamesPtr = nullptr;
        uint8_t pinNameLen = 0;

        static uint32_t *timeRefsTblPtr;
        static uint32_t *blinkIntervlsTblPtr;
        static uint8_t *blinkRepsTblPtr;
        static uint8_t *blinkCntrsTblPtr;

        bool isEnabled = false;
        #ifdef LC_DEBUG_DIGITALOUTPUT
        static char modeNames[2][10+1];
        #endif
};


#endif