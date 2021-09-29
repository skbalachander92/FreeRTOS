
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
void vHandlerTask(void* params);
void vPeriodicTask(void* params);


static void pvtHardwareSetup(void);
static void UART_Setup(void);
static void GPIO_Setup(void);

void printmsg(char* msg);

//variables
TaskHandle_t xTask1Handle=NULL;
TaskHandle_t xTask2Handle=NULL;

SemaphoreHandle_t xCountingSemaphore;



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

    sprintf(user_msg,"\rDemo of counting semaphore for sync between isr-tasks,event latch\r\n");
    printmsg(user_msg);

    //Start recording
    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Start();

    //before a semaphore is used it should be explicitly created-create counting semaphore
    xCountingSemaphore=xSemaphoreCreateCounting(10,0);



    //check if semaphore created successfully
    if(xCountingSemaphore !=NULL){
    	//CREATE THE MANAGER AND EMPLOYEE TASK
        xTaskCreate(vHandlerTask, "HANDLER", 500, NULL,1, &xTask1Handle);
        xTaskCreate(vPeriodicTask, "PERIODIC", 500, NULL,3, &xTask2Handle);
        //start the scheduler
		vTaskStartScheduler();


    }

    sprintf(user_msg,"\rsema creation failed\r\n");
	printmsg(user_msg);



    for(;;);
}

void vHandlerTask(void* params){
	while(1){

		//task gets blocked on unavailability of semaphore and waits for signal by semaphore count
		xSemaphoreTake(xCountingSemaphore,portMAX_DELAY);

		/* To get here the event must have occurred.  Process the event (in this
		case we just print out a message). */

		sprintf(user_msg,"Handler Task-processing event\r\n");
		printmsg(user_msg);

	}

}
void vPeriodicTask(void* params){
	while(1){

		//this task makes a software interrupt periodically fo every 500 ms
		vTaskDelay(pdMS_TO_TICKS(500));

		/* Generate the interrupt, printing a message both before hand and
		afterwards so the sequence of execution is evident from the output. */
		sprintf(user_msg,"periodic task pending the interrupt\r\n");
		printmsg(user_msg);

		//pend the interrupt
		NVIC_SetPendingIRQ(EXTI15_10_IRQn);

		sprintf(user_msg, "Periodic task - Resuming.\r\n" );
		printmsg(user_msg);



	}
}


static void GPIO_Setup(void){
	GPIO_InitTypeDef buttonGPIO,LED_GPIO;
	memset(&buttonGPIO,0,sizeof(buttonGPIO));
	memset(&LED_GPIO,0,sizeof(LED_GPIO));

	//enable clock for gpioa and gpioc and syscfg
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//configure led pin pa5 as output
	LED_GPIO.GPIO_Pin=GPIO_Pin_5;
	LED_GPIO.GPIO_Mode=GPIO_Mode_OUT;
	LED_GPIO.GPIO_OType=GPIO_OType_PP;
	LED_GPIO.GPIO_PuPd=GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA, &LED_GPIO);

	//configure button gpio pc13 as input
	buttonGPIO.GPIO_Pin=GPIO_Pin_13;
	buttonGPIO.GPIO_Mode=GPIO_Mode_IN;

	GPIO_Init(GPIOC, &buttonGPIO);
	//interrupt configuration for the button pc13
	//1.system configuration for exti lines
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
    //2.exti configuration(peripheral side)-13 th line,interrupt mode,falling edge
	EXTI_InitTypeDef EXTI_pc13;
	EXTI_pc13.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_pc13.EXTI_Line=EXTI_Line13;
	EXTI_pc13.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_pc13.EXTI_LineCmd=ENABLE;

	EXTI_Init(&EXTI_pc13);
	//3.nvic configuration(processor side),for exti 13 line
	NVIC_SetPriority(EXTI15_10_IRQn, 15);

	NVIC_EnableIRQ(EXTI15_10_IRQn);

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
	GPIO_Setup();



}

void printmsg(char* msg){

	for(uint32_t i=0;i<strlen(msg);i++){
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
		USART_SendData(USART2, msg[i]);
	}

}

void EXTI15_10_IRQHandler(void){

	//1.clear pending bit in exti_Pr, if it is not cleared by writing 1 to it it will produce cont. interrupt
	EXTI_ClearITPendingBit(EXTI_Line13);

	BaseType_t xHigherPriorityTaskWoken=pdFALSE;

	sprintf(user_msg,"==>Button_Handler\r\n");
	printmsg(user_msg);

	xSemaphoreGiveFromISR(xCountingSemaphore,&xHigherPriorityTaskWoken);
	xSemaphoreGiveFromISR(xCountingSemaphore,&xHigherPriorityTaskWoken);
	xSemaphoreGiveFromISR(xCountingSemaphore,&xHigherPriorityTaskWoken);
	xSemaphoreGiveFromISR(xCountingSemaphore,&xHigherPriorityTaskWoken);
	xSemaphoreGiveFromISR(xCountingSemaphore,&xHigherPriorityTaskWoken);
	/* Giving the semaphore may have unblocked a task - if it did and the
	    unblocked task has a priority equal to or above the currently executing
	    task then xHigherPriorityTaskWoken will have been set to pdTRUE and
	    portEND_SWITCHING_ISR() will force a context switch to the newly unblocked
	    higher priority task.

	    NOTE: The syntax for forcing a context switch within an ISR varies between
	    FreeRTOS ports.  The portEND_SWITCHING_ISR() macro is provided as part of
	    the Cortex M3 port layer for this purpose.  taskYIELD() must never be called
	    from an ISR! */

	    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);




}












