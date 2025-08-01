FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://usbguard.init \
"

inherit update-rc.d

INITSCRIPT_NAME = "usbguard"
INITSCRIPT_PARAMS = "defaults 42"

do_install:append() {
    if ${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', 'true', 'false', d)}; then
        install -d ${D}${sysconfdir}/init.d
        install -m 0755 ${WORKDIR}/usbguard.init ${D}${sysconfdir}/init.d/usbguard
        rm -f ${D}${sysconfdir}/volatiles.cache
    fi
}
