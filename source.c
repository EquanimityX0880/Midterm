// Name: Ryan Zacharias
// Description: Rudimentary Input to LED Display
// Date: 10/25/2020

#include <msp430g2553.h>

// variables
int i; // Variable for For loops
int counter = 0; // Assigns inputs to position Counter in array
int inputArray[] = {0,0}; // Holds information. May be used later to view larger lines

long const int DELAY = 1000000; // 1,000,000 is 1 second
int check = 0;

// Generic function to read inputs into an array.
void input(a) {
    inputArray[counter] = a;
    __delay_cycles(DELAY/4);
    counter++;
    check = 1;
}

void clear(void) {
    for(i = 0; i < 2; i++)
        inputArray[i] = 0; // For loop sets every value in array to 0
    counter = 0; // Next input goes to this position in the array
    check = 1; // Update the LEDs
}

void update(void) {
    //P2.3 // first LED1
    //P2.4 // second LED2
    if(check) {
        // If more inputs are given than there are LEDs,
        // flash the error message
        if(counter > 2) {
            for(i = 0; i < 3; i++) {
                P2OUT = 0xFF;
                __delay_cycles(DELAY/4);
                P2OUT ^= 0xFF;
                __delay_cycles(DELAY/4);
            }
        }
        // If there are less inputs than there are LEDs,
        // confirm that the input was received
        else {
            P2OUT = 0xFF;
            __delay_cycles(DELAY/4);
            P2OUT ^= 0xFF;
            __delay_cycles(DELAY/4);
        }

        // Update the LEDs according to the array.
        // checking what value i has prevents the wrong
        // LED from lighting up.
        for(i = 0; i < 2; i++) {
            if((inputArray[i] == 1) && (i == 0))
                P2OUT |= BIT3;
            else if((inputArray[i] == 1) && (i == 1))
                P2OUT |= BIT4;
        }
        check = 0;
    }
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // stop watchdog timer
    //
    // set pins and ports
    P2SEL = 0;
    P2SEL2 = 0;
    P2DIR = 0x18; // BIT3 and BIT4
    P2REN = ~0x18; // 1110 0111
    P2OUT = 0x00;
    //
    P1DIR &= ~(BIT0 | BIT1 | BIT2);
    P1REN |= (BIT0 | BIT1 | BIT2);
    P1OUT |= (BIT0 | BIT1 | BIT2);
    //
    //reset flash
    P2OUT = 0xFF;
    __delay_cycles(DELAY);
    P2OUT ^= 0xFF;
    //
    // Script
    while(1)
    { // start of while
        if (!(P1IN & BIT0)) // Clear
            clear();
        else if (!(P1IN & BIT1)) // 0, proceed to next space
            input(0);
        else if (!(P1IN & BIT2)) // 1, proceed to next space
            input(1);
        update();
    } // end of while
}
