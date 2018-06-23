
#include <SPI.h>
#include "RF24.h"

/*
Akhil and Alex:
This code continuously waits to receive a signal. When one is recieved information 
is printed to the console. Then it waits to receive a signal again. Begin running this code
before running the transmitter code.

Here is how events should occur:
1) Signal is transmitted
2) Transmitter is paused
3) Signal is received and printed to console
4) Transmitter is unpaused
5) Return to step 1
*/

RF24 radio(9,10);
byte addresses[][6] = {"1Node","2Node"};
double begin_time;
int count = 0;
double elapsed_sum = 0;
int average_len = 1000;
double elapsed = 0;
//Time variables
double now = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(F("*** Please wait until transmitter begins"));
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  radio.startListening();
  
}

void loop() {
    double sent_time;
    begin_time = micros();
    if(radio.available()){
        while (radio.available()) {                                   // While there is data ready
          radio.read( &sent_time, sizeof(double) );             // Get the payload
        }                                           
      now = micros();
      count++;
      elapsed =  (now - begin_time);
      elapsed_sum += elapsed;
      Serial.print("Count: ");
      Serial.println(count);
      Serial.print("Sent time: ");        
      Serial.print(begin_time);     
      Serial.print(", received time: ");
      Serial.print(now);
      Serial.print(", Elapsed: ");
      Serial.println(elapsed);
      
      

//      delay(10);
      }
    
    if(count == average_len){
      double average = elapsed_sum / ( (double) average_len);
      Serial.print("Average of multiple times ");
      Serial.println(average);
      delay(5000);
      average = 0;
      elapsed_sum = 0;
      now = 0;
      count = 0;
    }
 }



