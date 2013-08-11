#include <cstdio>
#include <unistd.h>
#include <iostream>

#include "door.hpp"

#ifndef armv6l
int wiringPiSetup() { return 0; }
void pinMode(int p, int m) { printf("pinMode\n"); }
void digitalWrite(int p, int v) { printf("digitalWrite\n"); }
int digitalRead(int p) { printf("digitalRead\n"); return 1; }
#endif

void initPins() {
  printf("initializing pins...");
  fflush(stdout);
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDBLUE, OUTPUT);

  setLED(OFF);

  setSenseMode(OUTPUT);

  digitalWrite(SENSE0, LOW);
  digitalWrite(SENSE1, LOW);
  printf("done\n");
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
  delay(1200);
}

void openLock() {
  printf("open door command\n");
  sendSequence(HIGH, HIGH);
  sendSequence(LOW, LOW);
  sendSequence(LOW, HIGH);
  setLED(BLUE);
}

void closeLock() {
  printf("close door command\n");
  sendSequence(HIGH, HIGH);
  sendSequence(LOW, LOW);
  sendSequence(HIGH, LOW);
  setLED(RED);
}

