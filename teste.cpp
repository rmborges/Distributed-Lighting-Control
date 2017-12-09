#include <pigpio.h>
#include <iostream>
using namespace std;


int main(){
    bsc_xfer_t xfer;
    gpioInitialise();
    xfer.control = (0x0A<<16) | 0x305; // Set I2C slave Address to 0x0A
    int status = bscXfer(&xfer);
    if (status >= 0)
    {
        xfer.rxCnt = 0;
        while(1){
            if (xfer.rxCnt > 0){
                cout << xfer.rxBuf << '\n';
            }
       }    
    }

    return 1;
} 