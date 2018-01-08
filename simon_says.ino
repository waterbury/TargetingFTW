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
We then define our setup function where we set our Arduino Pin Modes to OUTPUT for our LEDs and to INPUT_PULLUP for our pushbuttons. We also use a randomSeed to make sure our randmon sequence we're creating is different every time. Read more about randomSeed and pseudo random numbergenerators here: Arduino.cc/RandmomSeed

void setup(){
  Serial.begin(9600);
  
  randomSeed(analogRead(0));
  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_CORRECT, OUTPUT);
  pinMode(LED_WRONG, OUTPUT);
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
}
Our loop function contains our main game loop; we use a switch case to quickly choose in which modus we're situated. This is called a "Super Loop" programming design. This allows us to easily have specific cases or "modes" for different parts of the game

void loop(){
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
}
We begin with the "reset game" function where we fill up our GAME_SEQUENCE array

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

Our next game step would be the playback step where we play the sequence

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
After playing our sequence we wait for pushbutton input

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
Finally when you make a mistake we execute the "game over" modus:

void gameOver(){
  // Red RGB
  digitalWrite(LED_WRONG, HIGH);
  // Play Pwa Pwa Pwa
  tone(BUZZER, 98, TONE_DURATION);
  delay(TONE_DURATION);
  tone(BUZZER, 93, TONE_DURATION);
  delay(TONE_DURATION);
  tone(BUZZER, 87, TONE_DURATION);
  delay(TONE_DURATION);
  delay(GAME_SPEED);
}
We also have some helper functions that make our lives easier, they can be used anywhere in our code

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
      noTone(BUZZER);
      break;
    case 1:
      tone(BUZZER, RED_TONE, TONE_DURATION);
      break;
    case 2:
      tone(BUZZER, GREEN_TONE, TONE_DURATION);
      break;
    case 3:
      tone(BUZZER, BLUE_TONE, TONE_DURATION);
      break;
    case 4:
      tone(BUZZER, YELLOW_TONE, TONE_DURATION);
      break; 
  }
}
void setLEDs(boolean red, boolean green, boolean blue, int yellow ){
  if (red) digitalWrite(LED_RED, HIGH);
  else digitalWrite(LED_RED, LOW);
  if (green) digitalWrite(LED_GREEN, HIGH);
  else digitalWrite(LED_GREEN, LOW);
  if (blue) digitalWrite(LED_BLUE, HIGH);
  else digitalWrite(LED_BLUE, LOW);
  if (yellow) digitalWrite(LED_YELLOW, HIGH);
  else digitalWrite(LED_YELLOW, LOW);
}
void clearLEDs(){
  setLEDs(false,false,false,false);
}
int readButtons(void){
  if (digitalRead(BUTTON_RED) == 0) return 1;
  else if (digitalRead(BUTTON_GREEN) == 0) return 2;
  else if (digitalRead(BUTTON_BLUE) == 0) return 3;
  else if (digitalRead(BUTTON_YELLOW) == 0) return 4;
  
  return 0;
}