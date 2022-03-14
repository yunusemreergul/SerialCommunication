#ifndef UART_COM_H
#define UART_COM_H

#include "stdbool.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"

/*< UartCom_Definations **/

	#define COM_BYTE_RX_TIMEOUT 10
	
	typedef enum RX_DATA_STATUS{
		RX_DATA_FAIL 				= 0,
		RX_DATA_SUCCESSFUL 	= 1,
	}rxDataStatusType_t;

	typedef enum COM_STATUS{
		COM_RX_START  = 0,
		COM_RX_CMPLTD = 1,
		COM_START_TX  = 2,
		COM_TX_CMPLTD = 3,
		COM_IDLE 		  = 4,
		COM_BUSY 		  = 5,
	}comStatus_t;

	typedef struct COM_FLAGS{
		uint8_t receiveComplated 	: 1;
		uint8_t transmitComplated : 1;
	}comFlags_t;
	
/** UartCom_Definations >*/


/*< UartCom_Variables **/

	extern uint8_t rxBuffer[64];
	extern volatile uint8_t rxBufIndisCntr;
	extern uint8_t txBuffer[64];
	
	extern uint8_t msgBuffer[61];
	
	extern comStatus_t comStatus;
	
	extern uint32_t rxByteTimeout;
	
	extern uint32_t txTimeout;
	extern uint32_t txNewMsgTimeout;

/**< UartCom_Variables >*/


/*< UartCom_Function_Prototypes **/

	void comInit(void);
	void createComData(uint8_t *Msg, uint8_t Msg_Length);
	void sendComData(uint8_t *Data);
	void readComData(void);
	rxDataStatusType_t handleReceiveData(uint8_t *Data);
	void comHandler(void);
	
	void comTimeoutControl(void);	//Using static 'C' char.
	uint8_t calculateCheckSum(uint8_t *Data);

/** UartCom_Function_Prototypes >*/

#endif //UART_COM_H

