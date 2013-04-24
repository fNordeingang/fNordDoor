#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#ifndef NOTPI
#include <wiringPi.h>
#else
#define OUTPUT 0
#define INPUT  1
int wiringPiSetup();
void pinMode(int,int);
void digitalWrite(int,int);
int digitalRead(int);
#endif


#define BTNOPEN     7
#define BTNCLOSE   15
#define BTNDOOR    11

#define LEDRED      8
#define LEDBLUE     9

#define RED         1
#define BLUE        2
#define OFF         3


void init();
void initPins();
void initLock();
void setButtonMode(int mode);
void signalHandler(int sig);
void setLED(int mode);
void pressButton(int btn);

int isOpenPressed();
int isClosePressed();
int isDoorClosed();

void openLock();
void closeLock();


int main (void)
{
  printf ("fNordDoor started!\n") ;

  if (wiringPiSetup () == -1)
    return 1 ;

  init();

  for (;;)
  {
    if (isDoorClosed())
    {
      if(isOpenPressed()) {
        setLED(BLUE);
        fprintf(stdout, "opening door\n");
      } else if(isClosePressed()) {
        setLED(RED);
        fprintf(stdout, "closing door\n");
      }
    }
    #ifdef NOTPI
    usleep(1000000);
    #else
    usleep(10000);
    #endif
  }
  return 0 ;
}

void init() {
  initPins();
  initLock();

  signal(SIGUSR1, signalHandler);
  signal(SIGUSR2, signalHandler);
}

void initPins() {
  printf("initializing pins...");
  fflush(stdout);
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDBLUE, OUTPUT);

  setLED(OFF);

  setButtonMode(OUTPUT);
  
  digitalWrite(BTNOPEN, 0);
  digitalWrite(BTNCLOSE, 0);
  printf("done\n");
}

void initLock() {
  printf("initializing doorlock...");
  fflush(stdout);
  digitalWrite(BTNOPEN, 1);
  usleep(500000);
  digitalWrite(BTNOPEN, 0);
  usleep(500000);
  digitalWrite(BTNOPEN, 1);
  usleep(4500000);
  digitalWrite(BTNOPEN, 0);
  printf("done\n");
}

void signalHandler(int sig) {
  if(sig == SIGUSR1) {
    openLock();
    setLED(BLUE);
  } else if(sig == SIGUSR2) {
    closeLock();
    setLED(RED);
  }
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
}

void closeLock() {
  printf("close door command\n");
  pressButton(BTNCLOSE);
}

int isOpenPressed() {
  return !digitalRead(BTNOPEN);
}

int isClosePressed() {
  return !digitalRead(BTNCLOSE);
}

int isDoorClosed() {
  return !digitalRead(BTNDOOR);
}

void pressButton(int btn) {
  digitalWrite(btn, 1);
  usleep(500000);
  digitalWrite(btn, 0);
}


#ifdef NOTPI

int wiringPiSetup() { return 0; }
void pinMode(int p, int m) { printf("pinMode\n"); }
void digitalWrite(int p, int v) { printf("digitalWrite\n"); }
int digitalRead(int p) { printf("digitalRead\n"); return 1; }

#endif
