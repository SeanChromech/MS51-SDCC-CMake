#include "MS51_16K.H"

int main() {
    MODIFY_HIRC(HIRC_24);
    ALL_GPIO_QUASI_MODE;

    Enable_UART0_VCOM_printf_24M_115200();
    printf("Hello World!\n");

    return 1;
}
