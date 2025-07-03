#include <AccelStepper.h>
#include <Stepper.h>
#include "Pins.h"
#include "Window.h"
#include "Log.h"


#define OPEN_MOVE_STEPS 200    // TODO: Muss wahrscheinlich noch angepasst werden!!
#define CLOSE_MOVE_STEPS -40    // TODO: Muss wahrscheinlich noch angepasst werden!!
#define ACCELERATION_SPEED 400  //TODO: Muss wahrscheinlich noch angepasst werden!!
#define MAX_SPEED 25          // TODO: Muss wahrscheinlich noch angepasst werden!!!


State g_state = State::Closed;
State g_lastDirection = State::Closing;  // Nur Opening oder Closing


AccelStepper g_lockStepper(AccelStepper::FULL4WIRE, LOCKMOTOR_PINS);  // Motor Pins
AccelStepper g_gearStepper(AccelStepper::DRIVER, GEARMOTOR_PINS);     // Motor Pins

uint32_t g_closeTime;
uint32_t g_closeTimeGoal = 5 * 1000 * 60;  // 5 Minuten


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
    case State::Paused:
      return "pausing";
    case State::Locking:
      return "locking";
    case State::Unlocking:
      return "unlocking";
    case State::Alarm:
      return "alarm";
  }
}

void window_setup() {
  g_gearStepper.setMaxSpeed(MAX_SPEED);
  g_gearStepper.setAcceleration(ACCELERATION_SPEED);
}

// Fenstersteuerung mit Button toggled durch
// (TODO:) Sensor- und Wetterdaten setzen entsprechend g_state
void window_buttonToggle() {
  switch (g_state) {
    case State::Alarm:
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
  if (g_state == State::Closed || g_state == State::Paused) {
    LOG("Opening!\n");
    g_state = State::Opening;
    g_lastDirection = State::Opening;
    g_gearStepper.moveTo(OPEN_MOVE_STEPS);
  }
}


void window_startClosing() {
  if (g_state == State::Open || g_state == State::Paused) {
    LOG("Closing!\n");
    g_state = State::Closing;
    g_lastDirection = State::Closing;
    g_gearStepper.moveTo(CLOSE_MOVE_STEPS);
  }
}

void window_stopMotor() {
  g_state = State::Paused;
  // TODO: Motoren werden abgeschaltet
}


void window_loop() {
  g_gearStepper.run();

  switch (g_state) {
    case State::Opening:
      {
        if (g_gearStepper.distanceToGo() == 0) {
          g_state = State::Open;
          g_closeTime = millis();
        };
        break;
      }
    case State::Closing:
      {
        if (g_gearStepper.distanceToGo() == 0) {
          g_state = State::Closed;
        }
        break;
      }
    case State::Open:
      {
        uint32_t time_now = millis();
        if (time_now - g_closeTime > g_closeTimeGoal) {
          window_startClosing();
        }
        break;
      }
  }
}