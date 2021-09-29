
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
#define PRESSED      TRUE
#define NOT_PRESSED  FALSE
//function prototype
void vTask1_handler(void* params);
void vTask2_handler(void* params);
static void pvtHardwareSetup(void);
static void UART2_Setup(void);
static void GPIO_Setup(void);
void printmsg(char* msg);
void rtos_delay(uint32_t Delay_In_Ms);

//variables
TaskHandle_t xTask1Handle=NULL;
TaskHandle_t xTask2Handle=NULL;
char user_msg[100];
uint8_t UART_ACCESS_KEY=AVAILABLE;
uint8_t Button_Status_Flag=NOT_PRESSED;


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

    sprintf(user_msg,"\rthis is demo of task delete project\r\n");
    printmsg(user_msg);

    //Start recording
    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Start();

    //create two tasks:LED-TASK and BUTTON-TASK
    xTaskCreate(vTask1_handler, "TASK-1", 500, NULL, 2, &xTask1Handle);//stack size:500*4=~2kb
    xTaskCreate(vTask2_handler, "TASK-2", 500, NULL, 3, &xTask2Handle);
    //start the scheduler
    vTaskStartScheduler();
	for(;;);
}

void vTask1_handler(void* params){


	UBaseType_t p1,p2;
    sprintf(user_msg,"task 1 is running\r\n");
	printmsg(user_msg);
	while(1){
    if(Button_Status_Flag==NOT_PRESSED){
		rtos_delay(200);
		//vTaskDelay(200);
		GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
    }else {
	    Button_Status_Flag=NOT_PRESSED;

		//button is pressed
		sprintf(user_msg,"task 1 PRIORITY is inversed\r\n");
		printmsg(user_msg);
		p1=uxTaskPriorityGet(xTask1Handle);
		p2=uxTaskPriorityGet(xTask2Handle);

		//make this task the low priority and task 1 high priority
		vTaskPrioritySet(xTask2Handle, p1);
		vTaskPrioritySet(xTask1Handle, p2);


}
	}

	}


void vTask2_handler(void* params){

	sprintf(user_msg,"task 2 is running\r\n");
	printmsg(user_msg);
	UBaseType_t p1,p2;


	while(1){
		if(Button_Status_Flag==NOT_PRESSED){
			//button is not pressed
			//1 s delay for led toggling

			rtos_delay(1000);
			//toggle led
			GPIO_ToggleBits(GPIOA, GPIO_Pin_5);



		}else {
			Button_Status_Flag=NOT_PRESSED;

			//button is pressed
			sprintf(user_msg,"task 2 PRIORITY is inversed\r\n");
			printmsg(user_msg);
			p1=uxTaskPriorityGet(xTask1Handle);
			p2=uxTaskPriorityGet(xTask2Handle);

			//make this task the low priority and task 1 high priority
			vTaskPrioritySet(xTask2Handle, p1);
			vTaskPrioritySet(xTask1Handle, p2);



		}
		}
	}

static void pvtHardwareSetup(void){
	UART2_Setup();
	//Configure button PC13 and LED PA5
	GPIO_Setup();


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

static void UART2_Setup(void){
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


void printmsg(char* msg){

	for(uint32_t i=0;i<strlen(msg);i++){
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
		USART_SendData(USART2, msg[i]);
	}

}

void rtos_delay(uint32_t Delay_In_Ms){
	uint32_t currect_tick_count=xTaskGetTickCount();
	//convert from ms to number of ticks
	uint32_t Delay_In_Ticks=(Delay_In_Ms*configTICK_RATE_HZ)/1000;
	while(xTaskGetTickCount()<(currect_tick_count+Delay_In_Ticks));

}

void EXTI15_10_IRQHandler(void){
	traceISR_ENTER();
	//1.clear pending bit in exti_Pr, if it is not cleared by writing 1 to it it will produce cont. interrupt
	EXTI_ClearITPendingBit(EXTI_Line13);
	//2.button is pressed update the flag
	Button_Status_Flag=PRESSED;

	traceISR_EXIT();

}








