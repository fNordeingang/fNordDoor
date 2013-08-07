#include "friendly_launchpad.h"


#define PORTOPEN   10
#define PORTCLOSE  11
#define PORTSENSE  12
#define LED        13

#define STATE0     0
#define STATE1     1
#define STATE2     2
#define STATE3     3

int state;

int sensePort();
int sequence(int d1, int d2, int next_state, int prev_state);

void setup() {
  state = STATE0;

  pinMode(PORTOPEN, OUTPUT);
  pinMode(PORTCLOSE, OUTPUT);
  pinMode(PORTSENSE, INPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(PORTOPEN, HIGH);
  digitalWrite(PORTCLOSE, HIGH);
  digitalWrite(LED, LOW);
}

void loop() {
  switch(state) {
    case STATE0:
      state = sequence(120,100,STATE1,STATE0);
      break;
    case STATE1:
      state = sequence(80,110,STATE2,STATE0);
      break;
    case STATE2:
      state = sequence(130,90,STATE3,STATE0);
      break;
    case STATE3:
      state = STATE0;
      digitalWrite(LED, HIGH);
      digitalWrite(PORTOPEN, LOW);
      delay(500);
      digitalWrite(PORTOPEN, HIGH);
      break;
    default:
      break;
  }
  
}

int sequence(int d1, int d2, int next_state, int prev_state) {
  int state;

  if(sensePort()) {
    delay(d1);
    if(!sensePort()) {
      delay(d2);
      if(sensePort())
        state = next_state;
      else state = prev_state;
    } else state = prev_state;
  } else state = prev_state;

  return state;
}

int sensePort() {
  return digitalRead(PORTSENSE) == HIGH;
}

