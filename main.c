#include "MS51_16K.H"

int main() {
    MODIFY_HIRC(HIRC_24);
    ALL_GPIO_QUASI_MODE;

    // Initialize UART1 for Console.
    P16_QUASI_MODE;
    P02_INPUT_MODE;
    UART_Open(24000000, UART1_Timer3, 115200);
    ENABLE_UART1_PRINTF;

    printf("Hello World!\n");

    return 1;
}
