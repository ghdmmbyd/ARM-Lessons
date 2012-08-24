#include <linux/module.h>
#include <linux/fs.h>	// register_chrdev
#include <asm/io.h>
#include <plat/map-base.h>
#include <plat/regs-serial.h>
//#include <asm/uaccess.h>

#define MA	243

MODULE_LICENSE("GPL");

struct uart_sfr
{
	int ulcon;
	int ucon;
	int ufcon;
	int umcon;
	int utrstat;
	int uerstat;
	int ufstat;
	int umstat;
	int utxh;
	int urxh;
	int ubrdiv;
	int udivslot;
};

typedef struct uart_sfr USFR;

static volatile USFR *puart;
static volatile USFR *puart2;

//#define printk 		noprintk	

int noprintk(char * fmt, ...)
{ 
	return 0; 
} 

#define PRINT(x)	printk(#x " = 0x%x\n", x);

#include <linux/irq.h>
#include <linux/interrupt.h>		// request_irq

// see it in arch/arm/plat-s5p/include/plat/irqs.h
#define UART_RX			(28 + 0)
#define UART_ERR		(28 + 1)
#define UART_TX			(28 + 2)

int flag = 0;
wait_queue_head_t  uart_waitq;

int myputchar(char);

char outputc;

static irqreturn_t uart_read_irq_handler(int irq, void * dev_id)
{
	char c;

	printk("in read irq!!!\n");

	flag = 1;
	wake_up_interruptible(&uart_waitq);

	c = puart2->urxh;
	myputchar(c);

	return IRQ_HANDLED;
}

static irqreturn_t uart_write_irq_handler(int irq, void * dev_id)
{
	static int i = 0;

	printk("in write irq!!! %d \n", i++);

	puart2->utxh = outputc;

	disable_irq_nosync(UART_TX);
	//disable_irq(UART_TX);		

	return IRQ_HANDLED;
}

int uart_open(struct inode * inode, struct file * filp)
{
	int major = MAJOR(inode->i_rdev);
	int minor = MINOR(inode->i_rdev);
	int * p;
	int i;
	int ri;

	printk("uart open: major %d, minor %d\n", major, minor);

//	puart = ioremap(0xe2900000, sizeof(USFR));	// uart0
//	puart = ioremap(0xe2900000, 0x10000);	// uart0
	puart = S3C24XX_VA_UART0;

	puart2 = ioremap(0xe2900c00, sizeof(USFR));	// uart3
//	puart2 = S3C24XX_VA_UART1;
	p = (int *)puart;
	PRINT((int)p);	
	for (i = 0; i < sizeof(USFR)/4; i++)
	{ 
		PRINT(*p++);	
	} 
//	puart = (USFR *)(((int)puart) + 0xc000);
//	puart->ufcon = 0;		// fifo is important!
	p = (int *)puart2;
	PRINT((int)p);	
	for (i = 0; i < sizeof(USFR)/4; i++)
	{
		PRINT(*p++);	
	}

	puart2->ucon = 0x7c5;
	puart2->ubrdiv = 0x23;		// 115200
	puart2->udivslot = 0x808;	// 115200

	p = (int *)puart2;
	PRINT((int)p);	
	for (i = 0; i < sizeof(USFR)/4; i++)
	{ PRINT(*p++);	
	}
#if 0
	puart->ulcon = 3;
	puart->ucon = 0x7c5;
//	puart->ufcon = 0x111;		// fifo is important!
	puart->umcon = 0;
	puart->ubrdiv = 0x23;		// 115200
	puart->udivslot = 0x808;	// 115200
#endif

	ri = request_irq(UART_RX, uart_read_irq_handler, 0, "read_irq", NULL);
	if (ri)
		printk("request irq %d failed! ri = %d\n", UART_RX, ri);
	else
		printk("request irq %d ok! ri = %d\n", UART_RX, ri);

	init_waitqueue_head(&uart_waitq);

	ri = request_irq(UART_TX, uart_write_irq_handler, 0, "write_irq", NULL);
	if (ri)
		printk("request irq %d failed! ri = %d\n", UART_TX, ri);
	else
		printk("request irq %d ok! ri = %d\n", UART_TX, ri);

	printk("uart open finished!\n"); 

	return 0;
}

int uart_release(struct inode * inode, struct file * filp)
{
	printk("uart release\n");

	free_irq(UART_RX, NULL);
	free_irq(UART_TX, NULL);
	printk("free irq %d ok! \n", UART_RX);
	printk("free irq %d ok! \n", UART_TX);

	return 0;
}

#if 0
int uart_putchar(char c)
{
	while ((puart->utrstat & (1<<2)) == 0)
		;

	puart->utxh = c;
#if 1
	while ((puart2->utrstat & (1<<2)) == 0)
		;

	puart2->utxh = c;
#endif

	return 0;
}
#else

int uart_putchar(char c)
{
	// uart0 putchar using polling 
#if 0 
	while ((puart->utrstat & (1<<2)) == 0)
		;

	puart->utxh = c;
#endif
	printk("uart putchar %c(%d)\n", c, c);

	// uart3 putchar using irq
	outputc = c;	
	enable_irq(UART_TX);

	return 1; 
}

#endif

int myputchar(char c)
{
	if (c == '\n')
		uart_putchar('\r');

	uart_putchar(c);

	return 0;
}


int uart_read(struct file * filp, char __user * buf, size_t count, loff_t *f_pos)
{
	printk("uart read\n");

	wait_event_interruptible(uart_waitq, flag != 0);
	flag = 0;

	return 0;
}

int uart_write(struct file * filp, const char __user * buf, size_t count, loff_t *f_pos)
{
	int i;

	printk("uart write\n");

	printk("buf = %c\n", buf[0]);
	printk("count = %d\n", count);

	for (i = 0; i < count; i++)
	{
		//myputchar(buf[i]);
		uart_putchar(buf[i]);
	}

//	myputchar(buf[0]);

	return count;
}

int uart_ioctl(struct inode * inode, struct file * filp, unsigned int cmd, unsigned long arg)
{
	printk("uart ioctl\n");

	printk("cmd = %d\n", cmd);

	if (cmd == 19200)
		puart->ubrdiv = 0xd5;

	if (cmd == 115200)
		puart->ubrdiv = 0x23;

	return 0;
}

struct file_operations uart_fops = 
{
	.owner = THIS_MODULE,
	.open = uart_open,
	.release = uart_release,
	.write = uart_write,
	.read = uart_read,
	//.ioctl = uart_ioctl,
};


int uart_init(void)
{
	int rc;

	rc = register_chrdev(MA, "myuart", &uart_fops);

	if (rc < 0)
	{
		printk("register chrdev failed! %d\n", rc);
		return -1;
	}

	printk("uart init ok \n");

	return 0;
}

void uart_exit(void)
{
	printk("uart exit ok \n");

	unregister_chrdev(MA, "myuart");


	return;
}


module_init(uart_init);
module_exit(uart_exit);
