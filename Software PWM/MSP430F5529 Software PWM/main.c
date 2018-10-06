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

    P1OUT &= ~BIT0; // initializes button output
    P1DIR |= BIT0; // sets P1.0 as output
    P1DIR &= ~BIT1; // sets P1.1 an input
    P1REN |= BIT1; // enables pull-up resistor on button
    P1OUT |= BIT1; // initializes button
    P1IE |= BIT1; // enables interrupt on button
    P1IFG &= ~BIT1; // clears interrupt flag
    P1IES |= BIT1; // hi to lo behavior

    TA0CCR0 = 1000; // sets frequency
    TA0CTL = TASSEL_2 + MC_1 + TACLR; // SMCLK, UP mode, clear timer
    TA1CCTL0 = CCIE; // CCR1 interrupt enabled
    TA1CCR0 = 50000; // sets frequency


    __bis_SR_register(GIE);
    for(;;)
    {
        if(TA0R <= pwm) P1OUT ^= BIT0; // pwm/1000 * 100% = duty cycle
        else if (TA0R > pwm) P1OUT &= ~BIT0;
    }
}
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
        switch(state)
        {
        case 0: // off to on
            TA1CTL = TASSEL_2 + MC_1 + TACLR;
            P1IFG &= ~BIT1; // clear interrupt flag
            P1IE &= ~BIT1; // disable interrupts for P1.1
            break;
        case 1: // on to off
            TA1CTL = TASSEL_2 + MC_1 + TACLR;
            P1IFG &= ~BIT1;
            P1IE &= ~BIT1;
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
            P1IE |= BIT1; // enable interrupts
            P1IES &= ~BIT1; // ho to lo behavior
            TA1CTL &= ~TASSEL_2; // stop timer
            TA1CTL |= TACLR; // clear timer
            state = 1;
            break;
        case 1: // off
            P1IE |= BIT1;
            P1IFG &= ~BIT1;
            P1IES |= BIT1;
            TA1CTL &= ~TASSEL_2;
            TA1CTL |= TACLR;
            state = 0;
            break;
        }


}
