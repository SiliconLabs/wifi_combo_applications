/*
 * common.h
 */

#ifndef FIRMWARE_UPDATE_COMMON_H_
#define FIRMWARE_UPDATE_COMMON_H_

//#define SECURE_UPDATE 1

#define BURN_NWP_FW       'B'
#define BURN_M4_FW        '4'
#define LOAD_M4_FW        '3'
#define SI91X_CHUNK_SIZE  4096

/* Select the macro for the respective firmware upgrade */

//#define TEST_TA_NONSECURE_FW
//#define TEST_TA_SECURE_FW
#define TEST_M4_NONSECURE_FW
//#define TEST_M4_SECURE_FW

#ifdef TEST_TA_NONSECURE_FW
  #define SIZE_OF_FW 0x19A400
#endif
#ifdef TEST_TA_SECURE_FW
  #define SIZE_OF_FW 0x19A430
#endif
#ifdef TEST_M4_NONSECURE_FW
  #define SIZE_OF_FW 0x9B840
#endif
#ifdef TEST_M4_SECURE_FW
  #define SIZE_OF_FW 0x19B10
#endif

#endif /* FIRMWARE_UPDATE_COMMON_H_ */
