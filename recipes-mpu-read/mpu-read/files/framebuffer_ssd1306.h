#include <linux/fb.h>
#include <stdint.h>
#include <array>

namespace fb_ssd
{

struct fb_ssd_t
{
    public:
    typedef enum {FB_SSD_OK, FB_SSD_ERR, FB_SSD_OUT_OF_RANGE} fb_error_t;
    bool in_state_of_error = false;
    char *fbp;

    fb_ssd_t(void);
    ~fb_ssd_t(void);
    fb_error_t init(void);
    void write_to_screen(size_t off_set, const void* src, size_t src_length);
    fb_error_t write_char_to_screen(std::array<uint8_t, 8> &char_to_write, uint8_t line, uint8_t column);
    inline long int get_screensize(void) const {return screensize;}

    private:
    int fbfd;
    struct fb_var_screeninfo vinfo;
    long int screensize;

};

}