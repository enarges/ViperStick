#ifndef VIPER_HPP
#define VIPER_HPP

#include <Thread.h>
#include "gamepad/PicoGamepad.h"

//Pin Assigments
//Chip1
#define TRIM_W  0
#define TRIM_PB 1
#define TRIM_E  2
#define TRIM_N  3

#define CMS_S 4
#define CMS_E 5
#define CMS_N 6
#define CMS_W 7

//Chip2
#define DMS_N 8
#define DMS_S 9
#define DMS_E 10
#define DMS_W 11

#define TRIM_S 13
#define WEPRLS 14
#define NWS    15

//Chip3
#define TMS_W 16
#define TMS_E 17
#define TMS_N 18
#define TMS_S 19

#define PINKIE 20
#define PADDLE 21
#define TRIG_2 22
#define TRIG_1 23

namespace stick
{

    class Viper
    {

    public:
        void setup();
        void readStick();

    private:
        PicoGamepad m_gamepad;
        
        //Analog Read Thread
        Thread      m_readThread;
        int         m_xReading;
        int         m_yReading;

        // ISR Pins on the Pico
        const uint8_t ISRLatchPin = 0; // connected to 74HC165 SH/LD (1) pin
        const uint8_t ISRClockPin = 1; // connected to 74HC165 CLK (2) pin
        const uint8_t ISRDataPin  = 2; // connected to 74HC165 QH (9) pin

        const u_int8_t ADC0_PIN = 26;
        const u_int8_t ADC1_PIN = 27;
        
        //Sample every 10ms
        const unsigned long SamplePeriod = 10;
        
        uint32_t    shift(pin_size_t dataPin, uint8_t clockPin, BitOrder bitOrder);
        int         isrDigitalRead(uint8_t pin);
    };
}

#endif