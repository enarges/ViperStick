#include "throttle.h"

namespace stick
{
    double throttle1Val;
    double throttle2Val;
    bool throttleBlock = false;

    void Throttle::setup()
    {

        PicoGamepad m_gamepad();

        pinMode(ISRDataPin, INPUT);
        pinMode(ISRLatchPin, OUTPUT);
        pinMode(ISRClockPin, OUTPUT);

        // Throttle 1 Hall Effect Sensor on pin 26
        pinMode(ADC_THROTTLE1_PIN, INPUT);
        // Throttle 2 Hall Effect Sensor on pin 27
        pinMode(ADC_THROTTLE2_PIN, INPUT);
;
    }

    /******************************************************************************
     * 16bit ISR Shift routine
     * Shifts the bits 1 bit at a time into a 16bit int
     *****************************************************************************/
    uint16_t Throttle::shift(pin_size_t dataPin, uint8_t clockPin, BitOrder bitOrder)
    {
        uint16_t value = 0;
        uint8_t i;

        digitalWrite(ISRClockPin, HIGH); // preset clock to retrieve first bit
        digitalWrite(ISRLatchPin, HIGH); // disable input latching and enable shifting

        // Loop through the bits in the registers
        for (i = 0; i < 16; ++i)
        {
            digitalWrite(clockPin, HIGH);
            if (bitOrder == LSBFIRST)
                value |= digitalRead(dataPin) << i;
            else
                value |= digitalRead(dataPin) << (15 - i);
            digitalWrite(clockPin, LOW);
        }

        digitalWrite(ISRLatchPin, LOW); // disable shifting and enable input latching
        return value;
    }

    /******************************************************************************
     * Read the hall sensors for the throttles
     *****************************************************************************/
    void Throttle::readThrottles()
    {
        double xSum = 0;
        double ySum = 0;

        for (int i = 0; i < 100; i++)
        {
            throttle1Val += analogRead(ADC_THROTTLE1_PIN);
            throttle2Val += analogRead(ADC_THROTTLE2_PIN);
            sleep_ms(1);
        }

        throttleBlock = true;
        throttle1Val = xSum / 100;
        throttle2Val = ySum / 100;
        throttleBlock = false;
    }

    /******************************************************************************
     * Read the value of a specific pin
     *****************************************************************************/
    int Throttle::isrDigitalRead(uint8_t pin)
    {
        uint32_t value = shift(ISRDataPin, ISRClockPin, MSBFIRST);
        return bitRead(value, pin);
    }

    void Throttle::readStick()
    {
        static unsigned long previousTime = 0;
        unsigned long currentTime = millis();
        if (currentTime - previousTime >= SamplePeriod)
        {
            // readInputsWithDigitalRead();
            previousTime = currentTime;

            // Read the buttons
            m_gamepad.SetButton(0, isrDigitalRead(PB));


            // Read the Hats
            if (isrDigitalRead(HAT_1_N))
            {
                m_gamepad.SetHat(0, HAT_DIR_N);
            }
            else if (isrDigitalRead(HAT_1_E))
            {
                m_gamepad.SetHat(0, HAT_DIR_E);
            }
            else if (isrDigitalRead(HAT_1_S))
            {
                m_gamepad.SetHat(0, HAT_DIR_S);
            }
            else if ((isrDigitalRead(HAT_1_W)))
            {
                m_gamepad.SetHat(0, HAT_DIR_W);
            }
            else
            {
                m_gamepad.SetHat(0, HAT_DIR_C);
            }

            if (isrDigitalRead(HAT_2_N))
            {
                m_gamepad.SetHat(1, HAT_DIR_N);
            }
            else if (isrDigitalRead(HAT_2_E))
            {
                m_gamepad.SetHat(1, HAT_DIR_E);
            }
            else if (isrDigitalRead(HAT_2_S))
            {
                m_gamepad.SetHat(1, HAT_DIR_S);
            }
            else if ((isrDigitalRead(HAT_2_W)))
            {
                m_gamepad.SetHat(1, HAT_DIR_W);
            }
            else
            {
                m_gamepad.SetHat(1, HAT_DIR_C);
            }

            if (isrDigitalRead(EXT))
            {
                m_gamepad.SetHat(2, HAT_DIR_W);
            }
            else if (isrDigitalRead(RET))
            {
                m_gamepad.SetHat(2, HAT_DIR_E);
            }
            else
            {
                m_gamepad.SetHat(2, HAT_DIR_C);
            }

            readThrottles();
            // Read the gimbals
            m_gamepad.SetThrottle(map(throttle1Val, 0, 1023, -32767, 32767));
            m_gamepad.SetS0(map(throttle2Val, 0, 1023, -32767, 32767));
#ifdef DEBUG
            for (int i=0; i<32; i++) {
                    Serial.print(isrDigitalRead(i));
                    
            }
            Serial.println();
#ifdef DEBUG_GIMBAL
            Serial.print("X= ");
            Serial.print(" ");
            Serial.print(xVal);
            Serial.print(" ");
            Serial.print(map(xVal, 0, 1023, -32767, 32767));
            Serial.print(" ");

            Serial.print("Y= ");
            Serial.print(" ");
            Serial.print(yVal);
            Serial.print(" ");
            Serial.print(map(yVal, 0, 1023, -32767, 32767));
            Serial.println();
#endif
#endif

            // Send all inputs via HID
            // Nothing is send to your computer until this is called.
            m_gamepad.send_update();
        }
    }
}