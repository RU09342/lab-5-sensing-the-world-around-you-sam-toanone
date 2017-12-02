/* Lab 5 Sensors and Signal Conditioning for the MSP430G2553
 * Sam Toanone & Alexander Revolus
 * This code converts a voltage applied to pin 1.3 into an binary value. The adc value will be output through Uart
 * every 1 second. The Uart pins are 1.1 for RXD and 1.2 for TXD. The baudrate for the UART is 9600. The reference voltage for the
 * ADC pin will be Vcc(3.3V) and Gnd(0V). This code will be used for all three circuits.
 */



#include <msp430.h>    //Include all msp430 libraries

unsigned int ADC_value=0;  //Variable assigned to the ADC value

void ConfigureAdc(void); //Function call to set ADC settings

void main(void)         //Main function
{
    {
        WDTCTL = WDTPW + WDTHOLD;        // Stop the watchdog timer


        if (CALBC1_1MHZ==0xFF)                    // If calibration constant is set to all 1's
        {
            while(1);                               //Trap the CPU in an infinite loop until calibration constant is fixed
        }
        /******CONFIGURE UART COMMUNICATION*******************/

        DCOCTL = 0;                               // Select lowest DCOx and MODx settings
        BCSCTL1 = CALBC1_1MHZ;                    // Set range
        DCOCTL = CALDCO_1MHZ;
        P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
        P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
        UCA0CTL1 |= UCSSEL_2;                     // Set clock to SMCLK = 1MHZ
        UCA0BR0 = 104;                            // Set divider to 104. 1MHZ / 104 = 9615.38 Baud rate
        UCA0BR1 = 0;                              // UCOS16=0, therefore, BR1 is 0.
        UCA0MCTL = UCBRS0;                        // Modulation equal to 0.
        UCA0CTL1 &= ~UCSWRST;                     // Enable USCI_A0
        IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
        BCSCTL2 &= ~(DIVS_3);            // SMCLK = 1MHZ
        /********CONFIGURE AND CALL ADC FUNCTION***************/
        P1SEL |= BIT3;                    // Set ADC input to 1.3
        ConfigureAdc();                    // ADC set-up function call
        __enable_interrupt();            // Enable interrupts.

      /*************TRANSMIT ADC VALUE OVER UART *************/
        while(1)
        {
            __delay_cycles(10000);                // Delay Transmit for 1 second
            ADC10CTL0 |= ENC + ADC10SC;            // Sampling and conversion start
            __bis_SR_register(CPUOFF + GIE);    // Low  Power Mode 0 with interrupts enabled
            ADC_value = ADC10MEM;    // Set ADC_value to the value in the ADC10MEM register
            UCA0TXBUF = ADC_value;   //Transmit the ADC value through UART


        }

    }
}


/*************************ADC10 INTERRUPT VECTOR********************/
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
    __bic_SR_register_on_exit(CPUOFF);        // Return to active mode
}

/*****************************FUNCTION CALLED TO CONFIGURE ADC*****************/
void ConfigureAdc(void)
{

    ADC10CTL1 = INCH_3 + ADC10DIV_3 ;  // From datasheet, inch_3=channel A3, ADC10DIV_3
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE; // Vcc & Vss as reference, Sample and hold for 64 Clock cycles, ADC on, ADC interrupt enable
    //SREF_0; VCC(3.3V) and VSS(0V) as reference voltages
    //ADC10SHT_3;
    //ADC10ON= Turn ADC 10 conversion on
    //ADC10IE; ADC10 interrupt enabled
    ADC10AE0 |= BIT3;    //ADC input pin 1.3
    }
