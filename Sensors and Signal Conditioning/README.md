Sensors and Signal Conditioning 

PURPOSE: 
The purpose of this lab was to combine hardware and software to read voltage outputs from 3 different sensors. The sensors that will be used are 1) Photoresistor, 2) Thermistor, 3) Phototransistor. The Photoresistor acts as a variable resistor. The value of the resistance will be dependent on the intensity of the light that hits the sensor. The thermistor will output a voltage that correlates to the ambient temperature in degrees Celsius. The Phototransistor will allow more current to pass as the intensity of light that hits the sensor increases.  The code that will be used will combine UART and ADC conversions. The ADC value of the voltage will be transmitted to Realterm through UART.

PHOTORESISTOR CIRCUIT:
The photoresistor circuit will be a voltage divider circuit with the input voltage to the MSP430 being between the two resistors. The value of the photoresistor will change depending on the intensity of the light. When light is hitting the resistor, the resistance decreases causing a higher voltage. If the light decreases, the resistance increases. The resistance values can range from 1M ohm when no light is hitting the resistor to 100 Ohms when light is hitting it.  The voltage source will be VCC from the MSP430  which is 3.6V for both the MSP430G2553 and the MSP430FR6989. The circuits are shown below with simulations.

THERMISTOR CIRCUIT:
The Thermistor circuit will output the voltage that relates to degrees Celsius. If the temperature in a room is 20 degrees C, the output voltage will be .2V. The LM35 DT has three pins. One is for VCC(3.6V), GND, and the last pin will be the input to the ADC pin on the MSP430. The circuits are shown below.


PHOTOTRANSISTOR CIRCUIT:
The circuit for the phototransistor needs to output a voltage from the changing current from the Transistor. To do this a Transimpedance amplifier will be used.  The circuit and the simulations are shown below.




MSP430G2553 Code:
For the MSP430G2553, the input ADC pin will be pin 1.3. The RX and TX pins for UART will be 1.1 and 1.2 respectively.  To configure ADC in the msp430, the following code will be used.
In main:

P1SEL |= BIT3;                    // Set ADC input to 1.3

        ConfigureAdc();                    // ADC set-up function call
        
        __enable_interrupt();            // Enable interrupts.
        
This code will set PIN 1.3 to input for the ADC. ConfigureAdc(); will call a function that will set all the ADC registers. 

__enable_interrupt(); will allow ADC interrupts.

In ConfigureAdc(); :

ADC10CTL1 = INCH_3 + ADC10DIV_3 ;  // From datasheet, inch_3=channel A3, ADC10DIV_3

    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE; // Vcc & Vss as reference, Sample and hold for 64 Clock cycles, ADC on, ADC interrupt enable
    
    //SREF_0; VCC(3.3V) and VSS(0V) as reference voltages
    
    //ADC10SHT_3;  
    
    //ADC10ON= Turn ADC 10 conversion on
    
    //ADC10IE; ADC10 interrupt enabled 
    
    ADC10AE0 |= BIT3;    //ADC input pin 1.3
    
ADC10CTL1 and ADC10CTL0 are the ADC memory control registers 1 and 0. Inch_3 chooses analog input 3 and ADC10DIV_3 divides the clock.

SREF_0 chooses the reference voltage which will be VCC. ADC10IE enables ADC interrupts.

