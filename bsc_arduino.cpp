#include <iostream>
#include "pigpio.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>


using namespace std;


void i2c(void) // argumentos??? id,tick
{
    
}

int main(int argc,char *argv[])
{
    int SDA = 18, SCL = 19, I2C_ADDR = 9;
    
    if(gpioInitialise()<0)
    {
        cout << "Error initialising pigpio" << endl;
        return 1; //failed
    }
    
    
    
    
    gpioTerminate();
    
    return 0;
}