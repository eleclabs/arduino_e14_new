 /*
	 * Connect the four control pins to any Arduino pins.
	 * This example uses digital pins 4, 5, 6, and 7.
	 * You will get analog output at the Analog pin that you connect to Sig pin of CD74HC4067
	 * This examples loops through all channels and prints the analog input at all 16 channels
	*/


	#include <light_CD74HC4067.h>


	            // s0 s1 s2 s3: select pins
	CD74HC4067 mux(9, 10, 11, 12);  // create a new CD74HC4067 object with its four select lines - 8,9,10,11


	const int signal_pin = A0; // Pin A0 - Connected to Sig pin of CD74HC4067


	void setup()
	{
	    Serial.begin(9600);
	    pinMode(signal_pin, INPUT); // Set as input for reading through signal pin
	}


	void loop()
	{
	  // loop through channels 0 - 15
	    for (byte i = 0; i < 16; i++) {
	        mux.channel(i);
	        int val = analogRead(signal_pin);                       // Read analog value
	        //Serial.print("Channel "+String(i)+": "+String(val));  // Print value
          Serial.print("\t"+String(val));
	        //delay(10);  
	    }
    Serial.println("");
	  delay(200);
    
	}