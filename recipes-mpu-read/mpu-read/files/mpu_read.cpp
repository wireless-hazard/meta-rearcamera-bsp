#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <cstdlib>

struct mpu_t {
    public:

    mpu_t(void)
    {
        line = new char[10];
    }

    ~mpu_t(void)
    {
        delete[] line;
    }

    int get_accel_x_raw(void)
    {
        int accel_x_raw = 0;

        file_descriptor = fopen("/sys/bus/i2c/devices/1-0068/iio:device0/in_accel_x_raw", "r");
        if (file_descriptor != NULL)
        {
            (void)fgets(line, 10, file_descriptor);
            accel_x_raw = std::atoi(line);
        }

        fclose(file_descriptor);

        return accel_x_raw;
    }

    private:
    FILE *file_descriptor = NULL;
    char *line = NULL;
};

int main(void)
{
    struct mpu_t mpu;

    for (uint8_t iterations = 0U; iterations < 10U; iterations++)
    {
        printf("%d\n", mpu.get_accel_x_raw());
        sleep(1);
    }

    return 0;
}