
#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<string.h>


#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

//macros
#define AVAILABLE      TRUE
#define NOT_AVAILABLE  FALSE
#define TRUE           1
#define FALSE          0
//functions
void vManager_Task(void* params);
void vEmployee_Task(void* params);
void EmployeeDoWork(unsigned char);

static void pvtHardwareSetup(void);
static void UART_Setup(void);
void printmsg(char* msg);

//variables
TaskHandle_t xTask1Handle=NULL;
TaskHandle_t xTask2Handle=NULL;

SemaphoreHandle_t xWork;
QueueHandle_t xWorkQueue;


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

    sprintf(user_msg,"\rDemo of binary semaphore for sync between two tasks\r\n");
    printmsg(user_msg);

    //Start recording
    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Start();

    srand(567);


    //before a semaphore is used it should be explicitly created-create binary semaphore

    xWork= xSemaphoreCreateBinary();

    //a queue is created to hold a maximum of one item as it is a binary semaphore

    xWorkQueue=xQueueCreate(1,sizeof(unsigned int));

    //check if semaphore and queue are created successfully
    if((xWork !=NULL)&&(xWorkQueue !=NULL)){
    	//CREATE THE MANAGER AND EMPLOYEE TASK
        xTaskCreate(vManager_Task, "Manager", 500, NULL,3, &xTask1Handle);
        xTaskCreate(vEmployee_Task, "Employee", 500, NULL, 1, &xTask2Handle);
        //start the scheduler
		vTaskStartScheduler();


    }

    sprintf(user_msg,"\rqueue/sema creation failed\r\n");
	printmsg(user_msg);



    for(;;);
}

void vManager_Task(void* params){
	unsigned int xWorkTicketID;
	BaseType_t xStatus;



	/* The semaphore is created in the 'empty' state, meaning the semaphore must
		 first be given using the xSemaphoreGive() API function before it
		 can subsequently be taken (obtained) */

	//xSemaphoreGive(xWork);

	for(;;){

		xWorkTicketID=(rand() & (0x1FF));

		xStatus=xQueueSend(xWorkQueue,&xWorkTicketID,portMAX_DELAY);
		if(xStatus != pdTRUE){
			//sending to queue has failed
			sprintf(user_msg,"\rsending to queue has failed\r\n");
			printmsg(user_msg);

		}else{
			xSemaphoreGive(xWork);
			taskYIELD();
		}


	}

}
void vEmployee_Task(void* params){
	unsigned char xWorkTicketID;
	BaseType_t xStatus;




	for(;;){
		xSemaphoreTake(xWork,0);
		xStatus=xQueueReceive(xWorkQueue, &xWorkTicketID, 0);
		if(xStatus==pdTRUE){
			//queue has received the ticket id
			EmployeeDoWork(xWorkTicketID);

		}else{
			//nothing is received
			sprintf(user_msg,"\r receiving in queue has failed\r\n");
			printmsg(user_msg);

		}



	}
}

void EmployeeDoWork(unsigned char TicketId){

	sprintf(user_msg,"\rEmployee task : Working on Ticked id : %u \r\n",TicketId);
	printmsg(user_msg);
	vTaskDelay(TicketId);



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













