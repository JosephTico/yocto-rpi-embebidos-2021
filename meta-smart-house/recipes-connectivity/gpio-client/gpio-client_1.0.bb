SUMMARY = "GPIO Client"
DESCRIPTION = "Client that uses custo GPIO library"

SECTION = "libs"
LICENSE = "MIT"

inherit systemd
SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE_${PN} = "gpio.service"

LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI += " file://gpio-client.c file://gpio-doors.c file://gpio.service"


S = "${WORKDIR}"

DEPENDS += "gpio"
 
do_compile() {
  ${CC} ${CFLAGS} ${LDFLAGS} -pthread ${WORKDIR}/gpio-client.c -o gpio-client -lpigpiod_if2 -lrt
  ${CC} ${CFLAGS} ${LDFLAGS} -pthread ${WORKDIR}/gpio-doors.c -o gpio-doors -lpigpiod_if2 -lrt
}

do_install() {
  install -d ${D}${bindir}

  install ${WORKDIR}/gpio-client ${D}${bindir}
  install ${WORKDIR}/gpio-doors ${D}${bindir}

  install -d ${D}/${systemd_unitdir}/system
  install -m 0644 ${WORKDIR}/gpio.service ${D}/${systemd_unitdir}/system
}

