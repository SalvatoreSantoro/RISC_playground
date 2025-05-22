#define UART_BASE 0x10000000

#define RHR_REG 0
#define THR_REG 0
#define IER_REG 1
#define ISR_REG 2
#define FCR_REG 2
#define LCR_REG 3
#define LSR_REG 5
#define SPR_REG 7

#define DLL_REG 0
#define DLM_REG 1
#define PSD_REG 5

// values
#define RESET 0x00
#define LCR_CWL8B (0b11 << 0)
#define LCR_DLAB (0b1 << 7)
#define FCR_ENBL (0b1)
#define IER_THR_EMPTY (0b1 << 0)
#define IER_DATA_READY (0b1 << 1)

static volatile unsigned char* uart_16650 = (unsigned char*)UART_BASE;

#define write_reg(x, val) (*(uart_16650 + x) = val)
#define read_reg(x) (*(uart_16650 + x))

#define RX_READY (read_reg(LSR_REG) & (0b1 << 0))
#define TX_READY (read_reg(LSR_REG) & (0b1 << 5))

void uart_init(void)
{
    // disable interruptions
    write_reg(IER_REG, RESET);

    // division latch
    write_reg(LCR_REG, LCR_DLAB);

    // baud-rate 115200
    write_reg(DLL_REG, 0x01);
    write_reg(DLM_REG, 0x00);

    // reset prescaler
    write_reg(PSD_REG, RESET);

    // DLAB to 0 + 8 bits char
    write_reg(LCR_REG, LCR_CWL8B);

    // enable fifos
    write_reg(FCR_REG, RESET | FCR_ENBL);

    // enable interrupts
    //write_reg(IER_REG, IER_DATA_READY | IER_THR_EMPTY);
};

void uart_putchar(char c)
{
    while (!TX_READY) { };
    write_reg(THR_REG, c);
};

char uart_getchar(void)
{
    while (!RX_READY) { };
    return read_reg(RHR_REG);
};
