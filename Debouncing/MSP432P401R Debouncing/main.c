/*
 * created by: joseph dibenedetto
 * created: 10/01/18
 * updated: 10/05/18
 */

#include "msp.h"
/**
 * main.c
 */

int main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // disables watchdog timer

    P1DIR = ~(uint8_t) BIT1; // sets P1.1 as an input
    P1OUT = BIT1; // initializes button
    P1REN = BIT1; // enables pull-up resistor on button
    P1SEL0 = 0; // selects 0 bits for MUX
    P1SEL1 = 0;
    P1IES = BIT1; // interrupt set to hi to lo behavior
    P1IFG = 0; // clear port 1 interrupt flag
    P1IE = BIT1; // enables interrupt for P1.1

    NVICISER[1] = 1 << ((PORT1_IRQn) & 31); // enables port 1 interrupt on the NVIC

    PJDIR |= (BIT0| BIT1 | BIT2 | BIT3); // initializes port J
    PJOUT &= ~(BIT0 | BIT1 | BIT2 | BIT3);

    __enable_irq(); // enables interrupts
}

void PORT1_IRQHandler(void)
{
    volatile uint32_t i;

    if(P1IFG & BIT1)
        P1OUT ^= BIT0; // toggles state of LED

    for(i = 0; i < 10000; i++) // switch debounce delay

    P1IFG &= ~BIT1;   // disables interrupt flag for P1.1
}
