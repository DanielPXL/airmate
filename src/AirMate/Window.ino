#include <Stepper.h>
#include "Pins.h"

#define STEPS 1024

enum State {
  Closed,
  Opening,
  Open,
  Closing
};

State g_state = State::Closed;

// Die beiden Zeilen blockieren aus irgendeinem Grund den restlichen Code...
// Erst unkommentieren, wenns gebraucht wird
// Stepper g_lockStepper(STEPS, LOCKMOTOR_PINS); // Motor Pins
// Stepper g_gearStepper(STEPS, GEARMOTOR_PINS); // Motor Pins

void window_setup()
{
  
}

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

void window_startOpening() {
  if (g_state != State::Closed) {
    return;
  }

  g_state = State::Opening;
}

void window_startClosing() {
  if (g_state != State::Open) {
    return;
  }

  g_state = State::Closing;
}

void window_loop() {
  switch (g_state) {
    case State::Closed: {

      break;
    }
    case State::Opening: {
      
      break;
    }
    case State::Open: {
      
      break;
    }
    case State::Closing: {
      
      break;
    }
  }
}

// void window_loop()
// {
//     on = digitalRead(ButtonPin);
//     Serial.println(on);
//     Serial.println(open);
//     if (on == LOW && open){

//       lockStepper.setSpeed(MOTOR_RPM);
//       lockStepper.step(-STEPS);
      
//       Serial.println("on is pushed!");
//       delay(1000);
//       open = false;

//     }else if ( on == LOW && !open){

//       lockStepper.setSpeed(MOTOR_RPM);
//       lockStepper.step(STEPS);
      
//       Serial.println("off is pushed!");
//       delay(1000);
//       open = true;

//     }
  
// }

