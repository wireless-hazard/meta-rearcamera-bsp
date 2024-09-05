#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "framebuffer_ssd1306.h"

static std::array<uint8_t, 8> oled_number0{0b11111111,0b11100111,0b11011011,0b11011011,0b11011011,0b11011011,0b11100111,0b11111111,};
static std::array<uint8_t, 8> oled_number1{0b11111111,0b11111111,0b11110111,0b11100111,0b11110111,0b11110111,0b11110111,0b11111111,};
static std::array<uint8_t, 8> oled_number2{0b11111111,0b11000011,0b10010011,0b10110011,0b11100111,0b11001111,0b11000011,0b11111111,};
static std::array<uint8_t, 8> oled_number3{0b11111111,0b11111111,0b11100011,0b11111011,0b11110111,0b11111011,0b11100011,0b11111111,};
static std::array<uint8_t, 8> oled_number4{0b11111111,0b11111111,0b11101011,0b11101011,0b11100011,0b11111011,0b11111011,0b11111111,};
static std::array<uint8_t, 8> oled_number5{0b11111111,0b11111111,0b11100011,0b11101111,0b11100111,0b11111011,0b11100111,0b11111111,};
static std::array<uint8_t, 8> oled_number6{0b11111111,0b11111111,0b11110011,0b11101111,0b11100111,0b11101011,0b11110111,0b11111111,};
static std::array<uint8_t, 8> oled_number7{0b11111111,0b11111111,0b11100011,0b11111011,0b11110111,0b11101111,0b11101111,0b11111111,};
static std::array<uint8_t, 8> oled_number8{0b11111111,0b11100111,0b11011011,0b11011011,0b11100111,0b11011011,0b11011011,0b11100111,};
static std::array<uint8_t, 8> oled_number9{0b11111111,0b11111111,0b11110111,0b11101011,0b11110011,0b11111011,0b11100111,0b11111111,};
static std::array<uint8_t, 8> oled_numberN{0b11111111,0b11111111,0b11011011,0b10000001,0b10000001,0b11011111,0b11111111,0b11111111,};

namespace fb_ssd 
{

static std::array<uint8_t, 8> &get_oled_char(const char *value)
{
    switch (*value)
    {
        case '0':
            return oled_number0;
        case '1':
            return oled_number1;
        case '2':
            return oled_number2;
        case '3':
            return oled_number3;
        case '4':
            return oled_number4;
        case '5':
            return oled_number5;
        case '6':
            return oled_number6;
        case '7':
            return oled_number7;
        case '8':
            return oled_number8;
        case '9':
            return oled_number9;
        case '-':
            return oled_numberN;
    }
}

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

fb_ssd_t::fb_error_t fb_ssd_t::write_char_to_screen(const char *single_char, oled_position_t position)
{
    if ((position.column > 15) || (position.line > 7))
    {
        return FB_SSD_OUT_OF_RANGE;
    }

    std::array<uint8_t, 8> &char_to_write = get_oled_char(single_char);

    for (size_t index = 0U; index < char_to_write.size(); index++)
    {
        *(this->fbp + (16*index) + (128*position.line) + position.column) = ~char_to_write[index];
    }

    return FB_SSD_OK;
}

fb_ssd_t::fb_error_t fb_ssd_t::write_string_to_screen(const char *const_string, oled_position_t position)
{
    oled_position_t position_copy = position;

    while (*const_string != '\0')
    {
        (void)this->write_char_to_screen(const_string, position_copy);

        position_copy.column++;
        const_string++;
    }

    return FB_SSD_OK;
}

}