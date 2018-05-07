#include "ucg_stm32.h"

#ifndef ARDUINO
void ucg_Delay(uint16_t val)
{
    HAL_Delay(val);
}

void ucg_MicroDelay(uint16_t arg)
{
    int i, j;
    for (j = 0; j < arg; j++)
        for (i = 0; i < 1000; i++);
}

void ucg_10MicroDelay(void)
{
    int i;
    for (i = 0; i < 10000; i++);
}
#endif

int16_t ucg_com_ssd1331_hw_spi_fn(ucg_t *ucg, int16_t msg, uint16_t arg, uint8_t *data)
{
  uint8_t rx_buffer[3] = {0, 0, 0};

  switch(msg)
  {
    case UCG_COM_MSG_POWER_UP:
      break;
    case UCG_COM_MSG_POWER_DOWN:
      break;
    case UCG_COM_MSG_DELAY:
      ucg_MicroDelay(arg);
      break;
    case UCG_COM_MSG_CHANGE_RESET_LINE:
      HAL_GPIO_WritePin(PIN_RST_GPIO_Port, PIN_RST_Pin, arg);
      break;
    case UCG_COM_MSG_CHANGE_CS_LINE:
      HAL_GPIO_WritePin(PIN_CS_GPIO_Port, PIN_CS_Pin, arg);
      break;
    case UCG_COM_MSG_CHANGE_CD_LINE:
      HAL_GPIO_WritePin(PIN_DC_GPIO_Port, PIN_DC_Pin, arg);
      break;
    case UCG_COM_MSG_SEND_BYTE:
      HAL_SPI_TransmitReceive(&SPI_HANDLER, &arg, rx_buffer, 1, SPI_TIMEOUT);
      break;
    case UCG_COM_MSG_REPEAT_1_BYTE:
      while( arg > 0 ) {
        HAL_SPI_TransmitReceive(&SPI_HANDLER, data, rx_buffer, 1, SPI_TIMEOUT);
        arg--;
      }
      break;
    case UCG_COM_MSG_REPEAT_2_BYTES:
      while( arg > 0 ) {
        HAL_SPI_TransmitReceive(&SPI_HANDLER, data, rx_buffer, 2, SPI_TIMEOUT);
        arg--;
      }
      break;
    case UCG_COM_MSG_REPEAT_3_BYTES:
      while( arg > 0 ) {
        HAL_SPI_TransmitReceive(&SPI_HANDLER, data, rx_buffer, 3, SPI_TIMEOUT);
        arg--;
      }
      break;
    case UCG_COM_MSG_SEND_STR:
      while( arg > 0 ) {
        HAL_SPI_TransmitReceive(&SPI_HANDLER, data++, rx_buffer, 1, SPI_TIMEOUT);
        arg--;
      }
      break;
    case UCG_COM_MSG_SEND_CD_DATA_SEQUENCE:
      while(arg > 0)
      {
	if ( *data != 0 )
	{
	  /* set the data line directly, ignore the setting from UCG_CFG_CD */
	  if ( *data == 1 )
	  {
            HAL_GPIO_WritePin(PIN_DC_GPIO_Port, PIN_DC_Pin, 0);
	  }
	  else
	  {
            HAL_GPIO_WritePin(PIN_DC_GPIO_Port, PIN_DC_Pin, 1);
	  }
	}
	data++;
        HAL_SPI_TransmitReceive(&SPI_HANDLER, data, rx_buffer, 1, SPI_TIMEOUT);
	data++;
	arg--;
      }
      break;
  }
  return 1;
}
