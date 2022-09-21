#include "viper.h"

namespace stick
{
    double xVal;
    double yVal;
    bool block = false;

    void Viper::setup()
    {

        PicoGamepad m_gamepad();

        pinMode(ISRDataPin, INPUT);
        pinMode(ISRLatchPin, OUTPUT);
        pinMode(ISRClockPin, OUTPUT);

        // X Hall Effect Sensor on pin 26
        pinMode(ADCX_PIN, INPUT);
        // Y Hall Effect Sensor on pin 27
        pinMode(ADCY_PIN, INPUT);
;
    }

    /******************************************************************************
     * 32bit ISR Shift routine
     * Shifts the bits 1 bit at a time into a 32bit int
     *****************************************************************************/
    uint32_t Viper::shift(pin_size_t dataPin, uint8_t clockPin, BitOrder bitOrder)
    {
        uint32_t value = 0;
        uint8_t i;

        digitalWrite(ISRClockPin, HIGH); // preset clock to retrieve first bit
        digitalWrite(ISRLatchPin, HIGH); // disable input latching and enable shifting

        // Loop through the bits in the registers
        for (i = 0; i < 32; ++i)
        {
            digitalWrite(clockPin, HIGH);
            if (bitOrder == LSBFIRST)
                value |= digitalRead(dataPin) << i;
            else
                value |= digitalRead(dataPin) << (31 - i);
            digitalWrite(clockPin, LOW);
        }

        digitalWrite(ISRLatchPin, LOW); // disable shifting and enable input latching
        return value;
    }

    /******************************************************************************
     * Read the hall sensors for the gimbals
     *****************************************************************************/
    void Viper::readGimbals()
    {
        double xSum = 0;
        double ySum = 0;

        for (int i = 0; i < 100; i++)
        {
            xSum += analogRead(ADCX_PIN);
            ySum += analogRead(ADCY_PIN);
            sleep_ms(1);
        }

        block = true;
        xVal = xSum / 100;
        yVal = ySum / 100;
        block = false;
    }

    /******************************************************************************
     * Read the value of a specific pin
     *****************************************************************************/
    int Viper::isrDigitalRead(uint8_t pin)
    {
        uint32_t value = shift(ISRDataPin, ISRClockPin, MSBFIRST);
        return bitRead(value, pin);
    }

    void Viper::readStick()
    {
        static unsigned long previousTime = 0;
        unsigned long currentTime = millis();
        if (currentTime - previousTime >= SamplePeriod)
        {
            // readInputsWithDigitalRead();
            previousTime = currentTime;
            // Read the buttons
            m_gamepad.SetButton(0, isrDigitalRead(PADDLE));
            m_gamepad.SetButton(1, isrDigitalRead(PINKIE));
            m_gamepad.SetButton(2, isrDigitalRead(TRIG_1));
            m_gamepad.SetButton(3, isrDigitalRead(TRIG_2));
            m_gamepad.SetButton(4, isrDigitalRead(WEPRLS));
            m_gamepad.SetButton(5, isrDigitalRead(NWS));
            m_gamepad.SetButton(6, isrDigitalRead(TRIM_PB));

            // Read the Hats
            if (isrDigitalRead(TRIM_N))
            {
                m_gamepad.SetHat(0, HAT_DIR_N);
            }
            else if (isrDigitalRead(TRIM_E))
            {
                m_gamepad.SetHat(0, HAT_DIR_E);
            }
            else if (isrDigitalRead(TRIM_S))
            {
                m_gamepad.SetHat(0, HAT_DIR_S);
            }
            else if ((isrDigitalRead(TRIM_W)))
            {
                m_gamepad.SetHat(0, HAT_DIR_W);
            }
            else
            {
                m_gamepad.SetHat(0, HAT_DIR_C);
            }

            if (isrDigitalRead(TMS_N))
            {
                m_gamepad.SetHat(1, HAT_DIR_N);
            }
            else if (isrDigitalRead(TMS_E))
            {
                m_gamepad.SetHat(1, HAT_DIR_E);
            }
            else if (isrDigitalRead(TMS_S))
            {
                m_gamepad.SetHat(1, HAT_DIR_S);
            }
            else if ((isrDigitalRead(TMS_W)))
            {
                m_gamepad.SetHat(1, HAT_DIR_W);
            }
            else
            {
                m_gamepad.SetHat(1, HAT_DIR_C);
            }

            if (isrDigitalRead(CMS_N))
            {
                m_gamepad.SetHat(2, HAT_DIR_N);
            }
            else if (isrDigitalRead(CMS_E))
            {
                m_gamepad.SetHat(2, HAT_DIR_E);
            }
            else if (isrDigitalRead(CMS_S))
            {
                m_gamepad.SetHat(2, HAT_DIR_S);
            }
            else if ((isrDigitalRead(CMS_W)))
            {
                m_gamepad.SetHat(2, HAT_DIR_W);
            }
            else
            {
                m_gamepad.SetHat(2, HAT_DIR_C);
            }

            if (isrDigitalRead(DMS_N))
            {
                m_gamepad.SetHat(3, HAT_DIR_N);
            }
            else if (isrDigitalRead(DMS_E))
            {
                m_gamepad.SetHat(3, HAT_DIR_E);
            }
            else if (isrDigitalRead(DMS_S))
            {
                m_gamepad.SetHat(3, HAT_DIR_S);
            }
            else if ((isrDigitalRead(DMS_W)))
            {
                m_gamepad.SetHat(3, HAT_DIR_W);
            }
            else
            {
                m_gamepad.SetHat(3, HAT_DIR_C);
            }

            readGimbals();
            // Read the gimbals
            m_gamepad.SetX(map(xVal, 0, 1023, -32767, 32767));
            m_gamepad.SetY(map(yVal, 0, 1023, -32767, 32767));
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