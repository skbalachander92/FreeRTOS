
#include "stm32f4xx.h"
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//macros
#define AVAILABLE      TRUE
#define NOT_AVAILABLE  FALSE
#define TRUE           1
#define FALSE          0
//functions
void prvPrintTask(void* params);
void prvNewPrintString(const portCHAR* pcString);
static void pvtHardwareSetup(void);
static void UART_Setup(void);
void printmsg(char* msg);

//variables
TaskHandle_t xTask1Handle=NULL;
TaskHandle_t xTask2Handle=NULL;
char user_msg[100];
uint8_t UART_ACCESS_KEY=AVAILABLE;
SemaphoreHandle_t xMutex = NULL;



#ifdef USE_SEMIHOSTING
extern void initialise_monitor_handles();
#endif

int main(void)
{
#ifdef USE_SEMIHOSTING

	initialise_monitor_handles();
    printf("Its a hello world application\r\n");
#endif

    DWT->CTRL |= (1<<0);//Data watch point trace->cyccnt(cycle count) in arm technical rm,it keeps time stamp

    //reset the RCC clock cconfiguration to default reset state
	//reset state is hsi=on,pll=off,hse=off, sysclk=16MHz

    RCC_DeInit();

    //update the system core clock variable
    SystemCoreClockUpdate();

    pvtHardwareSetup();

    sprintf(user_msg,"\rmutual exclusion using binary semaphore demo\r\n");
    printmsg(user_msg);

    //Start recording
    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Start();

    //create binary semaphore explicitly before starting to use it
    xMutex=xSemaphoreCreateMutex();

    if(xMutex !=NULL){
    //create two tasks:task-1 and task-2
    xTaskCreate(prvPrintTask, "print-1", 250, "TASK 1 ******\r\n", 2, &xTask1Handle);
    xTaskCreate(prvPrintTask, "print-2", 250, "TASK 2-------\r\n", 2, &xTask2Handle);


    //start the scheduler
    vTaskStartScheduler();
    }else{
        sprintf(user_msg,"\rbinary semaphore creation failed\r\n");
        printmsg(user_msg);



    }
	for(;;);
}

void prvNewPrintString(const portCHAR* pcString ){
	char cBuffer[80];

	xSemaphoreTake(xMutex,portMAX_DELAY);

	{
		sprintf(cBuffer,"%s",pcString);
		printmsg(cBuffer);
	}
	xSemaphoreGive(xMutex);

}

void prvPrintTask(void* params){
	char* pcStringToPrint;

	pcStringToPrint=(char*)params;

	srand(567);

	while(1){

		prvNewPrintString(pcStringToPrint);

		vTaskDelay(rand() & 0xF);


	}

}

static void UART_Setup(void){
	GPIO_InitTypeDef UART_Gpio;
		memset(&UART_Gpio,0,sizeof(UART_Gpio));
		USART_InitTypeDef UART_Handle;
		memset(&UART_Handle,0,sizeof(UART_Handle));

	//enable clock for uart peripheral and gpioa peripheral
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		//configure pa2 as uart_tx and pa3 as uart_rx

		UART_Gpio.GPIO_Pin=GPIO_Pin_2 | GPIO_Pin_3;
		UART_Gpio.GPIO_Mode=GPIO_Mode_AF;
		UART_Gpio.GPIO_PuPd=GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &UART_Gpio);

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);//PA2 CONFIG AF7
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);//PA3 CONFIG AF7

		//UART PERIPHERAL CONFIGURATION
		UART_Handle.USART_BaudRate=115200;
		UART_Handle.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
		UART_Handle.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
		UART_Handle.USART_Parity=USART_Parity_No;
		UART_Handle.USART_StopBits=USART_StopBits_1;
		UART_Handle.USART_WordLength=USART_WordLength_8b;

		USART_Init(USART2, &UART_Handle);
		USART_Cmd(USART2, ENABLE);

}

static void pvtHardwareSetup(void){
	UART_Setup();


}

void printmsg(char* msg){

	for(uint32_t i=0;i<strlen(msg);i++){
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
		USART_SendData(USART2, msg[i]);
	}

}













