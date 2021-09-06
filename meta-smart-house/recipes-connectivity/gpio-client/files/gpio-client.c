#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <gpio.h>
#define OUTPUT 0
#define INPUT 1

void intHandler(int dummy)
{
  endGpio();
  exit(0);
}

int main()
{

  signal(SIGINT, intHandler);

  printf("starting gpio\n");
  startGpio();
  printf("set 18 output\n");
  pinMode(18, OUTPUT);
  printf("set 2 input\n");
  pinMode(2, INPUT);
  printf("writting in 18 \n");
  digitalWrite(18, 1);
  printf("reading 2\n");
  digitalRead(2);
  printf("lets blink madafakas\n");
  blink(18, 0.5, 30);
  printf("byeee\n");
  endGpio();
}