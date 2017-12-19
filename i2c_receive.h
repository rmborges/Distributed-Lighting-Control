#ifndef I2C_H_
#define I2C_H_ 

#include <pigpio.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include "arduino.h"


using namespace std;

#define MSG_SIZE 16 // 16 ou 17?????????

class i2c_receive {

public:
	i2c_receive();
	
	int read_from_i2c();

};

#endif
