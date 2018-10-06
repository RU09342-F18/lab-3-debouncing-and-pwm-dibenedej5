/*
 * created by: joseph dibenedetto
 * created: 10/01/18
 * updated: 10/05/18
 */

#include <msp430.h> 
/**
 * main.c
 */

int pwm = 500;

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // disables watchdog timer

    P1DIR |= BIT6; // sets P1.0 as output
    P1SEL |= BIT6; // P1.0 option select
    P1DIR &= ~BIT3; // sets P1.3 as an input
    P1REN |= BIT3; // pull-up resistor enabled
    P1OUT |= BIT3; // initializes button
    P1IE |= BIT3; // interrupt enabled
    P1IFG &= ~BIT3; // interrupt flag cleared

    TA1CCTL0 = CCIE; // CCR1 interrupt enabled
    TA1CCR0 = 10000; // overflow every 10ms
    TA0CCTL1 = OUTMOD_7; // CCR1 reset/set
    TA0CCR0 = 1000-1; // overflow every 1ms
    TA0CCR1 = 500; // PWM starts at 50%
    TA0CTL = TASSEL_2 | MC_1; //SMCLK, up mode

    __bis_SR_register(LPM0_bits + GIE);

    return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{

    TA1CTL = TASSEL_2 + MC_1;
    P1IFG &= ~BIT3;
    P1IES &= ~BIT3;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{
    if(pwm < 1000) pwm += 100;
    else pwm = 0;
    P1IE |= BIT3;
    TA0CCR1 = pwm; // PWM increase by 100 then reset to zero
    TA1CTL = 0x00;
}
