#include "UartCom.h"
#include "main.h"
#include "usart.h"

/*< UartCom_Definations **/
	
/** UartCom_Definations >*/


/*< UartCom_Variables **/

	uint8_t rxBuffer[64];
	uint8_t volatile rxBufIndisCntr = 0;
	uint8_t txBuffer[64];
	uint8_t txBufIndisCntr;
	
	uint8_t msgBuffer[61];
	
	comStatus_t comStatus = COM_IDLE;
	
	uint32_t rxByteTimeout = 0;
	
	uint32_t txTimeout = 0;
	uint32_t txNewMsgTimeout = 0;
	
	uint32_t tickTimer;

/**< UartCom_Variables >*/


/*< UartCom_Functions **/

	//Com Init Function
		void comInit(void)
		{
			comStatus = COM_RX_START;
		}
	
		
	//Creat Com Data Function
		void createComData(uint8_t *Msg, uint8_t Msg_Length)
		{
			txBuffer[0] = 'S';
			txBuffer[1] = Msg_Length + 3;
			
			memcpy(&txBuffer[2], Msg, Msg_Length);
			
			txBuffer[txBuffer[1] - 1] = calculateCheckSum(txBuffer);
		}
	
		
	//Send Com Data Function
		void sendComData(uint8_t *Data)
		{
			HAL_UART_Transmit_IT(&huart1, Data, Data[1]);
		}
	
		
	//Read Com Data Function
		void readComData(void)
		{
			HAL_UART_Receive_IT(&huart1, &rxBuffer[rxBufIndisCntr], 1);
		}
	
		
	//Handle Receive Data Function
		rxDataStatusType_t handleReceiveData(uint8_t *Data)
		{
			uint8_t tempDataLength;
			uint8_t tempCheckSum;
			
			if(Data[0] != 'S') return RX_DATA_FAIL;
			
			tempDataLength = Data[1];
			tempCheckSum = calculateCheckSum(Data);
			
			//if(Data[tempDataLength - 1] != tempCheckSum) return RX_DATA_FAIL;
			
			return RX_DATA_SUCCESSFUL;
		}
	
		
	//Com Handler Function
		void comHandler(void)
		{
			//COM_IDLE
				if(comStatus == COM_IDLE)
				{
					comTimeoutControl();
				}
			
			//COM_RX_START
				if(comStatus == COM_RX_START)
				{
					readComData();
					comStatus = COM_IDLE;
				}
			
			//COM_RX_CMPLTD
				if(comStatus == COM_RX_CMPLTD)
				{
					rxBufIndisCntr = 0;
					
					if(handleReceiveData(rxBuffer) == RX_DATA_SUCCESSFUL)
					{
						comStatus = COM_START_TX;
						txTimeout = 1;	//Set tx timeout
					}
					else
					{
						comStatus = COM_RX_START;
					}
				}
			
			//COM_START_TX
				if(comStatus == COM_START_TX)
				{
					tickTimer = HAL_GetTick();
					
					msgBuffer[0] = (tickTimer & 0xFF000000) >> 24;
					msgBuffer[1] = (tickTimer & 0x00FF0000) >> 8;
					msgBuffer[2] = (tickTimer & 0x0000FF00) << 8;
					msgBuffer[3] = (tickTimer & 0x000000FF) << 24;
					
					createComData(msgBuffer, 4);
					sendComData(txBuffer);
					
					comStatus = COM_BUSY;
				}
				
			//COM_TX_CMPLTD
				if(comStatus == COM_TX_CMPLTD)
				{
					comStatus = COM_RX_START;
				}
		}
	
	
	//Com Timeout Function
		void comTimeoutControl(void)
		{
			
			//COM RX Byte Timeout
			if(rxByteTimeout)
			{
				if(rxByteTimeout > COM_BYTE_RX_TIMEOUT)
				{
					rxByteTimeout = 0;
					comStatus = COM_RX_CMPLTD;
				}
			}
			
			//COM TX Timeout
			if(txTimeout)
			{
				if(txTimeout < 2000)
				{
					if(txNewMsgTimeout > 500)
					{
						comStatus = COM_START_TX;
					}
				}
				else
				{
					txTimeout = 0;
					txNewMsgTimeout = 0;
				}
			}
			
		}
	
	//check Sum Calculate Function
		uint8_t calculateCheckSum(uint8_t *Data)
		{
			uint8_t checkSum;

			checkSum = 'C';
			
			return checkSum;
		}
		
/** UartCom_Functions >*/