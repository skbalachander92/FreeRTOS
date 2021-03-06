
#include<stdio.h>
#include<stdint.h>
#include<string.h>

#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"



//macros
#define TRUE           1
#define FALSE          0
#define AVAILABLE      TRUE
#define NOT_AVAILABLE  FALSE
#define PRESSED        TRUE
#define NOT_PRESSED    FALSE

#define LED_ON                1
#define LED_OFF               2
#define LED_TOGGLE            3
#define LED_TOGGLE_OFF        4
#define LED_READ_STATUS       5
#define RTC_PRINT_DATE_TIME   6
#define EXIT_APP              7



//tasks function prototype
void vTask1_menu_display(void* params);
void vTask2_cmd_handling(void* params);
void vTask3_cmd_processing(void* params);
void vTask4_uart_write(void* params);
//PROTOTYPES OF COMMAND HANDLING HELPER FUNCTION
void make_led_on(void);
void make_led_off(void);
void led_toggle_start(uint32_t duration);
void led_toggle_stop(void);
void read_led_status(char* task_msg);
void read_rtc_info(char* task_msg);
void print_error_message(char* task_msg);
void vLedTimerCallback( TimerHandle_t xTimer );


static void pvtHardwareSetup(void);
static void UART2_Setup(void);
static void GPIO_Setup(void);


void printmsg(char* msg);
void rtos_delay(uint32_t Delay_In_Ms);
uint8_t GetCommandCode(uint8_t* Buffer);


//variables
TaskHandle_t xTask1Handle=NULL;
TaskHandle_t xTask2Handle=NULL;
TaskHandle_t xTask3Handle=NULL;
TaskHandle_t xTask4Handle=NULL;

char user_msg[100];
uint8_t UART_ACCESS_KEY=AVAILABLE;
uint8_t Button_Status_Flag=NOT_PRESSED;

QueueHandle_t command_queue=NULL;
QueueHandle_t uart_write_queue=NULL;

uint8_t command_buffer[20];
uint8_t command_length=0;

TimerHandle_t LED_Timer_Handle=NULL;



//command structure
typedef struct{
	uint8_t CMD_NUMBER;
	uint8_t CMD_ARGS[10];
}APP_CMD_t;

//menu for menu display task
char menu[]={"\
\r\nLED_ON                 ----->1\
\r\nLED_OFF                ----->2\
\r\nLED_TOGGLE             ----->3\
\r\nLED_TOGGLE_OFF         ----->4\
\r\nLED_READ_STATUS        ----->5\
\r\nRTC_PRINT_DATE_TIME    ----->6\
\r\nEXIT_APP               ----->7\
\r\nType your option here:"};


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

    sprintf(user_msg,"\rthis is demo of queue command processing project\r\n");
    printmsg(user_msg);

    //Start recording
    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Start();

    //create command queue
    command_queue=xQueueCreate(10,sizeof(APP_CMD_t*));
    uart_write_queue=xQueueCreate(10,sizeof(char*));
    if((command_queue != NULL)&&(uart_write_queue != NULL)){
    	//create four tasks
    	    //1.printing menu to user,2.command processing,3.cmd handlinh, 4.uart write
    	    xTaskCreate(vTask1_menu_display, "menu-disp", 500, NULL, 1, &xTask1Handle);//stack size:500*4=~2kb
    	    xTaskCreate(vTask2_cmd_handling, "cmd-handle", 500, NULL, 2, &xTask2Handle);
    	    xTaskCreate(vTask3_cmd_processing, "cmd-proces", 500, NULL, 2, &xTask3Handle);
    	    xTaskCreate(vTask4_uart_write, "uart-wr", 500, NULL, 2, &xTask4Handle);

    	    //start the scheduler
    	    vTaskStartScheduler();

    }else{
        sprintf(user_msg,"\rqueue creation failed\r\n");
        printmsg(user_msg);


    }



    	for(;;);
}

void vTask1_menu_display(void* params){
	char* pData=menu;

	while(1){
		xQueueSend(uart_write_queue,(void*)&pData,portMAX_DELAY);
		//menu display task should block
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

	}

}
void vTask2_cmd_handling(void* params){
	uint8_t command_code=0;
	APP_CMD_t* new_cmd;

	while(1){
		//block the task for the notification from uart interrupt handler
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
		//port malloc returns the address of the heap where dynamic mem aloc for struc is created
		new_cmd=(APP_CMD_t*)pvPortMalloc(sizeof(APP_CMD_t));

		//send command code to queue
		//step:1 -extract command code from uart receive interrupt-uint8_t command_buffer[]
		//command_buffer is a global variable and its value can be updated anytime by uart interrupt
		//so it is a critical section and it is needed to be protected to avoid race condition
		//so we have to serialise the access of the global variable between isr and this task
		//that can be done by semaphore, mutex or by disabling the interrupt
		//disabling interrupt can be done by using the api taskENTER_CRITICAL()

		taskENTER_CRITICAL();
		command_code=GetCommandCode(command_buffer);
		//step2:use malloc to allocate the structure in heap and get its pointer
		//step3:use the heap pointer to put the command code value in CMD_NUMBER member element
		new_cmd->CMD_NUMBER=command_code;
		taskEXIT_CRITICAL();

		//send the command number to command queue
		xQueueSend(command_queue,&new_cmd,portMAX_DELAY);


		}

}
/*for reference, defined in the global space
#define LED_ON                1
#define LED_OFF               2
#define LED_TOGGLE            3
#define LED_TOGGLE_OFF        4
#define LED_READ_STATUS       5
#define RTC_PRINT_DATE_TIME   6
#define EXIT_APP              7
*/
void vTask3_cmd_processing(void* params){

	APP_CMD_t* new_cmd;
	char task_msg[50];

	uint32_t toggle_duration=pdMS_TO_TICKS(500);

	while(1){
		//block till cmd_handling updates the queue with  cmd_code
		xQueueReceive(command_queue, (void*)&new_cmd,portMAX_DELAY );
		if(new_cmd->CMD_NUMBER==LED_ON){
			//switch on the led
			make_led_on();
		}else if(new_cmd->CMD_NUMBER==LED_OFF){
			//switch off the led
			make_led_off();
		}else if(new_cmd->CMD_NUMBER==LED_TOGGLE){
			//switch on the led toggle
			led_toggle_start(toggle_duration);
	    }else if(new_cmd->CMD_NUMBER==LED_TOGGLE_OFF){
			//switch off the led toggle
	    	led_toggle_stop();
		}else if(new_cmd->CMD_NUMBER==LED_READ_STATUS){
			//read led status
			read_led_status(task_msg);
		}else if(new_cmd->CMD_NUMBER==RTC_PRINT_DATE_TIME){
			//print date and time
			read_rtc_info(task_msg);
		}else{
			//print error message
			print_error_message(task_msg);
		}
		//deallocate the memory for new_cmd
		vPortFree(new_cmd);

	}

}
void vTask4_uart_write(void* params){
	char* pData=NULL;
	while(1){
		xQueueReceive(uart_write_queue, &pData, portMAX_DELAY);
		printmsg(pData);

		}

}

uint8_t GetCommandCode(uint8_t* Buffer){
	return Buffer[0]-48;
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
		//enabling uart byte reception interrupt in the microcontroller
        //enabling interrupt in peripheral
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

        //enabling interrupt in the processor-NVIC
		NVIC_EnableIRQ(USART2_IRQn);
		NVIC_SetPriority(USART2_IRQn, 5);

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

void vApplicationIdleHook(void){
//send the cpu to sleep mode, clock to the procesor is stopped
	__WFI();
}
void USART2_IRQHandler(void){
	uint16_t data_byte;
	BaseType_t xHigherPriorityTaskWoken=pdFALSE;

	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE)){
		//a data byte is received from the user
		data_byte=USART_ReceiveData(USART2);

		command_buffer[command_length++]=(data_byte & 0xff);
		if(data_byte== '\r'){
			//user has finished entering the data-reset the command length
			command_length=0;
			//notify the command handling task
			xTaskNotifyFromISR(xTask2Handle,0,eNoAction,&xHigherPriorityTaskWoken);
			//notify the menu display task
			xTaskNotifyFromISR(xTask1Handle,0,eNoAction,&xHigherPriorityTaskWoken);

	    }

    }
//yield the processor if the above apis unblock any high priority task
	if(xHigherPriorityTaskWoken){
		taskYIELD();
	}
}

void make_led_on(void){
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);

}
void make_led_off(void){
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);


}

void vLedTimerCallback(TimerHandle_t xTimer){
	//TOGGLE GPIO
	GPIO_ToggleBits(GPIOA, GPIO_Pin_5);

}

void led_toggle_start(uint32_t duration){
	if(LED_Timer_Handle==NULL){
	//1.create the software timer
	LED_Timer_Handle=xTimerCreate("LED-TIMER",duration , pdTRUE, NULL, vLedTimerCallback);
    //2.start the software timer
	xTimerStart(LED_Timer_Handle,portMAX_DELAY);
	}else{
	    //handle is already created and it can be used
		xTimerStart(LED_Timer_Handle,portMAX_DELAY);


	 }
}
void led_toggle_stop(void){
	//stop the software timer
	xTimerStop(LED_Timer_Handle,portMAX_DELAY);


}
void read_led_status(char* task_msg){
	sprintf(task_msg,"\r\nThe status of the led is:%d\r\n",GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_5));
	xQueueSend(uart_write_queue,&task_msg,portMAX_DELAY);

}
void read_rtc_info(char* task_msg){
	RTC_TimeTypeDef RTC_Time;
	RTC_DateTypeDef RTC_Calendar;

	RTC_GetTime(RTC_Format_BIN,&RTC_Time);
	RTC_GetDate(RTC_Format_BIN,&RTC_Calendar);
	sprintf(task_msg,"\r\n time:%02u:02u:02u  \r\n date:%02u -02u -02u\r\n ",RTC_Time.RTC_Hours,RTC_Time.RTC_Minutes,RTC_Time.RTC_Seconds,\
			RTC_Calendar.RTC_Date,RTC_Calendar.RTC_Month,RTC_Calendar.RTC_Year);
    xQueueSend(uart_write_queue,&task_msg,portMAX_DELAY);
}



void print_error_message(char* task_msg){

	sprintf(task_msg,"\r\nInvalid command received\r\n");
	xQueueSend(uart_write_queue,&task_msg,portMAX_DELAY);
}










