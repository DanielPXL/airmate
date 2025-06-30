#include <AccelStepper.h>
#include <Stepper.h>
#include "Pins.h"
#include "Window.h"


#define TOTAL_MOVE_STEPS 800    // TODO: Muss wahrscheinlich noch angepasst werden!!
#define ACCAELERATION_SPEED 100; //TODO: Muss wahrscheinlich noch angepasst werden!!
#define MAX_SPEED 200 // TODO: Muss wahrscheinlich noch angepasst werden!!!


State g_state = State::Closed;
State g_lastDirection = State::Closing; // Nur Opening oder Closing


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
  g_gearStepper.setMaxSpeed(MAX_SPEED);
  g_gearStepper.setAcceleration(ACCAELERATION_SPEED);
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
  g_state = State::Opening;
  g_lastDirection = State::Opening;
  g_gearStepper.moveTo(TOTAL_MOVE_STEPS);
}


void window_startClosing() {
  g_state = State::Closing;
  g_lastDirection = State::Closing;
  gearStepper.moveTo(0);
}

void window_stopMotor() {
  g_state = Paused;
  // TODO: Motoren werden abgeschaltet
}


void window_loop() {
  g_gearStepper.run();

  switch (g_state) {
    case State::Opening: {
      if (gearStepper.distanceToGo() == 0) g_state = State::Open;
      break;
    }
    case State::Closing: {
      if (gearStepper.distanceToGo() == 0) g_state = State::Closed;
      // TODO: ReedSensor
      break;
  }
}