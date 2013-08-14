#include "friendly_launchpad.h"


#define LED         10
#define PORTOPEN    11
#define PORTCLOSE   12
#define PORTSENSE0  13
#define PORTSENSE1  14

#define STATE0      0
#define STATE1      1
#define STATE2      2
#define STATE3      3
#define STATE4      4

int state;
unsigned int state_counter;

int blink(int times);
int sequence(int sense0, int sense1, int next_state, int prev_state);
void closeLock();
void openLock();

void resetState() {
  if(state_counter > 3) {
    state = STATE0;
    state_counter = 0;
    return;
  }
  else {
    state_counter++;
  }
}

void setup() {
  state = STATE0;
  state_counter = 0;

  pinMode(PORTOPEN, OUTPUT);
  pinMode(PORTCLOSE, OUTPUT);
  pinMode(PORTSENSE0, INPUT);
  pinMode(PORTSENSE1, INPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(PORTOPEN, LOW);
  digitalWrite(PORTCLOSE, LOW);
  digitalWrite(PORTSENSE0, LOW);
  digitalWrite(PORTSENSE1, LOW);
  digitalWrite(LED, LOW);
}

void loop() {
  switch(state) {
    case STATE0:
      blink(1);
      state = sequence(HIGH,HIGH,STATE1,STATE0);
      break;
    case STATE1:
      blink(2);
      state = sequence(LOW,LOW,STATE2,STATE1);
      resetState();
      break;
    case STATE2:
      blink(3);
      state = sequence(HIGH,LOW,STATE3,STATE2);
      if(state == STATE2) {
        state = sequence(LOW,HIGH,STATE4,STATE2);
      }
      resetState();
      break;
    case STATE3:
      blink(4);
      state = STATE0;
      openLock();
      break;
    case STATE4:
      blink(5);
      state = STATE0;
      closeLock();
      break;
    default:
      break;
  }
  
}

void openLock() {
  digitalWrite(PORTOPEN, HIGH);
  delay(500);
  digitalWrite(PORTOPEN, LOW);
}

void closeLock() {
  digitalWrite(PORTCLOSE, HIGH);
  delay(500);
  digitalWrite(PORTCLOSE, LOW);
}

int sequence(int sense0, int sense1, int next_state, int prev_state) {
  if((digitalRead(PORTSENSE0) == sense0) && (digitalRead(PORTSENSE1) == sense1)) {
    return next_state;
  }
  else
    return prev_state;
}

int blink(int times) {
  for(int i = 0; i < times; i++) {
    digitalWrite(LED, HIGH);
    delay(80);
    digitalWrite(LED, LOW);
    delay(80);
  }
  delay(1000);
  return times;
}

