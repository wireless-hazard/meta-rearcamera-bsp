SUMMARY = "Example application to read the Lidar TF-Luna via the userspace"
SECTION = "lidar"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""
DEPENDS = "i2cdev i2c-tools"

SRC_URI = "git://github.com/wireless-hazard/TF_LUNA.git;branch=main;protocol=https"
SRCREV = "${AUTOREV}"

S = "${WORKDIR}/git"

TARGET_CC_ARCH += "${LDFLAGS}"

do_compile() {
    oe_runmake
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 read_distance ${D}${bindir}
}