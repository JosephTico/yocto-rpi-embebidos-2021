SUMMARY = "Smart House Web Client"
DESCRIPTION = "Web client for the smart house"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI += " file://index.js file://smart-house-web.service file://dist/"

inherit systemd
SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE_${PN} = "smart-house-web.service"


S = "${WORKDIR}"

#DEPENDS += "express nodejs"
 

do_install() {
  install -d ${D}${base_prefix}/opt/smart-house-web 
  install -m 0755 ${WORKDIR}/index.js ${D}${base_prefix}/opt/smart-house-web
  cp -R --no-dereference --preserve=mode,links -v ${WORKDIR}/dist ${D}${base_prefix}/opt/smart-house-web/

  install -d ${D}/${systemd_unitdir}/system
  install -m 0644 ${WORKDIR}/smart-house-web.service ${D}/${systemd_unitdir}/system
}

FILES_${PN} = "${base_prefix}/opt/*"
