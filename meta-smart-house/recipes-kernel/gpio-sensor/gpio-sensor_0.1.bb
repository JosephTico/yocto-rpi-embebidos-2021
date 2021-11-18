SUMMARY = "GPIO Sensor Kernel Driver"
DESCRIPTION = "Driver for GPIO sensor"

SECTION = "libs"
LICENSE = "MIT"

# Dependencies
DEPENDS = "virtual/kernel glibc"

LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI += " file://gpio-sensor.c file://Makefile"

S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.
inherit module

PR = "r0"
PV = "0.1"

# OE build directives
EXTRA_OEMAKE_append_task-install = "-C ${STAGING_KERNEL_DIR} M=${S}"
EXTRA_OEMAKE += "KDIR=${STAGING_KERNEL_DIR}"

KERNEL_MODULE_AUTOLOAD += "gpio-sensor"