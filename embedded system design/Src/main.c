/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "zlg7290.h"
#include "stdio.h"
#include "Dc_motor.h"

#include "i2c.h"

/////////////
#include <stdlib.h>

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define ZLG_READ_ADDRESS1         0x01
#define ZLG_READ_ADDRESS2         0x10

#define ZLG_WRITE_ADDRESS1        0x10
#define ZLG_WRITE_ADDRESS2        0x11
#define BUFFER_SIZE1              (countof(Tx1_Buffer))
#define BUFFER_SIZE2              (countof(Rx2_Buffer))
#define countof(a) (sizeof(a) / sizeof(*(a)))

uint8_t flag;//不同的按键有不同的标志位值
//uint8_t flag1 = 0;//中断标志位，每次按键产生一次中断，并开始读取8个数码管的值
uint8_t Rx2_Buffer[8]={0};
uint8_t Tx1_Buffer[8]={0};
uint8_t Rx1_Buffer[1]={0};
uint8_t parameter[10] = {0};

uint8_t judge = 0; 		//////标记前一块代码的代码块号
uint8_t check = 0 ; 		//////用于进行程序判断滤波
int cnt = 0;		//////用于判断是否超时
uint8_t length = 8;
int sign = 0;
uint8_t res = 0;
uint8_t nothing=0;

unsigned char seg7code[10]={ 0xFC,0x0C,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xE6}; //数码管字根
void Led(int date);	

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void swtich_key(void);
void switch_flag(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void sleep(int time){
		int res = 0;
		if(time<0) time = -time; 
		while(1){
			if(time<1) return;
			res = res + time;
			time--;
		}
		
}

uint8_t gencrc(uint8_t *data, size_t len)//生成unit8数组的，crc检验值
{
    uint8_t crc = 0xff;
    size_t i, j;
    for (i = 0; i < len; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0) //高位是否为1
                crc = (uint8_t)((crc << 1) ^ 0x31);// PLOY=0X31 =0011 0001  x8+x5+x4+1  (0x131)
            else   //高位为0，直接左移
                crc <<= 1;
        }
    }   
    return crc;
}

int crc8verify(uint8_t *data, size_t len,uint8_t vecrc){//vecrc 接收端收到的crc码。data[0]data[1]data[2]...vecrc mod ploy==0 说明没问题

    size_t i, j;
    uint8_t crc = 0xff;

    for (i = 0; i < len; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0) //高位是否为1
                crc = (uint8_t)((crc << 1) ^ 0x31);// PLOY=0X31 =0011 0001  x8+x5+x4+1  (0x131)
            else   //高位为0，直接左移
                crc <<= 1;
        }
    }   
    // data[0]data[1]data[2]...vecrc mod ploy==0 
    crc ^=vecrc;
            for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0) //高位是否为1
                crc = (uint8_t)((crc << 1) ^ 0x31);// PLOY=0X31 =0011 0001  x8+x5+x4+1  (0x131)
            else   //高位为0，直接左移
                crc <<= 1;
        }

    //printf("%x\n",crc);
    if(crc==0)
        return 1;
    else 
        printf("%x\n",crc); //出错的是第crc位 (需要对照余数表确认，不是简单的第crc位
        return 0;
}

int main(void)
{

  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/
	while(1){
		int random = 0;
		int i = 0;
		judge = 0;
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
		HAL_Init();

  /* Configure the system clock */
		SystemClock_Config();

  /* Initialize all configured peripherals */
		
		MX_GPIO_Init();
		MX_I2C1_Init();
		MX_USART1_UART_Init();
		MX_TIM3_Init();
		
  /* USER CODE BEGIN 2 */
		printf("\n\r");
		printf("\n\r-------------------------------------------------\r\n");
		printf("\n\r FS-STM32开发板 直流电机测试实验\r\n");	
		
		DC_Motor_Pin_Low();

		HAL_TIM_Base_Start_IT(&htim3);   //启动定时器3.
		
		judge = judge + 1;
		if(judge != 1) continue;

  /* USER CODE END 2 */

  /* Infinite loop */
	
  /* USER CODE BEGIN WHILE */
		
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  
  //judge为代码块号，若程序正常执行，则代码块按照顺序执行，judge依次为1，2，3...
  //若judge值不是按顺序的1，2，3...说明执行顺序出错
		while (1){
			judge = 1;
			sign = 0;
			if(flag1 == 1){
				cnt = 0;
				flag1 = 0;
				for(i=0;i<length;i++){			//重复读8次，取出现次数最多的结果。这里由于输入按键不会清零(按键时间间隔为几十毫秒,读数时间间隔只有几毫秒)
				//，应该是读取同一个输入8次，也可能由于新的输入/其他干扰导致变化
					I2C_ZLG7290_Read(&hi2c1,0x71,0x01,Rx1_Buffer,1);
					parameter[i] = Rx1_Buffer[0];
					if(res != Rx1_Buffer[0]){   //res，截止当前出现最多的输入。
							if(sign < 1){   
							//若sign<1，说明是第一次输入，重新赋值res；或者新出现了一个数，
							//这个数的出现次数比之前出现的次数最多的数出现次数还多，则替换res为这个数
								res = Rx1_Buffer[0];
								sign = 1;
							}
							else	sign --;
					}
					else sign ++;
				}
				sign = 0;
				//统计，若出现次数最多的数的次数超过总输入次数的一半，认为输入有效。否则认为输入错误。 综上实现输入滤波。
				for(i = 0;i<length;i++)
						if(parameter[i] == res) {
							sign++;
							parameter[i] = 0;
						}
						else	nothing = 0;
				if(sign < length/2){
					printf("Input Error/n");
					continue;
				}
				else{
					judge = judge + 1;
					Rx1_Buffer[0] = res;
					uint8_t crc = gencrc(Rx1_Buffer,1);  //生成最后确认的输入的CRC校验值
				}
				
				//随机延时。防止测信道攻击
				random = rand();
				random = random %100;
				sleep(random);
				
				if(judge != 2) continue;
				
				printf("\n\r按键键值 = %#x\r\n",Rx1_Buffer[0]);//向串口输入键值
				
				DC_Task(Rx1_Buffer[0]);
				
				judge = judge + 1;
				if(judge != 3) continue;
				
				
				//向串口输入键值后，用CRC检查Rx1_Buffer[0]，即输入有没有变化。
				//若有变化（CRC校验出错），认为有可能向串口输入了错误的键值，则出错。
				flag = crc8verify(Rx1_Buffer,1,crc);  
        if (!flag)	break;
				random = rand();
				random = random %100;
				sleep(random);
			}	
			else cnt++;
			
			//cnt为超时判断。若cnt过高，则认为程序执行超时。
			if(cnt > 1000000){
				printf("Time out!");
				break;
			}
    //DC_Motor_Data
			Led(DC_Motor_Data);
		
			HAL_Delay(100);
		}
  /* USER CODE END 3 */
	}
}


/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */


void Led(int date) //显示函数
{
/*****************数据转换*****************************/ 
unsigned int qian,bai,shi,ge;
  qian=date/1000;				 //求千位
  bai=date%1000/100;		 //求百位
  shi=date%1000%100/10;			 //求十位
  ge=date%10;						 //求个位	
	
//	I2C_Gets(ZLG7290_I2C_ADDR,0x10,1,Rx2_Buffer,8);//读8位数码管
	
	if(fs_flag==0) Tx1_Buffer[3] = 0x02;	
	if(fs_flag==1) Tx1_Buffer[3] = 0x00;
	if(fs_flag ==2) Tx1_Buffer[3] =seg7code[0];

	Tx1_Buffer[4] = seg7code[qian];
	Tx1_Buffer[5] = seg7code[bai];
	Tx1_Buffer[6] = seg7code[shi];
	Tx1_Buffer[7] = seg7code[ge];
	
  I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,8);
}


void swtich_key(void)
{
	switch(Rx1_Buffer[0])
	{
        case 0x1C:
					flag = 1;					
          break;
        case 0x1B:	
					flag = 2;
          break;
        case 0x1A:	
					flag = 3;
          break;
        case 0x14:
					flag = 4;
          break;   
				case 0x13:
					flag = 5;
					break;
        case 0x12:
					flag = 6;
          break;
        case 0x0C:
					flag = 7;
          break;
        case 0x0B:
          flag = 8;
          break;
				case 0x0A:
					flag = 9;
					break;
				case 0x03:
					flag = 15;
					break;
				case 0x19:
					flag = 10;
					break;
				case 0x11:
					flag = 11;
					break;
				case 0x09:
					flag = 12;
					break;
				case 0x01:
					flag = 13;
					break;
				case 0x02:
					flag = 14;
					break;
        default:
          break;
			}
}

void switch_flag(void){
	switch(flag){
			case 1:
				Tx1_Buffer[0] = 0x0c;
				if(Rx2_Buffer[0] == 0)
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
					}
					else
					{									
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);					
					}
				break;
			case 2:
				Tx1_Buffer[0] = 0xDA;
				if(Rx2_Buffer[0] == 0)
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
					}
					else
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);		
					}
				break;
			case 3:
				Tx1_Buffer[0] = 0xF2;
				if(Rx2_Buffer[0] == 0)
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
					}
					else
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);						
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);		
					}
				break;
			case 4:
				Tx1_Buffer[0] = 0x66;
				if(Rx2_Buffer[0] == 0)
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
					}
					else
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);						
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);					
					}
				break;
			case 5:
				Tx1_Buffer[0] = 0xB6;
				if(Rx2_Buffer[0] == 0)
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
					}
					else
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);						
					}
				break;
			case 6:
				Tx1_Buffer[0] = 0xBE;
				if(Rx2_Buffer[0] == 0)
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
					}
					else
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);						
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);						
					}
				break;
			case 7:
				Tx1_Buffer[0] = 0xE0;
				if(Rx2_Buffer[0] == 0)
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
					}
					else
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);						
					}
				break;
			case 8:
				Tx1_Buffer[0] = 0xFE;
				if(Rx2_Buffer[0] == 0)
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
					}
					else
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);					
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);							
					}
				break;
			case 9:
				Tx1_Buffer[0] = 0xE6;
				if(Rx2_Buffer[0] == 0)
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
					}
					else
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);					
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);					
					}
				break;
			case 10:
				Tx1_Buffer[0] = 0xEE;
				if(Rx2_Buffer[0] == 0)
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
					}
					else
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);					
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);						
					}
				break;
			case 11:
				Tx1_Buffer[0] = 0x3E;
				if(Rx2_Buffer[0] == 0)
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
					}
					else
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);							
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);						
					}
				break;
					case 12:
				Tx1_Buffer[0] = 0x9C;
				if(Rx2_Buffer[0] == 0)
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
					}
					else
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);						
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);								
					}
				break;
					case 13:
				Tx1_Buffer[0] = 0x7A;
				if(Rx2_Buffer[0] == 0)
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
					}
					else
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);						
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);									
					}
				break;
					case 14:
							Tx1_Buffer[0] = 0x00;
							I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,8);
						break;
					case 15:
				Tx1_Buffer[0] = 0xFC;
				if(Rx2_Buffer[0] == 0)
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);
					}
					else
					{
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS2,Rx2_Buffer,BUFFER_SIZE2);						
						I2C_ZLG7290_Write(&hi2c1,0x70,ZLG_WRITE_ADDRESS1,Tx1_Buffer,1);						
					}
				break;
			default:
				break;
		}
}


int fputc(int ch, FILE *f)
{ 
  uint8_t tmp[1]={0};
	tmp[0] = (uint8_t)ch;
	HAL_UART_Transmit(&huart1,tmp,1,10);	
	return ch;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
