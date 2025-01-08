/**
 * @file     rsi_mdnsd_records.h
 * @version  1.0
 * @date     26 Oct 2015
 *
 * Copyright(C) Redpine Signals 2014
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @section Description
 * This file contains the definitions of data structures & APIs used to 
 * create, update, set value and search the text record.
 *
 */


/**
 * Includes
 */
#ifndef RSI_MDNSD_RECORDS_H
#define RSI_MDNSD_RECORDS_H



typedef struct rsi_mdns_txt_rec_g
{
    uint8_t  *buffer;       // Pointer to data
    uint16_t buflen;        // Length of buffer
    uint16_t datalen;       // Length currently in use
    uint16_t malloced;  	// Non-zero if buffer was allocated via malloc()
} rsi_mdns_txt_rec_t;


void rsi_mdns_txt_rec_create( rsi_mdns_txt_rec_t *txtRecord, uint16_t bufferLen, void *buffer);
int8_t rsi_mdns_txt_rec_setvalue( rsi_mdns_txt_rec_t *txtRecord, const char *key, uint8_t valueSize, const void *value);
uint8_t *rsi_mdns_txt_rec_search(uint16_t txtLen, const void *txtRecord, const char *key, uint32_t *keylen);
int8_t  rsi_mdns_txt_rec_removevalue(rsi_mdns_txt_rec_t *txtRecord, const char *key);
const void * rsi_mdns_txt_get_bytes_ptr(rsi_mdns_txt_rec_t *txtRecord);

#endif
