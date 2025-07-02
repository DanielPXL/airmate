#ifndef WINDOW_H
#define WINDOW_H

enum class State {
  Closed,
  Opening,
  Open,
  Closing,
  Paused,
  Locking,
  Unlocking,
  Alarm
};

extern State g_state;
extern State g_lastDirection;

const char* window_getState();

void window_setup();
void window_buttonToggle();
void window_startOpening();
void window_startClosing();
void window_stopMotor();
void window_loop();

#endif