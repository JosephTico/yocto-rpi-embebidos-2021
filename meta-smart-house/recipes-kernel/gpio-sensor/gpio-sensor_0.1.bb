SUMMARY = "GPIO Sensor Kernel Driver"
DESCRIPTION = "Driver for GPIO sensor"

SECTION = "libs"
LICENSE = "MIT"


LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI += " file://gpio-sensor.c"

S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.
inherit module

PR = "r0"
PV = "0.1"

KERNEL_MODULE_AUTOLOAD += "gpio-sensor"