/*******************************************************************************
* @file  at_wifi_provisioning_over_ble.c
* @brief WiFi provisioning over BLE using AT Commands.
*******************************************************************************
* # License
* <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
*******************************************************************************
*
* The licensor of this software is Silicon Laboratories Inc. Your use of this
* software is governed by the terms of Silicon Labs Master Software License
* Agreement (MSLA) available at
* www.silabs.com/about-us/legal/master-software-license-agreement. This
* software is distributed to you in Source Code format and is governed by the
* sections of the MSLA applicable to Source Code.
*
******************************************************************************/

/**
 * Include files
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_device.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "efr32mg21a010F1024im32.h" //EFR32MG21A010F1024IM32_H
#define BUFFER_SIZE 2048UL

void rsi_delay_ms(uint32_t delay_ms); //Time delay in milli seconds --Function declaration
void rsi_hal_board_init(void);

uint8_t uart_rev_buf[BUFFER_SIZE] = { 0 };
volatile uint16_t index           = 0;
volatile uint8_t data_received    = 0;
volatile uint8_t data_rx_flag     = 0;
USART_TypeDef *uart               = USART1;

/* Private variables ---------------------------------------------------------*/
char fwversion[20]    = { 0 }; //Firmware Version (e.g. "1610.2.4.1.0.16")
char bdaddress[20]    = { 0 }; //BD address of the local device (e.g. "80-C9-55-C2-3B-02")
char servicehandle[8] = { 0 };
char starthandle;
char remotedevicesadd[20] = { 0 }; //BD Address of the connected device ()
int flag                  = 0;     //flag:    0 --> OK responses, 1--> AT commands, 2--> Error response
int connected             = 0;     //connected: 0 --> module not connected to AP,1 --> module connected to AP
int joinfailure           = 0;     //Checking if there was any errors while connecting to an AP
int wifiinitstatus        = 0;     //Keeping a check whether to initialize Wi-Fi or not
int ap_num                = 0;     //Number of APs
char sec_type[4]          = { 0 }; //Security Type of the selected AP (e.g. "30")
char sec_type_all[32]     = { 0 }; //Security Type of all detected APs
char ssid_list[16][100]   = { 0 }; //List containing information of all APs
char ap_list[16][100]     = { 0 }; //List containing names of APs
char handle_1aa1[4]       = { 0 };
char handle_1bb1[4]       = { 0 };
char handle_1cc1[4]       = { 0 };
char ssid_selected[100]   = { 0 };
char psk_ssid[100]        = { 0 };
int wrong_password        = 0;
int ip_error              = 0;
char mac_address[20]      = { 0 };
char ip_address[20]       = { 0 };

uint8_t frame_write = 1; //Flag for Uart TX
uint8_t frame_opermode[] =
  "at+rsi_opermode=851968,0,4,2147483648,2150629376,3221225472,0,1966080\r\n"; //Response is OK bt_loaded
uint8_t frame_fwversion[]      = "at+rsi_fwversion?\r\n";                      //Response format is OK1610.2.3.0.0001
uint8_t frame_ble_local_name[] = "at+rsibt_setlocalname=11,RS9116W_BLE\r\n";
uint8_t frame_ble_bd_addr[]    = "at+rsibt_getlocalbdaddr?\r\n";     //Response is the BD address [2:20) of local device
uint8_t frame_add_service[] = "at+rsibt_addservice=2,aabb,3,30\r\n"; //Response is Service handle[3,8) & starthandle [9]
uint8_t frame_band[]        = "at+rsi_band=0\r\n";
uint8_t frame_ble_set_adv[] = "at+rsibt_setadvertisedata=15,2,1,6,11,9,42,4C,45,5F,43,4F,4E,46,49,47,55,52,41,54,4F,"
                              "52\r\n"; //Advertising Data is BLE_CONFIGURATOR
uint8_t frame_ble_adv[] = "at+rsibt_advertise=1,128,0,0,0,32,32,0,7\r\n";
uint8_t frame_init[]    = "at+rsi_init\r\n";
uint8_t frame_scan[]    = "at+rsi_scan=0\r\n";
uint8_t frame_ip[]      = "at+rsi_ipconf=1,0,0,0\r\n";
uint8_t frame_dissoc[]  = "at+rsi_disassoc=0\r\n";
uint8_t frame_add_char11[100] =
  "at+rsibt_addattribute="; // characteristic will declared once service handler and start handle is received
uint8_t frame_add_char12[100] =
  "at+rsibt_addattribute="; // characteristic will declared once service handler and start handle is received
uint8_t frame_add_char21[100] =
  "at+rsibt_addattribute="; // characteristic will declared once service handler and start handle is received
uint8_t frame_add_char22[100] =
  "at+rsibt_addattribute="; // characteristic will declared once service handler and start handle is received
uint8_t frame_add_char31[100] =
  "at+rsibt_addattribute="; // characteristic will declared once service handler and start handle is received
uint8_t frame_add_char32[100] =
  "at+rsibt_addattribute="; // characteristic will declared once service handler and start handle is received
uint8_t frame_add_char33[100] =
  "at+rsibt_addattribute="; // characteristic will declared once service handler and start handle is received
uint8_t frame_errormsg[100] =
  "at+rsibt_setlocalattvalue="; // frame for sending error message to the provisioning device

typedef enum cmd_state_e {
  CMD_STATE_NONE = 0,           /*0*/
  CMD_STATE_OPERMODE_DONE,      /*1*/
  CMD_STATE_FWVERSION_DONE,     /*2*/
  CMD_STATE_BLE_LOCALNAME_DONE, /*3*/
  CMD_STATE_BLE_BDADDR_DONE,    /*4*/
  CMD_STATE_ADD_SERVICE_DONE,   /*5*/
  CMD_STATE_ADD_CHAR_DONE_11,   /*6*/
  CMD_STATE_ADD_CHAR_DONE_12,   /*7*/
  CMD_STATE_ADD_CHAR_DONE_21,   /*8*/
  CMD_STATE_ADD_CHAR_DONE_22,   /*9*/
  CMD_STATE_ADD_CHAR_DONE_31,   /*10*/
  CMD_STATE_ADD_CHAR_DONE_32,   /*11*/
  CMD_STATE_ADD_CHAR_DONE_33,   /*12*/
  CMD_STATE_BAND_DONE,          /*13*/
  CMD_STATE_BLE_SETADV_DONE,    /*14*/
  CMD_STATE_BLE_ADV,            /*15*/
} cmd_state_t;

typedef enum cmd_event_e {
  CMD_EVENT_NONE = 0,           /*0*/
  CMD_EVENT_FWVERSION,          /*1*/
  CMD_EVENT_WLAN_STATUS,        /*2*/
  CMD_EVENT_SCAN_INIT,          /*3*/
  CMD_EVENT_SCAN_RESP,          /*4*/
  CMD_EVENT_SEND_SCAN_RESP,     /*5*/
  CMD_EVENT_JOIN,               /*6*/
  CMD_EVENT_SECURITY,           /*7*/
  CMD_EVENT_PSK,                /*8*/
  CMD_EVENT_CONNECT,            /*9*/
  CMD_EVENT_IP,                 /*10*/
  CMD_EVENT_IP_SEND,            /*11*/
  CMD_EVENT_WRONG_PSK,          /*12*/
  CMD_EVENT_DISSOCIATE,         /*13*/
  CMD_EVENT_DISSOCIATE_SETLOC,  /*14*/
  CMD_EVENT_BLE_DISCONNECT,     /*15*/
  CMD_EVENT_BLE_EVK_DISCONNECT, /*16*/
  CMD_EVENT_ERROR_SETLOC,       /*17*/
} cmd_event;

cmd_state_t cmd_state   = CMD_STATE_NONE;
cmd_event cmd_event_num = CMD_EVENT_NONE;

/* Private functions ---------------------------------------------------------*/

void USART1_RX_IRQHandler(void)
{
  uart_rev_buf[index++] = USART1->RXDATA; // Get the character just received
  data_rx_flag          = 1;

  NVIC_DisableIRQ(USART1_RX_IRQn);
  NVIC_ClearPendingIRQ(USART1_RX_IRQn);
  NVIC_EnableIRQ(USART1_RX_IRQn);
  if (index >= BUFFER_SIZE) {
    index = 0;
  }
  data_received = 1;
}

int uart_tx_data(USART_TypeDef *uart, uint8_t *p, int n)
{

  uint16_t i = 0u;
  while (i < n) {
    USART_Tx(uart, p[i++]);
  }
  if (i == n) {
    return 0;
  } else {
    return -1;
  }
}

void ABRD(void)
{
  uint8_t abrd[1], abrd1[1], load[1], load_binary[1];

  abrd[0]  = 0x7C;
  abrd1[0] = 0x55;

  load_binary[0] = 'U';
  load[0]        = '1';

  do {
    uart_tx_data(USART1, (uint8_t *)abrd, 1);
    rsi_delay_ms(200);

  } while (uart_rev_buf[index - 1] != 0x55);

  index = 0;
  uart_tx_data(USART1, (uint8_t *)abrd1, 1);
  while (1) {
    rsi_delay_ms(300);
    if (data_rx_flag == 1) {
      data_rx_flag = 0;
      if (strstr((char *)uart_rev_buf, "JTAG Selection")) {
        // break the loop when find string "JTAG Selection"
        memset(uart_rev_buf, 0, sizeof(uart_rev_buf));
        index = 0;
        break;
      }
    }
  }

  index = 0;
  uart_tx_data(USART1, (uint8_t *)load_binary, 1);
  while (1) {
    rsi_delay_ms(200);
    if (data_rx_flag == 1) {
      data_rx_flag = 0;
      if (strstr((const char *)uart_rev_buf, "Enter Next Command")) {
        // break the loop when find string "Enter Wireless Image No(0-f)"
        memset(uart_rev_buf, 0, sizeof(uart_rev_buf));
        index = 0;
        break;
      }
    }
  }

  index = 0;
  uart_tx_data(USART1, (uint8_t *)load, 1);
  while (1) {
    rsi_delay_ms(100);
    if (data_rx_flag == 1) {
      data_rx_flag = 0;
      if (strstr((const char *)uart_rev_buf, "Loading...")) {
        // break the loop when find string "Enter Wireless Image No(0-f)"
        printf("ABRD Done.\r\n");
        memset(uart_rev_buf, 0, sizeof(uart_rev_buf));
        index = 0;
        break;
      }
    }
  }
}

void uart_init(void)
{

  USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
  init.baudrate                = 115200;

  // Enable oscillator to GPIO and USART1 modules
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_USART1, true);

  // set pin modes for UART TX and RX pins
  GPIO_PinModeSet(gpioPortA, 6, gpioModeInput, 0);
  GPIO_PinModeSet(gpioPortA, 5, gpioModePushPull, 1);

  // Initialize UART asynchronous mode and route pins
  USART_InitAsync(USART1, &init);

  GPIO->USARTROUTE[1].RXROUTE = (gpioPortA << _GPIO_USART_RXROUTE_PORT_SHIFT) | (6 << _GPIO_USART_RXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[1].TXROUTE = (gpioPortA << _GPIO_USART_TXROUTE_PORT_SHIFT) | (5 << _GPIO_USART_TXROUTE_PIN_SHIFT);

  // Enable USART interface pins
  GPIO->USARTROUTE[1].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN | GPIO_USART_ROUTEEN_TXPEN;

  // Enable interrupt
  NVIC_ClearPendingIRQ(USART1_RX_IRQn);
  NVIC_EnableIRQ(USART1_RX_IRQn);

  // Enable receive data valid interrupt
  USART_IntEnable(USART1, USART_IEN_RXDATAV);
}

void init_gpio_reset(void)
{
  //clock enable
  CMU_ClockEnable(cmuClock_GPIO, true);

  GPIO_PinModeSet(gpioPortD, 2, gpioModePushPull, 1); //Reset PD2
}

void reset_RS9116_module(void)
{

  //  GPIO_PinModeSet(gpioPortD,2,gpioModeWiredOrPullDown,0);
  GPIO_PinOutClear(gpioPortD, 2);

  rsi_delay_ms(100);

  //   GPIO_PinModeSet(gpioPortD,2,gpioModeWiredOrPullDown,1);
  GPIO_PinOutSet(gpioPortD, 2);
}
void string2hexString(char *input, char *output) //Converts a string(input) into hex string(output)
{
  int loop;
  int i;

  i    = 0;
  loop = 0;

  while (input[loop] != '\0') {
    sprintf((char *)(output + i), "%02X", input[loop]);
    loop += 1;
    i += 2;
  }
  output[i++] = '\0';
}

char int2char(int x) //Converts an integer(ascii value) into character
{
  char charValue = (x - 48) + '0';
  return charValue;
}

void int_to_hex_array(int quotient,
                      char *hexadecimalNumber) //Converts an integer into hexadecimal string in reverse order.
{

  int temp;
  int i = 0;
  while (quotient != 0) {
    temp = quotient % 16;

    if (temp < 10)
      temp = temp + 48;
    else
      temp = temp + 55;
    hexadecimalNumber[i++] = temp;
    quotient               = quotient / 16;
  }
}

int hex2int(char *hexVal) //Converts an hexadecimal string into integer.
{
  int len     = strlen(hexVal);
  int base    = 1;
  int dec_val = 0;
  for (int i = len - 1; i >= 0; i--) {
    if (hexVal[i] >= '0' && hexVal[i] <= '9') {
      dec_val += (((int)hexVal[i]) - 48) * base;
      base = base * 16;
    } else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
      dec_val += (((int)hexVal[i]) - 55) * base;
      base = base * 16;
    }
  }
  return dec_val;
}

int substringcheck(char *str1, char *str2) //Checks whether string(str1) is present within string(str2)
{
  int l, i, j;
  for (l = 0; str2[l] != '\0'; l++)
    ;

  for (i = 0, j = 0; str1[i] != '\0' && str2[j] != '\0'; i++) {
    if (str1[i] == str2[j]) {
      j++;
    } else {
      j = 0;
    }
  }

  if (j == l) {
    return 1;
  } else {
    return 0;
  }
}

int substringcount(char *str,
                   char *sub,
                   char *index) //Returns the count of substring(sub) in string(str) and their respective indices
{
  int i, j, l1, l2;
  l2         = strlen(sub);
  l1         = strlen(str);
  int count1 = 0;
  int count  = 0;
  for (i = 0; i <= l1 - l2; i++) {

    count = 0;
    for (j = 0; j < l2; j++) {
      if (str[i + count] == sub[j]) {
        count++;
      } else
        break;
    }
    if (count == l2) {
      index[count1] = i;
      count1++;
    }
  }
  return count1;
}

uint8_t check_response_type(void) //Returns the type of response received (AT command / OK response / Error Code)
{
  char str1[] = "AT", str2[] = "OK", str3[] = "ERROR";

  if (strstr((char *)uart_rev_buf, str1) != NULL) {
    return 1;
  } else if (strstr((char *)uart_rev_buf, str2) != NULL) {
    return 0;
  } else if (strstr((char *)uart_rev_buf, str3) != NULL) {
    return 2;
  }
  return -1;
}

void wait_for_response() //Validates whether data is received or not and returns their respective code.
{
  uint8_t status = 0;
repeat:
  if (data_received == 1) {
    rsi_delay_ms(200);
    status        = check_response_type();
    index         = 0;
    data_received = 0;
    if (status == 0) //OK response
    {
      flag = 0;
    } else if (status == 1) //AT commands received
    {

      flag = 1;
    } else if (status == 2) //Error data received
    {
      flag = 2;
    } else {
      goto repeat; //Junk data received
    }
  } else {
    goto repeat; //Keep waiting until data is received
  }
}

int32_t rsi_uart_frame_write(uint8_t *frame) //Performs TX operation
{
  memset(uart_rev_buf, 0, BUFFER_SIZE);
  uart_tx_data(USART1, frame, strlen((const char *)frame));
  index = 0;
  return 0;
}

void addcharacteristics() //Adds characteristics to the declared service AABB
{
  char temp11[40]       = ",2,2803,2,6,0,8,0,";
  char temp111[20]      = ",00,a1,1a\r\n";
  char temp12[80]       = ",2,1aa1,8,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\r\n";
  char temp21[40]       = ",2,2803,2,6,0,A,0,";
  char temp211[40]      = ",00,b1,1b\r\n";
  char temp22[80]       = ",2,1bb1,A,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\r\n";
  char temp31[40]       = ",2,2803,2,6,0,1A,0,";
  char temp311[40]      = ",00,c1,1c\r\n";
  char temp32[80]       = ",2,1cc1,1A,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\r\n";
  char temp33[40]       = ",2,2902,A,2,0,0,0\r\n";
  char temperror[20]    = ",14,2,0\r\n";
  char temp_hexarray[4] = { 0 };
  int frame_temp_len_1, frame_temp_len_2, frame_index_1, frame_index_2, errorlength;
  int int_starthandle = (int)starthandle;
  frame_temp_len_1    = strlen((char *)frame_add_char11);
  frame_index_1       = frame_temp_len_1;
  for (int j = 0; j < 5; j++, frame_index_1++) {
    frame_add_char11[frame_index_1] = servicehandle[j];
    frame_add_char12[frame_index_1] = servicehandle[j];
    frame_add_char21[frame_index_1] = servicehandle[j];
    frame_add_char22[frame_index_1] = servicehandle[j];
    frame_add_char31[frame_index_1] = servicehandle[j];
    frame_add_char32[frame_index_1] = servicehandle[j];
    frame_add_char33[frame_index_1] = servicehandle[j];
  }
  frame_add_char11[frame_index_1] = ',';
  frame_add_char12[frame_index_1] = ',';
  frame_add_char21[frame_index_1] = ',';
  frame_add_char22[frame_index_1] = ',';
  frame_add_char31[frame_index_1] = ',';
  frame_add_char32[frame_index_1] = ',';
  frame_add_char33[frame_index_1] = ',';
  frame_index_1++;
  int_starthandle = int_starthandle - 55;
  int_starthandle = int_starthandle + 1;
  frame_index_2   = frame_index_1;
  memset(temp_hexarray, 0, 4);
  int_to_hex_array(int_starthandle, temp_hexarray);
  for (int j = strlen(temp_hexarray) - 1; j >= 0; j--, frame_index_2++) {
    frame_add_char11[frame_index_2] = temp_hexarray[j];
  }

  int_starthandle = int_starthandle + 1;
  frame_index_2   = frame_index_1;
  memset(temp_hexarray, 0, 4);
  int_to_hex_array(int_starthandle, temp_hexarray);
  for (int j = strlen(temp_hexarray) - 1; j >= 0; j--, frame_index_2++) {
    frame_add_char12[frame_index_2] = temp_hexarray[j];
  }
  for (int j = strlen(temp_hexarray) - 1, p = 0; j >= 0; j--, p++) {
    handle_1aa1[p] = temp_hexarray[j];
  }
  int_starthandle = int_starthandle + 1;
  frame_index_2   = frame_index_1;
  memset(temp_hexarray, 0, 4);
  int_to_hex_array(int_starthandle, temp_hexarray);

  for (int j = strlen(temp_hexarray) - 1; j >= 0; j--, frame_index_2++) {
    frame_add_char21[frame_index_2] = temp_hexarray[j];
  }
  int_starthandle = int_starthandle + 1;
  frame_index_2   = frame_index_1;
  memset(temp_hexarray, 0, 4);
  int_to_hex_array(int_starthandle, temp_hexarray);

  for (int j = strlen(temp_hexarray) - 1; j >= 0; j--, frame_index_2++) {
    frame_add_char22[frame_index_2] = temp_hexarray[j];
  }
  for (int j = strlen(temp_hexarray) - 1, p = 0; j >= 0; j--, p++) {
    handle_1bb1[p] = temp_hexarray[j];
  }

  int_starthandle = int_starthandle + 1; //F
  frame_index_2   = frame_index_1;
  memset(temp_hexarray, 0, 4);
  int_to_hex_array(int_starthandle, temp_hexarray);

  for (int j = strlen(temp_hexarray) - 1; j >= 0; j--, frame_index_2++) {
    frame_add_char31[frame_index_2] = temp_hexarray[j];
  }
  int_starthandle = int_starthandle + 1; //10
  frame_index_2   = frame_index_1;
  memset(temp_hexarray, 0, 4);
  int_to_hex_array(int_starthandle, temp_hexarray);

  for (int j = strlen(temp_hexarray) - 1; j >= 0; j--, frame_index_2++) {
    frame_add_char32[frame_index_2] = temp_hexarray[j];
  }
  for (int j = strlen(temp_hexarray) - 1, p = 0; j >= 0; j--, p++) {
    handle_1cc1[p] = temp_hexarray[j];
  }

  int_starthandle = int_starthandle + 1; //11
  frame_index_2   = frame_index_1;
  memset(temp_hexarray, 0, 4);
  int_to_hex_array(int_starthandle, temp_hexarray);

  for (int j = strlen(temp_hexarray) - 1; j >= 0; j--, frame_index_2++) {
    frame_add_char33[frame_index_2] = temp_hexarray[j];
  }

  frame_temp_len_1 = strlen((char *)frame_add_char11);
  frame_temp_len_2 = strlen(temp11);
  for (int i = 0; i < frame_temp_len_2; i++, frame_temp_len_1++) {
    frame_add_char11[frame_temp_len_1] = temp11[i];
  }
  frame_temp_len_2 = strlen(handle_1aa1);
  for (int p = 0; p < frame_temp_len_2; p++) {
    frame_add_char11[frame_temp_len_1++] = handle_1aa1[p];
  }

  frame_temp_len_1 = strlen((char *)frame_add_char11);
  frame_temp_len_2 = strlen(temp111);
  for (int i = 0; i < frame_temp_len_2; i++, frame_temp_len_1++) {
    frame_add_char11[frame_temp_len_1] = temp111[i];
  }
  frame_temp_len_1 = strlen((char *)frame_add_char12);
  frame_temp_len_2 = strlen(temp12);
  for (int i = 0; i < frame_temp_len_2; i++, frame_temp_len_1++) {
    frame_add_char12[frame_temp_len_1] = temp12[i];
  }

  frame_temp_len_1 = strlen((char *)frame_add_char21);
  frame_temp_len_2 = strlen(temp21);
  for (int i = 0; i < frame_temp_len_2; i++, frame_temp_len_1++) {
    frame_add_char21[frame_temp_len_1] = temp21[i];
  }
  frame_temp_len_2 = strlen(handle_1bb1);
  for (int p = 0; p < frame_temp_len_2; p++) {
    frame_add_char21[frame_temp_len_1++] = handle_1bb1[p];
  }

  frame_temp_len_1 = strlen((char *)frame_add_char21);
  frame_temp_len_2 = strlen(temp211);
  for (int i = 0; i < frame_temp_len_2; i++, frame_temp_len_1++) {
    frame_add_char21[frame_temp_len_1] = temp211[i];
  }

  frame_temp_len_1 = strlen((char *)frame_add_char22);
  frame_temp_len_2 = strlen(temp22);
  for (int i = 0; i < frame_temp_len_2; i++, frame_temp_len_1++) {
    frame_add_char22[frame_temp_len_1] = temp22[i];
  }

  frame_temp_len_1 = strlen((char *)frame_add_char31);
  frame_temp_len_2 = strlen(temp31);
  for (int i = 0; i < frame_temp_len_2; i++, frame_temp_len_1++) {
    frame_add_char31[frame_temp_len_1] = temp31[i];
  }
  frame_temp_len_2 = strlen(handle_1cc1);
  for (int p = 0; p < frame_temp_len_2; p++) {
    frame_add_char31[frame_temp_len_1++] = handle_1cc1[p];
  }

  frame_temp_len_1 = strlen((char *)frame_add_char31);
  frame_temp_len_2 = strlen(temp311);
  for (int i = 0; i < frame_temp_len_2; i++, frame_temp_len_1++) {
    frame_add_char31[frame_temp_len_1] = temp311[i];
  }

  frame_temp_len_1 = strlen((char *)frame_add_char32);
  frame_temp_len_2 = strlen(temp32);
  for (int i = 0; i < frame_temp_len_2; i++, frame_temp_len_1++) {
    frame_add_char32[frame_temp_len_1] = temp32[i];
  }

  frame_temp_len_1 = strlen((char *)frame_add_char33);
  frame_temp_len_2 = strlen(temp33);
  for (int i = 0; i < frame_temp_len_2; i++, frame_temp_len_1++) {
    frame_add_char33[frame_temp_len_1] = temp33[i];
  }

  errorlength = strlen((char *)frame_errormsg);
  for (int p = 0; p < strlen(handle_1bb1); p++) {
    frame_errormsg[errorlength++] = handle_1bb1[p];
  }
  frame_temp_len_2 = strlen(temperror);
  for (int i = 0; i < frame_temp_len_2; i++, errorlength++) {
    frame_errormsg[errorlength] = temperror[i];
  }
}

int32_t process_uart_commands() //Setting up BLE Services
{
  uint8_t *frame;
  frame = (uint8_t *)malloc(400 * sizeof(uint8_t));

  while (1) {
    switch (cmd_state) {
      case CMD_STATE_NONE: {
        rsi_delay_ms(100);
        frame       = &frame_opermode[0];
        frame_write = 1;
        cmd_state   = CMD_STATE_OPERMODE_DONE;
        break;
      }
      case CMD_STATE_OPERMODE_DONE: {
        rsi_delay_ms(100);
        printf("\nOpermode Done");
        frame       = &frame_fwversion[0];
        frame_write = 1;
        cmd_state   = CMD_STATE_FWVERSION_DONE;
        break;
      }
      case CMD_STATE_FWVERSION_DONE: {
        rsi_delay_ms(100);
        for (int i = 2; i < 17; i++) {
          fwversion[i - 2] = uart_rev_buf[i];
        }
        printf("\nFirmware version is: ");
        puts(fwversion);
        frame       = &frame_ble_local_name[0];
        frame_write = 1;
        cmd_state   = CMD_STATE_BLE_LOCALNAME_DONE;
        break;
      }
      case CMD_STATE_BLE_LOCALNAME_DONE: {
        printf("\nLocal Name Set");
        rsi_delay_ms(100);
        frame       = &frame_ble_bd_addr[0];
        frame_write = 1;
        cmd_state   = CMD_STATE_BLE_BDADDR_DONE;
        break;
      }
      case CMD_STATE_BLE_BDADDR_DONE: {
        rsi_delay_ms(100);
        for (int i = 2; i < 20; i++) {
          bdaddress[i - 2] = uart_rev_buf[i];
        }
        printf("\nLocal device address is: ");
        puts(bdaddress);
        frame       = &frame_add_service[0]; //Response
        frame_write = 1;
        cmd_state   = CMD_STATE_ADD_SERVICE_DONE;
        break;
      }
      case CMD_STATE_ADD_SERVICE_DONE: {
        rsi_delay_ms(100);
        for (int i = 3; i < 8; i++) {
          servicehandle[i - 3] = uart_rev_buf[i];
        }
        starthandle = uart_rev_buf[9];
        printf("\nService Handle is :");
        puts(servicehandle);
        printf("\nStart Handle is : %c", starthandle);
        addcharacteristics();
        frame_write = 1;
        frame       = &frame_add_char11[0];
        cmd_state   = CMD_STATE_ADD_CHAR_DONE_11;
        break;
      }
      case CMD_STATE_ADD_CHAR_DONE_11: {
        rsi_delay_ms(100);
        frame       = &frame_add_char12[0];
        frame_write = 1;
        cmd_state   = CMD_STATE_ADD_CHAR_DONE_12;
        break;
      }
      case CMD_STATE_ADD_CHAR_DONE_12: {
        rsi_delay_ms(100);
        frame       = &frame_add_char21[0];
        frame_write = 1;
        cmd_state   = CMD_STATE_ADD_CHAR_DONE_21;
        break;
      }

      case CMD_STATE_ADD_CHAR_DONE_21: {

        rsi_delay_ms(100);
        frame       = &frame_add_char22[0];
        frame_write = 1;
        cmd_state   = CMD_STATE_ADD_CHAR_DONE_22;
        break;
      }
      case CMD_STATE_ADD_CHAR_DONE_22: {
        rsi_delay_ms(100);
        frame       = &frame_add_char31[0];
        frame_write = 1;
        cmd_state   = CMD_STATE_ADD_CHAR_DONE_31;
        break;
      }
      case CMD_STATE_ADD_CHAR_DONE_31: {

        rsi_delay_ms(100);
        frame       = &frame_add_char32[0];
        frame_write = 1;
        cmd_state   = CMD_STATE_ADD_CHAR_DONE_32;
        break;
      }
      case CMD_STATE_ADD_CHAR_DONE_32: {
        rsi_delay_ms(100);
        frame       = &frame_add_char33[0];
        frame_write = 1;
        cmd_state   = CMD_STATE_ADD_CHAR_DONE_33;
        break;
      }
      case CMD_STATE_ADD_CHAR_DONE_33: {
        rsi_delay_ms(100);
        frame       = &frame_band[0];
        frame_write = 1;
        cmd_state   = CMD_STATE_BAND_DONE;
        break;
      }
      case CMD_STATE_BAND_DONE: {

        rsi_delay_ms(100);
        frame       = &frame_ble_set_adv[0];
        frame_write = 1;
        cmd_state   = CMD_STATE_BLE_SETADV_DONE;
        break;
      }
      case CMD_STATE_BLE_SETADV_DONE: {
        rsi_delay_ms(100);
        printf("\nBLE Advertising Data set to BLE_CONFIGURATOR");
        frame       = &frame_ble_adv[0];
        frame_write = 1;
        cmd_state   = CMD_STATE_BLE_ADV;
        break;
      }
      case CMD_STATE_BLE_ADV: {
        printf("\nBLE Advertising Started...\n");
        frame_write = 0;
        break;
      } break;
    }

    if (frame_write == 1) {
      frame_write = 0;
      rsi_uart_frame_write(frame);
    } else {
      return 0;
    }
    wait_for_response();
  }
}

void defaultvalues() //Setting default values --> Erasing previous network data
{
  connected = 0;
  ap_num    = 0;
  memset(sec_type, 0, 4);
  memset(psk_ssid, 0, 100);
  memset(ssid_selected, 0, 100);
  memset(sec_type_all, 0, 32);
}

void communicate() //Communication between RS9116 and Provisioning Tool
{
  uint8_t *frame;
  frame                          = (uint8_t *)malloc(400 * sizeof(uint8_t));
  uint8_t temp_resp[BUFFER_SIZE] = { 0 };
  int temp_bufsize;
  char temp_disconnect[28]       = "AT+RSIBT_LE_DISCONNECTED";
  char temp_device_connected[36] = "AT+RSIBT_LE_DEVICE_CONNECTED";
  char temp_write[20]            = "AT+RSIBT_WRITE";
  char cmd_index[4]              = { 0 };
  char cmd_fwversion[4]          = "38";
  char cmd_wlan_status[4]        = "37";
  char cmd_psk[4]                = "36";
  char cmd_security[4]           = "35";
  char cmd_disconnect[4]         = "34";
  char cmd_scan[4]               = "33";
  char cmd_join[4]               = "32";

  while (1) {
    rsi_delay_ms(100);
    wait_for_response();
    if (flag == 0) //OK Response
    {
      switch (cmd_event_num) {
        case CMD_EVENT_FWVERSION: {
          printf("\nFirmware sent");
          cmd_event_num = CMD_EVENT_NONE;
          break;
        }
        case CMD_EVENT_WLAN_STATUS: {
          printf("\nWLAN Status sent");
          cmd_event_num = CMD_EVENT_NONE;
          break;
        }
        case CMD_EVENT_SCAN_INIT: {
          printf("\nWi-Fi Initialization Successful");
          wifiinitstatus = 1;
          joinfailure    = 0;
          cmd_event_num  = CMD_EVENT_SCAN_RESP;
          break;
        }
        case CMD_EVENT_SCAN_RESP: {
          if (wrong_password == 1) {
            wrong_password = 0;
            cmd_event_num  = CMD_EVENT_CONNECT;
          } else {
            char hex_temp_ap[4]         = { 0 };
            int temp_index              = 10;
            uint8_t temp_send_data[100] = "at+rsibt_setlocalattvalue=";
            char temp_len_string[4]     = { 0 };
            int len_tsd                 = strlen((char *)temp_send_data);

            sprintf(hex_temp_ap, "%X", (unsigned char)uart_rev_buf[2]);
            ap_num = hex2int(hex_temp_ap);
            for (int count = 0; count < ap_num; count++) {
              for (int i = 0; i < 46; i++) {
                char temp_hex[4] = { 0 };
                sprintf(temp_hex, "%X", (unsigned char)uart_rev_buf[temp_index]);
                if (strlen(temp_hex) == 2) {
                  ssid_list[count][2 * (i)]     = temp_hex[0];
                  ssid_list[count][2 * (i) + 1] = temp_hex[1];
                } else {
                  ssid_list[count][2 * (i)]     = '0';
                  ssid_list[count][2 * (i) + 1] = temp_hex[0];
                }
                temp_index++;
              }
            }
            printf("\nTotal number of APs: %d ", ap_num);
            for (int i = 0; i < ap_num; i++) {
              printf("\nAP[%d] is: ", i);
              for (int j = 4; j < 38; j++) {
                if (ssid_list[i][2 * j] == '0' && ssid_list[i][2 * j + 1] == '0')
                  break;

                ap_list[i][2 * (j - 4)]     = ssid_list[i][2 * j];
                ap_list[i][2 * (j - 4) + 1] = ssid_list[i][2 * j + 1];

                char temp_hex_val[4] = { 0 };
                temp_hex_val[0]      = ssid_list[i][2 * j];
                temp_hex_val[1]      = ssid_list[i][2 * j + 1];
                int temp_int_val     = hex2int(temp_hex_val);
                char result          = int2char(temp_int_val);
                printf("%c", result);
              }
            }

            for (int i = 0; i < ap_num; i++) {
              sec_type_all[i] = ssid_list[i][3];
              printf("\nSec_type[%d] is %c", i, sec_type_all[i]);
            }

            for (int i = 0; i < strlen(handle_1bb1); i++) {
              temp_send_data[len_tsd++] = handle_1bb1[i];
            }

            temp_send_data[len_tsd++] = ',';
            temp_send_data[len_tsd++] = '1';
            temp_send_data[len_tsd++] = '4';
            temp_send_data[len_tsd++] = ',';
            temp_send_data[len_tsd++] = '3';
            temp_send_data[len_tsd++] = '3';
            temp_send_data[len_tsd++] = ',';
            int_to_hex_array(ap_num, temp_len_string);

            for (int i = 0; i < strlen(temp_len_string); i++) {
              temp_send_data[len_tsd++] = temp_len_string[i];
            }
            temp_send_data[len_tsd++] = '\r';
            temp_send_data[len_tsd++] = '\n';

            frame = &temp_send_data[0];
            rsi_delay_ms(100);
            rsi_uart_frame_write(frame);
            cmd_event_num = CMD_EVENT_SEND_SCAN_RESP;
          }
          break;
        }
        case CMD_EVENT_SEND_SCAN_RESP: {
          if (ap_num--) {
            uint8_t temp_send_data[100] = "at+rsibt_setlocalattvalue=";
            int len_tsd                 = strlen((char *)temp_send_data);
            int len_ap                  = strlen(ap_list[ap_num]);
            for (int i = 0; i < strlen(handle_1cc1); i++) {
              temp_send_data[len_tsd++] = handle_1cc1[i];
            }
            temp_send_data[len_tsd++] = ',';
            temp_send_data[len_tsd++] = '1';
            temp_send_data[len_tsd++] = '4';
            temp_send_data[len_tsd++] = ',';
            temp_send_data[len_tsd++] = sec_type_all[ap_num];
            temp_send_data[len_tsd++] = ',';
            temp_send_data[len_tsd++] = '2';
            temp_send_data[len_tsd++] = 'c';

            for (int i = 0; i < len_ap; i = i + 2) {
              temp_send_data[len_tsd++] = ',';
              temp_send_data[len_tsd++] = ap_list[ap_num][i];
              temp_send_data[len_tsd++] = ap_list[ap_num][i + 1];
            }
            temp_send_data[len_tsd++] = '\r';
            temp_send_data[len_tsd++] = '\n';
            frame                     = &temp_send_data[0];
            rsi_delay_ms(100);
            rsi_uart_frame_write(frame);
          } else {
            cmd_event_num = CMD_EVENT_NONE;
          }
          break;
        }
        case CMD_EVENT_DISSOCIATE: {
          uint8_t temp_send_data[100] = "at+rsibt_setlocalattvalue=";
          int len_tsd                 = strlen((char *)temp_send_data);
          printf("\nWLAN Dissociation done");
          for (int i = 0; i < strlen(handle_1bb1); i++) {
            temp_send_data[len_tsd++] = handle_1bb1[i];
          }
          temp_send_data[len_tsd++] = ',';
          temp_send_data[len_tsd++] = '1';
          temp_send_data[len_tsd++] = '4';
          temp_send_data[len_tsd++] = ',';
          temp_send_data[len_tsd++] = '4';
          temp_send_data[len_tsd++] = ',';
          temp_send_data[len_tsd++] = '1';
          temp_send_data[len_tsd++] = '\r';
          temp_send_data[len_tsd++] = '\n';
          rsi_delay_ms(100);
          frame         = &temp_send_data[0];
          cmd_event_num = CMD_EVENT_DISSOCIATE_SETLOC;
          rsi_uart_frame_write(frame);
          break;
        }
        case CMD_EVENT_DISSOCIATE_SETLOC: {
          printf("\nWLAN Dissociation status sent");
          cmd_event_num = CMD_EVENT_NONE;
          break;
        }
        case CMD_EVENT_PSK: {
          printf("\nPassword set");
          cmd_event_num = CMD_EVENT_CONNECT;
          break;
        }
        case CMD_EVENT_CONNECT: {
          printf("\nJoin Successful");
          cmd_event_num = CMD_EVENT_IP;
          break;
        }
        case CMD_EVENT_IP: {
          uint8_t temp_send_data[200] = "at+rsibt_setlocalattvalue=";
          int len_tsd                 = strlen((char *)temp_send_data);
          memset(mac_address, '0', 20);
          memset(ip_address, '0', 20);
          for (int i = 2; i < 8; i++) {
            char temp_hex[4] = { 0 };
            sprintf(temp_hex, "%X", (unsigned char)uart_rev_buf[i]);
            if (strlen(temp_hex) == 2) {
              mac_address[2 * (i - 2)]     = temp_hex[0];
              mac_address[2 * (i - 2) + 1] = temp_hex[1];
            } else {
              mac_address[2 * (i - 2) + 1] = temp_hex[0];
            }
          }
          for (int i = 8; i < 12; i++) {
            char temp_hex[4] = { 0 };
            sprintf(temp_hex, "%X", (unsigned char)uart_rev_buf[i]);
            if (strlen(temp_hex) == 2) {
              ip_address[2 * (i - 8)]     = temp_hex[0];
              ip_address[2 * (i - 8) + 1] = temp_hex[1];
            } else {
              ip_address[2 * (i - 8) + 1] = temp_hex[0];
            }
          }
          printf("\nIP Address : ");
          for (int i = 0; i < 4; i++) {
            printf("%c%c ", mac_address[2 * i], ip_address[2 * i + 1]);
          }
          printf("\nMAC Address : ");
          for (int i = 0; i < 6; i++) {
            printf("%c%c ", ip_address[2 * i], mac_address[2 * i + 1]);
          }

          for (int i = 0; i < strlen(handle_1bb1); i++) {
            temp_send_data[len_tsd++] = handle_1bb1[i];
          }
          temp_send_data[len_tsd++] = ',';
          temp_send_data[len_tsd++] = '1';
          temp_send_data[len_tsd++] = '4';
          temp_send_data[len_tsd++] = ',';
          temp_send_data[len_tsd++] = '2';
          temp_send_data[len_tsd++] = ',';
          temp_send_data[len_tsd++] = '1';
          temp_send_data[len_tsd++] = ',';
          temp_send_data[len_tsd++] = '2';
          temp_send_data[len_tsd++] = 'c';
          temp_send_data[len_tsd++] = ',';

          for (int i = 0; i < 8; i = i + 2) {
            temp_send_data[len_tsd++] = ip_address[i];
            temp_send_data[len_tsd++] = ip_address[i + 1];
            temp_send_data[len_tsd++] = ',';
          }

          temp_send_data[len_tsd++] = '2';
          temp_send_data[len_tsd++] = 'c';

          for (int i = 0; i < 12; i = i + 2) {
            temp_send_data[len_tsd++] = ',';
            temp_send_data[len_tsd++] = mac_address[i];
            temp_send_data[len_tsd++] = mac_address[i + 1];
          }
          temp_send_data[len_tsd++] = '\r';
          temp_send_data[len_tsd++] = '\n';
          frame                     = &temp_send_data[0];
          rsi_delay_ms(100);
          rsi_uart_frame_write(frame);
          cmd_event_num = CMD_EVENT_IP_SEND;
          break;
        }
        case CMD_EVENT_IP_SEND: {
          connected = 1;
          printf("\nAP Joined");
          cmd_event_num = CMD_EVENT_NONE;
          break;
        }
        case CMD_EVENT_ERROR_SETLOC: {
          printf("\nError message sent");
          if (ip_error == 1) {
            ip_error       = 0;
            wifiinitstatus = 0;
            joinfailure    = 0;
            defaultvalues();
            cmd_event_num = CMD_EVENT_DISSOCIATE;
            rsi_delay_ms(100);
            frame = &frame_dissoc[0];
            rsi_uart_frame_write(frame);
          } else
            cmd_event_num = CMD_EVENT_NONE;
          break;
        }
        case CMD_EVENT_BLE_EVK_DISCONNECT: {
          rsi_delay_ms(100);
          frame = &frame_ble_set_adv[0];
          rsi_uart_frame_write(frame);
          printf("\nBLE Advertising Data set to BLE_CONFIGURATOR");
          cmd_event_num = CMD_EVENT_BLE_DISCONNECT;
          break;
        }
        case CMD_EVENT_BLE_DISCONNECT: {
          rsi_delay_ms(100);
          frame = &frame_ble_adv[0];
          rsi_uart_frame_write(frame);
          printf("\nBLE Advertising Started...\n");
          cmd_event_num = CMD_EVENT_NONE;
          break;
        }
        case CMD_EVENT_NONE: {
          break;
        }
        case CMD_EVENT_JOIN: {
          break;
        }
        case CMD_EVENT_SECURITY: {
          break;
        }
        case CMD_EVENT_WRONG_PSK: {
          break;
        } break;
      }
    }

    else if (flag == 1) // AT commands
    {
      int startindexofAT = 0;
      memset(temp_resp, 0, BUFFER_SIZE);
      temp_bufsize = strlen((char *)uart_rev_buf);

      while (uart_rev_buf[startindexofAT] != 'A') {
        startindexofAT++;
      }
      for (int i = startindexofAT; i < temp_bufsize; i++) {
        temp_resp[i - startindexofAT] = uart_rev_buf[i];
      }
      memset(uart_rev_buf, 0, BUFFER_SIZE);
      if (substringcheck((char *)temp_resp, temp_disconnect) == 1) //Checks if the event triggered is to disconnect BLE
      {
        cmd_event_num = CMD_EVENT_BLE_DISCONNECT;
        printf("\nBLE Disconnected");
        frame = &frame_ble_set_adv[0];
        rsi_delay_ms(100);
        rsi_uart_frame_write(frame);
      }

      else if (substringcheck((char *)temp_resp, temp_device_connected)
               == 1) //Checks if the event triggered is BLE connection established
      {
        char index_position[4] = { 0 };
        for (int i = 0; i < 18; i++) {
          remotedevicesadd[i] = temp_resp[31 + i + index_position[0]];
        }
        printf("\nRemote Device address is %s ", remotedevicesadd);
        cmd_event_num = CMD_EVENT_NONE;
      }

      else if (substringcheck((char *)temp_resp, temp_write) == 1) //Checks for the AT command id
      {
        int buf_index           = 0;
        char index_position[12] = { 0 };
        int index_count         = substringcount((char *)temp_resp, temp_write, index_position);
        int temp_buf_id;
        int count = 0;

        while (count != 4) {
          if (temp_resp[buf_index] == ',') {
            count++;
          }
          buf_index++;
        }
        temp_buf_id = buf_index;
        for (int kk = 0; kk < index_count; kk++) {
          buf_index    = temp_buf_id + index_position[kk];
          cmd_index[0] = temp_resp[buf_index];
          cmd_index[1] = temp_resp[buf_index + 1];
          if (substringcheck(cmd_index, cmd_fwversion) == 1) {
            cmd_event_num = CMD_EVENT_FWVERSION;
          } else if (substringcheck(cmd_index, cmd_wlan_status) == 1) {
            cmd_event_num = CMD_EVENT_WLAN_STATUS;
          } else if (substringcheck(cmd_index, cmd_scan) == 1) {
            cmd_event_num = CMD_EVENT_SCAN_INIT;
          } else if (substringcheck(cmd_index, cmd_security) == 1) {
            cmd_event_num = CMD_EVENT_SECURITY;
          } else if (substringcheck(cmd_index, cmd_psk) == 1) {
            cmd_event_num = CMD_EVENT_PSK;
          } else if (substringcheck(cmd_index, cmd_join) == 1) {
            cmd_event_num = CMD_EVENT_JOIN;
          } else if (substringcheck(cmd_index, cmd_disconnect) == 1) {
            cmd_event_num = CMD_EVENT_DISSOCIATE;
          } else
            continue;

          switch (cmd_event_num) {
            case CMD_EVENT_NONE: {
              break;
            }

            case CMD_EVENT_FWVERSION: {
              uint8_t temp_send_data[100] = "at+rsibt_setlocalattvalue=";
              int len_fwversion           = strlen(fwversion);
              int len_tsd                 = strlen((char *)temp_send_data);
              char temp_fwversion[2 * len_fwversion + 1];
              string2hexString(fwversion, temp_fwversion);
              for (int i = 0; i < strlen(handle_1bb1); i++) {
                temp_send_data[len_tsd++] = handle_1bb1[i];
              }
              temp_send_data[len_tsd++] = ',';
              temp_send_data[len_tsd++] = '1';
              temp_send_data[len_tsd++] = '4';
              temp_send_data[len_tsd++] = ',';
              temp_send_data[len_tsd++] = '8';
              temp_send_data[len_tsd++] = ',';
              temp_send_data[len_tsd++] = '8';
              for (int i = 0; i < len_fwversion; i++) {
                temp_send_data[len_tsd++] = ',';
                temp_send_data[len_tsd++] = temp_fwversion[2 * i];
                temp_send_data[len_tsd++] = temp_fwversion[2 * i + 1];
              }
              temp_send_data[len_tsd++] = '\r';
              temp_send_data[len_tsd++] = '\n';

              rsi_delay_ms(100);
              frame = &temp_send_data[0];
              rsi_uart_frame_write(frame);
              break;
            }

            case CMD_EVENT_WLAN_STATUS: {
              uint8_t temp_send_data[100] = "at+rsibt_setlocalattvalue=";
              int len_tsd                 = strlen((char *)temp_send_data);
              for (int i = 0; i < strlen(handle_1bb1); i++) {
                temp_send_data[len_tsd++] = handle_1bb1[i];
              }
              temp_send_data[len_tsd++] = ',';
              temp_send_data[len_tsd++] = '1';
              temp_send_data[len_tsd++] = '4';
              temp_send_data[len_tsd++] = ',';
              temp_send_data[len_tsd++] = '7';
              if (connected == 0) {
                temp_send_data[len_tsd++] = '\r';
                temp_send_data[len_tsd++] = '\n';
                printf("\nWLAN status : Not Connected");
              } else {
                temp_send_data[len_tsd++] = ',';
                temp_send_data[len_tsd++] = '1';
                temp_send_data[len_tsd++] = '\r';
                temp_send_data[len_tsd++] = '\n';
                printf("\nWLAN status : Already Connected");
              }
              rsi_delay_ms(100);
              frame = &temp_send_data[0];
              rsi_uart_frame_write(frame);

              break;
            }

            case CMD_EVENT_SCAN_INIT: {

              if (wifiinitstatus == 0) //This case is needed when ble disconnects and no Wi-Fi Initialization is needed
              {
                rsi_delay_ms(100);
                frame = &frame_init[0];
                rsi_uart_frame_write(frame);
              } else {
                cmd_event_num = CMD_EVENT_SCAN_RESP;
              }
              break;
            }

            case CMD_EVENT_JOIN: {
              char temp_ssid_selected[100] = { 0 };
              int ssid_index               = 0;
              int temp_buf_len             = strlen((char *)temp_resp);
              int comma                    = 0;
              memset(ssid_selected, 0, 100);
              while (comma != 3) {
                if (temp_resp[buf_index] == ',') {
                  comma++;
                }
                buf_index++;
              }
              int ssid_len = temp_buf_len - buf_index;
              for (int i = 0; temp_resp[i + buf_index] != '\r'; i++) {
                if (temp_resp[i + buf_index] == ',' && temp_resp[i + buf_index + 1] == '0')
                  break;
                else
                  temp_ssid_selected[i] = temp_resp[i + buf_index];
              }
              ssid_len = strlen(temp_ssid_selected);

              for (int i = 0; i < ssid_len; i = i + 3) {
                char temp_char[3] = { 0 };
                int temp_hex2int;
                char temp_int2char;
                temp_char[0]                = temp_ssid_selected[i];
                temp_char[1]                = temp_ssid_selected[i + 1];
                temp_hex2int                = hex2int(temp_char);
                temp_int2char               = int2char(temp_hex2int);
                ssid_selected[ssid_index++] = temp_int2char;
              }
              printf("\n\nSSID selected: ");
              puts(ssid_selected);
              break;
            }

            case CMD_EVENT_SECURITY: {
              sec_type[0] = temp_resp[buf_index + 9];
              sec_type[1] = temp_resp[buf_index + 10];
              if (sec_type[0] == '3' && sec_type[1] == '0') {
                printf("\nThe Security type of AP is OPEN");
                cmd_event_num = CMD_EVENT_CONNECT;
              } else {
                if (sec_type[1] == '1') {
                  printf("\nThe Security type of AP is WPA");
                } else if (sec_type[1] == '2') {
                  printf("\nThe Security type of AP is WPA2");
                } else if (sec_type[1] == '3') {
                  printf("\nThe Security type of AP is WEP");
                } else if (sec_type[1] == '4') {
                  printf("\nThe Security type of AP is WPA Enterprise");
                } else if (sec_type[1] == '5') {
                  printf("\nThe Security type of AP is WPA2 Enterprise");
                } else if (sec_type[1] == '6') {
                  printf("\nThe Security type of AP is WPA_WPA2_MIXED");
                }
              }
              break;
            }

            case CMD_EVENT_PSK: {
              char temp_psk_ssid[100] = { 0 };
              int psk_index           = 0;
              int temp_buf_len;
              int comma                   = 0;
              uint8_t temp_send_data[200] = "at+rsi_psk=4,";
              char temp_len_char[4]       = { 0 };
              int len_tsd                 = strlen((char *)temp_send_data);
              uint8_t psk_len;

              temp_buf_len = strlen((char *)temp_resp);
              memset(psk_ssid, 0, 100);
              while (comma != 3) {
                if (temp_resp[buf_index] == ',') {
                  comma++;
                }
                buf_index++;
              }
              psk_len = temp_buf_len - buf_index;
              for (int i = 0; temp_resp[i + buf_index] != '\r'; i++) {
                if (temp_resp[i + buf_index] == ',' && temp_resp[i + buf_index + 1] == '0')
                  break;
                else
                  temp_psk_ssid[i] = temp_resp[i + buf_index];
              }
              psk_len = strlen(temp_psk_ssid);

              for (int i = 0; i < psk_len; i = i + 3) {
                char temp_char[3]     = { 0 };
                temp_char[0]          = temp_psk_ssid[i];
                temp_char[1]          = temp_psk_ssid[i + 1];
                int temp_hex2int      = hex2int(temp_char);
                char temp_int2char    = int2char(temp_hex2int);
                psk_ssid[psk_index++] = temp_int2char;
              }

              psk_len = strlen(psk_ssid);
              sprintf(temp_len_char, "%d", psk_len);
              for (int i = 0; i < strlen(temp_len_char); i++) {
                temp_send_data[len_tsd++] = temp_len_char[i];
              }
              temp_send_data[len_tsd++] = ',';
              for (int i = 0; i < psk_len; i++) {
                temp_send_data[len_tsd++] = psk_ssid[i];
              }

              temp_send_data[len_tsd++] = '\r';
              temp_send_data[len_tsd++] = '\n';
              rsi_delay_ms(100);
              frame = &temp_send_data[0];
              rsi_uart_frame_write(frame);
              rsi_delay_ms(200);
              break;
            }

            case CMD_EVENT_DISSOCIATE: {
              wifiinitstatus = 0;
              joinfailure    = 0;
              defaultvalues();
              rsi_delay_ms(100);
              frame = &frame_dissoc[0];
              rsi_uart_frame_write(frame);
              break;
            }
            case CMD_EVENT_SCAN_RESP: {
              break;
            }
            case CMD_EVENT_SEND_SCAN_RESP: {
              break;
            }
            case CMD_EVENT_CONNECT: {
              break;
            }
            case CMD_EVENT_IP: {
              break;
            }
            case CMD_EVENT_IP_SEND: {
              break;
            }
            case CMD_EVENT_WRONG_PSK: {
              break;
            }
            case CMD_EVENT_DISSOCIATE_SETLOC: {
              break;
            }
            case CMD_EVENT_BLE_DISCONNECT: {
              break;
            }
            case CMD_EVENT_BLE_EVK_DISCONNECT: {
              break;
            }
            case CMD_EVENT_ERROR_SETLOC: {
              break;
            }

            break;
          }
        }
      }
    }

    else if (flag == 2) // Error response
    {
      switch (cmd_event_num) {
        case CMD_EVENT_FWVERSION: {
          uint8_t temp_send_data[100] = "at+rsibt_disconnect=";
          int len_bdaddress           = strlen(bdaddress);
          int len_temp_send_data      = strlen((char *)temp_send_data);
          printf("\nError Received while sending Firmware Version ");
          for (int i = 0; i < len_bdaddress; i++) {
            temp_send_data[len_temp_send_data++] = bdaddress[i];
          }
          temp_send_data[len_temp_send_data++] = '\r';
          temp_send_data[len_temp_send_data++] = '\n';
          frame                                = &temp_send_data[0];
          cmd_event_num                        = CMD_EVENT_BLE_EVK_DISCONNECT;
          rsi_delay_ms(100);
          rsi_uart_frame_write(frame);
          break;
        }
        case CMD_EVENT_WLAN_STATUS: {
          printf("\nError Received while sending WLAN Status ");
          cmd_event_num = CMD_EVENT_NONE;
          break;
        }
        case CMD_EVENT_SCAN_INIT: {
          printf("\nError Received during Wi-Fi Initialization ");
          cmd_event_num = CMD_EVENT_NONE;
          break;
        }
        case CMD_EVENT_SCAN_RESP: {
          printf("\nError Received while getting scan results ");
          cmd_event_num = CMD_EVENT_NONE;
          break;
        }
        case CMD_EVENT_DISSOCIATE: {
          printf("\nWLAN Dissociation Unsuccessful ");
          cmd_event_num = CMD_EVENT_NONE;
          break;
        }
        case CMD_EVENT_DISSOCIATE_SETLOC: {
          printf("\nWLAN Dissociation Status not sent ");
          cmd_event_num = CMD_EVENT_NONE;
          break;
        }
        case CMD_EVENT_PSK: {
          printf("\nInvalid length of Password ");
          defaultvalues();
          cmd_event_num = CMD_EVENT_ERROR_SETLOC;
          break;
        }
        case CMD_EVENT_CONNECT: {
          printf("\nIncorrect Password ");
          joinfailure    = 1;
          wifiinitstatus = 0;
          cmd_event_num  = CMD_EVENT_ERROR_SETLOC;
          break;
        }
        case CMD_EVENT_IP: {
          printf("\nError while receiving IP - MAC address ");
          cmd_event_num = CMD_EVENT_ERROR_SETLOC;
          ip_error      = 1;
          break;
        }
        case CMD_EVENT_IP_SEND: {
          printf("\nError while sending IP - MAC address. AP not connected! ");
          joinfailure    = 1;
          wifiinitstatus = 0;
          cmd_event_num  = CMD_EVENT_ERROR_SETLOC;
          ip_error       = 1;
          break;
        }
        case CMD_EVENT_NONE: {
          break;
        }
        case CMD_EVENT_SEND_SCAN_RESP: {
          break;
        }
        case CMD_EVENT_JOIN: {
          break;
        }
        case CMD_EVENT_SECURITY: {
          break;
        }
        case CMD_EVENT_WRONG_PSK: {
          break;
        }
        case CMD_EVENT_BLE_DISCONNECT: {
          break;
        }
        case CMD_EVENT_BLE_EVK_DISCONNECT: {
          break;
        }
        case CMD_EVENT_ERROR_SETLOC: {
          break;
        } break;
      }
      puts((char *)uart_rev_buf);
    } else {
      if (cmd_event_num == CMD_EVENT_PSK) {
        printf("\nPassword not set");
        defaultvalues();
        joinfailure    = 1;
        wifiinitstatus = 0;
        cmd_event_num  = CMD_EVENT_ERROR_SETLOC;
      } else if (cmd_event_num == CMD_EVENT_CONNECT) {
        printf("\nJoin not established");
        joinfailure    = 1;
        wifiinitstatus = 0;
        cmd_event_num  = CMD_EVENT_ERROR_SETLOC;
        break;
      }
    }
    if (cmd_event_num == CMD_EVENT_CONNECT) {
      if (joinfailure == 1) {
        joinfailure   = 0;
        cmd_event_num = CMD_EVENT_WRONG_PSK;
      } else {
        uint8_t temp_send_data[200] = "at+rsi_join=";
        int len_tsd                 = strlen((char *)temp_send_data);
        int len_ssid_selected       = strlen(ssid_selected);
        for (int i = 0; i < len_ssid_selected; i++) {
          temp_send_data[len_tsd++] = ssid_selected[i];
        }
        temp_send_data[len_tsd++] = ',';
        temp_send_data[len_tsd++] = '0';
        temp_send_data[len_tsd++] = ',';
        temp_send_data[len_tsd++] = '2';
        temp_send_data[len_tsd++] = ',';
        temp_send_data[len_tsd++] = sec_type[1];
        temp_send_data[len_tsd++] = '\r';
        temp_send_data[len_tsd++] = '\n';
        rsi_delay_ms(100);
        frame = &temp_send_data[0];
        rsi_uart_frame_write(frame);
        rsi_delay_ms(100);
      }
    } else if (cmd_event_num == CMD_EVENT_SCAN_RESP) {
      if (wifiinitstatus == 0) {
        rsi_delay_ms(100);
        frame = &frame_init[0];
        rsi_uart_frame_write(frame);
        cmd_event_num = CMD_EVENT_SCAN_INIT;
      } else {
        rsi_delay_ms(100);
        frame = &frame_scan[0];
        rsi_uart_frame_write(frame);
        rsi_delay_ms(1000);
      }
    } else if (cmd_event_num == CMD_EVENT_IP) {
      rsi_delay_ms(100);
      frame = &frame_ip[0];
      rsi_uart_frame_write(frame);
    } else if (cmd_event_num == CMD_EVENT_ERROR_SETLOC) {
      rsi_delay_ms(100);
      frame = &frame_errormsg[0];
      rsi_uart_frame_write(frame);
    }

    if (cmd_event_num == CMD_EVENT_WRONG_PSK) {
      wrong_password = 1;
      memset(psk_ssid, 0, 100);
      rsi_delay_ms(100);
      frame         = &frame_init[0];
      cmd_event_num = CMD_EVENT_SCAN_INIT;
      rsi_uart_frame_write(frame);
    }
  }
}

int main(void)
{
  rsi_hal_board_init();
  process_uart_commands();
  communicate();
}

/****END OF FILE****/
