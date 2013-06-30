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

  setButtonMode(OUTPUT);

  digitalWrite(BTNOPEN, 1);
  digitalWrite(BTNCLOSE, 1);
  printf("done\n");
}

void initLock() {
  printf("initializing doorlock...");
  fflush(stdout);
  digitalWrite(BTNOPEN, 0);
  usleep(500000);
  digitalWrite(BTNOPEN, 1);
  usleep(500000);
  digitalWrite(BTNOPEN, 0);
  usleep(4500000);
  digitalWrite(BTNOPEN, 1);
  usleep(10000000);
  pressButton(BTNCLOSE);
  printf("done\n");
}

void setButtonMode(int mode) {
  pinMode(BTNOPEN, mode);
  pinMode(BTNCLOSE, mode);
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

void openLock() {
  printf("open door command\n");
  pressButton(BTNOPEN);
  setLED(BLUE);
}

void closeLock() {
  printf("close door command\n");
  pressButton(BTNCLOSE);
  setLED(RED);
}

int isOpenPressed() {
  return digitalRead(BTNOPEN);
}

int isClosePressed() {
  return digitalRead(BTNCLOSE);
}

int isDoorClosed() {
  return !digitalRead(BTNDOOR);
}

void pressButton(int btn) {
  digitalWrite(btn, 0);
  usleep(500000);
  digitalWrite(btn, 1);
}

