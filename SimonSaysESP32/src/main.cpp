#include <Arduino.h>



#include "SimonGame.h"

SimonGame game;

void setup() {
  Serial.begin(115200);
  game.begin();
  game.showIntro();
}

void loop() {
  game.update();  // se encarga del estado del juego
  
}
