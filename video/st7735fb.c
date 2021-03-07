#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fb.h>
#include <linux/gpio.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/regulator/consumer.h>
#include <linux/platform_device.h>

#ifdef CONFIG_OF
#include <linux/of_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#endif

#include "st7735fb.h"

#ifdef LCD_177_SIZE
static struct st7735_function st7735_cfg_script[] = {
	{ ST7735_START, ST7735_START},
	{ ST7735_CMD, ST7735_SLPOUT},
	{ ST7735_DELAY, 120},
	{ ST7735_CMD,ST7735_FRMCTR1},     
	{ ST7735_DATA,0x01},   
	{ ST7735_DATA,0x08},   
	{ ST7735_DATA,0x05},   

	{ ST7735_CMD, ST7735_FRMCTR2},     
	{ ST7735_DATA,0x01},   
	{ ST7735_DATA,0x08},   
	{ ST7735_DATA,0x05},   

	{ ST7735_CMD, ST7735_FRMCTR3},     
	{ ST7735_DATA, 0x01},   
	{ ST7735_DATA, 0x08},   
	{ ST7735_DATA, 0x05},   
	{ ST7735_DATA, 0x05},   
	{ ST7735_DATA, 0x08}, 
	{ ST7735_DATA, 0x05},   

	{ ST7735_CMD,ST7735_INVCTR},     //   inversion
	{ ST7735_DATA,0x03},   
	                           
	{ ST7735_CMD,ST7735_PWCTR1},     
	{ ST7735_DATA,0x28},   
	{ ST7735_DATA,0x08},   
	{ ST7735_DATA,0x04},   

	{ ST7735_CMD,ST7735_PWCTR2},     
	{ ST7735_DATA,0xC0},   

	{ ST7735_CMD,ST7735_PWCTR3},     
	{ ST7735_DATA,0x0D},   
	{ ST7735_DATA,0x00},   

	{ ST7735_CMD,ST7735_PWCTR4},    
	{ ST7735_DATA,0x8D},   
	{ ST7735_DATA,0x2A},   

	{ ST7735_CMD,ST7735_PWCTR5},     
	{ ST7735_DATA,0x8D},   
	{ ST7735_DATA,0xEE},   

	{ ST7735_CMD,ST7735_VMCTR1},     //VCOM
	{ ST7735_DATA,0x2d},   

	{ ST7735_CMD,ST7735_MADCTL},     //MX, MY, RGB mode
	{ ST7735_DATA,0xC0},   

	{ ST7735_CMD,ST7735_GMCTRP1},     
	{ ST7735_DATA, 0x07},   
	{ ST7735_DATA, 0x18},   
	{ ST7735_DATA, 0x0C},   
	{ ST7735_DATA, 0x15},   
	{ ST7735_DATA, 0x2E},   
	{ ST7735_DATA, 0x2A},   
	{ ST7735_DATA, 0x23},   
	{ ST7735_DATA, 0x28},   
	{ ST7735_DATA, 0x28},   
	{ ST7735_DATA, 0x28},   
	{ ST7735_DATA, 0x2E},   
	{ ST7735_DATA, 0x39},   
	{ ST7735_DATA, 0x00},  
	{ ST7735_DATA, 0x03},   
	{ ST7735_DATA, 0x02},  
	{ ST7735_DATA, 0x10},  

	{ ST7735_CMD,ST7735_GMCTRN1},    
	{ ST7735_DATA,0x06}, 
	{ ST7735_DATA,0x23}, 
	{ ST7735_DATA,0x0D}, 
	{ ST7735_DATA,0x17}, 
	{ ST7735_DATA,0x35}, 
	{ ST7735_DATA,0x30}, 
	{ ST7735_DATA,0x2A}, 
	{ ST7735_DATA,0x2D}, 
	{ ST7735_DATA,0x2C}, 
	{ ST7735_DATA,0x29}, 
	{ ST7735_DATA,0x31}, 
	{ ST7735_DATA,0x3B}, 
	{ ST7735_DATA,0x00}, 
	{ ST7735_DATA,0x02}, 
	{ ST7735_DATA,0x03}, 
	{ ST7735_DATA,0x12}, 

	{ ST7735_CMD,ST7735_PWCTR6},
	{ ST7735_DATA,0x8C},

	{ ST7735_CMD,ST7735_COLMOD},
	{ ST7735_DATA,0x05},  

	{ ST7735_CMD,ST7735_DISPON},
	{ ST7735_CMD,0x2C},
	{ ST7735_END, ST7735_END},
};
#else
static struct st7735_function st7735_cfg_script[] = {
	{ ST7735_START, ST7735_START},
	{ ST7735_CMD, ST7735_SLPOUT},
	{ ST7735_DELAY, 300},
	{ ST7735_CMD, ST7735_FRMCTR1},
	{ ST7735_DATA, 0x02},
	{ ST7735_DATA, 0x35},
	{ ST7735_DATA, 0x36},
	{ ST7735_CMD, ST7735_FRMCTR2},
	{ ST7735_DATA, 0x02},
	{ ST7735_DATA, 0x35},
	{ ST7735_DATA, 0x36},
	{ ST7735_CMD, ST7735_FRMCTR3},
	{ ST7735_DATA, 0x02},
	{ ST7735_DATA, 0x35},
	{ ST7735_DATA, 0x36},
	{ ST7735_DATA, 0x02},
	{ ST7735_DATA, 0x35},
	{ ST7735_DATA, 0x36},
	{ ST7735_CMD, ST7735_INVCTR},
	{ ST7735_DATA, 0x00},
	{ ST7735_CMD, ST7735_PWCTR1},
	{ ST7735_DATA, 0xa2},
	{ ST7735_DATA, 0x02},
	{ ST7735_DATA, 0x84},
	{ ST7735_CMD, ST7735_PWCTR2},
	{ ST7735_DATA, 0xc5},
	{ ST7735_CMD, ST7735_PWCTR3},
	{ ST7735_DATA, 0x0d},
	{ ST7735_DATA, 0x00},
	{ ST7735_CMD, ST7735_PWCTR4},
	{ ST7735_DATA, 0x8a},
	{ ST7735_DATA, 0x2a},
	{ ST7735_CMD, ST7735_PWCTR5},
	{ ST7735_DATA, 0x8d},
	{ ST7735_DATA, 0xee},
	{ ST7735_CMD, ST7735_VMCTR1},
	{ ST7735_DATA, 0x12},
	{ ST7735_CMD, ST7735_MADCTL},
	{ ST7735_DATA, 0x68},
	{ ST7735_CMD, ST7735_COLMOD},
	{ ST7735_DATA, 0x05},
	{ ST7735_CMD, ST7735_CASET},
	{ ST7735_DATA, 0x00},
	{ ST7735_DATA, 0x02},
	{ ST7735_DATA, 0x00},
	{ ST7735_DATA, 0x81},
	{ ST7735_CMD, ST7735_RASET},
	{ ST7735_DATA, 0x00},
	{ ST7735_DATA, 0x03},
	{ ST7735_DATA, 0x00},
	{ ST7735_DATA, 0xa2},
	{ ST7735_CMD, ST7735_GMCTRP1},
	{ ST7735_DATA, 0x12},
	{ ST7735_DATA, 0x1c},
	{ ST7735_DATA, 0x10},
	{ ST7735_DATA, 0x18},
	{ ST7735_DATA, 0x33},
	{ ST7735_DATA, 0x2c},
	{ ST7735_DATA, 0x25},
	{ ST7735_DATA, 0x28},
	{ ST7735_DATA, 0x28},
	{ ST7735_DATA, 0x27},
	{ ST7735_DATA, 0x2f},
	{ ST7735_DATA, 0x3c},
	{ ST7735_DATA, 0x00},
	{ ST7735_DATA, 0x03},
	{ ST7735_DATA, 0x03},
	{ ST7735_DATA, 0x10},
	{ ST7735_CMD, ST7735_GMCTRN1},
	{ ST7735_DATA, 0x12},
	{ ST7735_DATA, 0x1c},
	{ ST7735_DATA, 0x10},
	{ ST7735_DATA, 0x18},
	{ ST7735_DATA, 0x2d},
	{ ST7735_DATA, 0x28},
	{ ST7735_DATA, 0x23},
	{ ST7735_DATA, 0x28},
	{ ST7735_DATA, 0x28},
	{ ST7735_DATA, 0x26},
	{ ST7735_DATA, 0x2f},
	{ ST7735_DATA, 0x3b},
	{ ST7735_DATA, 0x00},
	{ ST7735_DATA, 0x03},
	{ ST7735_DATA, 0x03},
	{ ST7735_DATA, 0x10},
	{ ST7735_CMD, ST7735_DISPON},
	{ ST7735_END, ST7735_END},
};
#endif
#define LEADT15DS26_SET_SLEEPMODE       _IOW('L', 1, unsigned int) /* enter or exit sleep-mode */
#define LEADT15DS26_SET_BACKLIGHT       _IOW('L', 2, unsigned int) /* turn on or off backlight */
#define LEADT15DS26_SET_BRIGHTNESS      _IOW('L', 3, unsigned int) /* set brightness value */
#define LEADT15DS26_FILLRECT      		_IOW('L', 4, unsigned int) /* fill rect region */

static u32  lcd_width = WIDTH;
static u32  lcd_height = HEIGHT;	
u32 direction_value = 0; 
static struct fb_fix_screeninfo st7735fb_fix;
static struct fb_var_screeninfo st7735fb_var;
char *picture= NULL;
//static u32 fb_poweron_flag = 1;

static u16 *real_mem;

#define LCD_144C034_NAME "quectel_lcd_144C034"
static int lcd_display_time_debug = 1;
static int lcd_attr_x_debug = 2;
static int lcd_attr_y_debug = 1;

struct _quectel_lcd_144C034{
	char name[20];
	struct class sdv_class;
	struct st7735fb_par *par;
} *quectel_lcd_144C034 = NULL;

void show_test_pure_color(unsigned short color);

static void set_st7735_cfg_array(u16 modify_cmd,u16 modify_data)
{
	//printk("%s\n", __func__);
	int i = 0;
	int end_script = 0;
	do {
		switch (st7735_cfg_script[i].cmd) {
		case ST7735_START:
			break;
		case ST7735_CMD:
			if(st7735_cfg_script[i].data == modify_cmd)
			{	
				st7735_cfg_script[i+1].data = modify_data;
				end_script = 1;
			}
			break;
		case ST7735_DATA:
			break;
		case ST7735_DELAY:
			break;
		case ST7735_END:
			end_script = 1;
		}
		i++;
	} while (!end_script);
}

static int st7735_write(struct st7735fb_par *par, u8 cmd_data, u8 data)
{
	struct spi_message	m;
	struct spi_transfer	xfer;
	u8			w = data;
	int			r;
	//printk("%s\n", __func__);
	spi_message_init(&m);
	memset(&xfer, 0, sizeof(xfer));
#if 0
	w = (cmd_data == 1) ? ((0x100) | data) : data;
	xfer.tx_buf = &w;
	xfer.bits_per_word = 8;
	xfer.len = 2;
#else
	xfer.tx_buf = &w;
	xfer.bits_per_word = 8;
	xfer.len = 1;
#endif
	spi_message_add_tail(&xfer, &m);
	r = spi_sync(par->spi, &m);
	if (r < 0)
		dev_warn(&par->spi->dev,  "failed to write to LCD reg (%d)\n",
				r);

	return r;
}

static void st7735_write_data(struct st7735fb_par *par, u8 data)
{
	int ret = 0;
	//printk("%s\n", __func__);
	gpio_set_value(par->a0, 1);
	//gpio_set_value(par->cs, 0);
	ret = st7735_write(par, 1, data);
	if (ret < 0)
		pr_err("%s: write data %02x failed with status %d\n",
			par->info->fix.id, data, ret);
	//gpio_set_value(par->cs, 1);
}

static int st7735_write_data_buf(struct st7735fb_par *par, u16 *txbuf, int size)
{

	struct spi_message	m;
	struct spi_transfer	xfer;
	int			ret;
	//printk("%s\n", __func__);
#if 0
	int i;
	#define SPI_BUFSIZ 16
	gpio_set_value(par->a0, 1);
	//gpio_set_value(par->cs, 0);
	for(i = 0; i < size; i+=SPI_BUFSIZ)
	{
		spi_message_init(&m);
	
		memset(&xfer, 0, sizeof(xfer));
		xfer.bits_per_word = 8;
		if((size-i) >= SPI_BUFSIZ)
		{
			xfer.tx_buf = (u8 *)(txbuf+i);
			xfer.len = SPI_BUFSIZ*2;
		}
		else
		{
			xfer.tx_buf = (u8 *)(txbuf+i);
			xfer.len = (size-i)*2;
		}
		spi_message_add_tail(&xfer, &m);
	
		ret = spi_sync(par->spi, &m);
		if (ret < 0)
			dev_warn(&par->spi->dev, "failed to write to LCD reg (%d)\n",
				ret);
	}
#else
	gpio_set_value(par->a0, 1);
	//gpio_set_value(par->cs, 0);
		spi_message_init(&m);
	
		memset(&xfer, 0, sizeof(xfer));
		xfer.bits_per_word = 8;

		xfer.tx_buf = txbuf;
		xfer.len = size*2;

		spi_message_add_tail(&xfer, &m);
	
		ret = spi_sync(par->spi, &m);
		if (ret < 0)
			dev_warn(&par->spi->dev, "failed to write to LCD reg (%d)\n",
				ret);
#endif
	//gpio_set_value(par->cs, 1);
	return 0;
}

static void st7735_write_cmd(struct st7735fb_par *par, u8 data)
{
	int ret = 0;
	//printk("%s\n", __func__);
	gpio_set_value(par->a0, 0);
	ret = st7735_write(par, 0, data);
	if (ret < 0)
		pr_err("%s: write command %02x failed with status %d\n",
			par->info->fix.id, data, ret);
}

static void __maybe_unused st7735_run_cfg_script(struct st7735fb_par *par)
{
	int i = 0;
	int end_script = 0;
	//printk("%s\n", __func__);

	do {
		switch (st7735_cfg_script[i].cmd) {
		case ST7735_START:
			break;
		case ST7735_CMD:
			st7735_write_cmd(par,
				st7735_cfg_script[i].data & 0xff);
			break;
		case ST7735_DATA:
			st7735_write_data(par,
				st7735_cfg_script[i].data & 0xff);
			break;
		case ST7735_DELAY:
			msleep(st7735_cfg_script[i].data);
			break;
		case ST7735_END:
			end_script = 1;
		}
		i++;
	} while (!end_script);
}

static void st7735_set_addr_win(struct st7735fb_par *par,
				int xs, int ys, int xe, int ye)
{
	int sx = xs+lcd_attr_x_debug, ex = xe-1+lcd_attr_x_debug;
	int sy = ys+lcd_attr_y_debug, ey = ye-1+lcd_attr_y_debug;
	//printk("%s\n", __func__);
//	printk("xs=%d xe=%d\n", sx, ex);
//	printk("ys=%d ye=%d\n", sy, ey);
	//st7735_write_cmd(par, ST7735_DISPON);
	st7735_write_cmd(par, ST7735_CASET);
	st7735_write_data(par, 0x00);
#ifdef LCD_177_SIZE
	st7735_write_data(par, sx);
#else
	st7735_write_data(par, xs+2);
#endif

	st7735_write_data(par, 0x00);
#ifdef LCD_177_SIZE
	st7735_write_data(par, ex);
#else
	st7735_write_data(par, xe+2);
#endif

	st7735_write_cmd(par, ST7735_RASET);
	st7735_write_data(par, 0x00);
#ifdef LCD_177_SIZE
	st7735_write_data(par, sy);
#else
	st7735_write_data(par, ys+3);
#endif

	st7735_write_data(par, 0x00);
#ifdef LCD_177_SIZE
	st7735_write_data(par, ey);
#else
	st7735_write_data(par, ye+3);
#endif
}

static void st7735_reset(struct st7735fb_par *par)
{
	//printk("%s\n", __func__);
	/* Reset controller */
	gpio_set_value(par->rst, 1);
	usleep_range(10000, 10100);
	gpio_set_value(par->rst, 0);
	usleep_range(10000, 10100);
	gpio_set_value(par->rst, 1);
	usleep_range(120000, 120100);
}

static void st7735_reset_on(struct st7735fb_par *par)
{
	printk("%s\n", __func__);
	gpio_set_value(par->rst, 0);
}

static void st7735fb_update_display(struct st7735fb_par *par)
{
	int ret = 0;
	//printk("%s\n", __func__);
#if 0
	int x, y;
	u8 *vmem = par->info->screen_base;
	u16 *vmem16 = (u16 *)real_mem;
	for (x = 0; x < lcd_width; x++) {
		for (y = 0; y < lcd_height; y++) {
			*(u16 *)(vmem16++) = 0x100 | *(u8 *)(vmem+1);
			*(u16 *)(vmem16++) = 0x100 | *(u8 *)(vmem);
			vmem += 2;
		}
	}
#endif
	st7735_set_addr_win(par, 0, 0, lcd_width, lcd_height);
	st7735_write_cmd(par, ST7735_RAMWR);

	ret = st7735_write_data_buf(par, (u16 *)real_mem, lcd_width*lcd_height*BPP/8/2);

	if (ret < 0)
		pr_err("%s: spi_write failed to update display buffer\n",
			par->info->fix.id);
}

static void st7735fb_display_picture(u8 p_x, u8 line, u8 W, u8 H, u8 *buf, struct st7735fb_par *par)
{
	int size;
	//printk("%s\n", __func__);
	size = W*H*BPP/8;
	//memcpy(real_mem, buf, size);
	st7735_set_addr_win(par, p_x, line, (p_x+W), (line+H));
	st7735_write_cmd(par, ST7735_RAMWR);

	st7735_write_data_buf(par, (u16 *)buf, size/2);
}


static void st7735fb_draw_logo(struct st7735fb_par *par){
	//printk("%s\n", __func__);
	if(direction_value == 1)
	{
		st7735fb_display_picture(0,0,lcd_width,lcd_height,(u8 *)logo_dlink,par);
	}	
}

static int st7735fb_init_display(struct st7735fb_par *par)
{
	/* TODO: Need some error checking on gpios */
	int error;
	gpio_request_one(par->rst, GPIOF_OUT_INIT_HIGH,
			"ST7735 Reset Pin");
	gpio_request_one(par->a0, GPIOF_OUT_INIT_HIGH,
			"ST7735 A0 Pin");
	//gpio_request_one(par->cs, GPIOF_OUT_INIT_HIGH,
	//		"ST7735 CS Pin");
	//printk("%s\n", __func__);
	if(par->lcd_avdd_status)
	{
		regulator_set_voltage(par->lcd_avdd, 3300000, 3300000);
		error = regulator_enable(par->lcd_avdd);
		if (error < 0) {
			pr_err("%s: lcd_iovdd(3.2v) enalbe failed!\n",
				__func__);
			gpio_free(par->rst);
			return -1;
		}
	}

	//st7735_reset(par);

	st7735_run_cfg_script(par);
	//st7735_set_addr_win(quectel_lcd_144C034->par, 0, 0, lcd_width, lcd_height);
	st7735fb_draw_logo(par);
	usleep_range(150000, 150100);
	//msleep(10);
	if(!wake_lock_active(&par->wake_lock)){
		wake_lock(&par->wake_lock);
	}
	gpio_request_one(par->bl, GPIOF_OUT_INIT_HIGH,
			"ST7735 BL Pin");
    gpio_set_value(par->bl,1);
	
	#ifdef COMPATIBLE_BL
	gpio_request_one(par->bl_46, GPIOF_OUT_INIT_HIGH,
			"ST7735 Compatible bl pin");
	#endif
	return 0;
}

static int st7735fb_reinit(void)
{
	int error;
	printk("\n%s\n", __func__);

	//printk("%s\n", __func__);
	if(quectel_lcd_144C034->par->lcd_avdd_status)
	{
		regulator_set_voltage(quectel_lcd_144C034->par->lcd_avdd, 3300000, 3300000);
		error = regulator_enable(quectel_lcd_144C034->par->lcd_avdd);
		if (error < 0) {
			pr_err("%s: lcd_iovdd(3.2v) enalbe failed!\n",
				__func__);
			return -1;
		}
	}
	st7735_reset(quectel_lcd_144C034->par);
	st7735_run_cfg_script(quectel_lcd_144C034->par);
	//usleep_range(150000, 150100);
	return 0;
}

static int st7735_power_on_status = 1;
static int st7735_power_on(struct st7735fb_par *par)
{
	if(direction_value == 1)
	{
		if(!wake_lock_active(&par->wake_lock)){
			wake_lock(&par->wake_lock);
		}
	}
	
	printk("%s\n", __func__);

	st7735fb_reinit();
	st7735_power_on_status = 1;

	return 0;
}

static int st7735_power_off(struct st7735fb_par *par){
	int ret = 0;
	printk("%s\n", __func__);

	show_test_pure_color(0x0000);

	if(par->lcd_avdd_status)
		ret = regulator_disable(par->lcd_avdd);

	st7735_reset_on(par);
	st7735_power_on_status = 0;

	if(!par){
		printk("%s:rjxu lcd is null\n", __func__);
		return -1;
	}
	if(direction_value == 1)
	{
		if(wake_lock_active(&par->wake_lock)){
			wake_unlock(&par->wake_lock);
		}
	}
	return ret;
}

static int st7735_power_is_on(int power)
{
	//printk("%s\n", __func__);
	return power <= FB_BLANK_NORMAL;
}

static int st7735_power(struct st7735fb_par *par, int power){
	int ret = 0;
	printk("%s\n", __func__);
	if(st7735_power_is_on(power) && !st7735_power_is_on(par->power))
		ret = st7735_power_on(par);
		//ret = st7735fb_init_display(par);
	else if(!st7735_power_is_on(power) && st7735_power_is_on(par->power))
		ret = st7735_power_off(par);
	if(!ret)
		par->power = power;
	return ret;
}

#if 0
static int st7735fb_blank(int blank, struct fb_info *info){
	struct st7735fb_par *par = info->par;
	int ret = 0;
	//printk("%s\n", __func__);
	//if(fb_poweron_flag == 0)
		//return ret;
	switch(blank){
		case FB_BLANK_UNBLANK:
			ret = st7735_power(par, FB_BLANK_UNBLANK);
			break;
		case FB_BLANK_NORMAL:
		case FB_BLANK_VSYNC_SUSPEND:
		case FB_BLANK_HSYNC_SUSPEND:
		case FB_BLANK_POWERDOWN:
			ret = st7735_power(par, FB_BLANK_POWERDOWN);
			break;
	}
	return ret;
}
#endif

static ssize_t st7735fb_write(struct fb_info *info, const char __user *buf,
		size_t count, loff_t *ppos)
{
	struct st7735fb_par *par = info->par;
	unsigned long p = *ppos;
	void *dst;
	int err = 0;
	unsigned long total_size;
	
	//if(fb_poweron_flag == 0)
		//return -EPERM;
	printk("%s\n", __func__);
	if (info->state != FBINFO_STATE_RUNNING)
		return -EPERM;

	total_size = info->fix.smem_len;

	if (p > total_size)
		return -EFBIG;

	if (count > total_size) {
		err = -EFBIG;
		count = total_size;
	}

	if (count + p > total_size) {
		if (!err)
			err = -ENOSPC;

		count = total_size - p;
	}

	//dst = (void __force *) (info->screen_base + p);
	dst = real_mem;

	if (copy_from_user(dst, buf, count))
		err = -EFAULT;

	if  (!err)
		*ppos += count;

	st7735fb_update_display(par);

	return (err) ? err : count;
}

int lcd_display_func(const char * data, unsigned int yoffset,unsigned int line_length)
{
	char *frame_data =NULL;
	
	int i, len = lcd_width*lcd_height;
	if(!data){
		return 0;
	}

	if(!picture){
		printk(" malloc failed \n");
		return 0;
	}
	if(yoffset > 0){ // use frame buffer 2
		frame_data = (char *)(data + yoffset*line_length);
	}else if(yoffset == 0){ // use frame buffer 1
		frame_data = (char *)data;
	}
	
	//Copy data to picture buffer
#if 0
	memcpy(picture,frame_data,lcd_width*lcd_height*BPP/8);
#else
    //little endian and big endian
	for(i = 0; i < len; i++)
	{
		picture[2*i+1] = frame_data[2*i];
		picture[2*i] = frame_data[2*i+1];
	}
#endif
	memcpy(real_mem, picture, lcd_width*lcd_height*BPP/8);
	st7735fb_display_picture(0,0,lcd_width,lcd_height,(u8 *)picture,quectel_lcd_144C034->par);
	return 0;
}

static int st7735fb_pan_display(struct fb_var_screeninfo *var, struct fb_info *info)
{
	//if(fb_poweron_flag == 0)
	//	return 0;
	if(st7735_power_on_status)
		lcd_display_func(info->screen_base, var->yoffset, info->fix.line_length);

	return 0;
}
	
ssize_t st7735fb_read(struct fb_info *info, char __user *buf,
		   size_t count, loff_t *ppos)
{
	printk("%s\n", __func__);
	//if(fb_poweron_flag == 0)
	//	return 0;
	if(copy_to_user(buf, real_mem, count))
	{}

	{
		int i,j, index = 0;
		for(i = 0; i < lcd_width; i++)
		{
			for(j = 0; j < lcd_height*BPP/8; j++)
			{
				if((j%8) == 0)
					printk("\n");
				index = i*(lcd_height*BPP/8) + j;
				printk("%02X ", real_mem[index]);
			}
		}
		printk("\n");
	}
	return 0;
}

int tft144C034fb_open(struct fb_info *info, int user)
{
	printk("%s\n", __func__);
	return 0;
}

int tft144C034fb_release(struct fb_info *info, int user)
{
	printk("%s\n", __func__);
	return 0;
}

int tft144C034fb_mmap(struct fb_info *info, struct vm_area_struct *vma)
{
	unsigned long size, offset;

	printk("%s\n", __func__);
	//if(fb_poweron_flag == 0)
	//	return 0;
	size = vma->vm_end - vma->vm_start;
	offset = vma->vm_pgoff << PAGE_SHIFT;
	if (offset + size > info->fix.smem_len)
		return -EINVAL;

	offset += info->fix.smem_start;
	if (remap_pfn_range(vma, vma->vm_start, offset >> PAGE_SHIFT,
			    size, vma->vm_page_prot))
		return -EAGAIN;

	printk(" mmap framebuffer P(%lx)->V(%lx)\n",offset, vma->vm_start);

	return 0;	
}

static int st7735fb_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg)
{
    unsigned char ioctl_arg;
    struct st7735fb_par *par = info->par;

    switch (cmd)
    {
        case LEADT15DS26_SET_SLEEPMODE:
        {
            if (copy_from_user(&ioctl_arg, (char *)arg, sizeof(unsigned char)))
        	{	
	            return -EFAULT;
        	}
            //leadt15ds26fb_set_sleep(info, (bool)ioctl_arg);
            break;
        }
        case LEADT15DS26_SET_BACKLIGHT:
        {
            if (copy_from_user(&ioctl_arg, (char *)arg, sizeof(unsigned char)))
			{
				return -EFAULT;
			}
            //leadt15ds26fb_set_backlight(info, (bool)ioctl_arg);
			if(ioctl_arg)
				gpio_set_value(par->bl,1);
			else
				gpio_set_value(par->bl,0);
            break;
        }
        case LEADT15DS26_SET_BRIGHTNESS:
        {
            if (copy_from_user(&ioctl_arg, (char *)arg, sizeof(unsigned char)))
			{
				return -EFAULT;
			}
            //leadt15ds26fb_set_brightness(info, ioctl_arg);
            break;
        }
        case LEADT15DS26_FILLRECT:
        {/*
            if (copy_from_user(&disp_rect, (void *)arg, sizeof(struct fb_fillrect)))
			{
				return -EFAULT;
			}
			
		if(disp_rect.width <= 320 && disp_rect.height <=320)
			{
            	leadt15ds26fb_fillrect(info, &disp_rect);
			}*/
            break;
        }
    default:
        return -EINVAL;
    }
    return 0;
}

static struct fb_ops st7735fb_ops = {
	.owner		= THIS_MODULE,
	.fb_open		= tft144C034fb_open ,
	.fb_release		= tft144C034fb_release ,
	.fb_read	= st7735fb_read,
	.fb_write	= st7735fb_write,
	.fb_fillrect	= cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_mmap		= tft144C034fb_mmap,
	.fb_imageblit	= cfb_imageblit,
	.fb_ioctl       = st7735fb_ioctl,
	//.fb_blank 		= st7735fb_blank,
	.fb_pan_display = st7735fb_pan_display,
};

static void st7735fb_deferred_io(struct fb_info * info,
	struct list_head * pagelist){
	//printk("%s\n", __func__);
	//st7735fb_update_display(info->par);
}

static struct fb_deferred_io st7735fb_defio = {
	.delay 			= HZ,
	.deferred_io 	= st7735fb_deferred_io,
};

void show_test_pure_color(unsigned short color)
{
	int j=0, i=0;
	unsigned char *ptr;
	int size;
	
	st7735_set_addr_win(quectel_lcd_144C034->par, 0, 0, lcd_width, lcd_height);
	
	printk("show_test_pure_color : 0x%04X\n", color);
	//printk("real_mem : 0x%X\n", real_mem);
	for(j=0; j < lcd_height; j++)
	{
		ptr = (unsigned char *)real_mem + (j*lcd_width)*BPP/8;
		for(i = 0; i <= lcd_width; i++)
		{
			*ptr++ = color>>8;
			*ptr++ = color&0xFF;
		}
	}
	size = lcd_width*lcd_height*BPP/8;
	st7735_write_cmd(quectel_lcd_144C034->par, ST7735_RAMWR);
	st7735_write_data_buf(quectel_lcd_144C034->par, real_mem, size/2);
}

static ssize_t get_lcd_display_test(struct class *class, struct class_attribute *attr,char *buf)
{
	printk(KERN_INFO "%s %d color(5:6:5) red:green:blue\n", __FUNCTION__, __LINE__);

	show_test_pure_color(0xF800);
	msleep(lcd_display_time_debug*1000);
	show_test_pure_color(0x07E0);
	msleep(lcd_display_time_debug*1000);
	show_test_pure_color(0x001F);
	msleep(lcd_display_time_debug*1000);
	show_test_pure_color(0x0000);
	msleep(lcd_display_time_debug*1000);
	show_test_pure_color(0xFFFF);

	return 0;
}

static ssize_t set_lcd_display_test(struct class *class, struct class_attribute *attr,
			const char *buf, size_t count)
{
  ssize_t	status;
  u32 value;

  status = sscanf(buf, "%d", &value);

  if (status != -1) {
		lcd_display_time_debug = value;
  }
	return count;
}

static ssize_t get_lcd_attrx_test(struct class *class, struct class_attribute *attr,char *buf)
{
	printk("lcd_attr_x_debug=%d\n", lcd_attr_x_debug);
	return 0;
}

static ssize_t set_lcd_attrx_test(struct class *class, struct class_attribute *attr,
			const char *buf, size_t count)
{
  ssize_t	status;
  u32 value;

  status = sscanf(buf, "%d", &value);

  if (status != -1) {
		lcd_attr_x_debug = value;
  }
	return count;
}

static ssize_t get_lcd_attry_test(struct class *class, struct class_attribute *attr,char *buf)
{
	printk("lcd_attr_y_debug=%d\n", lcd_attr_y_debug);
	return 0;
}

static ssize_t set_lcd_attry_test(struct class *class, struct class_attribute *attr,
			const char *buf, size_t count)
{
  ssize_t	status;
  u32 value;

  status = sscanf(buf, "%d", &value);

  if (status != -1) {
		lcd_attr_y_debug = value;
  }
	return count;
}

static ssize_t get_lcd_display_on(struct class *class, struct class_attribute *attr,char *buf)
{
	st7735_power(quectel_lcd_144C034->par, FB_BLANK_UNBLANK);

	return 0;
}

static ssize_t get_lcd_display_off(struct class *class, struct class_attribute *attr,char *buf)
{
	st7735_power(quectel_lcd_144C034->par, FB_BLANK_POWERDOWN);
	return 0;
}

static ssize_t get_lcd_display_logo(struct class *class, struct class_attribute *attr,char *buf)
{
	st7735fb_draw_logo(quectel_lcd_144C034->par);
	return 0;
}

static ssize_t get_lcd_reset(struct class *class, struct class_attribute *attr,char *buf)
{
  //printk("%s\n", __func__);
  ssize_t	status;
  int value;
	//st7735_reset(quectel_lcd_144C034->par);
	value = gpio_get_value(quectel_lcd_144C034->par->rst);
	status = sprintf(buf, "%d\n", value);
	return status;
}

static ssize_t set_lcd_reset(struct class *class, struct class_attribute *attr,
			const char *buf, size_t count)
{
  //printk("%s\n", __func__);
  ssize_t	status;
  u32 value;

  status = sscanf(buf, "%d", &value);

	gpio_set_value(quectel_lcd_144C034->par->rst, !!value);
	return count;
}

static ssize_t get_lcd_bl(struct class *class, struct class_attribute *attr,char *buf)
{
  //printk("%s\n", __func__);
  ssize_t	status;
  int value;
	//st7735_reset(quectel_lcd_144C034->par);
	value = gpio_get_value(quectel_lcd_144C034->par->bl);
	status = sprintf(buf, "%d\n", value);
	return status;
}

static ssize_t set_lcd_bl(struct class *class, struct class_attribute *attr,
			const char *buf, size_t count)
{
  ssize_t	status;
  u32 value;
  
  usleep_range(50000, 50100);

  status = sscanf(buf, "%d", &value);

  printk("%s value=%d\n", __func__, value);

	gpio_set_value(quectel_lcd_144C034->par->bl, !!value);
	return count;
}

static ssize_t get_lcd_init(struct class *class, struct class_attribute *attr,char *buf)
{
	st7735fb_reinit();
	return 0;
}

static int s_st7735_poweron = 1;
static ssize_t set_lcd_poweron(struct class *class, struct class_attribute *attr,
			const char *buf, size_t count)
{
  ssize_t	status;
  u32 value;

  status = sscanf(buf, "%d", &value);
  printk("%s\n value=%d", __func__, value);
	if(value)
	{
		s_st7735_poweron = 1;
		//fb_poweron_flag = 1;
		st7735_power(quectel_lcd_144C034->par, FB_BLANK_UNBLANK);
	}
	else
	{
		s_st7735_poweron = 0;
		//fb_poweron_flag = 0;
		st7735_power(quectel_lcd_144C034->par, FB_BLANK_POWERDOWN);
	}
		
	return count;
}

static ssize_t get_lcd_poweron(struct class *class, struct class_attribute *attr,char *buf)
{
  ssize_t	status;
  //int value;
  printk("%s\n", __func__);
	status = sprintf(buf, "%d\n", s_st7735_poweron);
	return status;
}

struct class_attribute lcd_144C034_attribute[]={
	__ATTR(display_test,S_IRUGO|S_IWUSR,get_lcd_display_test,set_lcd_display_test),
	__ATTR(display_on,S_IRUGO|S_IWUSR,get_lcd_display_on,NULL),
	__ATTR(display_off,S_IRUGO|S_IWUSR,get_lcd_display_off,NULL),
	__ATTR(display_logo,S_IRUGO|S_IWUSR,get_lcd_display_logo,NULL),
	__ATTR(display_attr_x_test,S_IRUGO|S_IWUSR,get_lcd_attrx_test,set_lcd_attrx_test),
	__ATTR(display_attr_y_test,S_IRUGO|S_IWUSR,get_lcd_attry_test,set_lcd_attry_test),
	__ATTR(display_reset,S_IRUGO|S_IWUSR,get_lcd_reset,set_lcd_reset),
	__ATTR(display_bl,S_IRUGO|S_IWUSR,get_lcd_bl,set_lcd_bl),
	__ATTR(display_init,S_IRUGO|S_IWUSR,get_lcd_init,NULL),
	__ATTR(display_poweron,S_IRUGO|S_IWUSR,get_lcd_poweron,set_lcd_poweron),
	__ATTR_NULL
};

int tft144C034_alloc_buffer(void)
{
	//printk("%s\n", __func__);
	quectel_lcd_144C034 = kzalloc(sizeof(*quectel_lcd_144C034), GFP_KERNEL);
	if (quectel_lcd_144C034 == NULL) {
	   printk("failed to allocate memory for quectel_lcd_144C034\n");
	   return -ENOMEM;
	}
#if 1
	real_mem = kzalloc(lcd_width*lcd_height*BPP/8, GFP_KERNEL);
	if (real_mem == NULL) {
	   printk("failed to allocate memory for real_mem\n");
	   return -ENOMEM;
	}
#endif
	return 0;
}

void tft144C034_free_buffer(void)
{
	//printk("%s\n", __func__);
	if(real_mem)
		kfree(real_mem);

	if(quectel_lcd_144C034)
		kfree(quectel_lcd_144C034);
}

#ifdef CONFIG_PM_SLEEP
static int st7735fb_suspend(struct device *dev)
{
	//int ret = 0;
	printk("\n%s\n", __func__);

	return 0;
}

static int st7735fb_resume(struct device *dev)
{
	printk("\n%s\n", __func__);
	

	if(direction_value == 1)
	{
		lcd_attr_x_debug = 2;
		lcd_attr_y_debug = 1;
	}
	
	//st7735fb_reinit();

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(st7735fb_pm_ops, st7735fb_suspend, st7735fb_resume);

static int st7735fb_probe (struct spi_device *spi)
{
	struct device_node *np = spi->dev.of_node;
	struct fb_info *info;
	struct st7735fb_par *par;
	int vmem_size = lcd_width * lcd_height * BPP / 8;
	u8 *vmem;
	int retval = -ENOMEM, i;
	//u32 *direction = NULL;
	u32 direction = 0;
	u32 tmp_lcd_size = 0;
	int j, len = lcd_width*lcd_height;
	//printk("%s\n", __func__);
	vmem = vzalloc(vmem_size);
	if (!vmem)
		return retval;

	retval = tft144C034_alloc_buffer();
	if (retval < 0) {
	   goto tft144C034_fail;
	}

	quectel_lcd_144C034->sdv_class.name = LCD_144C034_NAME;
	quectel_lcd_144C034->sdv_class.class_attrs = lcd_144C034_attribute;
	class_register(&quectel_lcd_144C034->sdv_class);
	
	picture = kzalloc((lcd_width*lcd_height*BPP/8)*sizeof(char),GFP_KERNEL);
	
	info = framebuffer_alloc(sizeof(struct st7735fb_par), &spi->dev);
	if (!info)
		goto fballoc_fail;
	//info->fix.smem_start = (unsigned long)vmem;
	info->screen_base = (u8 __force __iomem *)vmem;
	info->fbops = &st7735fb_ops;
	//info->fix = st7735fb_fix;
	//info->fix.smem_len = (unsigned long)vmem_size;
	
	info->var = st7735fb_var;
	info->fbdefio = &st7735fb_defio;
	/* Choose any packed pixel format as long as it's RGB565 */
	//info->var.red.offset = 11;
	//info->var.red.length = 5;
	//info->var.green.offset = 5;
	//info->var.green.length = 6;
	//info->var.blue.offset = 0;
	//info->var.blue.length = 5;
	//info->var.transp.offset = 0;
	//info->var.transp.length = 0;
	info->flags = FBINFO_FLAG_DEFAULT | FBINFO_VIRTFB;
	quectel_lcd_144C034->par = info->par;
	par = info->par;
	
	for (i = 0; i < 256; i++)
		par->palette_buffer[i] = PALETTE_BUFF_CLEAR;
	info->pseudo_palette      = &par->pseudo_pal;
	
	//par->info = info;
	par->spi = spi;
	//fb_deferred_io_init(info);
	//if (IS_ENABLED(CONFIG_OF)) {
		par->a0 = of_get_named_gpio(np, "lcd_a0", 0);
		par->a0 = 929;
		if (par->a0 < 0) {
			pr_err("%s: of_get_named_gpio failed\n", __func__);
			return -ENOMEM;
		}

		par->rst = of_get_named_gpio(np, "lcd_rst", 0);
		par->rst = 1006;
		if (par->rst < 0) {
			pr_err("%s: of_get_named_gpio failed\n", __func__);
			return -ENOMEM;
		}

		par->bl = of_get_named_gpio(np, "lcd_bl", 0);
		par->bl = 1016;
		if (par->bl < 0) {
			pr_err("%s: of_get_named_gpio failed\n", __func__);
			return -ENOMEM;
		}

		if (of_get_property(np, "avdd-supply", 0)) {
			par->lcd_avdd_status = 1;
			par->lcd_avdd = regulator_get(&spi->dev, "avdd");
			if (IS_ERR(par->lcd_avdd)) {
				pr_err("%s regulator get error!\n", __func__);
				return -ENOMEM;
			}
		}
		else
			par->lcd_avdd_status = 0;
			
		//direction = of_get_property(np, "display_direct", 0);
		direction = 1;
		if(direction)
		{
			direction_value = be32_to_cpu(direction);
			direction_value = 1;
			printk("direction_value =%d.\n",direction_value);
			if(direction_value == 0)
			{
			}
			else if(direction_value == 1)
			{
				for(j = 0; j < len; j++)
				{
					vmem[2*j+1] = logo_dlink[2*j];
					vmem[2*j] = logo_dlink[2*j+1];
				}
				
				printk("set_st7735_cfg_array,MADCTL to 0xC8.\n");
				set_st7735_cfg_array(ST7735_MADCTL,0xC0);
				tmp_lcd_size = lcd_width;
				lcd_width = lcd_height;
				lcd_height = tmp_lcd_size;
			}
			else if(direction_value == 3)
			{
				printk("set_st7735_cfg_array,MADCTL to 0x08.\n");
				set_st7735_cfg_array(ST7735_MADCTL,0x08);
				tmp_lcd_size = lcd_width;
				lcd_width = lcd_height;
				lcd_height = tmp_lcd_size;
			}
			else if(direction_value == 2)
			{
				printk("set_st7735_cfg_array,MADCTL to 0xA8.\n");
				set_st7735_cfg_array(ST7735_MADCTL,0xA8);
			}
		}
		strcpy(st7735fb_fix.id,"fb_st7735r");
		st7735fb_fix.type =		FB_TYPE_PACKED_PIXELS;
		st7735fb_fix.visual =	FB_VISUAL_PSEUDOCOLOR;
		st7735fb_fix.xpanstep =	0;
		st7735fb_fix.ypanstep =	0;
		st7735fb_fix.ywrapstep =	0;
		st7735fb_fix.line_length =	lcd_width * BPP / 8;
		st7735fb_fix.accel =	FB_ACCEL_NONE;
		
		st7735fb_var.xres =			lcd_width;
		st7735fb_var.yres =			lcd_height;
		st7735fb_var.xres_virtual =		lcd_width;
		st7735fb_var.yres_virtual =		lcd_height;
		st7735fb_var.bits_per_pixel =	BPP;
		
		info->fix.smem_start = (unsigned long)vmem;
		info->fix = st7735fb_fix;
		info->fix.smem_len = (unsigned long)vmem_size;
		info->var = st7735fb_var;
		info->var.red.offset = 11;
		info->var.red.length = 5;
		info->var.green.offset = 5;
		info->var.green.length = 6;
		info->var.blue.offset = 0;
		info->var.blue.length = 5;
		info->var.transp.offset = 0;
		info->var.transp.length = 0;
		par->info = info;
	//}
	fb_deferred_io_init(info);
	wake_lock_init(&par->wake_lock, WAKE_LOCK_SUSPEND, DRVNAME);
	if(direction_value == 0)
	{
		if(!wake_lock_active(&par->wake_lock)){
			wake_lock(&par->wake_lock);
		}
	}
	par->ssbuf = (u16 *)vmem;
	retval = register_framebuffer(info);
	if (retval < 0)
		goto fbreg_fail;

	spi_set_drvdata(spi, info);
	retval = st7735fb_init_display(par);
	if (retval < 0)
		goto init_fail;

	printk(KERN_INFO
		"fb%d: %s frame buffer device,\n\tusing %d KiB of video memory\n",
		info->node, info->fix.id, vmem_size/1024);

	return 0;
	/* TODO: release gpios on fail */
init_fail:
	spi_set_drvdata(spi, NULL);
fbreg_fail:
	framebuffer_release(info);
tft144C034_fail:
	tft144C034_free_buffer();
fballoc_fail:
	vfree(picture);
	vfree(vmem);

	return retval;
}

static int st7735fb_remove(struct spi_device *spi)
{
	struct fb_info *info = spi_get_drvdata(spi);
	struct st7735fb_par *par = info->par;
	spi_set_drvdata(spi, NULL);
	//printk("%s\n", __func__);
	if (info) {
	wake_lock_destroy(&par->wake_lock);
		fb_deferred_io_cleanup(info);
		vfree(info->screen_base);
		unregister_framebuffer(info);
		framebuffer_release(info);
	}

	/* TODO: release gpios */

	return 0;
}

#ifdef CONFIG_OF
static struct of_device_id st7735fb_of_match[] = {
	{ .compatible = "spi,lcd-spi" },
	{}
};

MODULE_DEVICE_TABLE(of, st7735fb_of_match);
#endif

static struct spi_driver st7735fb_driver = {
	.driver = {
		.name   = "fb_st7735r",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
		.pm	= &st7735fb_pm_ops,
#ifdef CONFIG_OF
		.of_match_table = of_match_ptr(st7735fb_of_match),
#endif
	},
	.probe  = st7735fb_probe,
	.remove = st7735fb_remove,
};

module_spi_driver(st7735fb_driver);

MODULE_DESCRIPTION("FB driver for ST7735 display controller");
MODULE_AUTHOR("Matt Porter");
MODULE_LICENSE("GPL");
