+
+/*
+* Getting Started example sketch for nRF24L01+ radios
+* This is a very basic example of how to send data from one node to another
+* Updated: Dec 2014 by TMRh20
+*/
+
+#include <SPI.h>
+#include "RF24.h"
+
+/****************** User Config ***************************/
+/***      Set this radio as radio number 0 or 1         ***/
+bool radioNumber = 0;
+
+/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
+int x = 0;
+RF24 radio(7,8);
+/**********************************************************/
+
+byte addresses[][6] = {"1Node","2Node"};
+
+// Used to control whether this node is sending or receiving
+bool role = 1;
+
+void setup() {
+  Serial.begin(115200);
+  Serial.println(F("RF24/examples/GettingStarted"));
+  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
+  
+  radio.begin();
+
+  // Set the PA Level low to prevent power supply related issues since this is a
+ // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
+  radio.setPALevel(RF24_PA_LOW);
+  
+  // Open a writing and reading pipe on each radio, with opposite addresses
+  if(radioNumber){
+    radio.openWritingPipe(addresses[1]);
+    radio.openReadingPipe(1,addresses[0]);
+  }else{
+    radio.openWritingPipe(addresses[0]);
+    radio.openReadingPipe(1,addresses[1]);
+  }
+  
+  // Start the radio listening for data
+  radio.startListening();
+}
+
+void loop() {
+  
+  
+/****************** Ping Out Role ***************************/  
+if (role == 1)  {
+    
+    radio.stopListening();                                    // First, stop listening so we can talk.
+    
+    
+    Serial.println(F("Now sending"));
+
+    double start_time[1000] = micros();                             // Take the time, and send it.  This will block until complete
+     if (!radio.write( &start_time, sizeof(unsigned long) )){
+       Serial.println(F("failed"));
+     }
+        
+    radio.startListening();                                    // Now, continue listening
+    
+    double started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
+    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
+    
+    while ( ! radio.available() ){                             // While nothing is received
+      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
+          timeout = true;
+          break;
+      }      
+    }
+        
+    if ( timeout ){                                             // Describe the results
+        Serial.println(F("Failed, response timed out."));
+    }else{
+        double got_time[1000];                                 // Grab the response, compare, and send to debugging spew
+        radio.read( &got_time[n], sizeof(double) );
+        double end_time[1000] = micros();
+        
+//         Spew it
+      for (int n=0; n<1000; n++)
+      {
+        Serial.print(F("Sent "));
+        Serial.print(start_time[n]);
+        Serial.print(F(", Got response "));
+        Serial.print(got_time[n]);
+        Serial.print(F(", Round-trip delay "));
+        Serial.print((end_time[n]-start_time[n])/1000);
+        Serial.println(F(" milliseconds"));
+      }
+//        Serial.print(x);
+
+//        Serial.println((end_time-start_time)/1000);
+double average_round_trip;
+for (int n=0; n<1000; n++)
+  average_round_trip=average_round_trip+(end_time[n]-start_time[n])/1000;
+average_round_trip=average_round_trip/size(end_time);//an average is taken
+     
+    }
+Serial.println("Average Round Trip: %d", %average_round_trip);
+    // Try again 1s later
+    x = x + 1;
+    delay(500);
+  }
+
+
+
+/****************** Pong Back Role ***************************/
+
+  if ( role == 0 )
+  {
+    double got_time;
+    int n=0;
+    if( radio.available()){
+                                                                    // Variable for the received timestamp
+      while (radio.available() && n<1000) {                                   // While there is data ready
+        radio.read( &got_time[n], sizeof(double) );             // Get the payload
+        n++;
+      }
+     
+      radio.stopListening();                                        // First, stop listening so we can talk   
+      for (int n=0; n<1000; n++)
+        radio.write( &got_time[n], sizeof(double) );              // Send the final one back.      
+      radio.startListening();                                       // Now, resume listening so we catch the next packets.     
+      Serial.print(F("Sent response "));
+      for (n=0; n<1000; n++)
+        Serial.println(got_time[n]);  
+   }
+ }
+
+
+
+
+/****************** Change Roles via Serial Commands ***************************/
+
+//  if ( Serial.available() )
+//  {
+//    char c = toupper(Serial.read());
+//    if ( c == 'T' && role == 0 ){      
+//      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
+//      role = 1;                  // Become the primary transmitter (ping out)
+//    
+//   }else
+//    if ( c == 'R' && role == 1 ){
+//      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
+//       role = 0;                // Become the primary receiver (pong back)
+//       radio.startListening();
+//       
+//    }
+//  }
+
+
 } // Loop
