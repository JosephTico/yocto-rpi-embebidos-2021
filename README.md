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

Compilar biblioteca intermediaria

```bash
$CC -L. -Wall -pthread -o test test.c -lpigpio
```

Compilar lpigpio:

```bash
$CC -O3 -Wall -pthread -fpic -c -o pigpio.o pigpio.c
$CC -O3 -Wall -pthread -fpic -c -o command.o command.c
$AR rcs libpigpio.a pigpio.o command.o
```


BBlayers:
```bash
bitbake-layers create-layer meta-(nombre)
bitbake-layers add-layer ../(nombre)
```
