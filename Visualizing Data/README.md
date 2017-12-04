# Visualizing Data
This lab will consist of two parts. First, A photoresistor circuit will be made. If the Photoresistor senses light, the LED will dim. 
When the Photoresistor does not sense light, The LED will brighten. The processor used was the msp430g2553. The second part will consist will consist of displaying low or high temperature for the msp430fr6989

## RGB LED
To have the MSP430G2553 control the brightness of a LED from the output voltage of a photoresistor, multiple peripheals will be used. The first is timers. The timer will be able to control the brightness of the LED. The second is analog to digital conversion. The ADC vlaue will change the PWM of the timer, which will change the brightness of the LED.

The following code will be used to configure the ADC of the MSP430 and to change the PWM of the LED.

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

The MSP430 ADC uses the VCC(3.3V) and gnd(0V) for the maximum and minimum values for the voltage. 1024 will be 3.3V and 0V will be 0. When the the the resistance of the photo resistor is low, a higher voltage will be input to the msp430g2553 adc pin. This value will be converted into a binary number and the higher the number is, the dimmer the LED will be. This happens because the TA1CCR0 value will decrease.


## LCD Display




