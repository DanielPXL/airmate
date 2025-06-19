#include <Stepper.h>
#include "Pins.h"

#define MOTOR_RPM 10
#define STEPS 1024

enum State {
  Closed,
  Opening,
  Open,
  Closing,
  Paused,
};

State g_state = State::Closed;
State g_lastDirection = State::Closed; // Nur Opening oder Closing

// Die beiden Zeilen blockieren aus irgendeinem Grund den restlichen Code...
// Erst unkommentieren, wenns gebraucht wird
// Stepper g_lockStepper(STEPS, LOCKMOTOR_PINS); // Motor Pins
Stepper g_gearStepper(STEPS, GEARMOTOR_PINS); // Motor Pins

void window_setup()
{
  g_gearStepper.setSpeed(MOTOR_RPM);
}

void window_ButtonToggle() {
  switch (g_state) {
    case Closed:
      window_startOpening();
      break;
    case Opening:
      g_state = Paused;
      break;
    case Closing:
      g_state = Paused;
      break;
    case Paused:
      if (g_lastDirection == Opening) {
        g_state = Closing;
      } else {
        g_state = Opening;
      }
      break;
    case Open:
      window_startClosing();
      break;
  }
}

void window_startOpening() {
  if (g_state == State::Closed || g_state == State::Paused) {
    g_state = State::Opening;
    g_lastDirection = State::Opening;
  }
}

void window_startClosing() {
  if (g_state == State::Open || g_state == State::Paused) {
    g_state = State::Closing;
    g_lastDirection = State::Closing;
  }
}

void window_loop() {
  switch (g_state) {
    case Closed: {
      // TODO: Motor aus, wenn Fenster zu --> Reedsensor
      break;
    }
    case Opening: {
      // TODO: Motor öffnet Fenster --> Überwachen, wie viele Steps, um zu wissen, wann komplett offen
      break;
    }
    case Open: {
      // TODO: Motor aus, wenn Fenster offen
      break;
    }
    case Closing: {
      // TODO: Motor schließt Fenster --> Überwachen, wie viel Steps, falls gestoppt wird. Zu wenn Reedsensor
      break;
    }
    case Paused: {
      // TODO: Motor stoppen
      break;
    }
  }
}


