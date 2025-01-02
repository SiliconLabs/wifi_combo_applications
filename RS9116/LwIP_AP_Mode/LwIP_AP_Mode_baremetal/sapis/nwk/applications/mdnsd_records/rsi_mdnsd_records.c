/**
 * @file     rsi_msdnsd_records.c
 * @version  1.0
 * @date     26 Oct 2015 
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief :
 *
 *  @section Description  This file contains 
 *
 *
 */
#include <rsi_data_types.h>
#include <rsi_common_apis.h>
#include "rsi_mdnsd_records.h"

/*==============================================*/
/**
 * @fn         void rsi_mdns_txt_rec_create( rsi_mdns_txt_rec_t *txtRecord, uint16_t bufferLen, void *buffer)
 * @brief      This API is used to create mdnsd text record
 * @param[in]  txtRecord - Pointer to text record
 * @param[in]  bufferLen - Length of buffer which is given by buffer
 * @param[in]  buffer 	 - Pointer to buffer, which is to be used to for text record
 * @param[out] None
 * @return     None
 * @section description
 * This API is used to deinit the MDNSD service
 *
 */
void rsi_mdns_txt_rec_create( rsi_mdns_txt_rec_t *txtRecord, uint16_t bufferLen, void *buffer)
{
    rsi_mdns_txt_rec_t* txtRec = txtRecord;

    txtRec->buffer   = buffer;
    txtRec->buflen   = buffer ? bufferLen : (uint16_t)0;
    txtRec->datalen  = 0;
    txtRec->malloced = 0;
}

/*==============================================*/
/**
 * @fn         int8_t rsi_mdns_txt_rec_setvalue( rsi_mdns_txt_rec_t *txtRecord, const char *key, uint8_t valueSize, const void *value)
 * @brief      Function to set value in MDSN text record
 * @param[in]  txtRecord - Pointer to text record
 * @param[in]  key 		 - Pointer to Key
 * @param[in]  valuesize - Size of value
 * @param[in]  value	 - Pointer to value
 * @param[out] None
 * @return     Non Zero - Error
 * 			   Zero     - True
 * @section description
 * This API is used to deinit the MDNSD service
 *
 */
int8_t rsi_mdns_txt_rec_setvalue( rsi_mdns_txt_rec_t *txtRecord, const char *key, uint8_t valueSize, const void *value)
{
    uint8_t *start, *p;
    const char *k;
    uint32_t keysize, keyvalsize;
    rsi_mdns_txt_rec_t* txtRec = txtRecord;

    for (k = key; *k; k++) if (*k < 0x20 || *k > 0x7E || *k == '=') return(RSI_FAILURE);

    keysize = (uint32_t)(k - key);

    keyvalsize = 1 + keysize + (value ? (1 + valueSize) : 0);

    if (keysize < 1 || keyvalsize > 255) return (RSI_FAILURE);
    rsi_mdns_txt_rec_removevalue(txtRecord, key);

    start = txtRec->buffer + txtRec->datalen;
    p = start + 1;
    memcpy(p, key, keysize);
    p += keysize;

    if (value)
    {
        *p++ = '=';
        memcpy(p, value, valueSize);
        p += valueSize;
    }

    *start = (uint8_t)(p - start - 1);
    txtRec->datalen += p - start;

    return(0);
}


/*==============================================*/
/**
 * @fn               uint8_t *rsi_mdns_txt_rec_search(uint16_t txtLen, const void *txtRecord, const char *key, uint32_t *keylen)
 * @brief            This function is used to search in current mdnsd text record
 * @param[in]        Text length
 * @param[in]        Pointer to Text Record
 * @param[in]        pointer to Key
 * @param[in]        pointer to Key Length
 * @return     uint8_t
 *             0  =  success
 *             <0 = failure
 * @section description
 * This function is used to search in current mdnsd text record
 */
uint8_t *rsi_mdns_txt_rec_search(uint16_t txtLen, const void *txtRecord, const char *key, uint32_t *keylen)
{
    uint8_t *p = (uint8_t*)txtRecord;
    uint8_t *e = p + txtLen;
    *keylen = (uint32_t) strlen(key);

    while (p<e)
    {
        uint8_t *x = p;
        p += 1 + p[0];
        if (p <= e && *keylen <= x[0] && !strncasecmp(key, (char*)x+1, *keylen))
            if (*keylen == x[0] || x[1+*keylen] == '=') return(x);
    }

    return(NULL);
}


/*==============================================*/
/**
 * @fn               int8_t  rsi_mdns_txt_rec_removevalue(rsi_mdns_txt_rec_t *txtRecord, const char *key)
 * @brief            This function is used to remove a value from MDNSD text record
 * @param[in]        Pointer to Text Record
 * @param[in]        Pointer to Key
 * @return     int8_t
 *             0  =  success
 *             <0 = failure
 * @section description
 * This function is used to remove a value from MDNSD text record
 *
 */
int8_t  rsi_mdns_txt_rec_removevalue(rsi_mdns_txt_rec_t *txtRecord, const char *key)
{
	uint32_t keylen, itemlen, remainder;
	rsi_mdns_txt_rec_t* txtRec = txtRecord;

	uint8_t *item = rsi_mdns_txt_rec_search(txtRec->datalen, txtRec->buffer, key, &keylen);

	if (!item) return(RSI_FAILURE);

	itemlen   = (uint32_t)(1 + item[0]);
	remainder = (uint32_t)((txtRec->buffer + txtRec->datalen) - (item + itemlen));

	// Use memmove because memcpy behavior is undefined for overlapping regions
	memmove(item, item + itemlen, remainder);
	txtRec->datalen -= itemlen;

	return(0);
}

//! Function to return MDNS text record buffer pointer
/*==============================================*/
/**
 * @fn               const void * rsi_mdns_get_txt_rec_buffer(const rsi_mdns_txt_rec_t *txtRecord)
 * @brief            This function is used to return MDNS text record buffer pointer
 * @param[in]        Pointer to text record
 * @return     const void * - Pointer to text record Buffer
 * @section description
 * This function is used to return MDNS text record buffer pointer
 */
const void * rsi_mdns_get_txt_rec_buffer(rsi_mdns_txt_rec_t *txtRecord)
{
    rsi_mdns_txt_rec_t* txtRec = txtRecord;

	txtRec->buffer[txtRec->buflen] = '\0';

  return(txtRec->buffer);
}

