# Software PWM
This part of the lab implements PWM through software. The LED on the processor should load at a 50% duty cycle. With each press of a button, the duty cycle is to increase by 10%, reset to 0 when 100% is reached, and then start to increase by 10% again. Another LED should also light up upon the press of the button.

# MSP430G2553
To begin, just as in the last part of the lab, port 1's pins had to be initialized to set thigns like pull-up resistors, clarify inputs vs. outputs and so on. In this section, two CCR frequencys were set to be compared to determine the duty cycles later in the program. Also in initialization, P1.6, another LED was activated for whenever the P1.3 button was pressed as said in the task. The rest of the functioning part of the program relied on case statements. The timer interrupt's case statement was where the duty cycle was incremented depending on the press of the button. The if statement: "if(pwm < 1000) pwm += 100;", would occur upon a button press and also clear the interrupt's flag and reset the interrupt. The other case for when the button was notbeing pressed would keep the program running as is and enable the interrupt.

# MSP430F5529
The only difference from this F5529 board from the G2553 board is the names of the pins that had the LEDs and buttons. The button was at P1.1 and the LED that blinks only when the button is pressed is at P1.0.

