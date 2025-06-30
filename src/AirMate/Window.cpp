#include <AccelStepper.h> // benötigt AccelStepper von Mike McCauley
#include "Pins.h"
#include "Window.h"

#define STEPSPERSECOND 100 // TODO: Muss wahrscheinlich noch angepasst werden die Steps!!!

State g_state = State::Closed;
State g_lastDirection = State::Closing; // Nur Opening oder Closing

// Die beiden Zeilen blockieren aus irgendeinem Grund den restlichen Code...
// Erst unkommentieren, wenns gebraucht wird
AccelStepper g_lockStepper(AccelStepper::FULL4WIRE, LOCKMOTOR_PINS); // Motor Pins
AccelStepper g_gearStepper(AccelStepper::DRIVER, GEARMOTOR_PINS); // Motor Pins

const char* window_getState() {
  switch (g_state) {
    case State::Closed:
      return "closed";
    case State::Opening:
      return "opening";
    case State::Open:
      return "open";
    case State::Closing:
      return "closing";
  }
}

void window_setup() {
  g_gearStepper.setMaxSpeed(STEPSPERSECOND);
}

// Fenstersteuerung mit Button toggled durch
// (TODO:) Sensor- und Wetterdaten setzen entsprechend g_state
void window_buttonToggle() {
  switch (g_state) {
    case State::Closed:
      window_startOpening();
      break;
    case State::Opening:
      window_stopMotor();
      break;
    case State::Closing:
      window_stopMotor();
      break;
    case State::Paused:
      if (g_lastDirection == State::Opening) {
        window_startClosing();
      } else if (g_lastDirection == State::Closing) {
        window_startOpening();
      }
      break;
    case State::Open:
      window_startClosing();
      break;
  }
}

// Drei Hilfsmethoden zum Ansteuern der Motoren
// Beachte:
// Wenn Fenster geschlossen, dann muss lockMotor agieren.
// Wenn geöffnet werden soll, muss lockMotor erst öffnen, dann gearMotor aktivieren 

void window_startOpening() {
  g_state = State::Opening;
  g_lastDirection = State::Opening;
}

void window_startClosing() {
  g_state = State::Closing;
  g_lastDirection = State::Closing;
}

void window_stopMotor() {
  g_state = State::Paused;
  // TODO: Motoren werden abgeschaltet
}

void window_loop() {
  switch (g_state) {
    case State::Closed: {
      // TODO: Motor aus (window_stopMotor()), wenn Fenster zu --> Reedsensor
      break;
    }
    case State::Opening: {
      // TODO: Motor öffnet Fenster --> Überwachen, wie viele Steps, um zu wissen, wann komplett offen
      break;
    }
    case State::Open: {
      // TODO: Motor aus (window_stopMotor()), wenn Fenster offen
      break;
    }
    case State::Closing: {
      // TODO: Motor schließt Fenster --> Überwachen, wie viel Steps, falls gestoppt wird. Zu wenn Reedsensor
      break;
    }
    case State::Paused: {
      // TODO: Motoren stoppen (also einfach nichts machen?) 
      break;
    }
    case State::Locking: {
      // Lockmotor bewegen in eine Richtung
      break;
    }
    case State::Unlocking: {
      // Lockmotor bewegen in andere Richtung
      break;
    }
  }
}