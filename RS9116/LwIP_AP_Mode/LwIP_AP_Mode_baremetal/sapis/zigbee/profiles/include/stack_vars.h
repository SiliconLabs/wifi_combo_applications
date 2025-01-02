/*******************************************************************************
 * stack_vars.h
 ********************************************************************************
 * Program Description:
 *
 *     This file contains the interface functions and data structures to be used
 *     by the NHLE for accessing management services.
 *
 *******************************************************************************/

#ifndef STACK_VARS_H
#define STACK_VARS_H

/*******************************************************************************
 * Includes
 *******************************************************************************/



/*******************************************************************************
 * Global Constants
 *******************************************************************************/


/*******************************************************************************
 * Type Definitions
 *******************************************************************************/



/*******************************************************************************
 * Memory Declarations
 *******************************************************************************/




/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/


/*******************************************************************************
 * InitStackTablesPointer
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   tablesPointer   			-  Pointer to the stack table
 *   totalsize       - 				total size

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application configuration file
 *
 * Purpose of the function:
 *       This function is called from application to initialize the stack table
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/
void InitStackTablesPointer( uint8_t* tablesPointer, uint16_t totalsize );


/*******************************************************************************
 * InitgpNeighborTable
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   maxNBEntries   			- max neighbor table entries
 *   neighborTableOffset       - neighbor table offset

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application configuration file
 *
 * Purpose of the function:
 *       This function is called from application to configure neighbor table
 *   	entries
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/

void InitgpNeighborTable( uint8_t maxNBEntries, uint32_t neighborTableOffset );

/*******************************************************************************
 * InitgpRouteTable
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   persistentNIBOffset   			- max route table entries
 *   neighborTableOffset       - route table offset

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application configuration file
 *
 * Purpose of the function:
 *       This function is called from application to configure persistant NB
 *   	entries
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/

#if( g_NWK_MESH_ROUTING_d == RSI_ENABLE )
void InitgpRouteTable( uint8_t maxRTEntries, uint32_t routetableOffset );
#endif

/*******************************************************************************
 * InitgpSecurityMaterialDesc
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   maxKeyEntries   			- max route table entries
 *   secMtTableOffset       - route table offset

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application configuration file
 *
 * Purpose of the function:
 *       This function is called from application to configure persistant NB
 *   	entries
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/

void InitgpSecurityMaterialDesc( uint8_t maxKeyEntries,
                               						uint32_t secMtTableOffset );

/*******************************************************************************
 * InitgpPersistentNIB
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   persistentNIBOffset   			- offset for persistent table offset

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application configuration file
 *
 * Purpose of the function:
 *       This function is called from application to configure persistant NB
 *   	entries
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/

void InitgpPersistentNIB( uint32_t persistentNIBOffset );

/*******************************************************************************
 * InitgpZDOInfoBase
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   ZDOInfoBaseOffset   			- offset for zdo info base table

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application configuration file
 *
 * Purpose of the function:
 *       This function is called from application to configure ZDO Info base
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/

void InitgpZDOInfoBase( uint32_t ZDOInfoBaseOffset );

/*******************************************************************************
 * InitgpRouteRecordTable
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   maxRouteRecordEntries   			- max route record table entries
 *   routeRecordTableOffset       -  route record table offset

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application configuration file
 *
 * Purpose of the function:
 *       This function is called from application to configure route record table
 *   	entries
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/

#if ( g_NWK_MANY_TO_ONE_ROUTING_d == RSI_ENABLE )
#if ( g_NWK_ROUTE_RECORD_TABLE_d == RSI_ENABLE )
void InitgpRouteRecordTable( uint8_t maxRouteRecordEntries,
											uint32_t routeRecordTableOffset );
#endif //g_NWK_ROUTE_RECORD_TABLE_d
#endif

/*******************************************************************************
 * InitgpAddressMapTable
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   maxAddrMapEntries   			- max adress map table entries
 *   addressmapTableOffset       -  adress map table offset

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application configuration file
 *
 * Purpose of the function:
 *       This function is called from application to configure address map table
 *   	entries
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/

void InitgpAddressMapTable( uint8_t maxAddrMapEntries,
						   					uint32_t addressmapTableOffset );

/*******************************************************************************
 * InitgpBindingTable
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   maxBindEntries   			- max bind table entries
 *   bindingTableOffset       - binding  table offset

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application configuration file
 *
 * Purpose of the function:
 *       This function is called from application to configure binding table
 *   	entries
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/

//#if ( g_APS_BINDING_CAPACITY_c == RSI_ENABLE )
void InitgpBindingTable( uint8_t maxBindEntries,uint32_t bindingTableOffset );
//#endif

/*******************************************************************************
 * InitgpGroupTable
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   maxGroupEntries   			- max group table entries
 *   groupTableOffset       - group table offset

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application configuration file
 *
 * Purpose of the function:
 *       This function is called from application to configure persistant NB
 *   	entries
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/

#if ( g_APS_GROUP_DATA_RX_d == 1 )
void InitgpGroupTable( uint8_t maxGroupEntries,  uint32_t groupTableOffset );
#endif


#if ( g_APS_LAYER_SEC_PROCESSING_d == RSI_ENABLE )

/*******************************************************************************
 * InitgpLinkKeyTable
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   maxLinkKeyEntries   			- max link key table entries
 *   linkKeyTableOffset       - link key table offset

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application configuration file
 *
 * Purpose of the function:
 *       This function is called from application to configure link key table
 *   	entries
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/
void InitgpLinkKeyTable( uint8_t maxLinkKeyEntries,  uint32_t linkKeyTableOffset );

/*******************************************************************************
 * InitgpLinkKeyFCtable
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   maxLinkKeyEntries   			- max link key table entries
 *   linkKeyFCTableOffset       - link key table offset

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application configuration file
 *
 * Purpose of the function:
 *       This function is called from application to configure link key frame
 *   	counter entries
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/
void InitgpLinkKeyFCtable( uint8_t maxLinkKeyEntries,  uint32_t linkKeyFCTableOffset );

#if ( ( g_TRUST_CENTRE_d == RSI_ENABLE ) && ( g_USE_HASH_KEY == RSI_DISABLE ) )

/*******************************************************************************
 * InitgpAppLinkKeyTable
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   maxAppLinkKeyEntries   			- max app link key table entries
 *   AppLinkKeyTableOffset       - link key table offset

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application configuration file
 *
 * Purpose of the function:
 *       This function is called from application to configure link key table
 *   	entries
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/
void InitgpAppLinkKeyTable( uint8_t maxAppLinkKeyEntries,
						   			uint32_t AppLinkKeyTableOffset );
#endif    /*( g_TRUST_CENTRE_d == RSI_ENABLE ) && ( g_USE_HASH_KEY == RSI_DISABLE ) */
#endif    /* g_APS_GROUP_DATA_RX_d*/


/*******************************************************************************
 * InitMaxNumberOfEndpoints
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   maxEnpoints   			- max number of endpointss

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application configuration file
 *
 * Purpose of the function:
 *       This function is called from application to configure link key table
 *   	entries
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/
void InitMaxNumberOfEndpoints( uint8_t maxEnpoints);


/*******************************************************************************
 * InitializeStartNVMLocation
 ********************************************************************************
 *
 * Return Value : None
 * Input Parameters:
 *   maxEnpoints   			- start NVM location

 * Output Parameters:
 *       NONE
 *
 * Scope of the function:
 *       This function is called by the application to initialize NVM location
 *
 * Purpose of the function:
 *       This function is called from application to initialize the NVM location
 *
 * Before the function is called:
 * None
 *
 * After the function is called:
 * None
 *******************************************************************************/
void InitializeStartNVMLocation( uint32_t NVM_Tables_StartLocation, 
    uint32_t NVM_SAS_StartLocation );


/*******************************************************************************
 * End Of File
 *******************************************************************************/

#endif                                                      /*STACK_VARS_H*/
