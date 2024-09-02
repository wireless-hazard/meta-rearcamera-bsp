#include <linux/fb.h>

namespace fb_ssd
{

struct fb_ssd_t
{
    public:
    typedef enum {FB_SSD_OK, FB_SSD_ERR} fb_error_t;
    bool in_state_of_error = false;
    char *fbp;

    fb_ssd_t(void);
    ~fb_ssd_t(void);
    fb_error_t init(void);
    inline long int get_screensize(void) const {return screensize;}

    private:
    int fbfd;
    struct fb_var_screeninfo vinfo;
    long int screensize;

};

}