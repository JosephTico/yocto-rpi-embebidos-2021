
SUMMARY = "PiGPIO Library"
DESCRIPTION = "GPIO management library"

SECTION = "libs"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI = "file://src/"


S = "${WORKDIR}"

inherit autotools
 
do_compile() {
  make
}