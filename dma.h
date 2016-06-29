#ifndef DMA_H
#define DMA_H

// Will fill comments in later
#define GPIO_LEN        0xb4
#define DMA_LEN         0x24
#define PWM_BASE        (BCM2708_PERI_BASE + 0x20C000)
#define PWM_LEN         0x28
#define CLK_BASE        (BCM2708_PERI_BASE + 0x101000)
#define CLK_LEN         0xA8

#define GPFSEL0         (0x00/4)
#define GPFSEL1         (0x04/4)
#define GPSET0          (0x1c/4)
#define GPCLR0          (0x28/4)

#define PWM_CTL         (0x00/4)
#define PWM_STA         (0x04/4)
#define PWM_DMAC        (0x08/4)
#define PWM_RNG1        (0x10/4)
#define PWM_FIFO        (0x18/4)

#define PWMCLK_CNTL     40
#define PWMCLK_DIV      41

#define PWMCTL_MODE1    (1<<1)
#define PWMCTL_PWEN1    (1<<0)
#define PWMCTL_CLRF     (1<<6)
#define PWMCTL_USEF1    (1<<5)

#define PWMDMAC_ENAB    (1<<31)
// I think this means it requests as soon as there is one free slot in the FIFO
// which is what we want as burst DMA would mess up our timing..
#define PWMDMAC_THRSHLD ((15<<8)|(15<<0))

#define DMA_CS          (BCM2708_DMA_CS/4)
#define DMA_CONBLK_AD   (BCM2708_DMA_ADDR/4)
#define DMA_DEBUG       (BCM2708_DMA_DEBUG/4)

#define BCM2708_DMA_END             (1<<1)  // Why is this not in mach/dma.h ?
#define BCM2708_DMA_NO_WIDE_BURSTS  (1<<26)


#endif
