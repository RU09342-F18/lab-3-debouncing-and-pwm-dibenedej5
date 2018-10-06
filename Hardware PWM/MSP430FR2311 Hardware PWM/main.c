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
    WDTCTL = WDTPW | WDTHOLD; // disables watchdog timer

    PM5CTL0 &= ~LOCKLPM5;

    P2DIR |= BIT0; // initializes port 1 and 2 registers fro LED and button and interrupts
    P2SEL0 |= BIT0;
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;
    P1IE |= BIT1;
    P1IFG &= ~BIT1;

    TB0CCTL0 = CCIE; // CCR1 interrupt enabled
    TB0CCR0 = 10000; // overflow every 10ms
    TB1CCTL1 = OUTMOD_7; // CCR1 reset/set
    TB1CCR0 = 1000-1; // overflow every 1ms
    TB1CCR1 = 500; // PWM starts at 50%
    TB1CTL = TBSSEL__SMCLK + MC__UP + TBCLR + ID_2;

    __bis_SR_register(LPM0_bits + GIE);

    return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{

    TB0CTL = TBSSEL__SMCLK + MC__UP + TBCLR + ID_2;

    P1IFG &= ~BIT1;
    P1IES &= ~BIT1;
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_A0 (void)
{
    if(pwm < 1000) pwm += 100;
    else pwm = 0;
    P1IE |= BIT1;
    TB1CCR1 = pwm;
    TB0CTL = 0x00;
}
