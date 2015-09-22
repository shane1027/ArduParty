/*************************************
 *  Shane Ryan
 *  
 *  Code to sample audio signal using
 *  msgeq07 and control RGB lights.
 *  
 *  v0.5 - added standby function 
 *  with fading random colors.
 *  
 *  Thank you to David Wang for code
 *  on audio sampling!
 *  
 *  Uses elapsedMillis library for
 *  timeouts and function changes.
 *************************************/


#include <elapsedMillis.h>

int analogPin = 0; // MSGEQ7 OUT
int strobePin = 2; // MSGEQ7 STROBE
int resetPin = 4; // MSGEQ7 RESET
int spectrumValue[7];

 
// MSGEQ7 OUT pin produces values around 50-80
// when there is no input, so use this value to
// filter out a lot of the chaff.
int filterValue = 200;
 
// LED pins connected to the PWM pins on the Arduino

int ledPinR = 9;
int ledPinG = 11;
int ledPinB = 10;

// Setup timer for entering standby mode.
elapsedMillis timer0;
#define interval 5000

// variable needed to determine state status.
int state = 0;

 
void setup()
{
  Serial.begin(9600);
  // Read from MSGEQ7 OUT
  pinMode(analogPin, INPUT);
  // Write to MSGEQ7 STROBE and RESET
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);

  // clear timer on setup
  timer0 = 0;
 
  // Set analogPin's reference voltage
  analogReference(DEFAULT); // 5V
 
  // Set startup values for pins
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
}
 
void loop()
{
  // Set reset pin low to enable strobe
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

 
  // Get all 7 spectrum values from the MSGEQ7
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // Allow output to settle
 
    spectrumValue[i] = analogRead(analogPin);
 
    // Constrain any value above 1023 or below filterValue
    spectrumValue[i] = constrain(spectrumValue[i], filterValue, 1023);
    

    // reset timer if music is playing
    if (spectrumValue[i] > filterValue) {
      timer0 = 0;   // reset timer if music is detected
      state = 0;   // reset state after calling fade during standby
    }

     
    // Remap the value to a number between 0 and 255
    spectrumValue[i] = map(spectrumValue[i], filterValue, 1023, 0, 255);

    Serial.print(spectrumValue[i]);

    // Remove serial stuff after debugging
    Serial.print(" ");
    digitalWrite(strobePin, HIGH);
   }    
 
   Serial.println();

   if (timer0 > interval || state == 1) {
    standby();
   }

   else {
    writeLEDs();
   }
}


// Write the PWM values to the LEDs
void writeLEDs()
{
   analogWrite(ledPinR, spectrumValue[1]); 
   analogWrite(ledPinG, spectrumValue[4]); 
   analogWrite(ledPinB, spectrumValue[6]); 
   delay(30);
}




// this function will run while there is no music playing
void standby()
{
  if (state = 0) {
    flash(1,3,100);
    flash(1,3,100);
    Serial.print("\n\nEntering Standby\n\n");
    delay(30);
  }
  
  flash(random(0,7),50,255);   // fade in and out of random colors
  state = 1;
}





// this function is used to flash the LEDs both as feedback for 
// which state the device is entering, and to fade in and out
// as a visually appealing standby mode.
void flash(int color, int duration, int brightness)
{
  analogWrite(ledPinG, 0);
  analogWrite(ledPinB, 0);
  analogWrite(ledPinR, 0);
  switch (color) {
    case 0:       
      analogWrite(ledPinR, 0);
      for (int fadeValue = 0; fadeValue <= brightness; fadeValue += 5){
        analogWrite(ledPinG, fadeValue);
        analogWrite(ledPinB, fadeValue);
        delay(duration);
      }
      analogWrite(ledPinR, 0);
      for (int fadeValue = brightness; fadeValue >= 0; fadeValue -= 5){
        analogWrite(ledPinG, fadeValue);
        analogWrite(ledPinB, fadeValue);
        delay(duration);
      }
      break;
      
    case 1:
      for (int fadeValue = 0; fadeValue <= brightness; fadeValue += 5){
        analogWrite(ledPinR, fadeValue);
        delay(duration);
      }
      for (int fadeValue = brightness; fadeValue >= 0; fadeValue -= 5){
        analogWrite(ledPinR, fadeValue);
        delay(duration);
      }
      break;

    case 2:
      for (int fadeValue = 0; fadeValue <= brightness; fadeValue += 5){
        analogWrite(ledPinG, fadeValue);
        delay(duration);
      }
      for (int fadeValue = brightness; fadeValue >= 0; fadeValue -= 5){
        analogWrite(ledPinG, fadeValue);
        delay(duration);
      }
      break;

    case 3:
      for (int fadeValue = 0; fadeValue <= brightness; fadeValue += 5){
        analogWrite(ledPinB, fadeValue);
        delay(duration);
      }
      for (int fadeValue = brightness; fadeValue >= 0; fadeValue -= 5){
        analogWrite(ledPinB, fadeValue);
        delay(duration);
      }
      break;

    case 4:        
      for (int fadeValue = 0; fadeValue <= brightness; fadeValue += 5){
        analogWrite(ledPinG, fadeValue/3);
        analogWrite(ledPinR, fadeValue);
        delay(duration);
      }
      analogWrite(ledPinR, 0);
      for (int fadeValue = brightness; fadeValue >= 0; fadeValue -= 5){
        analogWrite(ledPinR, fadeValue);
        analogWrite(ledPinG, fadeValue/3);
        delay(duration);
      }
      break;

    case 5:        
      for (int fadeValue = 0; fadeValue <= brightness; fadeValue += 5){
        analogWrite(ledPinR, fadeValue);
        analogWrite(ledPinB, fadeValue);
        delay(duration);
      }
      analogWrite(ledPinR, 0);
      for (int fadeValue = brightness; fadeValue >= 0; fadeValue -= 5){
        analogWrite(ledPinR, fadeValue);
        analogWrite(ledPinB, fadeValue);
        delay(duration);
      }
      break;

     case 6:        
      for (int fadeValue = 0; fadeValue <= brightness; fadeValue += 5){
        analogWrite(ledPinR, fadeValue);
        analogWrite(ledPinB, fadeValue);
        analogWrite(ledPinG, fadeValue);
        delay(duration);
      }
      analogWrite(ledPinR, 0);
      for (int fadeValue = brightness; fadeValue >= 0; fadeValue -= 5){
        analogWrite(ledPinR, fadeValue);
        analogWrite(ledPinB, fadeValue);
        analogWrite(ledPinG, fadeValue);
        delay(duration);
      }
      break;
      
      
    default:
      analogWrite(ledPinG, 0);
      analogWrite(ledPinB, 0);
      for (int fadeValue = 0; fadeValue <= brightness; fadeValue += 5){
        analogWrite(ledPinR, fadeValue);
        delay(duration);
      }
      for (int fadeValue = brightness; fadeValue >= 0; fadeValue -= 5){
        analogWrite(ledPinR, fadeValue);
        delay(duration);
      }
      break;
    }    
  timer0 = 0;
}












