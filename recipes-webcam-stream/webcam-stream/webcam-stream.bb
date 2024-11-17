SUMMARY = "Simple application to stream the webcam over the Local Network using GStreamer on Linux"
SECTION = "webcam"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""
DEPENDS = "gstreamer1.0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good pkgconfig-native"

SRC_URI = "git://github.com/wireless-hazard/WebcamOverLAN.git;branch=main;protocol=https"
SRCREV = "da3c66bbbf4aa616b15b18a378bdf2fd621b82c4"

S = "${WORKDIR}/git"

TARGET_CC_ARCH += "${LDFLAGS}"

do_compile() {
    ${CXX} main.cpp -o webcam_stream `pkg-config --cflags --libs gstreamer-1.0`
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 webcam_stream ${D}${bindir}
}