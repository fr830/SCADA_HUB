//TODO unused warnings from static keyword

#include "stm32f1xx_hal.h"
#include "SX1278Drv.h"
#include <string.h>
#include "main.h"
#include "cmsis_os.h"

#include "mb.h"
#include "mbfunc.h"

uint8_t MB_Reg;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim1;
osTimerId hRxTimer;
osThreadId hMainTask;
SX1278Drv_LoRaConfiguration cfg;

void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void RxTimerCallback(void const * argument);
static void MainTaskFxn(void const * argument);

volatile bool LoRaError; // TODO ugly workaroung
volatile bool LoRaSuccess; // TODO ugly workaroung
volatile uint8_t LoRaData[64]; // TODO ugly workaroung
volatile uint16_t LoRaResponse;

static  eMBException MB_Read_Callback(UCHAR * pucFrame, USHORT * pusLength);
static  eMBException MB_Write_Callback(UCHAR * pucFrame, USHORT * pusLength);

int main(void){

	HAL_Init();

	SystemClock_Config();
	MX_GPIO_Init();
	MX_SPI1_Init();

	uint8_t MB_address = 30;

	eMBInit(MB_RTU, &MB_address, 1, 0, 19200, MB_PAR_NONE);

	eMBRegisterCB( MB_FUNC_READ_HOLDING_REGISTER, MB_Read_Callback );
	eMBRegisterCB( MB_FUNC_WRITE_REGISTER, MB_Write_Callback );
  //eMBRegisterCB( MB_FUNC_READ_INPUT_REGISTER, MB_Read_Callback );
	 eMBEnable();

	osTimerDef(RxTimer, RxTimerCallback);
	hRxTimer = osTimerCreate(osTimer(RxTimer), osTimerOnce, NULL);

	osThreadDef(MainTask, MainTaskFxn, osPriorityNormal, 0, 512);
	hMainTask = osThreadCreate(osThread(MainTask), NULL);

	cfg.bw = SX1278Drv_RegLoRaModemConfig1_BW_125;
	cfg.cr = SX1278Drv_RegLoRaModemConfig1_CR_4_8;
	cfg.crc = SX1278Drv_RegLoRaModemConfig2_PayloadCrc_ON;
	cfg.frequency = 434e6;
	cfg.hdrMode = SX1278Drv_RegLoRaModemConfig1_HdrMode_Explicit;
	cfg.power = 17;
	cfg.preambleLength = 20;//
	cfg.sf = SX1278Drv_RegLoRaModemConfig2_SF_11;
	cfg.spi = &hspi1;
	cfg.spi_css_pin = &SPICSMyPin;
	//cfg.rx_en = &LoRaRxEnPin;
	//cfg.tx_en = &LoRaTxEnPin;
	cfg.sleepInIdle = true;

	SX1278Drv_Init(&cfg);

	uint16_t relayAddr = 1;
	uint16_t inputAddr = 2;

	SX1278Drv_SetAdresses(0, &relayAddr, 1);
	SX1278Drv_SetAdresses(1, &inputAddr, 1);

	osKernelStart();

	return 0;
}

void SystemClock_Config(void){
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
		Error_Handler();

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
		Error_Handler();

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);

}

/* SPI1 init function */
static void MX_SPI1_Init(void){
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
		Error_Handler();
	GPIO_PIN_SET(&SPICSPin);
}

static void MX_GPIO_Init(void){
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
}

/* USER CODE BEGIN 4 */
static  eMBException MB_Read_Callback(UCHAR * pucFrame, USHORT * pusLength)
{
	return eMBFuncReadHoldingRegister(pucFrame, pusLength);
}

static  eMBException MB_Write_Callback(UCHAR * pucFrame, USHORT * pusLength)
{
	return eMBFuncWriteHoldingRegister(pucFrame, pusLength);
}

eMBErrorCode    eMBRegHoldingCB( UCHAR * pucRegBuffer,  UCHAR ucMBAddress, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode ){
	LoRa_Message msg;
/*	if(ucMBAddress == 30){
		if((eMode == MB_REG_WRITE) &&
		   (usNRegs == 1) &&
		   (usAddress == 1)){
			msg.address = 1;
			memcpy(msg.payload, pucRegBuffer, 2);
			msg.payloadLength = 2;
			SX1278Drv_SendMessage(&msg);
			LoRaError = false;
			LoRaSuccess = false;
			osTimerStart(hRxTimer, 6000);
			while((!LoRaError) && (!LoRaSuccess));
			if(LoRaError)
				return MB_ETIMEDOUT;

			return MB_ENOERR;
		}
		else if((eMode == MB_REG_READ) &&
				(usNRegs == 1) &&
				(usAddress == 2)){
			msg.address = 2;
			msg.payload[0] = 1;
			msg.payloadLength = 1;
			SX1278Drv_SendMessage(&msg);
			LoRaError = false;
			LoRaSuccess = false;
			osTimerStart(hRxTimer, 5000);
			while((!LoRaError) && (!LoRaSuccess));
			if(LoRaError)
				return MB_ETIMEDOUT;

			memcpy(pucRegBuffer, (uint8_t *)&LoRaResponse, 2);
			return MB_ENOERR;
		}
	}*/

	return MB_ENOREG;

}
eMBErrorCode    eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress,
                               USHORT usNRegs )
{
	pucRegBuffer[0] = MB_Reg;
		MB_Reg++;
		return MB_ENOERR;
}

void Error_Handler(void){
  while(1);
}

static void MainTaskFxn(void const * argument){
	while(1){
		eMBPoll();
	}
}

static void RxTimerCallback(void const * argument){
	LoRaError = true;
}

void SX1278Drv_LoRaRxCallback(LoRa_Message *msg){
	memcpy(LoRaData, msg->payload + 4, msg->payloadLength - 4);
	LoRaSuccess = true;
}

void SX1278Drv_LoRaRxError(){
	//osTimerStop(hRxTimer);
	//LoRaError = true;
}

void SX1278Drv_LoRaTxCallback(LoRa_Message *msg){}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if (htim->Instance == TIM1)
    HAL_IncTick();
}
