#include <Stepper.h>
#include "Pins.h"

#define MOTOR_RPM 10 //
#define STEPS 1024 // TODO: Muss wahrscheinlich noch angepasst werden die Steps!!!

enum State {
  Closed,
  Opening,
  Open,
  Closing,
  Paused,
};

State g_state = State::Closed;
State g_lastDirection = State::Closing; // Nur Opening oder Closing

// Die beiden Zeilen blockieren aus irgendeinem Grund den restlichen Code...
// Erst unkommentieren, wenns gebraucht wird
Stepper g_lockStepper(STEPS, LOCKMOTOR_PINS); // Motor Pins
Stepper g_gearStepper(STEPS, GEARMOTOR_PINS); // Motor Pins

void window_setup() {
  g_gearStepper.setSpeed(MOTOR_RPM);
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
      if (g_lastDirection == Opening) {
        window_startClosing();
      } else if (g_lastDirection == Closing) {
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
  if (g_state == State::Closed) {
    // TODO: lockMotor ansteuern --> Überwachen, wie viele Steps, um zu wissen, wann Fenster komplett entsperrt
    // warte bis lockMotor gestoppt, dann continue
  }

  g_state = Opening;
  g_lastDirection = Opening;

  // TODO: stepMotor ansteuern zum öffnen --> Überwachen, wie viele Steps, um zu wissen, wann komplett offen

  //if (TODO) { // Wenn anzahl an STEPS erreicht, dann Fenster offen und stopMotor
  //  window_stopMotor();
  //}
}

void window_startClosing() {
  g_state = Closing;
  g_lastDirection = Closing;
  // TODO: Motoren ansteuern zum schließen
  //if (TODO) { // Reedsensor meldet sich, dann ist Fenster zu und stopMotor
  //  window_stopMotor();
  //  // lockMotor ansteuern --> Überwachen, wie viele Steps, um zu wissen, wann Fenster komplett gesperrt, lockMotor stoppen
  //}
}

void window_stopMotor() {
  g_state = Paused;
  // TODO: Motoren werden abgeschaltet
}