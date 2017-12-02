/* Sam Toanone and Alexander Revolus
 * This code uses the ADC value from the output of a photoresistor to
 * change the brightness of an LED.
 */

#include <msp430.h>

// Global variables
unsigned int ADC_value=0;

// Function prototypes
void ConfigureAdc(void);

void main(void)
{
    {
        WDTCTL = WDTPW + WDTHOLD;        // Stop WDT

        P1OUT &= ~BIT0;
        P1DIR |= BIT0;
        P2DIR |= BIT0;
        P2SEL |=BIT0;
        P2SEL &= ~BIT0;

        TA1CTL=TASSEL_2 + MC_1;
         TA1CCR0=1024;
          TA1CCTL1 |= OUTMOD_3;
            TA1CCTL2 |= OUTMOD_3;

            TA1CCR1 = 200;
            TA1CCR2 = 200;



        P1OUT &= ~BIT6;
        P1DIR |= BIT6;


        BCSCTL2 &= ~(DIVS_3);            // SMCLK = DCO = 1MHz
        P1SEL |= BIT3;                    // ADC input pin P1.3
        ConfigureAdc();                    // ADC set-up function call
        __enable_interrupt();            // Enable interrupts.



    }
}


// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
    TA1CCR0=1024-ADC_value;
    __bic_SR_register_on_exit(CPUOFF);        //
}

// Function containing ADC set-up
void ConfigureAdc(void)
{

    ADC10CTL1 = INCH_3 + ADC10DIV_3 ;         // Channel 3 divide clock by 3.
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  //Turn ADC on
    ADC10AE0 |= BIT3;                         // ADC input enable P1.3
    TA1CCR0=1024-ADC_value;   //use ADC value to control brightness of LED
}

