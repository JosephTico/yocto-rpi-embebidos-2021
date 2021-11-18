/*
 * chardev.c: Creates a read-only char device that says how many times you've
 * read from the dev file.
 *
 * You can have some fun with this by removing the module_get/put calls,
 * allowing the module to be removed while the file is still open.
 *
 * Compile with `make`. Load with `sudo insmod chardev.ko`. Check `dmesg | tail`
 * output to see the assigned device number and command to create a device file.
 *
 * From TLDP.org's LKMPG book.
 */

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h> /* for put_user */
#include <linux/gpio.h>
#include <linux/interrupt.h> // Required for the IRQ code

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joseph Vargas");                                 ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux driver for a GPIO sensor."); ///< The description -- see modinfo
MODULE_VERSION("0.1");
/*
 * Prototypes - this would normally go in a .h file
 */

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "gpio-sensor"
#define BUF_LEN 80

static int major = -1;
static struct cdev mycdev;
static struct class *myclass = NULL;

// GPIO definitions
#define GPIO_OUTPUT 1
#define GPIO_INPUT 0
#define GPIO_PIN 4

static unsigned int irqNumber;

/*
 * Global variables are declared as static, so are global within the file.
 */

static int Major;
static int Device_Open = 0;
static char msg[BUF_LEN];
static char *msg_Ptr;

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release};

/*
 * This function is called when cleaning the module to remove the device file
 */
static void cleanup(int device_created)
{
  if (device_created)
  {
    device_destroy(myclass, major);
    cdev_del(&mycdev);
  }
  if (myclass)
    class_destroy(myclass);
  if (major != -1)
    unregister_chrdev_region(major, 1);
}

static irq_handler_t gpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
  printk(KERN_INFO "GPIO_TEST: Interrupt! (MOTION SENSOR state is %d)\n", gpio_get_value(GPIO_PIN));
  return (irq_handler_t)IRQ_HANDLED; // Announce that the IRQ has been handled correctly
}

// Initialize pointers: performs memory mapping, exits if mapping fails
void gpio_driver_init(void)
{
  int result = 0;

  gpio_request(GPIO_PIN, "sysfs"); // Set up the gpio sensor
  gpio_direction_input(GPIO_PIN);  // Set the GPIO to be an input
  gpio_export(GPIO_PIN, false);    // Causes gpio115 to appear in /sys/class/gpio

  // Perform a quick test to see that the button is working as expected on LKM load
  printk(KERN_INFO "GPIO_TEST: The button state is currently: %d\n", gpio_get_value(GPIO_PIN));

  // GPIO numbers and IRQ numbers are not the same! This function performs the mapping for us
  irqNumber = gpio_to_irq(GPIO_PIN);
  printk(KERN_INFO "GPIO_TEST: The button is mapped to IRQ: %d\n", irqNumber);

  // This next call requests an interrupt line
  result = request_irq(irqNumber,                       // The interrupt number requested
                       (irq_handler_t)gpio_irq_handler, // The pointer to the handler function below
                       IRQF_TRIGGER_RISING,             // Interrupt on rising edge (button press, not release)
                       "gpio_irq_handler",              // Used in /proc/interrupts to identify the owner
                       NULL);                           // The *dev_id for shared interrupt lines, NULL is okay

  printk(KERN_INFO "GPIO_TEST: The interrupt request result is: %d\n", result);
}

/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
  int device_created = 0;

  /* cat /proc/devices */
  if (alloc_chrdev_region(&major, 0, 1, DEVICE_NAME "_proc") < 0)
    goto error;
  /* ls /sys/class */
  if ((myclass = class_create(THIS_MODULE, DEVICE_NAME "_sys")) == NULL)
    goto error;
  /* ls /dev/ */
  if (device_create(myclass, NULL, major, NULL, DEVICE_NAME) == NULL)
    goto error;
  device_created = 1;
  cdev_init(&mycdev, &fops);
  if (cdev_add(&mycdev, major, 1) == -1)
    goto error;
  gpio_driver_init();
  return 0;
error:
  cleanup(device_created);
  return -1;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
  /*
   * Unregister the device
   */
  cleanup(1);

  free_irq(irqNumber, NULL); // Free the IRQ number, no *dev_id required in this case
  gpio_unexport(GPIO_PIN);   // Unexport the Button GPIO
  gpio_free(GPIO_PIN);       // Free the LED GPIO
}

/*
 * Methods
 */

/*
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *filp)
{
  if (Device_Open)
    return -EBUSY;

  Device_Open++;
  sprintf(msg, "%d\n", gpio_get_value(GPIO_PIN));
  msg_Ptr = msg;
  /*
   * TODO: comment out the line below to have some fun!
   */
  try_module_get(THIS_MODULE);

  return SUCCESS;
}

/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *filp)
{
  Device_Open--;

  /*
   * Decrement the usage count, or else once you opened the file, you'll never
   * get rid of the module.
   *
   * TODO: comment out the line below to have some fun!
   */
  module_put(THIS_MODULE);

  return SUCCESS;
}

/*
 * Called when a process, which already opened the dev file, attempts to read
 * from it.
 */
static ssize_t device_read(struct file *filp, /* see include/linux/fs.h   */
                           char *buffer,      /* buffer to fill with data */
                           size_t length,     /* length of the buffer     */
                           loff_t *offset)
{
  /*
   * Number of bytes actually written to the buffer
   */
  int bytes_read = 0;

  /*
   * If we're at the end of the message, return 0 signifying end of file.
   */
  if (*msg_Ptr == 0)
    return 0;

  /*
   * Actually put the data into the buffer
   */
  while (length && *msg_Ptr)
  {
    /*
     * The buffer is in the user data segment, not the kernel segment so "*"
     * assignment won't work. We have to use put_user which copies data from the
     * kernel data segment to the user data segment.
     */
    put_user(*(msg_Ptr++), buffer++);
    length--;
    bytes_read++;
  }

  /*
   * Most read functions return the number of bytes put into the buffer
   */
  return bytes_read;
}

/*
 * Called when a process writes to dev file: echo "hi" > /dev/hello
 */
static ssize_t
device_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
  printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
  return -EINVAL;
}
