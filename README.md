# ArduParty

Real-time lighting control system written in C.  An MSGEQ7 digital signal processor performs a Fast-Fourier Transform on a live audio signal at seven different frequencies in the audio range and delivers magnitudes to an ATMega328-PU.

In turn, this microcontroller maps the magnitudes to usable, relative intensities of each music frequency, and averages lows, mids, and highs.  The three RGB grounds for an LED light strip are connected to individual power MOSFETs with the LED power line connected to a +12V supply.  Each FET is controlled by a pin  on the microcontroller with a PWM signal of duty cycle proportional to the intensity of the audio signal with lows mapped to Red, mids mapped to Green, and highs mapped to Blue.  A voltage regulator delivers 5V to the ATMega and MSGEQ7 using the same +12V supply, simplifying the embedded system design.

Similar attempts to such a project have been made, but the embedded nature and optimized control code in mine makes for a very responsive, compact, and unique light show :-)
