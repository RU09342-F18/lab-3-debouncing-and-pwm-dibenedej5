# Hardware PWM
This last section of the lab had to function the same way as the previous section. Instead of requiring case statements in software to enable the change in the duty cycle of the LED, the timer and it's interrupts would directly interface with the GPIO pins of the LED and modify it itself.

# MSP430G2553
The same intializations occurred here as in the last section. The difference is some settings on the timer control register were enabled in order to amke the timer fullt interact with the LED as an output. The capture and compare register had to be set into set/reset mode with the OUTMOD macro. With this setting and slightly modifying the values in the CCR register's which can be seen in the code, we are enabling the hardware to modify the duty cycle instead of relying on software.

# MSP430FR2311
The only difference here again is the pin assignments of the button and LEDs. The button is at P2.0 and the LED is at P1.1.
