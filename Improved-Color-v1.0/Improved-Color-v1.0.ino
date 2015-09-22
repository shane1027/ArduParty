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
int filterValue = 150;

// LED pins connected to the PWM pins on the Arduino

int ledPinR = 9;
int ledPinG = 11;
int ledPinB = 10;

// Setup timer for entering standby mode.
elapsedMillis timer0;
elapsedMillis timer1;
#define interval_1 10000
#define interval_2 180000


// variable needed to determine state status.
int state = 0;

// Now, to deal with manual color display.
// Palette is an array of structs to store colors values.
// defining symbolic constants eases manipulation later.

#define teal 0
#define blue 1
#define red 2
#define green 3
#define lime 4
#define white 5
#define pink 6

typedef struct
{
  int R;
  int G;
  int B;
} RGB;

RGB palette[20] = {
  {0, 1, 1},
  {0, 0, 1},
  {1, 0, 0},
  {0, 1, 0},
  {1, 1 / 3, 0},
  {1, 1, 1},
  {1, 1, 0},
  {1, 1 / 3, 1 / 3}
};



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

    //Serial.print(spectrumValue[i]);

    // Constrain any value above 1023 or below filterValue
    spectrumValue[i] = constrain(spectrumValue[i], filterValue, 1023);


    // reset timer if music is playing
    if (spectrumValue[i] > filterValue+80) {
      timer0 = 0;   // reset timer if music is detected
      state = 0;   // reset state after calling fade during standby
    }

    
    // Remap the value to a number between 0 and 255
    spectrumValue[i] = map(spectrumValue[i], filterValue, 1023, 0, 255);

    

    // Remove serial stuff after debugging
    //Serial.print(" ");
    digitalWrite(strobePin, HIGH);
  }

  //Serial.println();

  if (timer0 > interval_1 || state == 1) {
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
  if (state == 0) {
    //Serial.print("\n\nEntering Standby\n\n");
    fade(teal, 3);
    fade(teal, 3);
    delay(40);
  }

  fade(random(0, 8), random(25, 50)); // fade in and out of random color presets.
  state = 1;
}





// this function is used to flash the LEDs both as feedback for
// which state the device is entering, and to fade in and out
// as a visually appealing standby mode.
void fade(int color, int duration)
{
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    analogWrite(ledPinR, fadeValue * palette[color].R);
    analogWrite(ledPinG, fadeValue * palette[color].G);
    analogWrite(ledPinB, fadeValue * palette[color].B);
    delay(duration);
  }
  delay(pow(duration, 2));
  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(ledPinR, fadeValue * palette[color].R);
    analogWrite(ledPinG, fadeValue * palette[color].G);
    analogWrite(ledPinB, fadeValue * palette[color].B);
    delay(duration);
  }
  timer0 = 0;
}












