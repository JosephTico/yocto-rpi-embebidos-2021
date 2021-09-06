SUMMARY = "GPIO Client"
DESCRIPTION = "Client that uses custo GPIO library"

SECTION = "libs"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI += " file://gpio-client.c"

S = "${WORKDIR}"

DEPENDS += "gpio"
 
do_compile() {
  ${CC} ${CFLAGS} ${LDFLAGS} -pthread ${WORKDIR}/gpio-client.c -o gpio-client -lgpio
}

do_install() {
  install -d ${D}${bindir}

  install ${WORKDIR}/gpio-client ${D}${bindir}
}

