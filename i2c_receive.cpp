#include <pigpio.h>
#include <iostream>
using namespace std;

#include "i2c_receive.h"


/* FUNÇÃO bscXfer

typedef struct
{
   uint32_t control;          // N/A
   int rxCnt;                 // Read only
   char rxBuf[BSC_FIFO_SIZE]; // Read only
   int txCnt;                 // Write
   char txBuf[BSC_FIFO_SIZE]; // Write
} bsc_xfer_t;

txCnt is set to the number of bytes to be transmitted, possibly zero. The data itself should be copied to txBuf.

Any received data will be written to rxBuf with rxCnt set.

See bsc_xfer for details of the returned status value.

If there was an error the status will be less than zero (and will contain the error code).

Note that an i2c_address of 0 may be used to close the BSC device and reassign the used GPIO (18/19) as inputs. 
*/


i2c_receive::i2c_receive()
{

	read_from_i2c();
}

int i2c_receive::read_from_i2c()
{
	cout << "ENTREI NO I2C" << endl << endl;
	
	char i2c_msg[MSG_SIZE];

    if (gpioInitialise()<0) 
		return 1;
    
    while(1){
		bsc_xfer_t xfer;
		xfer.control = (0x9<<16) | 0x205; // Set I2C slave Address to 0x9
		xfer.txCnt = 0;
		int status = bscXfer(&xfer);

		if (status >= 0)
		{
	   
				if (xfer.rxCnt > 0){
					cout << xfer.rxBuf << endl;
				}
		   
		}
	}
    gpioTerminate();

    return 0;
} 
