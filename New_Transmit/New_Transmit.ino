#include <SPI.h>
#include "RF24.h"


/*
Akhil and Alex:
This code is meant to send a packet with the time current time every 1 second. 
It is important that it takes less than 1 second to receive or 
two packets will be sent and only one received. If the code doesn't run
hopefully you can debug any errors...
Start running the receiver code. Then run this code.
*/


RF24 radio(7,8);
byte addresses[][6] = {"1Node","2Node"};
int role = 0;

double start_time = 0;


void setup() {
  Serial.begin(115200);
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  radio.stopListening();
}

void loop() {
   if(role == 1)
   {
    start_time = micros();                            
     if (!radio.write( &start_time, sizeof(double) )){
       Serial.println(F("stanby"));
//       delay(5000);
     }
     else{
      Serial.print("Sending at time: ");
      Serial.println(start_time);
//      delay(100);
     }
//    delay(500); //ASSUMES SIGNAL IS RECEIVED IN UNDER ONE SECOND
   }

    //Serial Monitor Enable
if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == 0 ){      
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1;                  // Become the primary transmitter (ping out)
    
   }else
    if ( c == 'R' && role == 1 ){
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
       role = 0;                // Become the primary receiver (pong back)
       radio.startListening();
       
    }
  }

   
}




