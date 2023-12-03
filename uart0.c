#include "msp.h"
#include <file.h>
#include <stdio.h>

void uart0_init_115200_8n1(void) {
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SSEL__SMCLK | EUSCI_A_CTLW0_SWRST;
    EUSCI_A0->BRW = 104; // 12000000 / 115200 = 104.1667
    EUSCI_A0->MCTLW &= ~(EUSCI_A_MCTLW_OS16 | EUSCI_A_MCTLW_BRS_MASK | EUSCI_A_MCTLW_BRF_MASK); // clear reserved bit
    P1->SEL0 |= (BIT3 | BIT2);
    P1->SEL1 &= ~(BIT3 | BIT2);
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
    EUSCI_A0->IE &= ~(EUSCI_A_IE_RXIE | EUSCI_A_IE_TXIE | EUSCI_A_IE_STTIE | EUSCI_A_IE_TXCPTIE);
}

void uart0_outchar(char c) {
    while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = c;
}

char uart0_inchar(void) {
    while (!(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG));
    char c = (char)(EUSCI_A0->RXBUF);
    uart0_outchar(c); // echo
    return c;
}

void uart0_outstring(char *s) {
    while (*s != '\0') {
        uart0_outchar(*s);
        s++;
    }
}

static int uart0_open(const char *path, unsigned flags, int llv_fd) {
    return 0;
}
static int uart0_close(int dev_fd) {
    return 0;
}
static int uart0_read(int dev_fd, char *buf, unsigned count) {
    const char *end = buf + count;
    while (buf != end) {
        *buf = uart0_inchar();
        buf++;
    }
    return (int)count;
}
static int uart0_write(int dev_fd, const char *buf, unsigned count) {
    const char *end = buf + count;
    while (buf != end) {
        uart0_outchar(*buf);
        buf++;
    }
    return (int)count;
}
static off_t uart0_lseek(int dev_fd, off_t offset, int origin) {
    return 0; // unsupported
}
static int uart0_unlink(const char *path) {
    return 0; // ignore
}
static int uart0_rename(const char *old_name, const char *new_name) {
    return 0; // ignore
}

void uart0_init_stdio(void) {
    uart0_init_115200_8n1();
    if (add_device("uart0", _MSA, uart0_open, uart0_close, uart0_read, uart0_write, uart0_lseek, uart0_unlink, uart0_rename) != 0)
        return;
    freopen("uart0:", "w", stdout);
    freopen("uart0:", "w", stderr);
    freopen("uart0:", "r", stdin);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}
