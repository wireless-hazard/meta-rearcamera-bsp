SUMMARY = "Update /etc/network/interfaces"
DESCRIPTION = "Adds configuration to start wlan0 by default to /etc/network/interfaces"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://interfaces"