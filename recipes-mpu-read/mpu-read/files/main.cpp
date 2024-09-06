#include <string>
#include "mpu_read.h"
#include "framebuffer_ssd1306.h"

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

    const uint8_t current_column = std::atoi(argv[2]);
    const uint8_t current_line   = std::atoi(argv[1]);

    framebuffer.write_string_to_screen("Boa noite!",{.line=current_line,.column=current_column});
    framebuffer.write_string_to_screen("X: ",{.line=0,.column=0});
    framebuffer.write_string_to_screen("Y: ",{.line=1,.column=0});
    framebuffer.write_string_to_screen("Z: ",{.line=2,.column=0});

    while(true)
    {
        mpu_x = std::to_string(mpu.get_accel_raw<mpu::mpu_t::x>());
        mpu_y = std::to_string(mpu.get_accel_raw<mpu::mpu_t::y>());
        mpu_z = std::to_string(mpu.get_accel_raw<mpu::mpu_t::z>());

        framebuffer.write_string_to_screen(mpu_x.c_str(),{.line=0,.column=3});
        framebuffer.write_string_to_screen(mpu_y.c_str(),{.line=1,.column=3});
        framebuffer.write_string_to_screen(mpu_z.c_str(),{.line=2,.column=3});
    }

    return 0;
}