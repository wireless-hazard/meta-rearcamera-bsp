inherit devicetree

FILESEXTRAPATHS:prepend := "${THISDIR}/files/dts:"

FILES:${PN}:append = " /boot/overlays/*.dtb /boot/overlays/*.dtbo /arch/arm/boot/dts/overlays/*.dtbo"

SRC_URI = " \
    file://mpu6050.dts \
"

S = "${WORKDIR}"

devicetree_do_install:append() {
    for DTB_FILE in `ls *.dtb *.dtbo`; do
        install -Dm 0644 ${B}/${DTB_FILE} ${D}/boot/overlays/${DTB_FILE}
        install -Dm 0644 ${B}/${DTB_FILE} ${TMPDIR}/work/${MULTIMACH_TARGET_SYS}/linux-raspberrypi/6.6.22+git/linux-raspberrypi0-standard-build/arch/${ARCH}/boot/dts/overlays/${DTB_FILE}
    done
}

devicetree_do_deploy:append() {
    for DTB_FILE in `ls *.dtb *.dtbo`; do
        install -Dm 0644 ${B}/${DTB_FILE} ${DEPLOYDIR}/overlays/${DTB_FILE}
    done
}

# tmp/work/raspberrypi0-poky-linux-gnueabi/linux-raspberrypi/6.6.22+git/linux-raspberrypi0-standard-build/arch/arm/boot/dts/overlays/disable-wifi.dtbo

COMPATIBLE_MACHINE = "(.*)"