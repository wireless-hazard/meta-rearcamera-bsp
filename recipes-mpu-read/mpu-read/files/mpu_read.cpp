#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include "framebuffer_ssd1306.h"

namespace mpu
{
struct mpu_t {
    public:
    enum axis_t
    {x, y, z};

    mpu_t(void)
    {
        line = new char[10];
    }

    ~mpu_t(void)
    {
        delete[] line;
    }

    template<axis_t axis>
    int get_accel_raw(void)
    {
        int accel_raw = 0;

        if constexpr (axis == x)
        {
            file_descriptor = fopen("/sys/bus/i2c/devices/1-0068/iio:device0/in_accel_x_raw", "r");
        }
        else if constexpr (axis == y)
        {
            file_descriptor = fopen("/sys/bus/i2c/devices/1-0068/iio:device0/in_accel_y_raw", "r");
        }
        else
        {
            file_descriptor = fopen("/sys/bus/i2c/devices/1-0068/iio:device0/in_accel_z_raw", "r");
        }


        if (file_descriptor != NULL)
        {
            (void)fgets(line, 10, file_descriptor);
            accel_raw = std::atoi(line);
        }

        fclose(file_descriptor);

        return accel_raw;
    }

    private:
    FILE *file_descriptor = NULL;
    char *line = NULL;
};
}

static std::array<uint8_t, 8> oled_layer{0b10111101,0b10011001,0b11011011,0b11100111,0b11000011,0b10100101,0b00000000,0b10011001,};

int main(int argc, char *argv[])
{
    struct mpu::mpu_t mpu;
    struct fb_ssd::fb_ssd_t framebuffer;

    framebuffer.init();
    if (framebuffer.in_state_of_error)
    {
        printf("the framebuffer has NOT started!\nAn error occurred!");
    }
    printf("The screensize is: %ld\n", framebuffer.get_screensize());

    printf("Raw Accel x: %d\n", mpu.get_accel_raw<mpu::mpu_t::x>());
    printf("Raw Accel y: %d\n", mpu.get_accel_raw<mpu::mpu_t::y>());
    printf("Raw Accel z: %d\n", mpu.get_accel_raw<mpu::mpu_t::z>());

    const uint8_t current_column = std::atoi(argv[2]);
    const uint8_t current_line   = std::atoi(argv[1]);
    
    framebuffer.write_char_to_screen(oled_layer,current_line,current_column);

    return 0;
}