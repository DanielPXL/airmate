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
Stepper g_lockStepper(STEPS, LOCKMOTOR_PINS); // Motor Pins
Stepper g_gearStepper(STEPS, GEARMOTOR_PINS); // Motor Pins

void window_setup()
{
  g_gearStepper.setSpeed(MOTOR_RPM);
}

void window_buttonToggle() {
  switch (g_state) {
    case State::Closed:
      break;
    case State::Opening:
      g_state = Paused;
      g_lastDirection == State::Opening;
      break;
    case State::Closing:
      g_state = Paused;
      g_lastDirection == State::Closing;
      break;
    case State::Paused:
      if (g_lastDirection == Opening) {
        
        g_state = Closing;
        g_lastDirection == Closing;
      } else if (g_lastDirection == Closing) {
        
        g_state = Opening;
        g_lastDirection == Opening;
      }
      break;
    case State::Open:
      
      break;
  }
}

// Drei Hilfsmethoden zum Ansteuern der Motoren

void window_startOpening() {
  // TODO: Motoren ansteuern zum öffnen
}

void window_startClosing() {
  // TODO: Motoren ansteuern zum schließen
}

void window_stopMotor() {
  // TODO: Motoren werden abgeschaltet
}

void window_loop() {
  switch (g_state) {
    case Closed: {
      // TODO: Motor aus (window_stopMotor()), wenn Fenster zu --> Reedsensor
      break;
    }
    case Opening: {
      // TODO: Motor öffnet Fenster --> Überwachen, wie viele Steps, um zu wissen, wann komplett offen
      break;
    }
    case Open: {
      // TODO: Motor aus (window_stopMotor()), wenn Fenster offen
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

// Bedenken:
// Wenn Fenster geschlossen, dann muss lockMotor agieren.
// Wenn geöffnet werden soll, muss lockMotor erst öffnen, erst wenn fertig, dann gearMotor aktivieren 

