
/*
 * Copyright (c) 2006-2025, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-12-01     RT-Thread    first version
 */

#include <rtthread.h>

#include "bsp_sys.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

      float v0 = ads1115_get_ain0();
      float v1 = ads1115_get_ain1();

      if (v0 >= 0 && v1 >= 0) {
          rt_kprintf("AIN0: %.3f V   |   AIN1: %.3f V\r\n", v0, v1);
      } else {
          rt_kprintf("ADS1115 read failed!\r\n");
      }

      rt_thread_mdelay(500);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
