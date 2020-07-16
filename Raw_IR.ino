/* Raw IR decoder sketch!
 This Code is public domain, contains code from LadyAda Tutorials
 */

#define IRpin_PIN      PIND
#define IRpin          2
 
#include <math.h>

#define MAXPULSE 65000

#define RESOLUTION 16 
 
// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[100][2];  // pair is high and low pulse 
uint8_t currentpulse = 0; // index for pulses we're storing
 
void setup(void) {
  Serial.begin(115200);
}
 
void loop(void) {
  uint16_t highpulse, lowpulse;  // temporary storage timing
  highpulse = lowpulse = 0; // start out with no pulse length
  
  while (IRpin_PIN & (1 << IRpin)) {
     // pin is still HIGH
     highpulse++;
     delayMicroseconds(RESOLUTION);
 
     if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
       printpulses();
       currentpulse=0;
       return;
     }
  }
  pulses[currentpulse][0] = highpulse;
  
  // same as above
  while (! (IRpin_PIN & _BV(IRpin))) {
     // pin is still LOW
     lowpulse++;
     delayMicroseconds(RESOLUTION);
     if ((lowpulse >= MAXPULSE)  && (currentpulse != 0)) {
       printpulses();
       currentpulse=0;
       return;
     }
  }
  pulses[currentpulse][1] = lowpulse;
  currentpulse++;
}
 
void printpulses(void) {
  int outputval;
  Serial.println("Recieved Signal");
  for (uint8_t i = 0; i < currentpulse-1; i++) {
    // Print Lows
    outputval = 10 * round(pulses[i][1] / 10.0) / 50;
    for(int i=0; i<outputval; i++)
    {
      Serial.print("1");
    }
    // Print Highs
    outputval = 10 * round(pulses[i+1][0] / 10.0) / 50;
    if (outputval > 7){
      Serial.print("0000");
      Serial.println("");
    }
    else {
      for(int i=0; i<outputval; i++)
      {
        Serial.print("0");
      }
    }
  }
  // Print ending Low's
  outputval = 10 * round(pulses[currentpulse-1][1] / 10.0) / 50;
  for(int i=0; i<outputval; i++)
    {
      Serial.print("1");
    }
  Serial.print("0000");
  Serial.println("");

}
