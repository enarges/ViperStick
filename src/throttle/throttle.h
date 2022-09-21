#ifndef THROTTLE_HPP
#define THROTTLE_HPP

#include "gamepad/PicoGamepad.h"

#define DEBUG

//Pin Assignments
//Chip1
#define HAT_1_N 0
#define HAT_1_S 1
#define HAT_1_W 2
#define HAT_1_E 3

#define HAT_2_N 4
#define HAT_2_S 5
#define HAT_2_E 6
#define HAT_2_W 7

//Chip2
#define EXT 8
#define RET 9
#define PB 10

namespace stick
{

    class Throttle
    {
        public:
        void setup();
        void readStick();

        private:
        PicoGamepad m_gamepad;

        // ISR Pins on the Pico
        const uint8_t ISRLatchPin = 0; // connected to 74HC165 SH/LD (1) pin
        const uint8_t ISRClockPin = 1; // connected to 74HC165 CLK (2) pin
        const uint8_t ISRDataPin  = 2; // connected to 74HC165 QH (9) pin

        const u_int8_t ADC_THROTTLE1_PIN = 26;
        const u_int8_t ADC_THROTTLE2_PIN = 27;
        
        //Sample every 10ms
        const unsigned long SamplePeriod = 10;
        
        uint16_t    shift(pin_size_t dataPin, uint8_t clockPin, BitOrder bitOrder);
        int         isrDigitalRead(uint8_t pin);
        void        readThrottles();
    };
};
#endif
