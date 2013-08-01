#include <msp430.h>

int main(int argc, char *argv[])
{
  P1DIR = 0b01000000;
  P1OUT = 0b01000000;
  return 0;
}

