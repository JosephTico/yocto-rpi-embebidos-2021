# Primer proyecto, Embebidos 2021 Ing. en Computadores

## Comandos utiles

Compilar biblioteca intermediaria

` $CC -L. -Wall -pthread -o test test.c -lpigpio

Compilar lpigpio:

```
$CC -O3 -Wall -pthread -fpic -c -o pigpio.o pigpio.c
$CC -O3 -Wall -pthread -fpic -c -o command.o command.c
$AR rcs libpigpio.a pigpio.o command.o
```


BBlayers:
```bitbake-layers create-layer meta-(nombre)
bitbake-layers add-layer ../(nombre)
```
