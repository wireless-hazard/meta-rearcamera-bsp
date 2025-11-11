#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio/consumer.h>

#define IO_REGULATOR 44 // CAM_GPIO0
#define IO_CAM_LED   40 // CAM_GPIO1

#define IO_OFFSET   512 // gpiochip512 corresponds to pinctrl-bcm2835

static struct gpio_desc *ov5647_reg = NULL;

static int __init my_init(void)
{
    ov5647_reg = gpio_to_desc(IO_REGULATOR + IO_OFFSET);
    
    if (ov5647_reg == NULL)
    {
        printk("Error getting the pin!\n");
        return -ENODEV;
    }

    int status = gpiod_direction_output(ov5647_reg, 0);
    
    if (status != 0)
    {
        printk("Error setting the pin as output!\n");
        return status;
    }

    gpiod_set_value(ov5647_reg, 1);

    return 0;
}

static void __exit my_exit(void)
{
    gpiod_set_value(ov5647_reg, 0);
    printk("goodbye!");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");