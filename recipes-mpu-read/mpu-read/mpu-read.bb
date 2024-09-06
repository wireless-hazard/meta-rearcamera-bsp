SUMMARY = "Simple Userspace application to read the mpu"
SECTION = "mpu-read"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

SRC_URI = "file://main.cpp file://mpu_read.h file://framebuffer_ssd1306.h file://framebuffer_ssd1306.cpp file://font_8x8_definitions.h"

S = "${WORKDIR}"

TARGET_CC_ARCH += "${LDFLAGS}"

do_compile() {
    ${CXX} main.cpp -Ifont_8x8_definitions.h -Impu_read.h framebuffer_ssd1306.cpp -Iframebuffer_ssd1306.h -o mpu_test
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 mpu_test ${D}${bindir}
}