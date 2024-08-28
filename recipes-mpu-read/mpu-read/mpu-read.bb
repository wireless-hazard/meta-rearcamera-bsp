SUMMARY = "Simple Userspace application to read the mpu"
SECTION = "mpu-read"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

SRC_URI = "file://mpu_read.cpp"

S = "${WORKDIR}"

TARGET_CC_ARCH += "${LDFLAGS}"

do_compile() {
    ${CXX} mpu_read.cpp -o mpu_test
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 mpu_test ${D}${bindir}
}