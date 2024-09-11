SUMMARY = "Update wpa-supplicant.conf"
DESCRIPTION = "Adds default Access Point credentials to base wpa_supplicant.conf"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://wpa_supplicant.conf-sane"