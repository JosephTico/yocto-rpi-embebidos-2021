#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pigpiod_if2.h>
#define OUTPUT 0
#define INPUT 1
const int led_pins[] = {5, 6, 13, 19, 26};

char *optHost = NULL;
char *optPort = NULL;
int pi = -1;

void intHandler(int dummy)
{
  printf("Closing Gpio...\n");
  pigpio_stop(pi);
  // endGpio();
  exit(0);
}

int ledToPin(int led)
{
  return led_pins[led - 1];
}

void ledOn(int led, int pin)
{
  set_mode(pi, pin, OUTPUT);
  gpio_write(pi, pin, 1);
  // pinMode(pin, OUTPUT);
  // digitalWrite(pin, 1);
  printf("Led %d is ON\n", led);
}

void ledOff(int led, int pin)
{
  set_mode(pi, pin, OUTPUT);
  gpio_write(pi, pin, 0);
  // pinMode(pin, OUTPUT);
  // digitalWrite(pin, 0);
  printf("Led %d is OFF\n", led);
}

void readDoors(){
  printf("Door Status: %d %d %d %d", 
    gpio_read(pi, 12),
    gpio_read(pi, 16),
    gpio_read(pi, 20),
    gpio_read(pi, 21)
  );
}

int main(int argc, char *argv[])
{
  signal(SIGINT, intHandler);
  printf("Starting gpio...\n");
  // startGpio();
  pi = pigpio_start(optHost, optPort);

  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s [n/f/r] [led number]\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  else if (pi >= 0)
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
      case 'r':
        readDoors();
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
  else
  {
    fprintf(stderr, "Error on pigpio_start(): pi < 0\n");
  }
  printf("Closing Gpio...\n");
  pigpio_stop(pi);
  // endGpio();
  return 0;
}
