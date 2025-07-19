#include "SimonGame.h"
#include <FastLED.h>



#define LED_COUNT 4

#define BUZZER_PIN 4  // si tenés un zumbador

// Notas de la octava 4
#define NOTE_C4   262
#define NOTE_CS4  277  // C#4
#define NOTE_DF4  277  // Db4
#define NOTE_D4   294
#define NOTE_DS4  311  // D#4
#define NOTE_EB4  311  // Eb4
#define NOTE_E4   330
#define NOTE_F4   349
#define NOTE_FS4  370  // F#4
#define NOTE_GF4  370  // Gb4
#define NOTE_G4   392
#define NOTE_GS4  415  // G#4
#define NOTE_AB4  415  // Ab4
#define NOTE_A4   440
#define NOTE_AS4  466  // A#4
#define NOTE_BB4  466  // Bb4
#define NOTE_B4   494

// Octava 5 (una más aguda)
#define NOTE_C5   523
#define NOTE_CS5  554
#define NOTE_D5   587
#define NOTE_DS5  622
#define NOTE_E5   659
#define NOTE_F5   698
#define NOTE_FS5  740
#define NOTE_G5   784
#define NOTE_GS5  831
#define NOTE_A5   880
#define NOTE_AS5  932
#define NOTE_B5   988

#define NOTE_REST 0
const uint8_t ledPins[LED_COUNT] = {23, 22, 21, 19};  // ajustá a tus pines
const uint8_t btnPins[LED_COUNT] = {14, 12, 25, 26};
const uint LedTones[LED_COUNT] = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_B4};
void SimonGame::begin() {
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(btnPins[i], INPUT_PULLUP);
  }
  level = 0;
  state = IDLE;
}

void SimonGame::update()
{
    switch (state)
    {
        case IDLE:
            delay(500);  
            generateSequence();  
               
            state = WAITINIT;
            break;
        case WAITINIT:
            LedBlinking(); 
            WaitButtonPresset();            
            break;
        case SHOW_SEQUENCE:
            playSequence();
            state = USER_INPUT;
            break;

        case USER_INPUT:
            getUserInput();
            checkInput();
            break;

        case GAME_OVER:
            gameOverSequence();
            
            break;
    }
}

void SimonGame::generateSequence() {
  sequence[level] = random(LED_COUNT);
  level++;  
  Serial.print("Level:");Serial.println(level);
}

void SimonGame::playSequence() {
  for (uint8_t i = 0; i < level; i++) {
    digitalWrite(ledPins[sequence[i]], HIGH);
    tone(BUZZER_PIN, LedTones[sequence[i]], 200);
    delay(200);
    digitalWrite(ledPins[sequence[i]], LOW);
    delay(200);
  }
  Serial.print("Play sequence");Serial.println(level);
}


void SimonGame::LedBlinking(){

    EVERY_N_MILLISECONDS (300){
        digitalWrite(ledPins[sequence[0]], BitToggle); 
        BitToggle = !BitToggle;               
    }

}

void SimonGame::WaitButtonPresset(){
    
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        if (digitalRead(btnPins[i]) == LOW) {
            while (digitalRead(btnPins[i]) == LOW); // esperar que suelte
            delay(200);
            digitalWrite(ledPins[sequence[0]], LOW);
            delay(200);
            state = SHOW_SEQUENCE;
            return;
        }
    }
}

void SimonGame::getUserInput() {
  for (uint8_t i = 0; i < LED_COUNT; i++) {
   
    if (digitalRead(btnPins[i]) == LOW) {
        ButtonPressed = true;
        userInput[currentStep] = i;
        digitalWrite(ledPins[i],HIGH);
        tone(BUZZER_PIN, LedTones[i], 100);
        delay(250);
        Serial.print("Input:");Serial.println(i);
        currentStep++;
        //Serial.print("Get Input step:");Serial.println(currentStep);
        while (digitalRead(btnPins[i]) == LOW); // esperar que suelte
        delay(100);
        digitalWrite(ledPins[i],LOW);
        break;
    }
  }
}


void SimonGame::checkInput()
{     
    if (ButtonPressed)
    {
        for (uint8_t i = 0; i < currentStep; i++)
        {
            // Serial.print("CurrentStep:");
            // Serial.println(currentStep);
            // Serial.print("secuencia:");
            // Serial.println(sequence[i]);
            // Serial.print("Boton:");
            Serial.println(userInput[i]);
            if (userInput[i] != sequence[i])
            {
                state = GAME_OVER;
                Serial.print("GAME_OVER:");
                Serial.println(currentStep);
                return;
            }        
        }
         
        if(currentStep == level){
            delay(500);
            generateSequence();
            state = SHOW_SEQUENCE;
            currentStep = 0;
            Serial.print("SHOW_SEQUENCE");
                
        }        
        ButtonPressed = false;
    }
}

void SimonGame::gameOverSequence(){
    int melody[] = {
    NOTE_B4, NOTE_AS4, NOTE_A4, NOTE_GS4, NOTE_G4, NOTE_FS4,
    NOTE_F4, NOTE_E4, NOTE_DS4, NOTE_D4, NOTE_CS4, NOTE_C4
    };


    EVERY_N_MILLISECONDS(80){   
              
        switch (GameOverSeqState) {            
            case 0:

                digitalWrite(ledPins[0],LOW);
                digitalWrite(ledPins[1],LOW);
                digitalWrite(ledPins[2],LOW);
                digitalWrite(ledPins[3],LOW);
                tone(BUZZER_PIN, NOTE_C4/2, 80); 
                GameOverSeqState = 1;  
                if (GameOverAnimate > 8)
                {
                    Serial.print("CurrentStep:");
                    Serial.println(currentStep);
                    GameOverSeqState = 2;
                    Serial.println("Paso 1"); 
                }   
                           
                break;
            case 1:
                digitalWrite(ledPins[0],HIGH);
                digitalWrite(ledPins[1],HIGH);
                digitalWrite(ledPins[2],HIGH);
                digitalWrite(ledPins[3],HIGH);
                noTone(BUZZER_PIN); 
                GameOverSeqState = 0; 
                Serial.println("Paso 2");                 
                break;
            case 2:
                noTone(BUZZER_PIN); 
                GameOverSeqState = 3;
                GameOverAnimate = 0;
                Serial.println("Paso 3"); 
                break;  
            case 3:
                tone(BUZZER_PIN, melody[GameOverAnimate], 70);
                
                if (GameOverAnimate > 11)
                {
                    GameOverAnimate =0;
                    state = IDLE;
                    GameOverSeqState = 0;
                    level = 0;
                    currentStep = 0;
                    Serial.println("salir");
                }
            break;
        }  
        GameOverAnimate++; 
        
        Serial.println(GameOverSeqState);
        Serial.println(GameOverAnimate);
    }      


}


void SimonGame::showIntro() {
  Serial.println("=== Bienvenido al juego SIMON DICE ===");
  Serial.println("Presiona los botones en el orden correcto...");
  Serial.println("Preparando el juego...");

  // Efecto de LEDs secuencial
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < LED_COUNT; j++) {
      digitalWrite(ledPins[j], HIGH);      
      delay(100);
      digitalWrite(ledPins[j], LOW);
    }
  }
    int melody[] = {
    NOTE_C5, NOTE_GS4, NOTE_CS5, NOTE_GS4, NOTE_DS5, NOTE_REST
  };
  int durations[] = {
    200, 100, 200, 100, 350, 150
  };

  for (int i = 0; i < 6; i++) {
    if (melody[i] != NOTE_REST) {
      tone(BUZZER_PIN, melody[i], durations[i]);
    }
    delay(durations[i] + 20);
    noTone(BUZZER_PIN);
  }
  // Sonido de inicio (si tenés buzzer)
//   pinMode(BUZZER_PIN, OUTPUT);
//   tone(BUZZER_PIN, NOTE_C5, 200); delay(300);
//   tone(BUZZER_PIN, NOTE_GS4, 100); delay(150);
//   tone(BUZZER_PIN, NOTE_CS5, 200); delay(300);
//   tone(BUZZER_PIN, NOTE_GS4, 100); delay(150);
//   tone(BUZZER_PIN, NOTE_DS5, 400); delay(500);
 
//   noTone(BUZZER_PIN);
}

