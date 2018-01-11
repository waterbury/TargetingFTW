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

//for simon says
int randomArray[100]; //Intentionally long to store up to 100 inputs (doubtful anyone will get this far)
int inputArray[100];


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
	randomSeed(analogRead(0));
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
	
	if ( pollButtons() )
		return S_CLOCK_LED_LOOP;
	
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
		return S_WHACK_A_MOLE;
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


//Red = Upper Left
//Green = upper right
//Yellow = Lower right
//Blue = Lower left


state fsm_test_parry(){
	return S_CLOCK_LED_LOOP; //temporary


	
	return S_SPEED_TEST_PARRY;
}




  

  
  

int pollButtons(){
	static int exit_flag = 0; //intent of user to exit current state
	
	for (j=0;j<13;j++){
		if (digitalRead(buttonArray[0][j]) == 0)
				buttonHeldTime[j]++;
    else        
				buttonHeldTime[j] = 0;
		
    }
	delay(10);
	
	if (buttonHeldTime[0] >= 3 && buttonHeldTime[1] >= 3)
		exit_flag = 1; //user wants to exit current state
	
	if (exit_flag) 
		if (buttonHeldTime[0] == 0 && buttonHeldTime[1] == 0){ //buttons no longer being held
			exit_flag = 0;
			return 1;
		}

	return 0;
}
//whackAmole();
  


state whackAmole(){
  randNumber = random(13);
  lightDLeds(120,120,120);
  lightSingleLed(randNumber,255,255,255);  
  flag = 0;
  
for (timerVariable =0;timerVariable<100000 && flag == 0;timerVariable++){
  
	if( pollButtons() )
		return S_CLOCK_LED_LOOP;
  
    for (j=0;j<13;j++){
    if (buttonHeldTime[j] > 0){
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

//--------------------------------------------------------------


// LED pin definitions
#define LED_RED     11
#define LED_GREEN   9
#define LED_BLUE    7
#define LED_YELLOW  5
#define LED_CORRECT 4
#define LED_WRONG   2
// Button pin definitions
#define BUTTON_RED    12
#define BUTTON_GREEN  10
#define BUTTON_BLUE   8
#define BUTTON_YELLOW 6
// Buzzer definitions
#define BUZZER  3
#define RED_TONE 220
#define GREEN_TONE 262
#define BLUE_TONE 330
#define YELLOW_TONE 392
#define TONE_DURATION 250
// Game Variables
int GAME_SPEED = 250;
int GAME_STATUS = 0;
int const GAME_MAX_SEQUENCE = 50;
int GAME_SEQUENCE[GAME_MAX_SEQUENCE];
int GAME_STEP = 0;
int READ_STEP = 0;



//We begin with the "reset game" function where we fill up our GAME_SEQUENCE array
void resetGame(){
  // reset steps
  READ_STEP = 0;
  GAME_STEP = 0;
  
  // create random sequence
  for(int i=0; i<GAME_MAX_SEQUENCE; i++){
    GAME_SEQUENCE[i] = random(4) + 1;
  }

  // Go to next game state; show led sequence
  GAME_STATUS = 1;
}

//Our next game step would be the playback step where we play the sequence
void playSequence(){
  // play a step of our sequence
  for(int i=0; i<=GAME_STEP; i++){
    Serial.print("Set LED");
    Serial.println(GAME_SEQUENCE[i]);
    delay(GAME_SPEED*2);
    setLED(GAME_SEQUENCE[i]);
    playTone(GAME_SEQUENCE[i]);
    delay(GAME_SPEED);
    clearLEDs();
    
  }
  // Go to next step: reading buttons
  GAME_STATUS = 2;
}

//After playing our sequence we wait for pushbutton input
void readSequence(){
  // read our buttons
  int button_value = readButtons();
  
  if(button_value > 0){
    // a button has been pressed
    if(button_value == GAME_SEQUENCE[READ_STEP]){
      // correct value!
      setLED(button_value);
      playTone(button_value);
      digitalWrite(LED_CORRECT, HIGH);
      delay(GAME_SPEED);
      clearLEDs();
      digitalWrite(LED_CORRECT, LOW);
      
      // Lets speed it up!
      GAME_SPEED = GAME_SPEED-15;
      
      Serial.println("Correct!");
      
      if(READ_STEP == GAME_STEP){
        // reset read step
        READ_STEP = 0;
        // Go to next game step
        GAME_STEP++;
        // Go to playback sequence mode of our game
        GAME_STATUS = 1;
        Serial.println("Go To Next Step");
        
        // Light all LEDs to indicate next sequence
        setLEDs(true,true,true,true);
        delay(GAME_SPEED);
        setLEDs(false,false,false,false);
        
        
      }else{
        READ_STEP++;
      }
      
      delay(10);
      
    }else{
      // wrong value!
      // Go to game over mode
      GAME_STATUS = 3;
      Serial.println("Game Over!");
    }
  }
  
  delay(10);
}

//Finally when you make a mistake we execute the "game over" modus:
void gameOver(){
  // Red RGB
  lightDLeds(255,0,0);//digitalWrite(LED_WRONG, HIGH);
  // Play Pwa Pwa Pwa
 // tone(BUZZER, 98, TONE_DURATION);
 // delay(TONE_DURATION);
 // tone(BUZZER, 93, TONE_DURATION);
 // delay(TONE_DURATION);
 // tone(BUZZER, 87, TONE_DURATION);
 // delay(TONE_DURATION);
 // delay(GAME_SPEED);
}

//We also have some helper functions that make our lives easier, they can be used anywhere in our code
void setLED(int id){
  switch(id){
    case 0:
      setLEDs(false,false,false,false);
      break;
    case 1:
      setLEDs(true,false,false,false);
      break;
    case 2:
      setLEDs(false,true,false,false);
      break;
    case 3:
      setLEDs(false,false,true,false);
      break;
    case 4:
      setLEDs(false,false,false,true);
      break; 
  }
}

void playTone(int id){
  switch(id){
    case 0:
      //noTone(BUZZER);
      break;
    case 1:
      //tone(BUZZER, RED_TONE, TONE_DURATION);
      break;
    case 2:
      //tone(BUZZER, GREEN_TONE, TONE_DURATION);
      break;
    case 3:
      //tone(BUZZER, BLUE_TONE, TONE_DURATION);
      break;
    case 4:
      //tone(BUZZER, YELLOW_TONE, TONE_DURATION);
      break; 
  }
}

void setLEDs(boolean red, boolean green, boolean blue, int yellow ){
  
	if (red)
		digitalWrite(LED_RED, HIGH);
	else
		digitalWrite(LED_RED, LOW);
	
	if (green)
		digitalWrite(LED_GREEN, HIGH);
	else 
		digitalWrite(LED_GREEN, LOW);
 
	if (blue)
		digitalWrite(LED_BLUE, HIGH);
	else
		digitalWrite(LED_BLUE, LOW);
	
	if (yellow) 
		digitalWrite(LED_YELLOW, HIGH);
	else 
		digitalWrite(LED_YELLOW, LOW);
}

void clearLEDs(){
  setLEDs(false,false,false,false);
}

int readButtons(void){
  if (digitalRead(BUTTON_RED) == 0)
	  return 1;
  else if (digitalRead(BUTTON_GREEN) == 0)
	  return 2;
  else if (digitalRead(BUTTON_BLUE) == 0)
	  return 3;
  else if (digitalRead(BUTTON_YELLOW) == 0)
	  return 4;
  
  return 0;
}

//Our loop function contains our main game loop; we use a switch case to quickly choose in which modus we're situated. This is called a "Super Loop" programming design. This allows us to easily have specific cases or "modes" for different parts of the game
state fsm_simon_says(){
	//return S_CLOCK_LED_LOOP; //temporary
	
  // In what mode are we?
  switch(GAME_STATUS){
    case 0:
      resetGame();
      break;
    case 1:
      playSequence();
      break;
    case 2:
      readSequence();
      break;
    case 3:
      gameOver();
      break;
  }
  	return S_SIMON_SAYS;
}