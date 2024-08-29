#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <cstdlib>

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

int main(void)
{
    struct mpu::mpu_t mpu;

    for (uint8_t iterations = 0U; iterations < 10U; iterations++)
    {
        printf("Raw Accel x: %d\n", mpu.get_accel_raw<mpu::mpu_t::x>());
        printf("Raw Accel y: %d\n", mpu.get_accel_raw<mpu::mpu_t::y>());
        printf("Raw Accel z: %d\n", mpu.get_accel_raw<mpu::mpu_t::z>());
        sleep(1);
    }

    return 0;
}