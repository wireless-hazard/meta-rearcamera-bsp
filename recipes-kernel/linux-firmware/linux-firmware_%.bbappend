SUMMARY = "Patch to linux-firmware"
DESCRIPTION = "Add link to the necessary files for brcmfmac"

# The kernel Module brcmfmac searches for brcmfmac43430-sdio.raspberrypi,model-zero-w.bin in lib/firmware/brcm to be able 
# to initialize the wireless interface device driver. 
# The actual binary brcmfmac needs is cypress/cyfmac43430-sdio.bin, so a simple link
# to it is created in the brcm folder, with the correct name expected by the brcmfmac kernel module

do_install:append() {
    cd ${D}/lib/firmware/brcm/
    ln -sf ../cypress/cyfmac43430-sdio.bin brcmfmac43430-sdio.raspberrypi,model-zero-w.bin 
}