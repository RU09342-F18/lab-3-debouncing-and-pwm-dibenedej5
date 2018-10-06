/*
 * created by: joseph dibenedetto
 * created: 10/01/18
 * updated: 10/05/18
 */

#include <msp430.h> 
/**
 * main.c
 */

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // disables watchdog timer

    P1DIR |= BIT0; // sets P1.0 to an output
    P1REN |= BIT3; // sets P1.3 to be a pull-up resistor
    P1OUT |= BIT3; // initializes state of button
    P1IE |= BIT3;  // enables interrupt on the button
    P1IES |= 0; // enables "high to low" behavior on interrupt

    TACCTL0 = 0x0010; // CCR0 interrupt enabled
    TACCR0 =  60000; // sets frequency
    TA0CTL = TASSEL_2 + MC_0; // selects SMCLK and initializes timer to stop mode

    __enable_interrupt(); //enables interrupts

    __bis_SR_register(LPM0 + GIE);
}

    #pragma vector=PORT1_VECTOR // makes interrupt tick
    __interrupt void Interrupt_1(void)
    {
        P1OUT ^= BIT0; // toggles state of LED
        P1IE &= ~BIT3; // disables button interrupt
        TA0CTL = TASSEL_2 + MC_1; // selects SMCLK and sets timer to UP mode
    }

    #pragma vector = TIMER0_A0_VECTOR // timer0 A0 interruptth service routine
    __interrupt void Interrupt_2(void)
    {
        TA0CTL = MC_0; // timer to stop mode
        P1IE |= BIT3; // enables button interrupt
        P1IFG &= ~BIT3; // disables interrupt flag for button
    }


