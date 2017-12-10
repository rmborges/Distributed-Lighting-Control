// bsc_xfer:= a structure defining the transfer

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "pigpio.h"

#define BSC_FIFO_SIZE 16

typedef struct
{
   uint32_t control;          // Write
   int rxCnt;                 // Read only
   char rxBuf[BSC_FIFO_SIZE]; // Read only
   int txCnt;                 // Write
   char txBuf[BSC_FIFO_SIZE]; // Write
} bsc_xfer_t;

int main(void)
{

    bsc_xfer_t xfer;

    xfer.control = (0x13<<16) | 0x305;

    memcpy(xfer.txBuf, "ABCD", 4);

    xfer.txCnt = 4;

    status = bscXfer(&xfer);

    if (status >= 0)
    {
        // process transfer
        printf("Hello World!!");
    }

    return 0;
}