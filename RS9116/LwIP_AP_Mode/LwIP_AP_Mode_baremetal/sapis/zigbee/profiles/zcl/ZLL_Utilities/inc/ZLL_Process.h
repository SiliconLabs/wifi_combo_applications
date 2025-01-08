/******************************************************************************
* ZLL_PROCESS_H
*******************************************************************************
* Program Description:
*
*
******************************************************************************/

#ifndef ZLL_PROCESS_H
#define ZLL_PROCESS_H

/*****************************************************************************
* Includes
*****************************************************************************/

#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"



/*****************************************************************************
* Global Constants
*****************************************************************************/

/*****************************************************************************
* Public Functions Prototype
*****************************************************************************/

uint8_t  ZLL_Process_ScanCommands( uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd) ;
uint8_t  ZLL_Process_DeviceInformationCommands( uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd );
uint8_t  ZLL_Process_IdentifyRequestCommands(  uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd );
uint8_t  ZLL_Process_NetworkStartCommands( uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd );
uint8_t  ZLL_Process_NetworkJoinCommands( uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd );
uint8_t  ZLL_Process_NetworkUpdateCommands(uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd );
uint8_t  ZLL_Process_NetworkJoinCommands(uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd );
uint8_t  ZLL_Process_NetworkUpdateCommands(uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd );
void ZLL_Update_Scan_ConfirmationDetails( ZigBeeNetworkDetails networkInformation,
                                        uint8_t scanStatus );
void ZLL_StartUpParameters(void);
void  ZLL_Process_ResetToFactory( uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd );
/*****************************************************************************
* Private Functions Prototype
*****************************************************************************/

/* None */

/*****************************************************************************
* Interrupt Service Routines
*****************************************************************************/

/* None */



#endif /*ZLL_PROCESS_H*/
/*****************************************************************************
* End Of File                                       ZLL_Process.h
*****************************************************************************/