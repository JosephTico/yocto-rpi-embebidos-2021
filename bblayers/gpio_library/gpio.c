// Your First C++ Program
#include <stdio.h>
#include "pigpio/pigpio.h"


int startGpio(){
  int result = 0;
  if (gpioInitialise() < 0){
    fprintf(stderr, "pigpio initialisation failed\n");
    result = 1;
  }
  return result;
}

int pinMode (pin, MODE){
  int result = 0;
  if (MODE == 0){
    gpioSetMode(pin, PI_OUTPUT);
  } else if (MODE == 1) {
    gpioSetMode(pin, PI_INPUT);
  } else {
    fprintf(stderr, "Wrong MODE\n");
    result = 1;
  }
  return result;
}

int digitalWrite(pin, value){
  int result = 1;
  if (value == 1 || value == 0){
    if (gpioGetMode(pin)==PI_OUTPUT){
      if (gpioWrite(pin, value) != 0){ 
        result = 0;
      } else {
        fprintf(stderr, "Error writing pin\n");
      }
    } else {
      fprintf(stderr, "Pin is in input mode\n");
    }
  } else {
    fprintf(stderr, "Bad value for pin (use 1 or 0)\n");
  }
  return result;
}

int digitalRead(pin){
  int input_data = -1;
  if (gpioGetMode(pin)==PI_INPUT){
    int input_data = gpioRead(pin);
    printf("digitalRead, Data = %d\n", input_data);
    if (input_data==PI_BAD_GPIO){
      fprintf(stderr, "Error reading pin\n");
    }
  } else {
    fprintf(stderr, "Pin is in output mode\n");
  }
  return input_data;
}

int blink(pin, freq, duration){
 
  if (gpioGetMode(pin)==PI_OUTPUT){ 
    double start = time_time();
    while ((time_time() - start) < duration){
      gpioWrite(pin, 1); /* on */
      time_sleep(freq);
      gpioWrite(pin, 0); /* off */
      time_sleep(freq);
    }
  } else {
    fprintf(stderr, "Pin is in input mode\n");
  }
}

void endGpio(){
  gpioTerminate();
}