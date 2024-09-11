SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI += "file://mpu-6050.cfg"