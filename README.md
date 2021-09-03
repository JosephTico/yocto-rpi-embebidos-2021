# Primer proyecto, Embebidos 2021 Ing. en Computadores

## Comandos utiles

Inicializar ambiente
```bash
source oe-init-build-env rpi2
```

Inicializar toolchain
```bash
source /opt/poky/3.3.2/environment-setup-cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi
```

Compilación normal de la aplicación que usa la bilioteca:
```bash
$CC -L. -Wall -pthread -o gpto_test gpto_test.c -lpigpio -lgpio
```

Compilar .o biblioteca intermediaria DINÁMICA:
```bash
$CC -L. -Wall -pthread -fpic -c -o gpio.o gpio.c
```

Linkear biblioteca intermediaria DINÁMICA:
```bash
$CC -L. -shared -o libgpio.so gpio.o -lpigpio -lgpio
```

Compilar .o de biblioteca intermediaria ESTÁTICA (única diferencia: no ocupa el -fpic):

```bash
$CC -L. -Wall -pthread -c -o gpio.o gpio.c 
```

Compilar lpigpio (o algo ESTÁTICO):

```bash
$CC -O3 -Wall -pthread -fpic -c -o pigpio.o pigpio.c
$CC -O3 -Wall -pthread -fpic -c -o command.o command.c
$AR -L. rcs libpigpio.a pigpio.o command.o -lpigpio
```


BBlayers:
```bash
bitbake-layers create-layer meta-(nombre)
bitbake-layers add-layer ../(nombre)
```
