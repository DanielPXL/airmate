#include <Stepper.h>
#include "Pins.h"
#include "Window.h"

#define MOTOR_MAX_RPM 10
#define MOTOR_MIN_RPM 2
#define STEPS 1024

#define ACCEL_STEPS 200         // Steps zum Beschleunigen
#define DECEL_STEPS 200         // Steps zum Abbremsen
#define TOTAL_MOVE_STEPS 800    // Gesamtbewegung (z.B. 800 Schritte bis Fensterende)

/* alter Code: 
#define MOTOR_RPM 10 //
#define STEPS 1024 // TODO: Muss wahrscheinlich noch angepasst werden die Steps!!!
*/

State g_state = State::Closed;
State g_lastDirection = State::Closing; // Nur Opening oder Closing

// Die beiden Zeilen blockieren aus irgendeinem Grund den restlichen Code...
// Erst unkommentieren, wenns gebraucht wird
Stepper g_lockStepper(STEPS, LOCKMOTOR_PINS); // Motor Pins
Stepper g_gearStepper(STEPS, GEARMOTOR_PINS); // Motor Pins

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
  g_gearStepper.setSpeed(MOTOR_MIN_RPM);
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
  g_state = Opening;
  g_lastDirection = Opening;
  g_stepPosition = 0; // Reset Position für Bewegung
}

void window_startClosing() {
  g_state = Closing;
  g_lastDirection = Closing;
  g_stepPosition = TOTAL_MOVE_STEPS; // Start von "offen"
}

void window_stopMotor() {
  g_state = Paused;
  // TODO: Motoren werden abgeschaltet
}

void dynamicMovement(){
if (stepsRemaining > TOTAL_MOVE_STEPS - ACCEL_STEPS) {
      // Beschleunigung
      int delta = TOTAL_MOVE_STEPS - stepsRemaining;
      int rpm = MOTOR_MIN_RPM + (delta * (MOTOR_MAX_RPM - MOTOR_MIN_RPM)) / ACCEL_STEPS;
      g_gearStepper.setSpeed(min(rpm, MOTOR_MAX_RPM));
    } else if (stepsRemaining < DECEL_STEPS) {
      // Abbremsen
      int rpm = MOTOR_MIN_RPM + (stepsRemaining * (MOTOR_MAX_RPM - MOTOR_MIN_RPM)) / DECEL_STEPS;
      g_gearStepper.setSpeed(max(rpm, MOTOR_MIN_RPM));
    } else {
      // Konstante Fahrt
      g_gearStepper.setSpeed(MOTOR_MAX_RPM);
    }

  // Bewege 1 Step pro Loop
    g_gearStepper.step(direction);
    g_stepPosition += direction;


    // beendung vom öffnen oder schließen 
    if (g_stepPosition <= 0) {
      g_state = Closed;
      window_stopMotor();
    } else if (g_stepPosition >= TOTAL_MOVE_STEPS) {
      g_state = Open;
      window_stopMotor();
    }
}

void calcPosition() {
  int stepsRemaining = (g_state == Opening)
        ? TOTAL_MOVE_STEPS - g_stepPosition
        : g_stepPosition;
}

void window_loop() {
  if (g_state == Opening || g_state == Closing) {
    int direction = (g_state == Opening) ? 1 : -1;

    calcPosition();
    dynamicMovement();
    
  }
}