/***************************************************************************//**
 * @file
 * @brief WFX FMAC driver host implementation
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "sl_wfx.h"
#include "sl_wfx_host_api.h"
#include "sl_wfx_host_cfg.h"

#include "em_core.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_cmu.h"
#include "em_ldma.h"
#include "em_bus.h"
#if   defined(SLEXP802X)
#include "brd8022a_pds.h"
#include "brd8023a_pds.h"
#elif defined(EFM32GG11B820F2048GM64) //WGM160PX22KGA2
#include "brd4001a_pds.h"
#else
#error "WFX200 EXP board type must be specified"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* Firmware include */
#include "sl_wfx_wf200_C0.h"

#include "lwip_bm.h"
#include "lwip/sys.h"
#include "dhcp_server.h"
#include "sl_wfx_host.h"

extern char event_log[];

#define SL_WFX_EVENT_MAX_SIZE  512
#define SL_WFX_EVENT_LIST_SIZE 1
#define SL_WFX_MAX_STATIONS    8
#define SL_WFX_MAX_SCAN_RESULTS 50

scan_result_list_t scan_list[SL_WFX_MAX_SCAN_RESULTS];
uint8_t scan_count = 0;
uint8_t scan_count_web = 0;
volatile uint8_t wf200_interrupt_event = 0;

static uint16_t control_register = 0;

struct {
  uint32_t wf200_firmware_download_progress;
  uint8_t waited_event_id;
  uint8_t posted_event_id;
} host_context;

#ifdef SLEEP_ENABLED
extern sl_wfx_context_t wifi;
#endif

/* WF200 host callbacks */
void sl_wfx_connect_callback(uint8_t* mac, uint32_t status);
void sl_wfx_disconnect_callback(uint8_t* mac, uint16_t reason);
void sl_wfx_start_ap_callback(uint32_t status);
void sl_wfx_stop_ap_callback(void);
void sl_wfx_host_received_frame_callback(sl_wfx_received_ind_t* rx_buffer);
void sl_wfx_scan_result_callback(sl_wfx_scan_result_ind_body_t* scan_result);
void sl_wfx_scan_complete_callback(uint32_t status);
void sl_wfx_generic_status_callback(sl_wfx_generic_ind_t* frame);
void sl_wfx_client_connected_callback(uint8_t* mac);
void sl_wfx_ap_client_disconnected_callback(uint32_t status, uint8_t* mac);

/**************************************************************************//**
 * WFX FMAC driver host interface initialization
 *****************************************************************************/
sl_status_t sl_wfx_host_init(void)
{
  host_context.wf200_firmware_download_progress = 0;
  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Get firmware data
 *****************************************************************************/
sl_status_t sl_wfx_host_get_firmware_data(const uint8_t** data, uint32_t data_size)
{
  *data = &sl_wfx_firmware[host_context.wf200_firmware_download_progress];
  host_context.wf200_firmware_download_progress += data_size;
  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Get firmware size
 *****************************************************************************/
sl_status_t sl_wfx_host_get_firmware_size(uint32_t* firmware_size)
{
  *firmware_size = sizeof(sl_wfx_firmware);
  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Get PDS data
 *****************************************************************************/
sl_status_t sl_wfx_host_get_pds_data(const char **pds_data, uint16_t index)
{
#ifdef SLEXP802X
  // Manage dynamically the PDS in function of the chip connected
  if (strncmp("WFM200", (char *)sl_wfx_context->wfx_opn, 6) == 0) {
    *pds_data = pds_table_brd8023a[index];
  } else {
    *pds_data = pds_table_brd8022a[index];
  }
#else
  *pds_data = pds_table_brd4001a[index];
#endif
  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Get PDS size
 *****************************************************************************/
sl_status_t sl_wfx_host_get_pds_size(uint16_t *pds_size)
{
#ifdef SLEXP802X
  // Manage dynamically the PDS in function of the chip connected
  if (strncmp("WFM200", (char *)sl_wfx_context->wfx_opn, 6) == 0) {
    *pds_size = SL_WFX_ARRAY_COUNT(pds_table_brd8023a);
  } else {
    *pds_size = SL_WFX_ARRAY_COUNT(pds_table_brd8022a);
  }
#else
  *pds_size = SL_WFX_ARRAY_COUNT(pds_table_brd4001a);
#endif
  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Deinit host interface
 *****************************************************************************/
sl_status_t sl_wfx_host_deinit(void)
{
  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Allocate buffer
 *****************************************************************************/
sl_status_t sl_wfx_host_allocate_buffer(void **buffer,
                                        sl_wfx_buffer_type_t type,
                                        uint32_t buffer_size)
{
  SL_WFX_UNUSED_PARAMETER(type);
  *buffer = malloc(buffer_size);
  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Free host buffer
 *****************************************************************************/
sl_status_t sl_wfx_host_free_buffer(void* buffer, sl_wfx_buffer_type_t type)
{
  SL_WFX_UNUSED_PARAMETER(type);
  free(buffer);
  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Set reset pin low
 *****************************************************************************/
sl_status_t sl_wfx_host_hold_in_reset(void)
{
  GPIO_PinOutClear(SL_WFX_HOST_CFG_RESET_PORT, SL_WFX_HOST_CFG_RESET_PIN);
  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Set wakeup pin status
 *****************************************************************************/
sl_status_t sl_wfx_host_set_wake_up_pin(uint8_t state)
{
  if (state > 0) {
    GPIO_PinOutSet(SL_WFX_HOST_CFG_WUP_PORT, SL_WFX_HOST_CFG_WUP_PIN);
  } else {
    GPIO_PinOutClear(SL_WFX_HOST_CFG_WUP_PORT, SL_WFX_HOST_CFG_WUP_PIN);
  }
  return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_reset_chip(void)
{
  // Pull it low for at least 1 ms to issue a reset sequence
  GPIO_PinOutClear(SL_WFX_HOST_CFG_RESET_PORT, SL_WFX_HOST_CFG_RESET_PIN);
  // Delay for 10ms
  delay_ms(10);

  // Hold pin high to get chip out of reset
  GPIO_PinOutSet(SL_WFX_HOST_CFG_RESET_PORT, SL_WFX_HOST_CFG_RESET_PIN);
  // Delay for 3ms
  delay_ms(3);
  return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_wait_for_wake_up(void)
{
  delay_ms(3);
  return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_wait(uint32_t wait_time)
{
  delay_ms(wait_time);
  return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_setup_waited_event(uint8_t event_id)
{
  host_context.waited_event_id = event_id;

  return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_wait_for_confirmation(uint8_t confirmation_id,
                                              uint32_t timeout_ms,
                                              void** event_payload_out)
{
  uint64_t tmo = sys_now() + timeout_ms;
  sl_status_t status = SL_STATUS_TIMEOUT;

  do {
    // Make sure that frames have been received
    if ((wf200_interrupt_event == 1)
        || ((control_register & SL_WFX_CONT_NEXT_LEN_MASK) != 0)) {

      // Reset the interrupt flag
      wf200_interrupt_event = 0;
      // Treat frame received
      sl_wfx_receive_frame(&control_register);

#ifdef SL_WFX_USE_SDIO
      sl_wfx_host_enable_platform_interrupt();
#endif

      // Make sure the waited event has not been overwritten
      sl_wfx_host_setup_waited_event(confirmation_id);

      if (confirmation_id == host_context.posted_event_id) {
        // The waited frame has been received,
        // update structure and stop waiting

        host_context.posted_event_id = 0;
        if (event_payload_out != NULL) {
          *event_payload_out = sl_wfx_context->event_payload_buffer;
        }
        status = SL_STATUS_OK;
        break;

      }
    }
    // FIXME overlap after running almost 50days
  } while (sys_now() < tmo);

  return status;
}

void sl_wfx_process(void)
{
  // Make sure that frames have been received
  if ((wf200_interrupt_event == 1)
      || ((control_register & SL_WFX_CONT_NEXT_LEN_MASK) != 0)) {

    // Reset the interrupt flag
    wf200_interrupt_event = 0;

    do
    {
      sl_wfx_receive_frame(&control_register);
    }while ( (control_register & SL_WFX_CONT_NEXT_LEN_MASK) != 0 );

#ifdef SL_WFX_USE_SDIO
    sl_wfx_host_enable_platform_interrupt();
#endif
  }
}

/**************************************************************************//**
 * Called when the driver needs to lock its access
 *
 * @returns Returns SL_STATUS_OK if successful, SL_STATUS_FAIL otherwise
 *****************************************************************************/
sl_status_t sl_wfx_host_lock(void)
{
  // Not used in bare metal context
  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Called when the driver needs to unlock its access
 *
 * @returns Returns SL_STATUS_OK if successful, SL_STATUS_FAIL otherwise
 *****************************************************************************/
sl_status_t sl_wfx_host_unlock(void)
{
  // Not used in bare metal context
  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Called when the driver needs to post an event
 *
 * @returns Returns SL_STATUS_OK if successful, SL_STATUS_FAIL otherwise
 *****************************************************************************/
sl_status_t sl_wfx_host_post_event(sl_wfx_generic_message_t *event_payload)
{
  switch (event_payload->header.id ) {
    /******** INDICATION ********/
    case SL_WFX_CONNECT_IND_ID:
    {
      sl_wfx_connect_ind_t* connect_indication = (sl_wfx_connect_ind_t*) event_payload;
      sl_wfx_connect_callback(connect_indication->body.mac, connect_indication->body.status);
      break;
    }
    case SL_WFX_DISCONNECT_IND_ID:
    {
      sl_wfx_disconnect_ind_t* disconnect_indication = (sl_wfx_disconnect_ind_t*) event_payload;
      sl_wfx_disconnect_callback(disconnect_indication->body.mac, disconnect_indication->body.reason);
      break;
    }
    case SL_WFX_START_AP_IND_ID:
    {
      sl_wfx_start_ap_ind_t* start_ap_indication = (sl_wfx_start_ap_ind_t*) event_payload;
      sl_wfx_start_ap_callback(start_ap_indication->body.status);
      break;
    }
    case SL_WFX_STOP_AP_IND_ID:
    {
      sl_wfx_stop_ap_callback();
      break;
    }
    case SL_WFX_RECEIVED_IND_ID:
    {
      sl_wfx_received_ind_t* ethernet_frame = (sl_wfx_received_ind_t*) event_payload;
      if ( ethernet_frame->body.frame_type == 0 ) {
        sl_wfx_host_received_frame_callback(ethernet_frame);
      }
      break;
    }
    case SL_WFX_SCAN_RESULT_IND_ID:
    {
      sl_wfx_scan_result_ind_t* scan_result = (sl_wfx_scan_result_ind_t*) event_payload;
      sl_wfx_scan_result_callback(&scan_result->body);
      break;
    }
    case SL_WFX_SCAN_COMPLETE_IND_ID:
    {
      sl_wfx_scan_complete_ind_t* scan_complete = (sl_wfx_scan_complete_ind_t*) event_payload;
      sl_wfx_scan_complete_callback(scan_complete->body.status);
      break;
    }
    case SL_WFX_AP_CLIENT_CONNECTED_IND_ID:
    {
      sl_wfx_ap_client_connected_ind_t* client_connected_indication = (sl_wfx_ap_client_connected_ind_t*) event_payload;
      sl_wfx_client_connected_callback(client_connected_indication->body.mac);
      break;
    }
    case SL_WFX_AP_CLIENT_REJECTED_IND_ID:
    {
      break;
    }
    case SL_WFX_AP_CLIENT_DISCONNECTED_IND_ID:
    {
      sl_wfx_ap_client_disconnected_ind_t* ap_client_disconnected_indication = (sl_wfx_ap_client_disconnected_ind_t*) event_payload;
      sl_wfx_ap_client_disconnected_callback(ap_client_disconnected_indication->body.reason, ap_client_disconnected_indication->body.mac);
      break;
    }
#ifdef SL_WFX_USE_SECURE_LINK
    case SL_WFX_SECURELINK_EXCHANGE_PUB_KEYS_IND_ID:
    {
      if(host_context.waited_event_id != SL_WFX_SECURELINK_EXCHANGE_PUB_KEYS_IND_ID) {
        memcpy((void*)&sl_wfx_context->secure_link_exchange_ind,(void*)event_payload, event_payload->header.length);
      }
      break;
    }
#endif
    case SL_WFX_GENERIC_IND_ID:
    {
      sl_wfx_generic_ind_t* generic_status = (sl_wfx_generic_ind_t*) event_payload;
      sl_wfx_generic_status_callback(generic_status);
      break;
    }
    case SL_WFX_EXCEPTION_IND_ID:
    {
      sl_wfx_exception_ind_t *firmware_exception = (sl_wfx_exception_ind_t*) event_payload;
      uint8_t *exception_tmp = (uint8_t *) firmware_exception;
      printf("firmware exception %lu\r\n", firmware_exception->body.reason);
      for (uint16_t i = 0; i < firmware_exception->header.length; i += 16) {
        printf("hif: %.8x:", i);
        for (uint8_t j = 0; (j < 16) && ((i + j) < firmware_exception->header.length); j ++) {
            printf(" %.2x", *exception_tmp);
            exception_tmp++;
        }
        printf("\r\n");
      }
      break;
    }
    case SL_WFX_ERROR_IND_ID:
    {
      sl_wfx_error_ind_t *firmware_error = (sl_wfx_error_ind_t*) event_payload;
      uint8_t *error_tmp = (uint8_t *) firmware_error;
      printf("firmware error %lu\r\n", firmware_error->body.type);
      for (uint16_t i = 0; i < firmware_error->header.length; i += 16) {
        printf("hif: %.8x:", i);
        for (uint8_t j = 0; (j < 16) && ((i + j) < firmware_error->header.length); j ++) {
            printf(" %.2x", *error_tmp);
            error_tmp++;
        }
        printf("\r\n");
      }
      break;
    }
    /******** CONFIRMATION ********/
    case SL_WFX_SEND_FRAME_CNF_ID:
    {
      break;
    }
  }

  if ( host_context.waited_event_id == event_payload->header.id ) {
    /* Post the event in the queue */
    memcpy(sl_wfx_context->event_payload_buffer,
           (void*) event_payload,
           event_payload->header.length);
    host_context.posted_event_id = event_payload->header.id;
  }

  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Called when the driver needs to transmit a frame
 *
 * @returns Returns SL_STATUS_OK if successful, SL_STATUS_FAIL otherwise
 *****************************************************************************/
sl_status_t sl_wfx_host_transmit_frame(void* frame, uint32_t frame_len)
{
  return sl_wfx_data_write(frame, frame_len);
}

/**************************************************************************//**
 * Callback for individual scan result
 *****************************************************************************/
void sl_wfx_scan_result_callback(sl_wfx_scan_result_ind_body_t* scan_result)
{
  scan_count++;
  printf(
    "# %2d %2d  %03d %02X:%02X:%02X:%02X:%02X:%02X  %s",
    scan_count,
    scan_result->channel,
    ((int16_t)(scan_result->rcpi - 220) / 2),
    scan_result->mac[0], scan_result->mac[1],
    scan_result->mac[2], scan_result->mac[3],
    scan_result->mac[4], scan_result->mac[5],
    scan_result->ssid_def.ssid);
  /*Report one AP information*/
  printf("\r\n");
  if (scan_count <= SL_WFX_MAX_SCAN_RESULTS) {
    scan_list[scan_count - 1].ssid_def = scan_result->ssid_def;
    scan_list[scan_count - 1].channel = scan_result->channel;
    scan_list[scan_count - 1].security_mode = scan_result->security_mode;
    scan_list[scan_count - 1].rcpi = scan_result->rcpi;
    memcpy(scan_list[scan_count - 1].mac, scan_result->mac, 6);
  }
}

/**************************************************************************//**
 * Callback for scan complete
 *****************************************************************************/
void sl_wfx_scan_complete_callback(uint32_t status)
{
  (void)(status);
  scan_count_web = scan_count;
  scan_count = 0;
}

/**************************************************************************//**
 * Callback when station connects
 *****************************************************************************/
void sl_wfx_connect_callback(uint8_t* mac, uint32_t status)
{
  switch(status){
    case WFM_STATUS_SUCCESS:
    {
      printf("Connected\r\n");
      sl_wfx_context->state |= SL_WFX_STA_INTERFACE_CONNECTED;
      lwip_set_sta_link_up();

#ifdef SLEEP_ENABLED
      if (!(wifi.state & SL_WFX_AP_INTERFACE_UP)) {
        // Enable the power save
        sl_wfx_set_power_mode(WFM_PM_MODE_PS, 0);
        sl_wfx_enable_device_power_save();
      }
#endif
      break;
    }
    case WFM_STATUS_NO_MATCHING_AP:
    {
      strcpy(event_log, "Connection failed, access point not found");
      printf("%s\r\n", event_log);
      break;
    }
    case WFM_STATUS_CONNECTION_ABORTED:
    {
      strcpy(event_log, "Connection aborted");
      printf("%s\r\n", event_log);
      break;
    }
    case WFM_STATUS_CONNECTION_TIMEOUT:
    {
      strcpy(event_log, "Connection timeout");
      printf("%s\r\n", event_log);
      break;
    }
    case WFM_STATUS_CONNECTION_REJECTED_BY_AP:
    {
      strcpy(event_log, "Connection rejected by the access point");
      printf("%s\r\n", event_log);
      break;
    }
    case WFM_STATUS_CONNECTION_AUTH_FAILURE:
    {
      strcpy(event_log, "Connection authentication failure");
      printf("%s\r\n", event_log);
      break;
    }
    default:
    {
      strcpy(event_log, "Connection attempt error");
      printf("%s\r\n", event_log);
    }
  }
}

/**************************************************************************//**
 * Callback for station disconnect
 *****************************************************************************/
void sl_wfx_disconnect_callback(uint8_t* mac, uint16_t reason)
{
  (void)(mac);
  printf("Disconnected %d\r\n", reason);
  sl_wfx_context->state &= ~SL_WFX_STA_INTERFACE_CONNECTED;
  lwip_set_sta_link_down();
}

/**************************************************************************//**
 * Callback for AP started
 *****************************************************************************/
void sl_wfx_start_ap_callback(uint32_t status)
{
  if (status == 0) {
    printf("AP started\r\n");
    printf("Join the AP with SSID: %s\r\n", softap_ssid);
    sl_wfx_context->state |= SL_WFX_AP_INTERFACE_UP;
    lwip_set_ap_link_up();

#ifdef SLEEP_ENABLED
    // Power save always disabled when SoftAP mode enabled
    sl_wfx_set_power_mode(WFM_PM_MODE_ACTIVE, 0);
    sl_wfx_disable_device_power_save();
#endif
  } else {
    printf("AP start failed\r\n");
    strcpy(event_log, "AP start failed");
  }
}

/**************************************************************************//**
 * Callback for AP stopped
 *****************************************************************************/
void sl_wfx_stop_ap_callback(void)
{
  dhcpserver_clear_stored_mac ();
  printf("SoftAP stopped\r\n");
  sl_wfx_context->state &= ~SL_WFX_AP_INTERFACE_UP;
  lwip_set_ap_link_down();

#ifdef SLEEP_ENABLED
  if (wifi.state & SL_WFX_STA_INTERFACE_CONNECTED) {
    // Enable the power save
    sl_wfx_set_power_mode(WFM_PM_MODE_PS, 0);
    sl_wfx_enable_device_power_save();
  }
#endif
}

/**************************************************************************//**
 * Callback for client connect to AP
 *****************************************************************************/
void sl_wfx_client_connected_callback(uint8_t* mac)
{
  printf("Client connected, MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",
         mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  printf("Open a web browser and go to http://%d.%d.%d.%d\r\n",
         ap_ip_addr0, ap_ip_addr1, ap_ip_addr2, ap_ip_addr3);
}

/**************************************************************************//**
 * Callback for client rejected from AP
 *****************************************************************************/
void sl_wfx_ap_client_rejected_callback(uint32_t status, uint8_t* mac)
{
  struct eth_addr mac_addr;
  memcpy(&mac_addr, mac, SL_WFX_BSSID_SIZE);
  dhcpserver_remove_mac(&mac_addr);
  printf("Client rejected, reason: %d, MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",
         (int)status, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/**************************************************************************//**
 * Callback for AP client disconnect
 *****************************************************************************/
void sl_wfx_ap_client_disconnected_callback(uint32_t status, uint8_t* mac)
{
  struct eth_addr mac_addr;
  memcpy(&mac_addr, mac, SL_WFX_BSSID_SIZE);
  dhcpserver_remove_mac(&mac_addr);
  printf("Client disconnected, reason: %d, MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",
         (int)status, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/**************************************************************************//**
 * Callback for generic status received
 *****************************************************************************/
void sl_wfx_generic_status_callback(sl_wfx_generic_ind_t* frame)
{
  (void)(frame);
  printf("Generic status received\r\n");
}

/**************************************************************************//**
 * Called when the driver is considering putting the WFx in
 * sleep mode
 * @returns SL_WIFI_SLEEP_GRANTED to let the WFx go to sleep,
 * SL_WIFI_SLEEP_NOT_GRANTED otherwise
 *****************************************************************************/
sl_status_t sl_wfx_host_sleep_grant(sl_wfx_host_bus_transfer_type_t type,
                                    sl_wfx_register_address_t address,
                                    uint32_t length)
{
  (void)(type);
  (void)(address);
  (void)(length);

  return SL_STATUS_WIFI_SLEEP_GRANTED;
}

#if SL_WFX_DEBUG_MASK
void sl_wfx_host_log(const char *string, ...)
{
  va_list valist;

  va_start(valist, string);
  vprintf(string, valist);
  va_end(valist);
}
#endif
