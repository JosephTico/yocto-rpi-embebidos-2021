SUMMARY = "GPIO Library"
DESCRIPTION = "GPIO management library"

SECTION = "libs"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI += " file://."

inherit systemd
SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE_${PN} = "pigpiod.service"

S = "${WORKDIR}"
 
inherit cmake

do_install_append () {
  install -d ${D}/${systemd_unitdir}/system
  install -m 0644 ${WORKDIR}/pigpiod.service ${D}/${systemd_unitdir}/system
}