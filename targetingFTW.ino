// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            7

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      19

// 3,4,5,6,9,10,11 A0, A1, A2, A3, A4, A5 

#define BUTTON_A    9
#define LED_A       17

#define BUTTON_B1   A5
#define LED_B1       8

#define BUTTON_B2   A2
#define LED_B2       3

#define BUTTON_B3   A4
#define LED_B3       2

#define BUTTON_B4   6
#define LED_B4       1

#define BUTTON_B5   A1
#define LED_B5       18

#define BUTTON_B6   4
#define LED_B6       16

#define BUTTON_B7   3
#define LED_B7       14

#define BUTTON_B8   10
#define LED_B8       13

#define BUTTON_C1   11
#define LED_C1       12

#define BUTTON_C2   A3
#define LED_C2       4

#define BUTTON_C3   A0
#define LED_C3       0

#define BUTTON_C4   5
#define LED_C4       15

#define LED_D1       11
#define LED_D2       10
#define LED_D3       9
#define LED_D4       7
#define LED_D5       6
#define LED_D6       5

#define tempButton A5

int buttonArray [2][13] = {{
  BUTTON_C1,
  BUTTON_C2,
  BUTTON_B1,
  BUTTON_B8,
  BUTTON_B2,
  BUTTON_B7,
  BUTTON_A,
  BUTTON_B3,
  BUTTON_B6,
  BUTTON_B4,
  BUTTON_B5,
  BUTTON_C4,
  BUTTON_C3
},
{
  12,
  4,
  8,
  13,
  3,
  14,
  17,
  2,
  16,
  1,
  18,
  15,
  0
}};


uint32_t buttonHeldTime[13];

int D_led_array[6] = {LED_D1, LED_D2, LED_D3, LED_D4, LED_D5, LED_D6};

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 1000; // delay for half a second
int currentLed = 0;
int j = 0;
long randNumber;
uint32_t timerVariable = 0;
int flag = 0;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  randomSeed(analogRead(6));
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.

  for (j=0;j<13;j++){
  pinMode(buttonArray[0][j], INPUT);
  digitalWrite(buttonArray[0][j], HIGH);
  }
}


void loop() {
	
	for (j=0;j<13;j++){
		if (digitalRead(buttonArray[0][j]) == 0){
			if (j == randNumber) 
				buttonHeldTime[j]++;
			else 
				buttonHeldTime[j] = 0;
		}
    }
	delay(100);

whackAmole();
  
}

void whackAmole(){
  randNumber = random(13);
  lightDLeds(255,255,255);
  lightSingleLed(randNumber);  
  flag = 0;
  
for (timerVariable =0;timerVariable<100000 && flag == 0;timerVariable++){
  
    for (j=0;j<13;j++){
    if (digitalRead(buttonArray[0][j]) == 0){
     if (j == randNumber) 
      flag = 1;
     else 
       timerVariable = 1000000000 ;
    }
    }
}

if (flag)
  lightDLeds(0,255,0);    
else
   lightDLeds(255,0,0);

delay(500);
}

void lightSingleLed(int ledNumber){
int i =0;

   for(int i=0;i<13;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255

    if (i == ledNumber){
     pixels.setPixelColor(buttonArray[1][i], 0, 255, 0);
    }
    else{
     pixels.setPixelColor(buttonArray[1][i], 0, 0, 0);
    }
    pixels.show(); // This sends the updated pixel color to the hardware.
    //lightDLeds(255,0,0);
   

   }
   Serial.println(ledNumber);

}

void lightDLeds(int R_val, int G_val, int B_val){
int i = 0;

for (i = 0 ; i<6; i++){
 pixels.setPixelColor(D_led_array[i], G_val, R_val, B_val);
  
}
pixels.show();

}



