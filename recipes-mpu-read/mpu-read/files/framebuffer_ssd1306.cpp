#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "framebuffer_ssd1306.h"

static uint8_t oled_number0[8] = {0b11111111,0b11000011,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011,0b11000011,}; // 0
static uint8_t oled_number1[8] = {0b11111111,0b11101111,0b11101111,0b11101111,0b11101111,0b11101111,0b11101111,0b11101111,}; // 1
static uint8_t oled_number2[8] = {0b11111111,0b11000011,0b11011111,0b11011111,0b11000011,0b11111011,0b11111011,0b11000011,}; // 2
static uint8_t oled_number3[8] = {0b11111111,0b11000011,0b11011111,0b11011111,0b11000011,0b11011111,0b11011111,0b11000011,}; // 3
static uint8_t oled_number4[8] = {0b11111111,0b11011011,0b11011011,0b11011011,0b11000011,0b11011111,0b11011111,0b11011111,}; // 4
static uint8_t oled_number5[8] = {0b11111111,0b11000011,0b11111011,0b11111011,0b11000011,0b11011111,0b11011111,0b11000011,}; // 5
static uint8_t oled_number6[8] = {0b11111111,0b11000011,0b11111011,0b11111011,0b11000011,0b11011011,0b11011011,0b11000011,}; // 6
static uint8_t oled_number7[8] = {0b11111111,0b11000011,0b11011111,0b11011111,0b11011111,0b11011111,0b11011111,0b11011111,}; // 7
static uint8_t oled_number8[8] = {0b11111111,0b11000011,0b11011011,0b11011011,0b11100111,0b11011011,0b11011011,0b11000011,}; // 8
static uint8_t oled_number9[8] = {0b11111111,0b11000011,0b11011011,0b11011011,0b11000011,0b11011111,0b11011111,0b11000011,}; // 9
static uint8_t oled_numberN[8] = {0b11111111,0b11111111,0b11111111,0b11111111,0b11000011,0b11111111,0b11111111,0b11111111,}; // -
static uint8_t oled_numberERR[8] = {0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,}; // -

static uint8_t *lookup_table_chars_to_oled[] = {
    &oled_numberN[0], /*[45] = */
    &oled_numberERR[0],
    &oled_numberERR[0],
    &oled_number0[0],
    &oled_number1[0],
    &oled_number2[0],
    &oled_number3[0],
    &oled_number4[0],
    &oled_number5[0],
    &oled_number6[0],
    &oled_number7[0],
    &oled_number8[0],
    &oled_number9[0]    
    };

namespace fb_ssd 
{

static inline uint8_t *get_oled_char(const char *value)
{
    return lookup_table_chars_to_oled[(*value)-45];
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

    uint8_t *char_to_write = get_oled_char(single_char);

    for (size_t index = 0U; index < 8; index++)
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