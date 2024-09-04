#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
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
        this->screensize = this->vinfo.xres/8 * this->vinfo.yres;
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

void fb_ssd_t::write_to_screen(size_t off_set, const void* src, size_t src_length)
{
    (void)std::memcpy((void *)(this->fbp + off_set), src, src_length);
}

fb_ssd_t::fb_error_t fb_ssd_t::write_char_to_screen(std::array<uint8_t, 8> &char_to_write, uint8_t line, uint8_t column)
{
    if ((column > 15) || (line > 7))
    {
        return FB_SSD_OUT_OF_RANGE;
    }

    for (size_t index = 0U; index < char_to_write.size(); index++)
    {
        *(this->fbp + (16*index) + (128*line) + column) = ~char_to_write[index];
    }

    return FB_SSD_OK;
}