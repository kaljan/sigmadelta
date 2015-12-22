/*!
 --------------------------------------------------------------------
 \file
 \author	Navrotski Mikalai
 \version	0.0.0
 \date
 \brief
 
 --------------------------------------------------------------------
 */
/*
 --------------------------------------------------------------------
 Что было бы не плохо сделать в этой программе.
 1. Навести порядок во всём коде. Упорядочить и перераспределить всё.
   Если быть чуть точнее то упорядочить нужно по следующим параметрам,
   ну или следующим образом:
   
   - Определить что подключено к этому контроллеру и используется, а это:
     • LCD дисплей S6D1121
     • Touch screen на микросхемке XPT2046
     • Микросхема памяти SST25VF016
     • SD-карта
     • 4 кнопки
     • 4 светодиода
     • MEMS датчик
     • Вывод звука CS43L22
     • USB(реализовать)
     • UART(Его нужно вывести хотябы один)
	 
   - Какая периферия используется:
     • SPI1
	 • SPI2
	 • I2C1
	 • I2S3(SPI3)
	 • FSMC
	 • GPIO
	 • TIM7
	 • SysTickTimer
	 • EXTI
	 • USART
	 • USB
	 • ADC
	 • DMA
	 • IRQ
	 
   - Разобраться с прерываниями. Нужно расставить приоритеты прерываний.
   
   - Разобраться с DMA, расставить их приоритеты и расписать таблицу для
     чего какой канал используется.
   
   - Переорганизовать сруктуру проекта.
   
 2. Доделать:
   - SD-карта:
     • Доделать мультиблочную чтение-запись.
	 • Доделать обработку ошибок.
	 
   - I2C. организовать работу с ошибками.
   
   - Добавить декодирование MP3. +
   
   - Добавить определение типа WAV файла и подстройку под него микросхемы
     и реинициализацию I2S.
	 
   - Touch Screen. он вообще не реализован.
   
   - Вывод на экран. 
     • Привести в порядок.
	 • Добавить рисование графики.
	 
   - USB. Реализовать
   
   - USART. реализовать
   
   - Приделать RTC. Для этого нужно будет перенести светодиоды c 
     PC13-PC15 на другие порты. прилепить часовой кварц и батарейку.
	 
   - Сделать главный файл main.h
   
   - Реализовать функицю работы с кнопками и главное меню.
   
   - Нужно допилить файлманагер и назвать его fmgr.c и fmgr.h
   и объеденить его с подсистемой работы с памятью.
   memmgr
   
   - Допилить работу с дисплеем, полностью. допилить туда тачскрин.
   и сделать отдельный раздел dmgr или dsys, или dpsys типа
   подсистема дисплея.
   
   переорганизовать всё это чудо.
   
   разделить всё по уровням, и придумать API для работы со всеми этими
   чудесами. хотя переносимость для этого дела пока что не нужна.
   
   довести исходный код до ума и выкатить его в доксиген и на гитхаб.
   
   было бы не плохо, всё это деле перепилить так, что бы можно было
   включить оптимизатор.
   
 --------------------------------------------------------------------
 */
#include "main.h"

// Prototypes -------------------------------------------------------
void Read_Image(void);
void travel(void);

// Variables --------------------------------------------------------
unsigned short b565_data[2][320];

/*!
 --------------------------------------------------------------------
 \brief
 
 --------------------------------------------------------------------
 */
int main(void)
{	
	RCC_Enable_Clocks();
	GPIO_Init();
	FSMC_init();
	TIM7_config();
	SPI2_DMA_Init();
	
	display_init();
	
	SysTick_Init();
	
	travel();
	
	Read_Image();
	
	while(1) {
		FM_Check_SD();	
	}
}

/*!
 --------------------------------------------------------------------
 \brief
 
 --------------------------------------------------------------------
 */
void Read_Image(void)
{
	int i;
	char nb = 0;
	
	DMA_TransferComplete = 1;
	
	for (i = 0; i < 240; i++) {
		
		sst25_read_block(i * 320 * 2, (unsigned char *)(&b565_data[nb][0]), 320 * 2); 

		while(DMA_TransferComplete == 0);
		DMA_TransferComplete = 0;
		
		display_write_data(0, 239 - i, 319, (239 - i) + 1, 
			(unsigned short *)(&b565_data[nb][0]), 320);
		
		if (nb == 0) nb = 1; else nb = 0;

	}
}

/*!
 --------------------------------------------------------------------
 \brief
 
 --------------------------------------------------------------------
 */
void travel(void)
{
	msg_travel();
	while(1) {
		if (enter_button.pressed == 1) {
			enter_button.pressed = 0;
			break;
		}
		
		if (up_button.pressed == 1) {
			up_button.pressed = 0;
			break;
		}
		
		if (down_button.pressed == 1) {
			down_button.pressed = 0;
			break;
		}
		
		if (exit_button.pressed == 1) {
			exit_button.pressed = 0;
			break;
		}
	}
}
