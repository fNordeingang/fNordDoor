#include <cstdio>
#include <unistd.h>
#include <iostream>

#include "door.hpp"
#include "dbg.h"

#ifndef armv6l
int wiringPiSetup() { return 0; }
void pinMode(int p, int m) { log_info("pinMode"); }
void digitalWrite(int p, int v) { log_info("digitalWrite"); }
int digitalRead(int p) { log_info("digitalRead"); return 1; }
void delay(int i) { usleep(i*100); }
#endif

void initPins() {
  log_info("initializing pins...");
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDBLUE, OUTPUT);

  setLED(OFF);

  setSenseMode(OUTPUT);

  digitalWrite(SENSE0, LOW);
  digitalWrite(SENSE1, LOW);
  log_info("done");
}

void setSenseMode(int mode) {
  pinMode(SENSE0, mode);
  pinMode(SENSE1, mode);
}

void setLED(int mode) {
  if (mode == OFF) {
    digitalWrite(LEDRED, 1);
    digitalWrite(LEDBLUE, 1);
  } else if(mode == BLUE) {
    digitalWrite(LEDRED, 1);
    digitalWrite(LEDBLUE, 0);
  } else if(mode == RED) {
    digitalWrite(LEDRED, 0);
    digitalWrite(LEDBLUE, 1);
  }
}

void sendSequence(int sense0, int sense1) {
  digitalWrite(SENSE0, sense0);
  digitalWrite(SENSE1, sense1);
  delay(1600);
}

void openLock() {
  log_info("open door command");
  sendSequence(HIGH, HIGH);
  sendSequence(LOW, LOW);
  sendSequence(HIGH, LOW);
  setLED(BLUE);
}

void closeLock() {
  log_info("close door command");
  sendSequence(HIGH, HIGH);
  sendSequence(LOW, LOW);
  sendSequence(LOW, HIGH);
  setLED(RED);
}

