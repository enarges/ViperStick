#ifndef VIPER_HPP
#define VIPER_HPP

#include "gamepad/PicoGamepad.h"

#define DEBUG

//Pin Assigments
//Chip1
#define DMS_N 4
#define DMS_E 5
#define DMS_W 6
#define DMS_S 7

//Chip2
#define TRIM_N  8
#define TRIM_S  9
#define TRIM_PB 10
#define NWS     11

#define PADDLE 12
#define PINKIE 13
#define TRIG_1 14
#define TRIG_2 15

//Chip3
#define TRIM_W  16
#define TRIM_E  17
#define WEPRLS 18

#define TMS_E 20
#define TMS_S 21
#define TMS_N 22
#define TMS_W 23

//Chip4
#define CMS_S 24
#define CMS_N 25
#define CMS_E 26
#define CMS_W 27








namespace stick
{

    class Viper
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

        const u_int8_t ADCX_PIN = 26;
        const u_int8_t ADCY_PIN = 27;
        
        //Sample every 10ms
        const unsigned long SamplePeriod = 10;
        
        uint32_t    shift(pin_size_t dataPin, uint8_t clockPin, BitOrder bitOrder);
        int         isrDigitalRead(uint8_t pin);
        void        readGimbals();
    };
}

#endif