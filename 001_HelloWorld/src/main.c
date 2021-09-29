
#include "stm32f4xx.h"
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include "FreeRTOS.h"
#include "task.h"

//macros
#define AVAILABLE      TRUE
#define NOT_AVAILABLE  FALSE
#define TRUE           1
#define FALSE          0
//functions
void vTask1_handler(void* params);
void vTask2_handler(void* params);
static void pvtHardwareSetup(void);
static void UART_Setup(void);
void printmsg(char* msg);

//variables
TaskHandle_t xTask1Handle=NULL;
TaskHandle_t xTask2Handle=NULL;
char user_msg[100];
uint8_t UART_ACCESS_KEY=AVAILABLE;


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

    sprintf(user_msg,"\rthis is the beginning of hello world project\r\n");
    printmsg(user_msg);

    //Start recording
    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Start();

    //create two tasks:task-1 and task-2
    xTaskCreate(vTask1_handler, "Task-1", configMINIMAL_STACK_SIZE, NULL, 2, &xTask1Handle);
    xTaskCreate(vTask2_handler, "Task-2", configMINIMAL_STACK_SIZE, NULL, 2, &xTask2Handle);
    //start the scheduler
    vTaskStartScheduler();
	for(;;);
}

void vTask1_handler(void* params){
	while(1){
		if(UART_ACCESS_KEY==AVAILABLE){
			UART_ACCESS_KEY=NOT_AVAILABLE;
			printmsg("UART message from task-1\r\n");
			UART_ACCESS_KEY=AVAILABLE;
			taskYIELD();

		}

	}

}
void vTask2_handler(void* params){
	while(1){
		if(UART_ACCESS_KEY==AVAILABLE){
					UART_ACCESS_KEY=NOT_AVAILABLE;
					printmsg("UART message from task-2\r\n");
					UART_ACCESS_KEY=AVAILABLE;
					taskYIELD();

		}
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













