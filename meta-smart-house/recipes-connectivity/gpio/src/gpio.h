
int startGpio();
int pinMode (int pin, int MODE);
int digitalWrite(int pin, int value);
int digitalRead(int pin);
int blink(int pin, float freq, float duration);
void endGpio();