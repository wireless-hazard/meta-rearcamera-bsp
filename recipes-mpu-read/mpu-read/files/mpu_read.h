#include <unistd.h>
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
    const char *get_accel_raw(void)
    {
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
        }

        fclose(file_descriptor);

        return line;
    }

    private:
    FILE *file_descriptor = NULL;
    char *line = NULL;
};
}