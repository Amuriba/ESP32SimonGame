#pragma once
#include <Arduino.h>

class SimonGame { // clase pcon el construcctor de Juego
public:
  void begin();
  void update();
  void showIntro();
private:
  void generateSequence();
  void playSequence();
  void getUserInput();
  void checkInput();
  void gameOverSequence();
  void WaitButtonPresset();
  void LedBlinking();
  uint8_t sequence[100];
  uint8_t userInput[100];
  uint8_t currentStep;
  uint8_t level;
  uint8_t ledindexGameOver;
  uint8_t GameOverSeqState;
  uint8_t GameOverAnimate;
  bool ButtonPressed;
  bool BitToggle;

  enum GameState { IDLE, WAITINIT, SHOW_SEQUENCE, USER_INPUT, GAME_OVER } state;
};