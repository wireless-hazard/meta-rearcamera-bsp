#include <unistd.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>
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

    std::string mpu_x;
    std::string mpu_y;
    std::string mpu_z;

    for (uint8_t count = 0; count < 40; count ++)
    {
        mpu_x = std::to_string(mpu.get_accel_raw<mpu::mpu_t::x>());
        mpu_y = std::to_string(mpu.get_accel_raw<mpu::mpu_t::y>());
        mpu_z = std::to_string(mpu.get_accel_raw<mpu::mpu_t::z>());

        framebuffer.write_string_to_screen(mpu_x.c_str(),{.line=0,.column=0});
        framebuffer.write_string_to_screen(mpu_y.c_str(),{.line=1,.column=0});
        framebuffer.write_string_to_screen(mpu_z.c_str(),{.line=2,.column=0});

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    return 0;
}