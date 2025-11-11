SUMMARY = "GPIO Linux kernel module to use as test for ov5647 enablement"
DESCRIPTION = "${SUMMARY}"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

inherit module

SRC_URI = "file://Makefile \
          file://ov5647-regulator.c \
          "

S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.

RPROVIDES_${PN} += "kernel-module-ov5647-regulator"