#include "uart.h"

const char* hello = "Hello World!\n";

int main(void)
{
    uart_init();
    while (*hello != '\0')
        uart_putchar(*(hello++));
    while (1);
}
