# Orbit Course

# Bluetooth_Low_Energy_Introduction

**Prerequisites:** Download Visual Studio Code, and nRF Connect for Desktop -> Toolchain manager and install the latest version of nRF Connect SDK (2.1.1 when this guide was written). Install nRF Connect for Visual studio (instructions from Toolchain Manager).
Start by adding the "hello_world" sample from *NCS\zephyr\samples\hello_world* as an application in nRF Connect for Visual Studio Code. </br></br>

# HW requirements
- nRF52840 Development Kit. 

# SW Requirements
As mentioned in the prerequisites, you'll need:
- [nRF Connect for Desktop](https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-desktop)
- [Visual Studio Code](https://code.visualstudio.com/)
- [nRF Connect for Visual Studio Code plugin](https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-VS-Code) (If you want to, you can use *west* directly instead of nRF Connect for Visual Studio code, but we will use VS Code in this guide).



</br>

This tutorial will show you how to create a custom service with two custom value characteristics. One which the central can read and subscribe to (notifications) and one that the central can write to. We will be using the nRF Connect SDK (v1.9.1 or later). This tutorial can be seen as a practical implementations of the guides:
- [Bluetooth low energy Advertising, a beginner's tutorial](https://devzone.nordicsemi.com/guides/short-range-guides/b/bluetooth-low-energy/posts/ble-advertising-a-beginners-tutorial)
- [Bluetooth low energy Services, a beginner's tutorial](https://devzone.nordicsemi.com/guides/short-range-guides/b/bluetooth-low-energy/posts/ble-services-a-beginners-tutorial)
- [Bluetooth low energy Characteristics, a beginner's tutorial](https://devzone.nordicsemi.com/guides/short-range-guides/b/bluetooth-low-energy/posts/ble-characteristics-a-beginners-tutorial)
</br>

Although these tutorials were written a while ago, when the nRF5 SDK was still the main SDK for nRF devices, the theory is the same, but in this guide we will be using the nRF Connect SDK, and the Softdevice Controller instead of the nRF5 SDK and the Softdevice.</br>
#If you are looking for the nRF5 SDK version of this guide, please see [this repository](https://github.com/edvinand/custom_ble_service_example).
</br>
The aim of this tutorial is to simply create one service with two characteristics without too much theory in between the steps. You don't need to download any .c or .h files, as we will start with the hello_world sample as a template.

# Tutorial Steps
### Step 1 - Getting started

If you haven't done it already, start by setting up nRF Connect for Visual Studio code by setting the environment parameters. Under the nRF Connect tab in Visual Studio Code (VSC) click "Open welcome page" and click "Quick Setup". 
Visual Studio Code settings | 
------------ |
<img src="https://github.com/edvinand/Orbit/blob/main/images/welcome_page.PNG" width="1000"> |

These are my settings, but the path may vary in your environment.
</br>
Next we need to add the hello_world sample as our application. The path to the sample is *NCS\zephyr\samples\hello_world*. Note that this sample is from the "zephyr" part of nRF Connect SDK (NCS), but there are plenty of samples found in *NCS\nrf\samples* as well. 
</br>
Start by selecting *Create a new application from sample* in the *nRF Connect* -> *Welcome* tab, and choose settings similar to this the screenshot below. I recommend that you create a folder outside the NCS root folder where you store your custom applications. In our case, we will call this folder *custom_ble_sample*, and the name of the application is *remote_controller*. The application that we copy to our custom folder with our custom name is the *hello_world* sample. 

Setup Application from Sample | 
------------ |
<img src="https://github.com/edvinand/Orbit/blob/main/images/application_from_sample.PNG"> |

</br>

Now we have copied the sample to our custom applications folder, but we need to create a build environment before we can compile and flash the sample to our board. 
Below the *nRF Connect* -> *APPLICATIONS* tab, expand the "remote_controller" application, and click the *No build configurations*, which will create a new build configuration.
The only thing you need to do here is to set the board that you are using. Depending on the board you are using, you need to enter the NCS name of the DK:
- nRF52832 DK: nrf52dk_nrf52832
- nRF52833 DK: nrf52833dk_nrf52833
- nRF52840 DK: nrf52840dk_nrf52840

<br>

If you are using another board than the ones listed above, you can probably find the name of it in the folder: *NCS\zephyr\boards\arm*. When you have entered the name of your board, click Build Configuration.
When you built the configuration, it should compile/build the sample as well. If everything went well, you should be able to connect your DK using the micro USB port on the short end of the DK, and flash using the Flash button from the *ACTIONS* tab. </br>
If everything goes well, you should have flashed the *hello_world* sample to your board. We can see from the main.c file that it is printing some data using printk(), but unless we connect the nRF Terminal in VSC or another UART terminal, we will not see what it prints. Therefore, in the *CONNECTED DEVICES* tab, you should see your DK. Click the arrow on the left hand side to expand the board and click the left icon you see when you hover the mouse over the line saying VCOM0

Connect to board's UART | 
------------ |
<img src="https://github.com/edvinand/Orbit/blob/main/images/connect_uart.PNG"> |

</br>

A popup will occur with some UART settings. Just hit the enter key to select *115200 8n1*, and open the *NRF_TERMINAL* in your bottom terminal. It should print something like:
*Hello World! nrf52840dk_nrf52840*

### Step 2 - Enabling some basic application features
Congratulations! You have built and flashed our first application. Let's move on by doing some minor modifications. If you explore some of the samples from the *nrf* folder in NCS, you'll see that most of them use our logging module, which is what we will use as well. In order to do so, please replace the line `#include <sys/printk.h>` with `#include <logging/log.h>`. In order to use the log module, we need to add a few things in the prj.conf file. You will find it from the application tab (called remote_controller if you didn't change it) -> Input files -> prj.conf. At this point, it should just say `#nothing here`.
</br>
Add the following:
```
# Configure logger
CONFIG_LOG=y
CONFIG_USE_SEGGER_RTT=n
CONFIG_LOG_BACKEND_UART=y
CONFIG_LOG_DEFAULT_LEVEL=3
```
They are quite self explaining, but what we are doing here is enabling the log module, deselecting the default RTT backend, selecting the UART backend, and setting the log level to 3 (INFO). <br>
Back in main.c, try replacing the `printk()` with `LOG_INF();` and add the following snippet before `void main(void)`
```C
#define LOG_MODULE_NAME app
LOG_MODULE_REGISTER(LOG_MODULE_NAME);
```
Compile and flash the application again, and you should see that it still prints over UART, but now we are using the log module

</br>

#### Configure buttons and LEDs
Before we start adding Bluetooth, we want to set up some LEDs that we can use to indicate that our application is still running, and hasn't crashed. We also want to set up some buttons that we can use later to trigger certain BLE calls.
Start by including `<dk_buttons_and_leds.h>` in your main.c file.

<br>

*Hint: If you write `#include <dk_buttons_and_leds.h>` in your main.c file, and then right click it and select "Go to definition", it will open that file.*

<br>

Next, create a function to initiate the LEDs and buttons. I will call mine `static void configure_dk_buttons_leds(void)`.
The first thing we need to do in this function is to enable the LEDs. Looking in `dk_buttons_and_leds.h`, we can look for a function that does about that. Try adding `dk_leds_init()` to your configure_dk_buttons_leds() function. Since this function returns and int, we would like to check the return value. 

```C
    int err;
    err = dk_leds_init();
    if (err) {
        LOG_ERR("Couldn't init LEDS (err %d)", err);
    }
```

<br>

You may see that if you try to compile the sample after adding a function from the `dk_buttons_and_leds.h`, it will fail. The reason for this is that while we included the `dk_buttons_and_leds.h`, we didn't include the dk_buttons_and_leds.c file yet. This means that the compiler will not find the definitions of the functions that the header file claims that we have. We need to tell our application how to add the `dk_buttons_and_leds.c` file. There are two ways of doing this. If you create your own files, you can add them manually, which we will do later for some custom files. But for now we want to add a file that belongs to NCS, and therefore we include it using configuration switches.

<br>

in `prj.conf`, add the following:

```
# Configure buttons and LEDs.
CONFIG_GPIO=y
CONFIG_DK_LIBRARY=y
```

This snippet will enable the GPIOs and include the DK library. The way this is done in NCS/Zephyr is a bit complex. If you are interrested in how this works, you can look into the CMakeLists.txt file found in NCS\nrf\lib\CMakeLists.txt, and see how it includes files and folders based on the configurations. For now we will accept that this is just how it works.

<br>

Let us add a specific LED and a blinking interval near the top of main.c
```C
#define RUN_STATUS_LED DK_LED1
#define RUN_LED_BLINK_INTERVAL 1000
```



Open `dk_buttons_and_leds.h` to see if there is any ways you can turn on and off this LED from your main function. Our goal is to toggle the LED in a `for(;;)` loop (equivalent to a while(true) loop). There are several ways to do this. Try to find one that works. </br>
*Hint: You can use k_sleep() to wait a given amount of time, and there is a macro called K_MSEC() that takes an input of ms, and converts it to ticks.*

Now, let us look for a function that can enable the buttons in the `dk_buttons_and_leds.h` file. Remember to check the return value of the button init function. When you have a button handler set up, try to use it to print something in the log, so that we can see that it triggers correctly. We can tweak the button handler later.
</br>

*Hint: As this function initializes our buttons, it has an input parameter which is a callback handler. Use the "Go to definition" on the button handler type to see what kind of callback function it expects. If the button handler type that is expected is defined like this:* 

```C
typedef void (*callback_handler)(uint8_t first_parameter, uint16_t second_parameter);
```

*it means that you can define your callback e.g. like this:*

```C
void my_callback_function(uint8_t my_8_bit_parameter, uint16_t my_16_bit_parameter)
```

*You would probably choose some different names for the function and the parameters, but this is an example on how to interpret the callback typedefs.*

</br>

If you successfully compiled your application and flash it, you should now see that LED1 toggles every second, and that you receive a callback whenever a button is pressed or released. Remember to call `configure_dk_buttons_and_leds()` from your `main()` function.

<br>

**Challenge:** </br>
***Without peeking at the solution below, try to implement your button handler so that it stores the button number of the button that was pressed, and prints it in the log only when the button was pressed (and not released). Try printing out the parameters `button_state` and `has_changed` to see what they look like when you press the buttons. You may find a methid that is even more elegant than the suggested method below.***
</br>
</br>
At this point, your main.c file should look something like this. You can use this as a template if you got stuck somewhere before this point:

```C
/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <logging/log.h>
#include <dk_buttons_and_leds.h>

#define LOG_MODULE_NAME app
LOG_MODULE_REGISTER(LOG_MODULE_NAME);
#define RUN_STATUS_LED DK_LED1
#define RUN_LED_BLINK_INTERVAL 1000

/* Callbacks */
void button_handler(uint32_t button_state, uint32_t has_changed)
{
	int button_pressed = 0;
	if (has_changed & button_state)
	{
		switch (has_changed)
		{
			case DK_BTN1_MSK:
				button_pressed = 1;
				break;
			case DK_BTN2_MSK:
				button_pressed = 2;
				break;
			case DK_BTN3_MSK:
				button_pressed = 3;
				break;
			case DK_BTN4_MSK:
				button_pressed = 4;
				break;
			default:
				break;
		}
		LOG_INF("Button %d pressed.", button_pressed);
	}
}

/* Configurations */
static void configure_dk_buttons_leds(void)
{
    int err;
    err = dk_leds_init();
    if (err) {
        LOG_ERR("Couldn't init LEDS (err %d)", err);
    }
    err = dk_buttons_init(button_handler);
    if (err) {
        LOG_ERR("Couldn't init buttons (err %d)", err);
    }
}

/* Main */
void main(void)
{
    int blink_status = 0;
	LOG_INF("Hello World! %s\n", CONFIG_BOARD);

    configure_dk_buttons_leds();

    LOG_INF("Running...");
    for (;;) {
        dk_set_led(RUN_STATUS_LED, (blink_status++)%2);
        k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
    }
}
```

### Step 3 - Sensor data
Now we want to actually fetch some data from the real world. We will be using the MPU 6050 from InvenSense (we will call it the *MPU* from now on), which is a 6 axis accellerometer and gyroscope. You can read about it in the [datasheet from InvenSense](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf).

</br>

There are two main approaches to adapt the MPU into our application. Since NCS, which is built on top of Zephyr is an RTOS, it is possible to use the built in drivers for this particular sensor just by enabling some configurations in our prj.conf file. You can look into this later if you have the time, but for the purpose of learning, we will do this the "old fashion way".
The MPU is using something called I2C (pronounced "I square C") to communicate with the nRF. This is a serial communication protocol where we can have one I2C master communicating with several I2C slaves only by connecting two wires, one for the clock and one for the data. In our case we will only use one SPI slave, but if you want to add more I2C devices at a later time, you can do so without using any additional GPIO pins.

</br>

Let us start by adding a couple of new files to our project to keep things clean. Create a folder called `custom_files` inside your application folder: *remote_controller\src\custom_files*. You can either do this from Visual Studio Code, or you can do it from your operating system (Windows File Explorer). Inside that folder, create two new files: `mpu_sensor.h` and `mpu_sensor.c`. To include these custom files to your project, open CMakeLists.txt, and add the following snippet at the end:
```C
# Custom files and folders

target_sources(app PRIVATE
    src/custom_files/mpu_sensor.c
)

zephyr_library_include_directories(src/custom_files)
```

Application Tree | 
------------ |
<img src="https://github.com/edvinand/Orbit/blob/main/images/application_tree_0.PNG"> |

If all goes well, the project should compile, and we should be able to see our `mpu_sensor.c` in our application tree. Open `mpu_sensor.c` by double clicking it, and start by adding this line to the very top:
```C
#include "mpu_sensor.h"
```

If you right click `mpu_sensor.h` that you just wrote and click "Go to Definition" it should open the `mpu_sensor.h` file in Visual Studio Code. In this header file, add:
```C
#include <zephyr.h>
#include <logging/log.h>
```

**Challenge:** </br>
Now try to create a function called `mpu_sensor_init()` in your `mpu_sensor.c` file, that you also need to declare in `mpu_sensor.h`. Make the function return 0, and check this return value in `main()` from your `main.c` file. Add whatever is needed in these two files so that you can use this function to print *"Initializing MPU Sensor"* to our log. Remember to include `mpu_sensor.h` from your main.c file. 
</br>
*Hint 1: You shouldn't need to include any more files in `mpu_sensor.c`. Only in `mpu_sensor.h`. The files you need to include are also included in `main.c`.*
</br>
*Hint 2: Give `mpu_sensor.c` another log module name, so that it is easy to see from what file the log messages are coming from.*
</br>
*Hint 3: using the `LOG_MODULE_NAME mpu` does not work. It is probably used by something else in Zephyr. Giving it the name `mpu_sensor` should work.*

</br>
</br>
Congratulations. You have successfully written your first function in a different .c file. Now, let us start reading some sensor data. As I mentioned, the MPU6050 sensor uses I2C for communication. The first thing we want to do is to add I2C to our project. To do this, add the following snippet to your prj.conf file:

```C
# I2C
CONFIG_NRFX_TWIM0=y
```

Note that TWI (Two Wire Interface) is another name for I2C. The reason for the two names has to do with trademarks and copyrights. Other than the name, they are identical.
TWIM is short for "Two Wire Interface Master", which is what we want to use. Our sensor will be a Two Wire Interface Slave (TWIS), or an I2C slave. 
</br>
Unfortunately, adding this config alone is not enough. It tells the nrfx drivers what instance of I2C we want to use. We also need to tell our compiler to actually enable the I2C0 peripheral. The way we do this is by creating a file in our project folder (`remote_controller`). Call it `nrf52840dk_nrf52840.overlay`. It needs to have that exact name, because when a .overlay file with the board name that we are using, it will take that file, and overwrite the default settings that you are found in the board file. 
</br>
In your `nrf52840dk_nrf52840.overlay` file, add the following:
```C
&i2c0 {
    status = "okay";
    compatible = "nordic,nrf-twim";
    clock-frequency = < I2C_BITRATE_STANDARD >;
};
```

Then, inside your `mpu_sensor_init()`, we want to initialize our I2C/TWIM sensor. It requires a few steps. First, we need to start the I2C, and then we need to send some I2C commands to tell our sensor to turn on. Let us start by adding a function called `twi_init()`, and call it inside mpu_sensor_init().

```C
int mpu_sensor_init(void)
{
    int err;

    LOG_INF("Initializing MPU Sensor");

    err = twi_init();
    if (err) {
        return err; // I know that this seems unnecessary, since we return err right below. But we will add more in between later.
    }
    
    return err;
```

Then, start by implementing your `twi_init()` function like this (write this *above* `mpu_sensor_init()` inside `mpu_sensor.c`. Since this will only be used from inside this file locally, we don't need to declare it in `mpu_sensor.h`.

```C
int twi_init(void)
{
    // Setup peripheral interrupt.
    IRQ_CONNECT(DT_IRQN(DT_NODELABEL(i2c0)),DT_IRQ(DT_NODELABEL(i2c0), priority), nrfx_isr, nrfx_twim_0_irq_handler,0);
    irq_enable(DT_IRQN(DT_NODELABEL(i2c0)));

    int err = 0;
    
} 
```

What we are doing here is that we are telling the application that we want to connect the I2C0 interrupts to our nrfx_twim_0_irq_handler. This callback handler is implemented inside the nrfx i2c driver (found in nrfx_twim.c). We don't need to use this handler directly. We will set up our own handler shortly.
Next, we need to configure our I2C to use the pins that we want to use, and the speed that we want to use, in addition to setting our own interrupt handler.
If you open `nrfx_twim.c` (<ctrl> + <shift> + <P> and write nrfx_twim.c and hit enter), you should see a function called `nrfx_twim_init()`in there. We want to use this to initialize our TWI. If you look up the same function in nrfx_twim.h, you can see what input parameters it requires, with some description above.

```C
nrfx_err_t nrfx_twim_init(nrfx_twim_t const *        p_instance,
                          nrfx_twim_config_t const * p_config,
                          nrfx_twim_evt_handler_t    event_handler,
                          void *                     p_context);
```

So we need to create most of these.
`p_instance` is a pointer to the i2c0 instance, which we can create by adding this line close to the top of your `mpu_sensor.c`:

```C
const nrfx_twim_t m_twim_instance       = NRFX_TWIM_INSTANCE(0);
```

Then we need to create our config parameter. We can do it like this (inside our `twi_init()` function) :
```C
    const nrfx_twim_config_t twim_config = {                          \
        .scl                = 4,                                      \
        .sda                = 3,                                      \
        .frequency          = NRF_TWIM_FREQ_400K,                     \
        .interrupt_priority = NRFX_TWIM_DEFAULT_CONFIG_IRQ_PRIORITY,  \
        .hold_bus_uninit    = false,                                  \
    };
```

In addition we need an `nrfx_twim_evt_handler_t` type event handler. Look at the definition of `nrfx_twim_evt_handler_t` in nrfx_twim.h:

```C
/** @brief TWI event handler prototype. */
typedef void (* nrfx_twim_evt_handler_t)(nrfx_twim_evt_t const * p_event,
                                         void *                  p_context);
```

What this means is that the callback handler is expected to return nothing (void), and it has two parameters, p_event and p_contect. So create this function in your `mpu_sensor.c` :

```C
void my_twim_handler(nrfx_twim_evt_t const * p_event, void * p_context)
{
    LOG_INF("TWIM callback");
}
```

The last parameter, p_context, we will just set to NULL.
So try calling nrfx_twim_init() with the parameters that we just created as inputs. Remember that if it expects to be an input parameter to be a pointer, you probably need an `&` before you enter it. Also note that my_twim_handler will not have an `&` in front of it. Remember to check the return value from nrfx_twim_init(), and make sure it returns `NRFX_SUCCESS`.

Lastly, after initializing our TWI, we need to enable it. Look for a function in `nrfx_twim.c`/`nrfx_twim.h` that we can use for this, and then call this after `nrfx_twim_init()`


If you did succeeded with initializing and enabling your TWIM, your twi_init() function should look something like this:

```C

// Somewhere close to the top of mpu_sensor.c:
const nrfx_twim_t m_twim_instance       = NRFX_TWIM_INSTANCE(0);
[...]


int twi_init(void)
{
    // Setup peripheral interrupt.
    IRQ_CONNECT(DT_IRQN(DT_NODELABEL(i2c0)),DT_IRQ(DT_NODELABEL(i2c0), priority), nrfx_isr, nrfx_twim_0_irq_handler,0);
    irq_enable(DT_IRQN(DT_NODELABEL(i2c0)));

    int err = 0;

    const nrfx_twim_config_t twim_config = {                          \
        .scl                = 4,                                      \
        .sda                = 3,                                      \
        .frequency          = NRF_TWIM_FREQ_400K,                     \
        .interrupt_priority = NRFX_TWIM_DEFAULT_CONFIG_IRQ_PRIORITY,  \
        .hold_bus_uninit    = false,                                  \
    };
    err = nrfx_twim_init(&m_twim_instance,
                          &twim_config,
                          my_twim_handler,
                          NULL);
                          
    if (err != NRFX_SUCCESS) {
        LOG_ERR("twim_init failed. (err %x)", err);
        return err;
    }

    nrfx_twim_enable(&m_twim_instance);
    
    return 0;
}
```

Please note that NRFX_SUCCESS is not equal to 0, which is why we set err back to 0 after checking whether it returned NRFX_SUCCESS.
</br>
</br>
Now we have initialized and enabled our I2C, and we are ready to start communicating with our MPU. But before we do that, we need to know what data to send to our MPU, and how to interpret the data coming back. I2C slaves will always wait for a message from the I2C master, and then it will reply according to that message. 

Now is the time to open up the datasheet to our sensor again. [(link)](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf)
</br>
To save some time, you can download this file, which I stole from a colleague, who found it somewhere (not sure of the source). 
<br>
[mpu6050_registers.h](https://github.com/edvinand/Orbit/blob/main/remote_controller/src/custom_files/mpu6050_registers.h)
</br>
If you cloned this repository, it is found under `remote_controller\src\custom_files\mpu6050_registers.h`, but if not, create a file with that name, and just copy paste the content into your newly created file. Include it from your mpu_senror.h file. 
What this file contains is all the register names and addresses. We will use this when we communicate with our sensor. TWI drivers may seem a bit scary. Therefore, I will just dump a bunch of functions here that you can paste into your `mpu_sensor.c`, somewhere before `my_twim_handler()`. I will explain them briefly below.

```C
#define MPU_TWI_BUFFER_SIZE             14
#define MPU_TWI_TIMEOUT                 10000
#define MPU_ADDRESS                     0x68
volatile static bool twi_xfer_done      = false;
uint8_t twi_tx_buffer[MPU_TWI_BUFFER_SIZE];

int app_mpu_tx(const nrfx_twim_t *  p_instance,
                uint8_t             address,
                uint8_t *           p_data,
                uint8_t             length,
                bool                no_stop)
{
    int err;

    nrfx_twim_xfer_desc_t xfer = NRFX_TWIM_XFER_DESC_TX(address, p_data, length);
    err = nrfx_twim_xfer(p_instance, &xfer, 0);
    if (err != NRFX_SUCCESS) {
        return err;
    }

    return 0;
}

int app_mpu_rx(const nrfx_twim_t *   p_instance,
               uint8_t               address,
               uint8_t *             p_data,
               uint8_t               length)
{
    int err;
    nrfx_twim_xfer_desc_t xfer = NRFX_TWIM_XFER_DESC_RX(address, p_data, length);

    err = nrfx_twim_xfer(p_instance, &xfer, 0);
    if (err != NRFX_SUCCESS) {
        return err;
    }
    return 0;
}

int wait_for_xfer_done(void)
{
    int timeout = MPU_TWI_TIMEOUT;
    while ((!twi_xfer_done) && --timeout)
    {
        // Wait...
    }
    if (timeout == 0) {
        return NRFX_ERROR_TIMEOUT;
    }
    return 0;
}

int app_mpu_write_single_register(uint8_t reg, uint8_t data)
{
    int err;

    uint8_t packet[2] = {reg, data};

    twi_xfer_done = false;  // reset for new xfer
    err = app_mpu_tx(&m_twim_instance, MPU_ADDRESS, packet, 2, false);
    if (err) {
        return err;
    }
    err = wait_for_xfer_done();
    if (err == NRFX_ERROR_TIMEOUT) {
        return err;
    }
    
    return 0;
}

int app_mpu_write_registers(uint8_t reg, uint8_t * p_data, uint8_t length)
{
    int err;
    
    twi_tx_buffer[0] = reg;
    memcpy((twi_tx_buffer + 1), p_data, length);

    nrfx_twim_xfer_desc_t xfer = {0};
    xfer.address = MPU_ADDRESS;
    xfer.type = NRFX_TWIM_XFER_TX;
    xfer.primary_length = length+1;
    xfer.p_primary_buf = twi_tx_buffer;

    twi_xfer_done = false;  // reset for new xfer
    err = nrfx_twim_xfer(&m_twim_instance, &xfer,0);
    if (err != NRFX_SUCCESS) {
        return err;
    }
    err = wait_for_xfer_done();
    if (err == NRFX_ERROR_TIMEOUT) {
        return err;
    }

    return 0;
    
}

int app_mpu_read_registers(uint8_t reg, uint8_t * p_data, uint8_t length)
{
    int err;

    twi_xfer_done = false;  // reset for new xfer
    err = app_mpu_tx(&m_twim_instance, MPU_ADDRESS, &reg, 1, false);
    if (err) {
        return err;
    }
    err = wait_for_xfer_done();
    if (err == NRFX_ERROR_TIMEOUT) {
        return err;
    }

    twi_xfer_done = false;  // reset for new xfer
    err = app_mpu_rx(&m_twim_instance,MPU_ADDRESS, p_data, length);
    if (err) {
        LOG_ERR("app_mpu_rx returned %08x", err);
        return err;
    }
    err = wait_for_xfer_done();
    if (err == NRFX_ERROR_TIMEOUT) {
        return err;
    }

    return 0;
}
```

Explanation of functions:
**app_mpu_tx():** will send `length` bytes of data to the device with the `address`. The data it sends is stored in `p_data`.
</br>
**app_mpu_rx():** will read `length` bytes of data from the device with the `address`. The data is stored in `p_data`.
</br>
**wait_for_xfer_done():** will be used to wait for the next TWIM callback in our callback handler. We will implement this functionality in the TWI callback next. If it times out, the function will return NRFX_ERROR_TIMEOUT. If successful, it will return 0.
</br>
**app_mpu_write_single_register()** will send one byte of payload `data` to the register `reg` given as an input parameter.
</br>
**app_mpu_write_registers():** will send `length` bytes to the registers starting at `reg`. The data it sends is stored in `p_data`.
</br>
**app_mpu_read_registers():** will read `length` bytes starting from the register `reg`.
</br>
In addition, we added some parameters and definitions at the top:
**MPU_TWI_BUFFER_SIZE:** the maximum number of bytes we can use in a transfer. The size is selected so that it will be possible to read the accellerometer (6 bytes), gyroscope (6 bytes) and temperature (2 bytes) in one transmission.
</br>
**MPU_TWI_TIMEOUT:** the number of iterations we will wait for the TWI transfer to complete before we time out.
</br>
**MPU_ADDRESS:** This is the TWI address of our sensor. This will be used in all of our transfers. It is hardcoded into our read and write functions.
</br>
**twi_xfer_done:** The parameters used to tell `wait_for_xfer_done()` that we received our callback.
</br>
**twi_tx_buffer:** The actuall buffer (with size MPU_TWI_BUFFER_SIZE) that we use to read registers from the MPU.
</br>


### Step 4 - Motor control
Time to add some movement to our PWM motor. The motor that we used is the Tower Pro SG90. You can find a very simplified datasheet [here](http://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf). For some background information on how PWM motors work, you can check out [this guide](https://www.jameco.com/Jameco/workshop/Howitworks/how-servo-motors-work.html).
Basically, we want to output a PWM signal, and the duty cycle of the PWM signal determines what angle/position the rotor will maintain. In our case, the motor wants a duty cycle between 1 and 2 ms, and a PWM period of 20ms. 
Because we still want to keep main.c as clutter free as possible, we will try to do most of the PWM configurations and handling in another file, and implement some simple functions that we can call from main.c. Therefore we will add a few more custom files. Inside your `custom_files` folder, create two new files: `motor_control.h` and `motor_control.c`. To include these to your project, open CMakeLists.txt, and add the following snippet at the end:
</br>
**Note the `;` after `mpu_sensor.c`**
```C
# Custom files and folders

target_sources(app PRIVATE
    src/custom_files/mpu_sensor.c;
    src/custom_files/motor_control.c
)

zephyr_library_include_directories(src/custom_files)
```



If all goes well, the project should compile, and we should be able to see our motor_control.c in our application tree as well. Open `motor_control.c` and start by adding this line to the very top:

```C
#include "motor_control.h"
```

Open `motor_control.h` and add:
```C
#include <zephyr.h>
#include <logging/log.h>
```

**Challenge:** </br>
Just like we did in the `mpu_sensor` files, create a function called `motor_init()` that returns 0 and prints *"Initializing Motor Control"* to your log.


**PWM control**
When we are using the nRF Connect SDK, we have several driver options to control the PWM. We have our own drivers that are tailored for the Nordic Semiconductor devices, or we can use Zephyr's drivers, which will use Nordic's drivers beneath the hood. In this tutorial we will use Nordic's drivers directly, for simplicity. 
Let us start by adding some configurations to our prj.conf file:

```C
# PWM
CONFIG_PWM=y
CONFIG_NRFX_PWM1=y
```

What we are doig here is that we enable the PWM in general, and we tell it to use PWM instance 1.
Then we can continue by adding the pwm header file near the top of `motor_control.h`. `motor_control.c` will inherit this, as long as it includes `motor_control.h`.

```C
#include "nrfx_pwm.h"
```

Open `nrfx_pwm.h` and see if you can find a function that will initialize the PWM driver. `nrfx_err_t nrfx_pwm_init(...)` looks promising. This function has 4 parameters. We only need the first two. The rest we can set to NULL, as we don't need any callbacks or context pointer to pass on to the callback. 
The first parameter is the pointer to an instance. We can use the macro *NRFX_PWM_INSTANCE()* (from nrfx_pwm.h) to define this instance. The second parameter is the configuration that we want to use. The configuration holds information such as the pin number, the PWM frequency, and so on. We can use another macro, *NRFX_PWM_DEFAULT_CONFIG* to set most things, and then we can tweak it later. 

<br>

Short background: The way that the PWM works is that it is a counter counting from 0 to .top_value at the speed of 1MHz. 
When it reaches our threshold/pwm_duty_cycle (which we will set later) it will set the pin in active state, and when the counter's .top_value is reached, it puts the pin in inactive state, and resets the counter. 
<br>
If you study the configuration in *NRFX_PWM_DEFAULT_CONFIG* in nrfx_pwm.h, we see that most of these are ok. What we need to adjust a few things. The PWM period that we want to use is 20ms = 20000µs. Since our PWM clock is running at 1MHz (from *NRFX_PWM_DEFAULT_CONFIG*), we want to set our config's .top_value = 20000. 
We also want to set the parameter .load_mode. I will not go into details, but it has to do with how many bits in a pwm value that is used. The last thing we want to decide is what GPIO pin that we will use for the PWM signal. 
If you look at the backside of the DK, you can see that some of the pins are used for LEDs, buttons, NFC, serial communication, etc. Try to avoid these. P0.03 (and close to both GND and VDD), so we can use that. 




```C
// Near the top of motor_control.c:
#define SERVO_PIN                           29
#define PWM_PERIOD                          20000
static nrfx_pwm_t pwm = NRFX_PWM_INSTANCE(1);

// implementation of motor_init():
int motor_init(void)
{
    LOG_INF("Initializing Motor Control");

    nrfx_err_t err;
    nrfx_pwm_config_t pwm_config    = NRFX_PWM_DEFAULT_CONFIG(SERVO_PIN, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED);
    pwm_config.top_value            = PWM_PERIOD;
    pwm_config.load_mode            = NRF_PWM_LOAD_INDIVIDUAL;
    
    err = nrfx_pwm_init(&pwm, &pwm_config, NULL, NULL);
    if (err != NRFX_SUCCESS) {  // NB: NRFX_SUCCESS != 0
        LOG_ERR("nrfx_pwm_init() failed, err %d", err);
    }
    
    return 0;
}

```

In theory (!) our motor is now operational as soon as you connect it. Connect the brown wire to GND, the red wire to VDD and the orange to the PWM signal pin (P0.03). However, we haven't actually told the PWM driver to set a PWM signal yet. Let us start by adding this to our motor_init() function to test that it works. Look for nrfx_pwm_simple_playback() in nrfx_pwm.h, and see if you can apply it to set the PWM signal of your motor. Remember to call nrfx_pwm_simple_playback before your `return 0;`.
How to set this frequency is not trivial, since the second parameter is a double pointer to the actual duty cycle, so we will walk through it. If you want to try for yourself, you can stop reading here, and look into the `nrfx_pwm.h` file. 

<br>

The nrfx_pwm_simple_playback() expects a pointer to a nrf_pwm_sequence_t parameter. It also holds a parameter indicating the length of this array. This is all we will use, and we will set the rest to 0. The array with the PWM values is a parameter with the type nrf_pwm_sequence_t, so let us start by implementing this near the top of motor_control.c:

```C
static  nrf_pwm_values_individual_t position_1[] = {
    {19000},
};
static  nrf_pwm_values_individual_t position_2[] = {
    {18000},
};
```

You may remember that our motor's datasheet said that the PWM duty cycle should be between 1ms and 2ms on logic high, and low for the rest of the period. This would be 1000 and 2000 ticks in our case. However, this PWM driver has an active low configuration, so the easiest workaround for this is to just say that the duty cycle is the period minus the actual duty cycle. So for 1ms that would be 20000 - 1000 = 19000. 

Then we will implement the nrf_pwm_sequence_t parameter, which uses the nrf_pwm_values_individual_t:

```C
static nrf_pwm_sequence_t position_1_sequence = {
    .values.p_individual    = position_1,
    .length                 = NRF_PWM_VALUES_LENGTH(position_1),
    .repeats                = 0,
    .end_delay              = 0
};
static nrf_pwm_sequence_t position_2_sequence = {
    .values.p_individual    = position_2,
    .length                 = NRF_PWM_VALUES_LENGTH(position_2),
    .repeats                = 0,
    .end_delay              = 0
};
```

Now we can finally use these "sequences" to set the angle of our motor by adding this line to our `motor_init()`:
```C
nrfx_pwm_simple_playback(&pwm, &position_1_sequence, 50, NRFX_PWM_FLAG_STOP);
```

This will cause the motor to go to "position 1", and stay there for 50 PWM periods (1 second), and then turn off. 

**Challenge:**
<br>
Use what you now know to make two of the buttons in the button handler from main.c control the motor between "position_1" and "position_2"

<br>

*Hint: If you are stuck, you can see a suggested solution for main.c, and motor_control.c/h here:

<br>

[main.c](https://github.com/edvinand/Orbit/blob/main/temp_files/snapshot0/main.c)</br>
[motor_control.c](https://github.com/edvinand/Orbit/blob/main/temp_files/snapshot1/custom_files/motor_control.c)</br>
[motor_control.h](https://github.com/edvinand/Orbit/blob/main/temp_files/snapshot1/custom_files/motor_control.h)</br>

### Step 5 - Adding Bluetooth
It is finally time to add bluetooth to our project. A hint was given in the project name, but in case you missed it, we will write an application that mimics some sort of bluetooth remote, where we will be able to send button presses to a connected Bluetooth Low Energy Central. We will also add the oppurtynity to write back to the remote control. That may not be a typical feature for a remote control, but for the purpose of learning how to communicate in both directions we will add this. The connected central can either be your phone, a computer, or another nRF52. For this guide we will use a separate DK and nRF Connect for Desktop -> Bluetooth, but if you only have one DK, you can use [nRF Connect for iOS or Android.](https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-mobile)
</br>
</br>
Because we want to keep our main.c clean, we will try to do most of the bluetooth configuration and handling in another file, just like we did with PWM. Therefore we will start by adding some more custom files. Inside the `custom_files` folder, create two more files: `remote.h` and `remote.c`. To include these custom files to your project, open CMakeLists.txt, add on to what we changed earlier:

```C
# Custom files and folders

target_sources(app PRIVATE
    src/custom_files/motor_control.c;
    src/custom_files/remote.c
)

zephyr_library_include_directories(src/custom_files)
```

</br>
When you build your application again, you should see remote.c appear next to motor_control.c.

</br>
Open remote.c and add the line at the very top: </br>

```C
#include "remote.h"
```

And in `remote.h`, add the following:

```C
#include <zephyr.h>
#include <logging/log.h>
```

Now, try to create a function called `bluetooth_init()` in your remote.c file that you also need to declare in remote.h. Make the function return `0`, and check this return value in `main()`. Just like before, add whatever is needed in these two files so that you can use this function to log "Initializing Bluetooth". Remember to include remote.h from your main.c file.
</br>


Now that we have our own file to do most of the Bluetooth, let us start by adding these four header files in our remote.h file:
```C
#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/hci.h>
```
For most Bluetooth Low Energy, these four files will do the job. Let us start by adding `bt_enable()` to our `bluetooth_init()` function. In order to see what input bt_enable() takes, you may want to build, and probably you will find out that bt_enable is not defined yet. The reason for this is, like earlier, that we have not enabled Bluetooth in our prj.conf file. Try to add the following:
```
# Configure Bluetooth
CONFIG_BT=y
CONFIG_BT_PERIPHERAL=y
CONFIG_BT_DEVICE_NAME="Remote_controller"
CONFIG_BT_DEVICE_APPEARANCE=0
CONFIG_BT_MAX_CONN=1
CONFIG_BT_LL_SOFTDEVICE=y

CONFIG_ASSERT=y
````
What we do here is:
- Enable Bluetooth,
- Support the peripheral (advertising) role
- Set our device_name, which we will use later
- Set the appearance. Look in the description of this configuration to see what this does.
- Set the maximum number of simultaneous connections to 1.
- Tell it to use the Nordic Softdevice Controller. 

</br>
After this (rebuild required), it is time to see what bt_enable does. In nRF Connect for VS Code, if you hold ctrl and hover bt_enable(), you should see the declaration of the function. If you ctrl click it, it should bring you to the definition. We can use this to see what it returns and what input parameters it takes.
</br>

Visual Studio Code Navigation | 
------------ |
<img src="https://github.com/edvinand/Orbit/blob/main/images/VSC_hint.png"> |

</br>

So we see that it returns an `int` and it takes an input `bt_ready_cb_t`. By going to the definition of `bt_ready_cb_t` you'll see that it is:

```C
typedef void (*bt_ready_cb_t)(int err);
```

This means a function pointer. It means that it takes a callback function as an input parameter. The callback is on the form: `void callback_name(int err)`. Let us use a callback called `bt_ready`, which we will implement above `bluetooth_init()` in remote.c, and pass it onto `bt_enable()`.

```C
void bt_ready(int err)
{
    if (err) {
        LOG_ERR("bt_enable returned %d", err);
    }
}
```

We want to wait for our callback before we continue with our application. In order to do this, we will use a semaphore. Define the semaphore near the top of remote.c:

```C
static K_SEM_DEFINE(bt_init_ok, 0, 1);
```

After `bt_enable(bt_ready);` try to take the semaphore, so that the application waits until it is given from somewhere else, and then try to give it in the bt_ready callback. </br>
*Hint: The k_sem_take() requires a timeout. You can use K_FOREVER.*

</br>
</br>

#### Advertising
So far we have enabled Bluetooth, but now we didn't use it for anything. Let us add some Bluetooth advertising. We want to include two things in our advertisements. The device name and the UUID of the service that we will implement later. Let us start by adding the UUID (Universally Unique Identifier). I typically use an online UUID generator. Try using [this online UUID Generator](https://www.uuidgenerator.net/version4). In my case, I got a UUID which I translated to this format:
 
 ```C
 /* Add this to remote.h */
 /** @brief UUID of the Remote Service. **/
#define BT_UUID_REMOTE_SERV_VAL \
	BT_UUID_128_ENCODE(0xe9ea0001, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)
```

Copy your own generated UUID into the same format, and set the two last bytes of the first sections to 0001, like I did. This is so that it is easier to recognize them later. 
Also add the following line below the definition of your UUID:

```C
#define BT_UUID_REMOTE_SERVICE  BT_UUID_DECLARE_128(BT_UUID_REMOTE_SERV_VAL)
```

These are just two ways to define the same UUID, which we will use later. Now, open remote.c, and let us define the advertising packets. We will use two advertising packets. The normal advertising data, and something called scan response data.

Add this a suitable place in remote.c:

```C
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME)-1)


static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)
};

static const struct bt_data sd[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_REMOTE_SERV_VAL),
};
```

So we use the CONFIG_BT_DEVICE_NAME from our prj.conf file as our device name, and we apply this name in our advertising packet `ad[]`. In our Scan response packet, `sd[]` we add our randomly generated UUID. Now that we have the data we want to advertise, we can start the advertising from bluetooth_init(), after the bt_init_ok semaphore has been taken.

<br>

**This is a good time to change the `CONFIG_BT_DEVICE_NAME` in your prj.conf file to something unique, to distinguish your device from your neighbour's device.**

```C
/* This snippet belongs in bluetooth_init() in remote.c */
    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (err){
        LOG_ERR("couldn't start advertising (err = %d", err);
        return err;
    }
```

Now your device should advertise if you flash it with the latest build. Open nRF Connect for Desktop/iOS/Android and start scanning for the device. If there are many BLE devices nearby, try to sort by RSSI (Received Signal Strength Indicator), or ad a filter to the advertising name:

<br>

Note that all the screenshots are using nRF Connect for iOS. If you are using nRF Connect for Android, the app may look a bit different.

</br>

Scan uisng nRF Connect for Mobile | 
------------ |
<img src="https://github.com/edvinand/Orbit/blob/main/images/scan_advertisements_mobile.PNG" width="300"> |

If you select this device, you should be able to see some information from the advertisements. The name should appear as we set it in our prj.conf, and the service should match the service UUID should be visible, and match the service UUID that we defined in remote.h.

Advertisement description | Listed UUIDs |
------------ | ------------ |
<img src="https://github.com/edvinand/Orbit/blob/main/images/advertisement_description_mobile.PNG" width="300"> | <img src="https://github.com/edvinand/Orbit/blob/main/images/UUID_list_mobile.PNG" width="300"> |


</br>
</br>
You should see that you can actually connect to your device as well, since we claimed in the `BT_LE_ADV_CONN` (from bt_le_adv_start()) that we are connectable. However, if you try to connect to it, you will see that other than the Generic Attribute and the Generic Access services, we don't actually have the custom service that we claimed to have in the our advertising packet (Listed as "Advertised Services"). We will fix that later, but first, let us try to inform our application that something actually connected to us.
</br>


</br>
We want to receive these events in our main.c file, so that we can keep track of the state of our device. Let us start by adding a struct containing the callbacks in main.c:
</br>


```C
struct bt_conn_cb bluetooth_callbacks = {
	.connected 	= on_connected,
	.disconnected 	= on_disconnected,
};
```

Try "control clicking" bt_conn_cb, to see what callback types the .connected and .disconnected callback events are to understand why they have the function parameters that they have. We will use these functions to print to our log that we are connected, and to set a connection pointer that we use to keep track of whether or not we are in a connection.

```C
/* Near top of main.c */
static struct bt_conn *current_conn;    // Used to keep track of current connection status.

/* Callbacks, also in main.c: */

void on_connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        LOG_ERR("connection failed, err %d", err);
    }
    current_conn = bt_conn_ref(conn);
    dk_set_led_on(CONN_STATUS_LED);
}

void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("Disconnected (reason: %d)", reason);
	dk_set_led_off(CONN_STATUS_LED);
	if(current_conn) {
		bt_conn_unref(current_conn);
		current_conn = NULL;
	}
}
```

Although it may not be completely clear, what you need to know here is that we fetch a connection reference from the connection event. We will use this later. When we disconnect we remove this reference, and set the current_conn parameter back to a NULL-pointer.


**Challenge:** </br>
***Forward these callbacks (the bluetooth_callbacks parameter) to bluetooth init, and use bt_conn_cb_register() to register them to our Bluetooth stack. Do so before you call bt_enable(). In order to forward the bluetooth_callbacks I suggest that you make the bluetooth_init() function take them in as a pointer. If you are stuck, you will find a solution below.***

</br>
</br>

If you followed the guide this far, your files should look something like this. You can use this in case you got stuck somewhere. Please note that I also added some new code to the connected and disconnected events in main.c, and a current_conn parameter to keep track of the current connection. 

</br>

[main.c](https://github.com/edvinand/Orbit/blob/main/temp_files/snapshot1/main.c)</br>
[remote.c](https://github.com/edvinand/Orbit/blob/main/temp_files/snapshot1/custom_files/remote.c)</br>
[remote.h](https://github.com/edvinand/Orbit/blob/main/temp_files/snapshot1/custom_files/remote.h)</br>


### Step 6 - Adding our First Bluetooth Service
Let us add the service that we claim that we have when we advertise. We will use the macro BT_GATT_SERVICE_DEFINE to add our service. It is quite simple at the same time as it is quite complex. When we use this macro to create and add our service, the rest is done "under the hood" of NCS/Zephyr. By just adding this snippet to remote.c

```C
/* This code snippet belongs to remote.c */
BT_GATT_SERVICE_DEFINE(remote_srv,
BT_GATT_PRIMARY_SERVICE(BT_UUID_REMOTE_SERVICE),
);
```

And voila! We have our first Bluetooth Low Energy service. Try to connect to it using nRF Connect, and see that you can see the service.
Our first service | 
------------ |
<img src="https://github.com/edvinand/Orbit/blob/main/images/custom_service_mobile.jpg" width="300"> |

However, a service without any characteristics isn't very impressive. Let us add a characteristic that we can read from our Central. </br>
We start by defining a new UUID for our characteristic. Basically, you can copy your previous UUID define and increment the two bytes that you set to 0001 to 0002:

```C
/* This code snippet belongs to remote.h */
/** @brief UUID of the Button Characteristic. **/
#define BT_UUID_REMOTE_BUTTON_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0002, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)
```
Also, add the handle like we did with the service UUID:

```C
/* This code snippet belongs to remote.h */
#define BT_UUID_REMOTE_BUTTON_CHRC 	BT_UUID_DECLARE_128(BT_UUID_REMOTE_BUTTON_CHRC_VAL)
```

I called my handle BT_UUID_REMOTE_BUTTON_CHRC. Whatever you call it, we will now add it to our service macro:

```C
BT_GATT_SERVICE_DEFINE(remote_srv,
BT_GATT_PRIMARY_SERVICE(BT_UUID_REMOTE_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_BUTTON_CHRC,
                    BT_GATT_CHRC_READ,
                    BT_GATT_PERM_READ,
                    read_button_characteristic_cb, NULL, NULL),
);
```

What we are doing here is saying that we want to add a characteristic to our service using the UUID that we just defined. We claim that it is possible to read it, and then we give it the permission to be read. The read_button_characteristic is a callback that is triggered whenever someone is reading our characteristic. The first NULL is the callback for when someone is writing to our characteristic, which will never happen since it is not possible to write to this characteristic. The last NULL is the actual value. We will set that later. </br>
First we need to implement the read_button_characteristic_cb callback function. It is a bit tricky to navigate to the callback definition of this macro, but if you look in gatt.h, where the BT_GATT_CHARACTERISTIC macro is defined, and search for "struct bt_gatt_attr" then this will hold the callbacks that we will use for read, and later for write callbacks. </br>
So we see that the read callback should look something like:

```C
	ssize_t (*read)(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			void *buf, uint16_t len, uint16_t offset);
```

Replace `(*read)` with the name we passed in `BT_GATT_CHARACTERISTIC` and return the return value from bt_gatt_attr_read():

```C
ssize_t bt_gatt_attr_read(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			  void *buf, uint16_t buf_len, uint16_t offset,
			  const void *value, uint16_t value_len)
```

We need this function to return the length of the value being read, and it needs to be returned using bt_gatt_attr_read().

```C
/* This snippet belongs in remote.c */
static uint8_t button_value = 0;

static ssize_t read_button_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                                            void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &button_value, sizeof(button_value));
}
```

**Challenge:**

<br>

***Before we try to connect again, create a function in remote.c that we can call from main.c (add declaration in remote.h) that changes the value of the parameter `button_value` based on an input parameter. Call it "set_button_value()" and call it in the button_handler from main.c, with the button_pressed parameter as the input.***

<br>
</br>


*Hint: If you are stuck, I uploaded another snapshot of the project here:*

[main.c](https://github.com/edvinand/Orbit/blob/main/temp_files/snapshot2/main.c), 

[remote.c](https://github.com/edvinand/Orbit/blob/main/temp_files/snapshot2/custom_files/remote.c), 

[remote.h](https://github.com/edvinand/Orbit/blob/main/temp_files/snapshot2/custom_files/remote.h).

</br>
</br>
Now, try to connect to your device using nRF Connect, and see that you have a characteristic that you can read using the read button in nRF Connect (the button with the down pointing arrow). Whenever you push a button on your DK and read it again, you should see that the is updated.


### Step 7 - Characteristic Notifications
We do not want to keep having to ask our peripheral about the last pressed button all the time. It requires a lot of read requests and read response packets on the air, which is not very power efficient. Therefore we have something called "notifications", which allows the peripheral to push changes to the central whenever they occur. This is set using something called Client Characteristic Configuration Descriptor (CCCD or CCC). The first thing we need to do is to add this descriptor to our characteristic. Do this by adding the last line to your Service macro in remote.c:

```C
/* This code snippet belongs to remote.c */
BT_GATT_SERVICE_DEFINE(remote_srv,
BT_GATT_PRIMARY_SERVICE(BT_UUID_REMOTE_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_BUTTON_CHRC,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ,
                    read_button_characteristic_cb, NULL, NULL),
    BT_GATT_CCC(button_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);
```

**Note that we added ` |BT_GATT_CHRC_NOTIFY` on the line that previously just said `BT_GATT_CHRC_READ`.**

The first parameter is a callback that is triggered whenever someone writes to the CCC. The last parameter is the read/write permissions. Here we allow the central to both read and write to this configuration. This means that it can check whether or not notifications are enabled, and enable/disable it. Please note that we also added the BT_GATT_CHRC_NOTIFY in our properties for the characteristic itself, as we are now adding the possiblilty to enable notifications.

In a similar way to what we did earlier, we can use the BT_GATT_CCC macro definition to find the expected type of callback. See if you can find it in gatt.h. </br></br>
*Hint: it is a callback that is called whenever the CCC has changed*
</br></br>
The implementation of this callback itself is not that complex. We don't have to return anything. We'll just log that notifications were either enabled or disabled. 
 ```C
 /* This code snippet belongs to remote.c */
void button_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
    LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

}
```

The aim of this callback is to keep track of whether or not the central has enabled notifications. We would like to propagate this event to our main.c file. In order to keep things tidy, let us create an Enum that says whether notifications was enabled or disabled, and we want a callback struct similar to the one that we used to receive the connected and disconnected events. The only difference in this callback struct is that since this is a custom service, we need to implement these callbacks ourself.
</br>
Let us start by adding the enum:

```C
/* This code snippet belongs to remote.h */
enum bt_button_notifications_enabled {
	BT_BUTTON_NOTIFICATIONS_ENABLED,
	BT_BUTTON_NOTIFICATIONS_DISABLED,
};
```

Then let us declare our callback struct. Since we create this from scratch, we can call it whatever we like:

```C
/* This code snippet belongs to remote.h */
struct bt_remote_service_cb {
	void (*notif_changed)(enum bt_button_notifications_enabled status);
};
```

So far the only function that we want to forward is the notif_changed, which we will use to forward the callback whenever notifications are enabled or disabled. In order to do so, we will need an instance of the struct in remote.c as well:

```C
/* This code snippet belongs to remote.c */
static struct bt_remote_service_cb remote_service_callbacks;
```

And finally an instance actually containing the callbacks in main.c:

```C
/* This code snippet belongs to main.c */
struct bt_remote_service_cb remote_callbacks = {
	.notif_changed = on_notif_changed,
};
```

Then we need to forward this remote_callbacks struct into bluetooth_init similar to the way we did with the bluetooth_callbacks struct.

Then inside bluetooth_init() in remote.c we will first check that they are not NULL pointers, and then populate them manually:

```C
/* This code snippet belongs to remote.c */
    int err;
    LOG_INF("Initializing Bluetooth");

    if (bt_cb == NULL || remote_cb == NULL) {
        return NRFX_ERROR_NULL;
    }
    bt_conn_cb_register(bt_cb);
    remote_service_callbacks.notif_changed = remote_cb->notif_changed;
    ...
```

now that we have this callback struct in remote.c, it means we can trigger the callback in main.c from remote.c. Add the following to button_chrc_ccc_cfg_changed():

```C
/* This code snippet belongs to remote.c */
void button_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
    LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");
    if (remote_service_callbacks.notif_changed) {
        remote_service_callbacks.notif_changed(notif_enabled?BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED);
    }
}
```

The reason we check the `if (remote_service_callback.notif_changed)` is that we need to check that this very callback function (notif_changed) is set before we call it. Now all we need is to implement the on_notif_changed() function in main.c. All we need it to do is to print that notifications are enabled or disabled:

```C
/* This code snippet belongs to main.c */
void on_notif_changed(enum bt_button_notifications_enabled status)
{
    if (status == BT_BUTTON_NOTIFICATIONS_ENABLED) {
        LOG_INF("Notifications enabled");
    } else {
        LOG_INF("Notifications disabled");
    }
}
```

</br>
Now we are only a few steps away from sending our very first notification (!!).
Let us add a function, send_button_notification(), in remote.c that we can call from main.c (declare it in remote.h).

```C
/* This code snippet belongs to remote.c */
void on_sent(struct bt_conn *conn, void *user_data)
{
    ARG_UNUSED(user_data);
    LOG_INF("Notification sent on connection %p", (void *)conn);
}

int send_button_notification(struct bt_conn *conn, uint8_t value)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_srv.attrs[2];

    params.attr = attr;
    params.data = &value;
    params.len = 1;
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}
```

This one is a lot to take in, but let us see what is going on. In send_button_notification we take two input parameters. The pointer to the connection that we want to send the notification to, and the value of the characteristic, which is the actual payload data being transmitted. If you were to implement this from scratch, start by using the function bt_gatt_notify_cb() and look at what parameters it takes. The params parameter is the struct bt_gatt_notify_params. It holds a lot, but we only populate what we need in our case. We need the attribute, which points to the characteristic where we are actually sending the notification, and we need the value, the length of the value, and a callback function. This callback may be very useful in some cases where you are sending a lot of data, to keep track on when the data is sent. We will just use it to print that we have successfully sent a notification.
</br>
</br>
Now try to call this function from the button handler, check the return value and see if you can send a notification from your peripheral to the connected central. Remember to enable notifications on the characteristic from your phone by pressing the button with the three arrows pointing down.
</br>
</br>
*In case you got stuck anywhere since the last snapshot, I'll upload a 3rd snapshot here:</br>
[main.c](https://github.com/edvinand/Orbit/blob/main/temp_files/snapshot3/main.c)</br>
[remote.c](https://github.com/edvinand/Orbit/blob/main/temp_files/snapshot3/custom_files/remote.c)</br>
[remote.h](https://github.com/edvinand/Orbit/blob/main/temp_files/snapshot3/custom_files/remote.h)*

### Step 8 - Writing Bck to our Peripheral
So now we can send button presses from our remote to our phone. Pretty cool. But since we have a wireless device connected to our phone, and this device has a motor connected to it, it would be nice to be able to control the motor from the phone, right? For this we could use the same characteristic that we already have to send communications both ways, but let us create a new characteristic for this purpose.

**Todo:**</br>
***Add a third UUID where you increment the byte that we did for the previous UUID once more. Call the UUID BT_UUID_REMOTE_MESSAGE_CHRC_VAL and call the characteristic handle BT_UUID_REMOTE_MESSAGE_CHRC***

Let us add the new characteristic to our service macro:

```C
/* This code snippet belongs to remote.c */
BT_GATT_SERVICE_DEFINE(remote_srv,
BT_GATT_PRIMARY_SERVICE(BT_UUID_REMOTE_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_BUTTON_CHRC,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ,
                    read_button_characteristic_cb, NULL, NULL),
    BT_GATT_CCC(button_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_MESSAGE_CHRC,
                    BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                    BT_GATT_PERM_WRITE,
                    NULL, on_write, NULL),
);
```

We have seen this before. We are adding a characteristic to our old service, we claim that the central can write to it, and we give the central the permission to write to it. We don't need a read callback, but we add the `on_write` callback. We do not care about the start value of the characteristic as it is not possible for the central to read it.
</br>
</br>
What remains is to implement our on_write callback. This is a typical callback you would want to propagate to main.c, as in a commercial product, we want to control some other peripherals (PWM) depending on the content of the message from the central. Luckily we already have a way of forwarding events from remote.c to our main.c file. Let us add another event callback in our bt_remote_service_cb struct in remote.h:

```C
/* This code snippet belongs to remote.h */
struct bt_remote_service_cb {
	void (*notif_changed)(enum bt_button_notifications_enabled status);
	void (*data_received)(struct bt_conn *conn, const uint8_t *const data, uint16_t len);
};
```

The parameters that we pass on into our data_received event is totally up to you. For now we will pass the connection pointer, the actual data, and the length of the data. Now we add it to our main.c instance of our remote service callbacks:

```C
/* This code snippet belongs to main.c */
struct bt_remote_service_cb remote_callbacks = {
    .notif_changed = on_notif_changed,
    .data_received = on_data_received,
};
```
Then we need to remember to populate this callback in bluetooth_init():

```C
/* This code snippet belongs to remote.c -> bluetooth_init() */
    bt_conn_cb_register(bt_cb);
    remote_service_callbacks.notif_changed = remote_cb->notif_changed;
    remote_service_callbacks.data_received = remote_cb->data_received;
```

Before we implement the callback in main, let us look at the callback in remote.c. As we saw in the start of this tutorial, it is a bit tricky to find the callback type for the write and read callbacks. Look for the definition of the "struct bt_gatt_attr" in gatt.h, and look at the `(*write)` type. There are a lot of parameters, so let us look into them:

```C
static ssize_t on_write(struct bt_conn *conn,
			  const struct bt_gatt_attr *attr,
			  const void *buf,
			  uint16_t len,
			  uint16_t offset,
			  uint8_t flags)
{
    LOG_INF("Received data, handle %d, conn %p",
        attr->handle, (void *)conn);

    if (remote_service_callbacks.data_received) {
        remote_service_callbacks.data_received(conn, buf, len);
    }
    return len;
}
```

You can see that there are a lot of parameters, but all we really need to do is to forward the important ones to our custom callback and return the length of the message (telling the stack that we have handled the entire message).

Finally, in order to print the message in main.c, you can add the following:

```C
/* This code snippet belongs to main.c */
void on_data_received(struct bt_conn *conn, const uint8_t *const data, uint16_t len)
{
    uint8_t temp_str[len+1];
    memcpy(temp_str, data, len);
    temp_str[len] = 0x00;

    LOG_INF("Received data on conn %p. Len: %d", (void *)conn, len);
    LOG_INF("Data: %s", temp_str);
}
```
What we are doing here is first that we copy the content of the data pointer to a temporary string. This is not strictly necessary, but in this case we want to print the data to the log, and one way to do that is to use the log_strdup() which is looking for a zero-terminated string. To avoid writing to the actual data buffer (which is a very bad idea) we copy the content and add a 0x00 byte at the end.
Then we print who sent the data, the length of the data, and the actual message. 

<br>

Try to write to your new characteristic. Note that when you press the write button (button with upwards pointing arrow), you need to select whether you want to write raw bytes using "ByteArray", or if you want to send a string of data using "UTF8". You can also use Bool or UnsignedInt, but that is not something we need to use. 

Writing to a Characteristic | 
------------ |
<img src="https://github.com/edvinand/Orbit/blob/main/images/write_to_characteristic.jpg" width="300"> |

<br>

But we said that we wanted to control our motors using these messages. Try to check the incoming data, and use this to control the motor. To check the first byte of the data, you can use `data[0]`. Try to set the motor in different positions depending on the first byte of the message.


<br>

*Hint: Perhaps just check whether the first byte is 0x00 or 0x01, and set the motor to one of two predefined angles.*


You can find the final version of the files in the NCS project [here](https://github.com/edvinand/Orbit/tree/main/remote_controller/src).
