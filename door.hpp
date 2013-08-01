#ifndef DOORLOCK_HPP
#define DOORLOCK_HPP

#ifdef armv6l
#include "wiringPi/wiringPi/wiringPi.h"
#else
#define OUTPUT 0
#define INPUT  1
#define HIGH   1
#define LOW    0
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
void sendSequence(int d1, int d2, int port);

#endif
