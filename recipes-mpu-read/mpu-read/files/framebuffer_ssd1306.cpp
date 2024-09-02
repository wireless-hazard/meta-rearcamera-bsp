#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "framebuffer_ssd1306.h"

using namespace fb_ssd;

fb_ssd_t::fb_ssd_t(void)
{
    // Open the file for reading and writing
    this->fbfd = open("/dev/fb0", O_RDWR);

    if (this->fbfd != -1)
    {
        ioctl(fbfd, FBIOGET_VSCREENINFO, &this->vinfo);
        this->screensize = this->vinfo.xres * this->vinfo.yres;
    }
    else
    {
        // Sets the error flag
        this->in_state_of_error = true;
    }
    
}

fb_ssd_t::~fb_ssd_t(void)
{
    // Closes memory mapped and file descriptor
    munmap(this->fbp, this->screensize);
    close(this->fbfd);
}

fb_ssd_t::fb_error_t fb_ssd_t::init(void)
{
    fb_ssd_t::fb_error_t err = FB_SSD_ERR;

    if (!this->in_state_of_error)
    {
        // Maps the device to memory
        this->fbp = (char *) mmap(NULL, this->screensize, PROT_READ | PROT_WRITE, MAP_SHARED, this->fbfd, 0);

        if ((long) this->fbp != -1)
        {
            err = FB_SSD_OK;
        }
    }

    return err;
}