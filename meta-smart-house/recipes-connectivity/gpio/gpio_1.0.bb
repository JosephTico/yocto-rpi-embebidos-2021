SUMMARY = "GPIO Library"
DESCRIPTION = "GPIO management library"

SECTION = "libs"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI += " file://."

S = "${WORKDIR}"
TARGET_CC_ARCH += "${LDFLAGS}"
 
do_compile() {
  oe_runmake
}

do_install() {
  install -d ${D}${includedir}

  # install the header file in /usr/include with default permissions
  install ${S}/gpio.h ${D}${includedir}

  # add the /usr/lib folder to the sysroot for this recipe, to be
  # added to the final rootfs
  install -d ${D}${libdir}

  # install the prebuilt library in /usr/lib with default permissions
  oe_soinstall ${S}/libgpio.so.${PV} ${D}${libdir}

}
