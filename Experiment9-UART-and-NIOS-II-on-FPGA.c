#include "sys/alt_stdio.h"

#include "stdio.h"

#include "system.h"

#include "altera_avalon_pio_regs.h"

int main() {
    //alt_putstr("Hello World"); 
    /* Event loop never exits. */
    while (1) {
      IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0b1000);
      int value = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
      switch (value) {
      case 16:
        printf("1\n");
        break;
      case 8:
        printf("4\n");
        break;
      case 4:
        printf("7\n");
        break;
      case 2:
        printf("*\n");
        break;
      }
      IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0b0100);
      value = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
      switch (value) {
      case 16:
        printf("2\n");
        break;
      case 8:
        printf("5\n");
        break;
      case 4:
        printf("8\n");
        break;
      case 2:
        printf("0\n");
        break;
      }
      IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0b0010);
      value = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
      switch (value) {
      case 16:
        printf("3\n");
        break;
      case 8:
        printf("6\n");
        break;
      case 4:
        printf("9\n");
        break;
      case 2:
        printf("#\n");
        break;
      }
      IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0b0001);
      value = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
      switch (value) {
      case 16:
        printf("A\n");
        break;
      case 8:
        printf("B\n");
        break;
      case 4:
        printf("C\n");
        break;
      case 2:
        printf("D\n");
        break;
      }
    }
    return 0;
  }

  **
  **

  int main() {
    //alt_putstr("Hello World"); 
    /* Event loop never exits. */
    int f = 0;
    int f1 = 0;
    int f2 = 0;
    int f3 = 0;
    while (1) {
      IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0b1000);
      int value = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
      if (value == 0) {
        f = 0;
      }
      switch (value) {
      case 8:
        if (f == 0) {
          printf("D\n");
          f = 1;
        }
        break;
      case 4:
        if (f == 0) {
          printf("C\n");
          f = 1;
        }
        break;
      case 2:
        if (f == 0) {
          printf("B\n");
          f = 1;
        }
        break;
      case 1:
        if (f == 0) {
          printf("A\n");
          f = 1;
        }
        break;

      }
      IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0b0100);
      value = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
      if (value == 0) {
        f1 = 0;
      }
      switch (value) {
      case 8:
        if (f1 == 0) {
          printf("#\n");
          f1 = 1;
        }
        break;
      case 4:
        if (f1 == 0) {
          printf("9\n");
          f1 = 1;
        }
        break;
      case 2:
        if (f1 == 0) {
          printf("6\n");
          f1 = 1;
        }
        break;
      case 1:
        if (f1 == 0) {
          printf("3\n");
          f1 = 1;
        }
        break;
      }
      IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0b0010);
      value = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
      if (value == 0) {
        f2 = 0;
      }
      switch (value) {
      case 8:
        if (f2 == 0) {
          printf("O\n");
          f2 = 1;
        }
        break;
      case 4:
        if (f2 == 0) {
          printf("8\n");
          f2 = 1;
        }
        break;
      case 2:
        if (f2 == 0) {
          printf("5\n");
          f2 = 1;
        }
        break;
      case 1:
        if (f2 == 0) {
          printf("2\n");
          f2 = 1;
        }
        break;
      }
      IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0b0001);
      value = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
      if (value == 0) {
        f3 = 0;
      }
      switch (value) {
      case 8:
        if (f3 == 0) {
          printf("*\n");
          f3 = 1;
        }
        break;
      case 4:
        if (f3 == 0) {
          printf("7\n");
          f3 = 1;
        }
        break;
      case 2:
        if (f3 == 0) {
          printf("4\n");
          f3 = 1;
        }
        break;
      case 1:
        if (f3 == 0) {
          printf("1\n");
          f3 = 1;
        }
        break;
      }
      for (int i = 0; i < 5000; i++) {}

    }
    return 0;
  }
