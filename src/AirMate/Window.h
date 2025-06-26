#ifndef WINDOW_H
#define WINDOW_H

enum State {
  Closed,
  Opening,
  Open,
  Closing,
  Paused,
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