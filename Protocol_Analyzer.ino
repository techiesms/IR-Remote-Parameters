/*
This is the code provided by AnalysIR for recording Long IR signal of around 800 bits 
or more.So this code is written by AnalysIR and Modified by Sachin Soni(#techiesms).
Link of the original code by AnalysIR is 
http://www.analysir.com/blog/2014/03/19/air-conditioners-problems-recording-long-infrared-remote-control-signals-arduino/


In this code, you can record the signal as long as 800 bits.
AnalysIR has recorded the time duration in micrseconds and saved it in an array.
I have seperated this whole array into two parts i.e. High time and Low time. So that 
it becomes easy to plot a graph and analyse the protocol of the same.


Please press the button of any remote only once.

In AC remotes whenever we press the button, the whole data is sent only once while in other remote like TV,Radio etc.
they send the signal continuosly as long as we press the button.

So decoding the AC remote signal with this code is very much easy as it will stop recording as soon as the signal is completed.
But in case of other remotes,this code will continuosly record the signal even if the data for particular button is completely sent.
So in this case we have to look after the stop bit of the protocols. Every protocol has a start bit and a stop bit. So in case of
TV remote, we will consider only that much amount of data which is available above stop bit(Including stop bit).
Recognizing stop bit is also very much easy. Stop bit of any protocol is a long LOW signal which is very much long as compared 
to rest of the bits. Hence it easily gets highlighted in the serial monitor.

For more details regarding this project and lot more others, visit my blog http://techiesms.blogspot.com
#techiesms


Connections:
IR Receiver      Arduino
V+          ->  +5v
GND          ->  GND
Signal Out   ->  Digital Pin 2
(If using a 3V Arduino, you may connect V+ to +3V)

*/
#define LEDPIN 13
int maximum=500;  // you can extend the limit to around 800. But most of the remote's protocol will be covered in 500 bits.

unsigned int data[500],low[450],high[450]; 
unsigned int x = 0;
 
void setup() {
  Serial.begin(115200); 
  attachInterrupt(0, Rec_Interrupt, CHANGE);
  pinMode(LEDPIN,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println(F("Press the button once"));
  delay(5000); // pause 5 secs
  if (x) { //if a signal is captured
    digitalWrite(LEDPIN, HIGH);//visual indicator that signal received
    Serial.println();
    Serial.print(F("Raw: (")); //dump raw header format - for library
    Serial.print((x - 1));
    Serial.print(F(") "));
    detachInterrupt(0);//stop interrupts & capture until finshed here
    int j=0;
    for (int i = 1; i < x; i++) { //now dump the times
      if(i%2==0)
      {
        low[j]=data[i] - data[i - 1];
        j++;
        Serial.print("LOW ");
        Serial.println(data[i] - data[i - 1]);
      }

      else
      {
        high[j]=(data[i] - data[i - 1]);
        j++;
        Serial.print("HIGH ");
        Serial.println(data[i] - data[i - 1]);
      }
    }
     x = 0;
    Serial.println();
    Serial.println();
    digitalWrite(LEDPIN, LOW);//end of visual indicator, for this time
    attachInterrupt(0, Rec_Interrupt, CHANGE);//re-enable ISR for receiving IR signal
  }
}

void Rec_Interrupt() {
  if (x > maximum)return; //ignore if exceeds maximum
  data[x++] = micros(); //record

}
  
