#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <gpio.h>
#define OUTPUT 0
#define INPUT 1
const int pins[] = {5, 6, 13, 19, 26};

void intHandler(int dummy)
{
  printf("Closing Gpio...\n");
  endGpio();
  exit(0);
}

int ledToPin(int led)
{
  return pins[led - 1];
}

void ledOn(int led, int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 1);
  printf("Led %d is ON\n", led);
}

void ledOff(int led, int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 0);
  printf("Led %d is OFF\n", led);
}

int main(int argc, char *argv[])
{
  signal(SIGINT, intHandler);
  printf("Starting gpio...\n");
  startGpio();

  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s [n/f/r] [led number]\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  else
  {
    char *a = argv[2];
    int led = atoi(a);

    if (1 <= led && led <= 5)
    {
      switch (*argv[1])
      {
      case 'n':
        ledOn(led, ledToPin(led));
        break;
      case 'f':
        ledOff(led, ledToPin(led));
        break;
      default:
        fprintf(stderr, "Usage: %s [o/f/r] [led]\n", argv[0]);
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      fprintf(stderr, "Led value must be between 1 and 5\n");
      exit(EXIT_FAILURE);
    }
  }
  printf("Closing Gpio...\n");
  endGpio();
  return 0;
}

// int main()
// {

//   signal(SIGINT, intHandler);

//   printf("starting gpio\n");
//   startGpio();
//   printf("set 18 output\n");
//   pinMode(18, OUTPUT);
//   printf("set 2 input\n");
//   pinMode(2, INPUT);
//   printf("writting in 18 \n");
//   digitalWrite(18, 1);
//   printf("reading 2\n");
//   digitalRead(2);
//   printf("lets blink madafakas\n");
//   blink(18, 0.5, 30);
//   printf("byeee\n");
//   endGpio();
// }