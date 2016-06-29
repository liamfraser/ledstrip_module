// PWM peripheral is used to control the rate of data transfer to the GPIO
// using DMA. Based on:
// https://github.com/richardghirst/PiBits/blob/master/ServoBlaster/kernel/servoblaster.c

#include <mach/platform.h>
#include <asm/uaccess.h>
#include <mach/dma.h>
#include "dma.h"

// RGB pins
static const uint8_t pins[] = {17, 27, 22};

// Structure of our control data, stored in a 4K page, and accessed by dma controller
struct ctldata_s {
    // gpio-hi, delay, gpio-lo, delay, for each pin
    struct bcm2708_dma_cb cb[4 * 3];   
    // set-pin, clear-pin values, per pin
    uint32_t gpiodata[3];
    uint32_t pwmdata;
};

static struct ctldata_s *ctl;

// Registers we will use
static volatile uint32_t *gpio_reg;
static volatile uint32_t *dma_reg;
static volatile uint32_t *clk_reg;
static volatile uint32_t *pwm_reg;

void dma_init() {
    // Allocate 4K page to control block. Should check ctl is !=0
    ctl = (struct ctldata_s *) = get_zeroed_page(GFP_KERNEL);

    // Map register pointers onto actual registers
    gpio_reg = (uint32_t *)ioremap(GPIO_BASE, GPIO_LEN);
    dma_reg  = (uint32_t *)ioremap(DMA_BASE,  DMA_LEN);
    clk_reg  = (uint32_t *)ioremap(CLK_BASE,  CLK_LEN);
    pwm_reg  = (uint32_t *)ioremap(PWM_BASE,  PWM_LEN);
}
