#include "i2c_receive.h"

// Identificadores das mensagens do I2C (primeiro caracter)
#define I2C_ID "I"

using namespace std;

std::vector<arduino*> arduino_list;

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
	cout << " I2C" << endl;
	//this->read_from_i2c();
	
}

//extern std::vector<arduino*> arduino_list;

int i2c_receive::read_from_i2c()
{
	//extern std::vector<arduino*> arduino_list;
	/*
	#define BSC_FIFO_SIZE 30

	typedef struct
	{
	   uint32_t control;          
	   int rxCnt;                 
	   char rxBuf[BSC_FIFO_SIZE]; 
	   int txCnt;                 
	   char txBuf[BSC_FIFO_SIZE]; 
	} bsc_xfer_t;
	*/
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
				//cout << xfer.rxBuf << endl;
				string msg_arduino = string(xfer.rxBuf);
				if (msg_arduino.find(I2C_ID) != string::npos) { // para verificar se msg tem identificador
					bool ja_existe = false;
					vector<string> strs;
					boost::split(strs,msg_arduino, boost::is_any_of(" "));
					string arduino_id = strs[1]; // ID do arduino que enviou a mensagem
					if (::arduino_list.empty() == false) {
						for (auto ard : arduino_list) {
							if (ard->arduino_ID == arduino_id && ja_existe == false) {
								//ard->parse_i2c(msg_arduino);  // chama rotina que trata os dados
								ard->parse_i2c("1 150 103.120 60.2 25.14 20");
								ja_existe = true;
							}
						}
					}
					if (ja_existe == false) {
						cout << arduino_id << endl;
						arduino *ard = new arduino(arduino_id);
						arduino_list.push_back(ard);
					}
				}
				//cout << "tenho " << ::arduino_list.size() << "!\n";
			}
			memset(&xfer.rxBuf,0,sizeof(xfer.rxBuf));
		}
	} 
	gpioTerminate();
	return 0;
}
