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
  S_SPEED_TEST_PARRY,
  S_WHACK_A_MOLE
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
  LED_C1,
  LED_C2,
  LED_B1,
  LED_B8,
  LED_B2,
  LED_B7,
  LED_A,
  LED_B3,
  LED_B6,
  LED_B4,
  LED_B5,
  LED_C4,
  LED_C3
}};


uint32_t buttonHeldTime[13];

int D_led_array[6] = {LED_D1, LED_D2, LED_D3, LED_D4, LED_D5, LED_D6};

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

int delayval = 1000; // delay for half a second
int currentLed = 0;
int j = 0;
long randNumber;
uint32_t timerVariable = 0;
int flag = 0;

void setup() {

	fsm = S_INIT;
}


void loop()
{
  switch (fsm)
  {
    case S_INIT:
      fsm = fsm_init();
      break;
    case S_CLOCK_LED_LOOP:
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
	case S_WHACK_A_MOLE:
		fsm = whackAmole();
		break;

  default:
    fsm = S_INIT;
    break;
  }
}

state fsm_init(){
	int i = 0;
	
	//randomSeed(analogRead(6));
	Serial.begin(9600);
	pixels.begin(); // This initializes the NeoPixel library.

	for (i=0;i<13;i++){
		pinMode(buttonArray[0][i], INPUT);
		digitalWrite(buttonArray[0][i], HIGH);
	}	

	return S_CLOCK_LED_LOOP;
	
}

state fsm_clock_led_loop(){
	static uint32_t i = 0;
	pollButtons();
	
	if (i > 7)
		i = 0;
	
	//2,4,7,9,10,8,5,3,
	switch (i)
	{
	  case 0:
		lightSingleLed(2,255,255,255);
		break;
	  case 1:
		lightSingleLed(4,255,255,255);
		break;
	  case 2:
		lightSingleLed(7,255,255,255);
		break;
	  case 3:
		lightSingleLed(9,255,255,255);
		break;
	  case 4:
		lightSingleLed(10,255,255,255);
		break;
	  case 5:
		lightSingleLed(8,255,255,255);
		break;
	  case 6:
		lightSingleLed(5,255,255,255);
		break;
	  case 7:
		lightSingleLed(3,255,255,255);
		break;		
	}
	i++;
  delay(100);
	
	//after holding C1 and C2 for some time
	if (buttonHeldTime[0] >= 3 && buttonHeldTime[1] >= 3){ 
		while (buttonHeldTime[0] != 0 || buttonHeldTime[1] != 0) //wait until user releases both buttons
			pollButtons();
		
		
		return	S_MAIN_MENU;
	}
		
	
	return S_CLOCK_LED_LOOP;
}

state fsm_main_menu(){
	uint32_t i = 0;
	uint32_t intensity = 0;
	int targetMode = 0;
	
	pollButtons();
	
	for (i = 2; i <= 10; i++){
		if (buttonHeldTime[i]){
			intensity = buttonHeldTime[i] * 5;
			if (intensity > 255)
				intensity = 255;
			
			
			pixels.setPixelColor(buttonArray[1][i], 0, intensity, 0);
			
			if (intensity == 255)
				targetMode = i;
			
		}
		
		else
			pixels.setPixelColor(buttonArray[1][i], 255, 255, 255);
		
		
		
	}
	pixels.show();
	
	
	if (targetMode){
		switch (targetMode){	
	 case 2:
		return S_SPEED_TEST_COUNTDOWN; 
		break;
	 case 3:
		//return S_FOOTWORK; 
		break;
	 case 4:
		return S_SPEED_TEST_COUNTUP;
		break;
	 case 5:
		return S_SIMON_SAYS;
		break;
	 case 6:
		
		break;
	 case 7:
		return S_ENDURO_TEST;
		break;
	 case 8:
		//return S_MULTI_CHOICE;
		break;
	 case 9:
		return S_LIGHT_ORDER_TEST;
		break;
	 case 10:
		//return S_RIGHTWRONG;
		break;	
	case 11:
		return S_SPEED_TEST_PARRY;
		break;		
				
		}
	}
	
	
	
	
	
	
	
	return S_MAIN_MENU;	
}

state fsm_pregame_countdown(){
	return S_CLOCK_LED_LOOP; //temporary

	return S_PREGAME_COUNTDOWN;
}

state fsm_game_over(){
	return S_CLOCK_LED_LOOP; //temporary


	return S_GAME_OVER;
}

state fsm_speed_test_countdown(){
	return S_CLOCK_LED_LOOP; //temporary	
	
	
	return S_SPEED_TEST_COUNTDOWN;
}

state fsm_speed_test_countup(){
	return S_CLOCK_LED_LOOP; //temporary

	
	return S_SPEED_TEST_COUNTUP;
}

state fsm_enduro_test(){
	return S_CLOCK_LED_LOOP; //temporary
	
	
	return S_ENDURO_TEST;
}

state fsm_light_order_test(){
	return S_CLOCK_LED_LOOP; //temporary

	
	
	return S_LIGHT_ORDER_TEST;
}

state fsm_simon_says(){
	return S_CLOCK_LED_LOOP; //temporary


	
	return S_SIMON_SAYS;
}

state fsm_test_parry(){
	return S_CLOCK_LED_LOOP; //temporary


	
	return S_SPEED_TEST_PARRY;
}




  

  
  

void pollButtons(){
	
	for (j=0;j<13;j++){
		if (digitalRead(buttonArray[0][j]) == 0)
				buttonHeldTime[j]++;
    else        
				buttonHeldTime[j] = 0;
		
    }
	delay(10);

}
//whackAmole();
  


state whackAmole(){
  randNumber = random(13);
  lightDLeds(120,120,120);
  lightSingleLed(randNumber,255,255,255);  
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
  lightDLeds(0,200,0);    
else
   lightDLeds(200,0,0);

delay(500);

return S_WHACK_A_MOLE;
}

void lightSingleLed(int ledNumber, int R_val, int G_val, int B_val){
int i =0;

   for(int i=0;i<13;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255

    if (i == ledNumber){
     pixels.setPixelColor(buttonArray[1][i], R_val, G_val, B_val);
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
 pixels.setPixelColor(D_led_array[i], R_val, G_val, B_val);
  
}
pixels.show();

}



