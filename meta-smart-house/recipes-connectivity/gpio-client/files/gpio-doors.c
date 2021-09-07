
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
//#include <pigpio.h>
#include <pigpiod_if2.h>

const int button_door1 = 12;
const int button_door2 = 16;
const int button_door3 = 20;
const int button_door4 = 21;

char *optHost = NULL;
char *optPort = NULL;
int pi = -1;

int doors_states[] = {0, 0, 0, 0};
FILE *fptr;

void intHandler(int dummy)
{
  printf("Closing Gpio...\n");
  fclose(fptr);
  pigpio_stop(pi);
  // endGpio();
  exit(0);
}

void updateStates()
{
  fseek(fptr, 0, SEEK_SET);
  for (int i = 0; i < 4; ++i)
  {
    fprintf(fptr, "%d ", doors_states[i]);
  }
}

void door1StateChange(int gpio, int level, uint32_t tick)
{
  if (level == 1)
  {
    int actual_state = doors_states[0];
    if (actual_state == 1)
    {
      printf("DOOR 1: CLOSED");
      doors_states[0] = 0;
    }
    else
    {
      printf("DOOR 1: OPEN");
      doors_states[0] = 1;
    }
    updateStates();
    printf("GPIO is now on %d\n", level);
  }
}
void door2StateChange(int gpio, int level, uint32_t tick)
{
  if (level == 1)
  {
    int actual_state = doors_states[1];
    if (actual_state == 1)
    {
      printf("DOOR 2: CLOSED");
      doors_states[1] = 0;
    }
    else
    {
      printf("DOOR 2: OPEN");
      doors_states[1] = 1;
    }
    updateStates();
    printf("GPIO is now on %d\n", level);
  }
}
void door3StateChange(int gpio, int level, uint32_t tick)
{
  if (level == 1)
  {
    int actual_state = doors_states[2];
    if (actual_state == 1)
    {
      printf("DOOR 3: CLOSED");
      doors_states[2] = 0;
    }
    else
    {
      printf("DOOR 3: OPEN");
      doors_states[2] = 1;
    }
    updateStates();
    printf("GPIO is now on %d\n", level);
  }
}
void door4StateChange(int gpio, int level, uint32_t tick)
{
  if (level == 1)
  {
    int actual_state = doors_states[3];
    if (actual_state == 1)
    {
      printf("DOOR 4: CLOSED");
      doors_states[3] = 0;
    }
    else
    {
      printf("DOOR 4: OPEN");
      doors_states[3] = 1;
    }
    updateStates();
    printf("GPIO is now on %d\n", level);
  }
}

int main(void)
{

  signal(SIGINT, intHandler);

  pi = pigpio_start(optHost, optPort);

  // int status = gpioInitialise();

  set_glitch_filter(pi, button_door1, 1000);
  set_glitch_filter(pi, button_door2, 1000);
  set_glitch_filter(pi, button_door3, 1000);
  set_glitch_filter(pi, button_door4, 1000);
  // gpioGlitchFilter(button_door1, 800);
  // gpioGlitchFilter(button_door2, 800);
  // gpioGlitchFilter(button_door3, 800);
  // gpioGlitchFilter(button_door4, 800);

  if (pi >= 0)
  {

    fptr = fopen("/home/root/doors_status.txt", "w");
    if (fptr == NULL)
    {
      printf("Error opening door_status.txt");
      exit(1);
    }

    set_mode(pi, button_door1, PI_INPUT);
    set_mode(pi, button_door2, PI_INPUT);
    set_mode(pi, button_door3, PI_INPUT);
    set_mode(pi, button_door4, PI_INPUT);
    // gpioSetMode(button_door1, PI_INPUT);
    // gpioSetMode(button_door2, PI_INPUT);
    // gpioSetMode(button_door3, PI_INPUT);
    // gpioSetMode(button_door4, PI_INPUT);

    callback(pi, button_door1, RISING_EDGE, door1StateChange);
    callback(pi, button_door2, RISING_EDGE, door2StateChange);
    callback(pi, button_door3, RISING_EDGE, door3StateChange);
    callback(pi, button_door4, RISING_EDGE, door4StateChange);
    // gpioSetAlertFunc(button_door1, door1StateChange);
    // gpioSetAlertFunc(button_door2, door2StateChange);
    // gpioSetAlertFunc(button_door3, door3StateChange);
    // gpioSetAlertFunc(button_door4, door4StateChange);

    while (1)
    {
      sleep(1);
    }
  }
  else
  {
    printf("Error on pigpio_start()");
  }
  fclose(fptr);
  // gpioTerminate();
  pigpio_stop(pi);
}