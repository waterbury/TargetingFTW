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

// Patrick's grand design for the state machine with typedefs and enums and shit
typedef enum state
{
  S_INIT,
  S_CLOCK_LED_LOOP,
  S_MAIN_MENU,
  S_PREGAME_COUNTDOWN,
  S_GAME_OVER,
  S_SPEED_TEST_COUNTDOWN,
  S_SPEED_TEST_COUNTUP,
  S_ENDURO_TEST,
  S_LIGHT_ORDER_TEST,
  S_SIMON_SAYS,
  S_SPEED_TEST_PARRY
} State;

State fsm;

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
  // End of trinket special code
  randomSeed(analogRead(6));
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.

  for (j=0;j<13;j++){
  pinMode(buttonArray[0][j], INPUT);
  digitalWrite(buttonArray[0][j], HIGH);
  }
}


void loop()
{
  switch (fsm)
  {
    case S_INIT:
      fsm = fsm_init();
      break;
    case S_CLOCK_LED_LOOP;
      fsm = fsm_clock_led_loop();
      break;
    case S_MAIN_MENU:
      fsm = fsm_main_menu();
      break;
    case S_PREGAME_COUNTDOWN:
      fsm = fsm_pregame_countdown();
      break;
    case S_GAME_OVER:
      fsm = fsm_game_over();
    break;
  case S_SPEED_TEST_COUNTDOWN:
    fsm = fsm_speed_test_countdown();
    break;
  case S_SPEED_TEST_COUNTUP:
    fsm = fsm_speed_test_countup();
    break;
  case S_ENDURO_TEST:
    fsm = fsm_enduro_test();
    break;
  case S_LIGHT_ORDER_TEST:
    fsm = fsm_light_order_test();
    break;
  case S_SIMON_SAYS:
    fsm = fsm_simon_says();
    break;
  case S_SPEED_TEST_PARRY:
    fsm = fsm_test_parry();
    break;
  default:
    fsm = S_INIT;
    break;
  }
}

  

  
  

	
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



