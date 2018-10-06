/*
 * created by: joseph dibenedetto
 * created: 10/01/18
 * updated: 10/05/18
 */

#include <msp430.h> 


/**
 * main.c
 */
int pwm = 500; // variables for PWM and debounce state
int state = 0;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // disables watchdog timer

    P1OUT &= ~BIT6; // initializes button output
    P1DIR |= BIT6; // sets P1.6 as an output
    P1DIR &= ~BIT3; // sets P1.3 as an input
    P1REN |= BIT3; // sets pull up resistor for button
    P1OUT |= BIT3; // initializes button state
    P1IE |= BIT3; // button interrupt enabled
    P1IFG &= ~BIT3; // button interrupt flag cleared
    P1IES |= BIT3; // hi to low behavior on button interrupt

    TA0CCR0 = 1000; // sets frequency
    TA0CTL = TASSEL_2 + MC_1 + TACLR; // chooses SMCLK, selects UP mode, clears timer
    TA1CCTL0 = CCIE; // CCR1 interrupt enabled
    TA1CCR0 = 50000; // sets frequency

    __bis_SR_register(GIE);
    for(;;)
    {
        if(TA0R <= pwm) P1OUT ^= BIT6; // pwm/1000 * 100% = duty cycle
        else if (TA0R > pwm) P1OUT &= ~BIT6;
    }
}
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
        switch(state)
        {
        case 0: // off to on
            TA1CTL = TASSEL_2 + MC_1 + TACLR;
            P1IFG &= ~BIT3; // clears interrupt flag
            P1IE &= ~BIT3; // disables interrupts for P1.3
            break;
        case 1: // on to off
            TA1CTL = TASSEL_2 + MC_1 + TACLR;
            P1IFG &= ~BIT3;
            P1IE &= ~BIT3;
            break;
        }
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A0 (void)
{
        switch(state)
        {
        case 0: // on
            if(pwm < 1000) pwm += 100;
            else pwm = 0;
            P1IE |= BIT3; // interrupt enabled
            P1IES &= ~BIT3; // hi to lo behavior enabled
            TA1CTL &= ~TASSEL_2; // stop timer
            TA1CTL |= TACLR; // clear timer
            state = 1;
            break;
        case 1: // off
            P1IE |= BIT3;
            P1IFG &= ~BIT3;
            P1IES |= BIT3;
            TA1CTL &= ~TASSEL_2;
            TA1CTL |= TACLR;
            state = 0;
            break;
        }


}
