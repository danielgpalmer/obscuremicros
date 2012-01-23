/*
 * LEDs driver for I-O DATA DEVICE, INC. "LANDISK Series" support.
 *
 * Copyright (C) 2011 Daniel Palmer <me@danielpalmer.co.uk>
 * -- Updated methods used to read/write bytes to the CPLD 
 * -- Fixed bits and bobs to make it build as an external module
 * -- Fixed registeration of LEDs 
 *
 * Copyright (C) 2007 kogiidena
 *
 * Based on the drivers/leds/leds-ams-delta.c by:
 * Copyright (C) 2006 Jonathan McDowell
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/leds.h>


#include <asm/io.h>
#include <mach-landisk/mach/iodata_landisk.h>

static enum {
       LANDISK = 0,
       USL_5P  = 1,
} landisk_product;

static DEFINE_SPINLOCK(landisk_led_lock);

static void landisk_led_set(struct led_classdev *led_cdev,
                            enum led_brightness value);

static struct led_classdev landisk_leds[] = {
       [0] = {
              .name = "power",
              .brightness_set = landisk_led_set,
              .default_trigger = "default-on",
              },
       [1] = {
              .name = "status",
              .brightness_set = landisk_led_set,
              .default_trigger = "bitpat",
              },
       [2] = {
              .name = "led1",
              .brightness_set = landisk_led_set,
              .default_trigger = "bitpat",
              },
       [3] = {
              .name = "led2",
              .brightness_set = landisk_led_set,
              .default_trigger = "bitpat",
              },
       [4] = {
              .name = "led3",
              .brightness_set = landisk_led_set,
              .default_trigger = "bitpat",
              },
       [5] = {
              .name = "led4",
              .brightness_set = landisk_led_set,
              .default_trigger = "bitpat",
              },
       [6] = {
              .name = "led5",
              .brightness_set = landisk_led_set,
              .default_trigger = "bitpat",
              },
       [7] = {
              .name = "buzzer",
              .brightness_set = landisk_led_set,
              .default_trigger = "bitpat",
              },
};

void ledtrig_bitpat_default(struct led_classdev *led_cdev,
                           unsigned long *delay, char *bitdata)
{
       int led;

       led = (led_cdev - &landisk_leds[0]);
       if ((led == 0) || (led == 1)) {
               strcpy(bitdata, "blink");
       }
       if (led == 7) {
               *delay = 250;
       }

}

static void landisk_led_set(struct led_classdev *led_cdev,
                           enum led_brightness value)
{



       u8 tmp, bitmask;
       unsigned long flags;

       bitmask = 0x01 << (led_cdev - &landisk_leds[0]);

       spin_lock_irqsave(&landisk_led_lock, flags);
       tmp = __raw_readb(PA_LED);

       if (value) {
               tmp |= bitmask;
       }
       else {
               tmp &= ~bitmask;
       }
	
	__raw_writeb(tmp,PA_LED);

       spin_unlock_irqrestore(&landisk_led_lock, flags);
}




static int __devinit landisk_led_probe(struct platform_device *pdev)
{

	printk("landisk_led_probe()\n");


       int i, nr_leds;
       int ret;

       nr_leds = (landisk_product == LANDISK) ? 2 : 8;

       for (i = ret = 0; ret >= 0 && i < nr_leds; i++) {
		printk(" registering led \n");
               ret = led_classdev_register(&pdev->dev, &landisk_leds[i]);
       }

       if (ret < 0 && i > 1) {
               nr_leds = i - 1;
               for (i = 0; i < nr_leds; i++)
                       led_classdev_unregister(&landisk_leds[i]);
       }
       return ret;
}

static int __devexit landisk_led_remove(struct platform_device *pdev)
{

       printk("landisk_led_remove()\n");

       int i, nr_leds;

       nr_leds = (landisk_product == LANDISK) ? 2 : 8;

       for (i = 0; i < nr_leds; i++) {
               led_classdev_unregister(&landisk_leds[i]);
       }
       return 0;
}


static struct platform_driver landisk_led_driver = {
       .probe = landisk_led_probe,
       .remove = __devexit_p(landisk_led_remove),
       .driver = {
                  .name = KBUILD_MODNAME,
		  .owner  = THIS_MODULE,  
                },
};

/* HDD-access-LED setting at landisk status LED */

static void landisk_disk_trig_activate(struct led_classdev *led_cdev)
{
       unsigned long flags;
       spin_lock_irqsave(&landisk_led_lock, flags);
       __raw_writeb((__raw_readb(PA_LED) & ~0x0c) | 0x04,PA_LED);
	spin_unlock_irqrestore(&landisk_led_lock, flags);
}

static void landisk_disk_trig_deactivate(struct led_classdev *led_cdev)
{
       unsigned long flags;
       spin_lock_irqsave(&landisk_led_lock, flags);
       __raw_writeb((__raw_readb(PA_LED) & ~0x0c) | 0x0c,PA_LED);
       spin_unlock_irqrestore(&landisk_led_lock, flags);
}

// Doesnt look like this is needed anymore?

//static int landisk_disk_trig_is_led_supported(struct led_classdev *led_cdev)
//{
//       int led;
//
//       led = (led_cdev - &landisk_leds[0]);
//       return ((landisk_product == LANDISK) && (led == 1));
//}

static struct led_trigger landisk_disk_led_trigger = {
       .name = "disk",
       .activate = landisk_disk_trig_activate,
       .deactivate = landisk_disk_trig_deactivate,
};

static struct platform_device *pdev;


static int __init landisk_led_init(void)
{

	printk("landisk_led_init()\n");

       u8 orig, test;
       int err = 0;

       orig = __raw_readb(PA_LED);
       __raw_writeb(0x40,PA_LED);


       test = __raw_readb(PA_LED);
       __raw_writeb(orig,PA_LED);

       if (test != 0x40) {
	       printk("hardware is LANDISK\n");
               landisk_product = LANDISK;
               __raw_writeb(orig | 0x07,PA_LED);
	       landisk_leds[1].default_trigger = "disk";
       } else {
               printk("hardware is USL-5P\n");
               landisk_product = USL_5P;
               __raw_writeb(orig | 0x03,PA_LED);
       }

       err = led_trigger_register(&landisk_disk_led_trigger);
       if (err)
              return err;

	pdev = platform_device_register_simple(KBUILD_MODNAME, -1, NULL, 0);
        if (!IS_ERR(pdev)) {
                err = platform_driver_probe(&landisk_led_driver, landisk_led_probe);
                if (err)
                        platform_device_unregister(pdev);
        } else
                err = PTR_ERR(pdev);

       //err = platform_driver_register(&landisk_led_driver);

	printk("error - %x\n", err);

	return err;
}

static void __exit landisk_led_exit(void)
{
       led_trigger_unregister(&landisk_disk_led_trigger);
       platform_driver_unregister(&landisk_led_driver);
}


module_init(landisk_led_init);
module_exit(landisk_led_exit);

MODULE_ALIAS("platform:landisk-leds");
MODULE_AUTHOR("kogiidena");
MODULE_DESCRIPTION("landisk LED driver");
MODULE_LICENSE("GPL");
