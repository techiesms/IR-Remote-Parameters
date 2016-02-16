
/*
This Arduino code will let you to measure some of the parameters of IR remote like Period, Frequency, High time and Low time.
This information is very much useful while doing reverse engineering of any IR remote. 

For measuring this quatities, we require a DSO, But everyone may not have one at there home, so they can use this Arduino code 
to measure all this parameters.

This code can be regarded as a Trial and Error method.Sometimes the readings capture by photodiode are not same as we have expected.
So I request you to carry recording 5-7 times to get accurate result.

When I was experimenting with different Remotes, I found that most of the remotes have below mentioned parameters only and they are fixed,

High Time = 12 microseconds
Low Time  = 16 microseconds
Period    = 28 microseconds
Frequency = 35.7kHz

For more details regarding this project and lot more others, visit my blog http://www.techiesms.blogspot.com
#techiesms

*/


#define IR_PIN 2  // Connect Photodiode to Pin 2.

unsigned int  now, start, capture[3], i, add = 0;
boolean flag_complete;



void setup()
{
  flag_complete = true;
  start = 0;
  now = 0;
  i = 0;
  
  Serial.begin(9600);  //Baud_Rate.
  delay(500);  // wait
  Serial.println("Press the button");
  attachInterrupt(0, Rec_Interrupt, CHANGE);  // Call the function, whenever
                                             // change in pulse is detected.
  
}

void loop()
{
  while (1) {
    if (flag_complete == 0) {
      for (i = 1; i < 3; i++) {
        if (i % 2 == 0)
        {
          Serial.print("LOW ");
          Serial.print(capture[i]);
          Serial.println(" microseconds");
        }

        else
        {
          Serial.print("HIGH ");
          Serial.print(capture[i]);
          Serial.println(" microseconds");
        }

        flag_complete = true;
      }
      for (int x = 1; x < 3; x++)
      {
        add += capture[x];  // Adding High Time and Low Time.
      }

      Serial.print("Period ");
      Serial.println(add);
      Serial.print("Frequency ");
      Serial.print((float)1000 / add, 5);
      Serial.println(" kHz");
      Serial.println(" ");
    }

    add = 0;  // clear 
  }
}


void Rec_Interrupt() {
  now = micros();                  // Capture current time.
  capture[i++] = now - start;      // Subtract Current time and Previous time.
  start = now;                     // Previous time is equal to current time.
  if (i >= 3)                      // If count is equal to or greater than 3
  {
    detachInterrupt(0);            // disable Interrupt 0
    flag_complete = false;         // clear flag.
  }
}

