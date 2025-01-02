/*******************************************************************************
 * apsme_interface.h
 ********************************************************************************
 * Program Description:
 *    This file consists of structures and API's for Management primitives
 *   of APS Layer
 *
 *******************************************************************************/
#ifndef APSME_INTERFACE_H
#define APSME_INTERFACE_H

/*******************************************************************************
 * Includes
 *******************************************************************************/

/*None */

/*******************************************************************************
 * Global Constants
 *******************************************************************************/
#define m_NO_ENTRY_c                          0xFF
#define m_SUCCESS_c                           0x00

/*Following is the enumeration for the different status values that can be
 Received from APSME*/
enum APSME_Status_Values {
    g_Apsme_Success_c = 0x00,
    g_Apsme_Invalid_Group_c = 0xa5,
    g_Apsme_Invalid_Parameter_c = 0xa6,
    g_Apsme_Table_Full_c = 0xae,
    g_Apsme_Unsupported_Attribute_c = 0xb0
};

enum Security_Modes {
    m_CBC_Mode_c = 0, m_CTR_Mode_c = 1, m_ECB_Mode_c = 2
};

/*******************************************************************************
 * Memory Declarations
 *******************************************************************************/
/* Maximum number of entries in the addr map table. It is made configurable */
extern uint8_t g_Num_Of_Addr_Map_Entries_c;

/*Num_Of_Binding_Table_Entries - specifies the maximum number of binding table entries
 supported for this device*/
extern uint8_t g_Num_Of_Binding_Table_Entries_c;

/*Maximum number of group table entries. It is made configurable*/
extern uint8_t g_Num_Of_Group_Table_Entries_c;

/* Maximum number of entries in the Link Key table. It is made configurable */
extern uint8_t g_Num_Of_Link_Key_Entries_c;

/*This variable holds the current working state of APS*/
extern uint8_t APS_Current_State;

/* The AIB attribute holding the trust center address */
extern uint8_t ga_APS_AIB_Trust_Centre_Address[];

//#if ( g_APS_BINDING_CAPACITY_c == 1 )
#if ( g_BIND_TABLE_NVM_STORAGE_d == RSI_DISABLE )
extern Source_Binding_Table_t *gp_Binding_Table;
#endif /*g_BIND_TABLE_NVM_STORAGE_d*/
//#endif /*#if ( g_APS_BINDING_CAPACITY_c == 1 )*/

#if ( g_GROUP_TABLE_NVM_STORAGE_d == RSI_DISABLE )
#if ( g_ZIGBEE_COORDINATOR_d == 1 ) || ( g_ZIGBEE_ROUTER_d == 1 ) || \
                                        ( g_ZIGBEE_ACTIVE_END_DEVICE_d == 1 )

/* Enable the group table only for Active devices */
/* Holds the group table contents */
extern Group_Table_t *gp_Group_Table;

#endif                                 /* ( g_ZIGBEE_END_DEVICE_d == 1 )
                                          && (g_ZIGBEE_ACTIVE_END_DEVICE_d==1) */
#endif /*( g_GROUP_TABLE_NVM_STORAGE_d == RSI_DISABLE )*/

/*******************************************************************************
 * Type Definitions
 *******************************************************************************/
typedef struct APS_Info_Base_Tag {
    uint8_t aps_designated_coordinator;
    uint8_t aps_channel_mask[4];
    uint8_t aps_use_extended_pan_id[8];
    uint8_t aps_non_member_radius;
    uint8_t aps_use_insecured_join;
    uint8_t aps_permission_configuration;
    uint16_t aps_last_channel_energy;
    uint16_t aps_last_channel_failure_rate;
    uint16_t aps_last_channel_timer;
} __attribute__((__packed__))APS_Info_Base_t;

/*******************************************************************************
 APSME_Update_Device_Indication_Tag
 This primitive is used to indicate the NHLE that an update device command is
 received.
 Field: device_short_address - 16 bit network address - that indicates the
 network address   of the device whose status is being updated.
 Field: a_device_address - 64 bit address whose status needs to be updated with
 the Trust Center
 Field: status - gives the status of the device
 0x00 - device secured join
 0x01 - device unsecured join
 0x02 - device left
 0x03 to
 0x08  - reserved
 Field: a_src_address - 64 bit address - gives the source address from where the
 update device command originated
 Field:  parent_src_address - network address of device from whom update device
 Command
 is received
 *******************************************************************************/
typedef struct APSME_Update_Device_Indication_Tag {
    uint16_t device_short_address;
    uint8_t a_device_address[8];
    uint8_t status;
    uint8_t a_src_address[8];
    uint16_t parent_src_address;
} __attribute__((__packed__))APSME_Update_Device_Indication_t;

/*******************************************************************************
 APSME_Remove_Device_Indication_Tag
 This primitive is used to indicate the ZDO that the referenced by the
 src address device is requesting the referenced child address to be removed
 from the Nwk.
 Field: a_src_address - gives the 64 bit address who initiated the remove device
 Request command
 Field: a_target_address - gives the 64 bit address of the child who should be
 removed from the Nwk
 *******************************************************************************/
typedef struct APSME_Remove_Device_Indication_Tag {
    uint16_t src_short_address[4];
    uint8_t a_target_address[8];
} __attribute__((__packed__))APSME_Remove_Device_Indication_t;

/*******************************************************************************
 APSME_Switch_Key_Indication_Tag
 This Primitive is used to inform the ZDO of the destined device that it
 received a switch key request command
 Field: a_src_address - 64 bit extended address to who initiated the switch key
 command.
 Field: key_seq_number   - a byte value sequence number assigned by the Trust
 center to differentiate the keys.
 *******************************************************************************/
typedef struct APSME_Switch_Key_Indication_Tag {
    uint16_t src_short_address[4];
    uint8_t key_seq_number;
} __attribute__((__packed__))APSME_Switch_Key_Indication_t;

/*******************************************************************************
 APSMELinkKeyIndication_Tag
 This Primitive is used to inform the ZDO of the destined device that it
 received a link key indication
 Field: a_src_address - 64 bit extended address to who initiated the switch key
 command.
 Field: key_seq_number   - a byte value sequence number assigned by the Trust
 center to differentiate the keys.
 *******************************************************************************/
typedef struct APSMELinkKeyIndication_Tag {
    uint16_t SourceAddress[4];
    uint8_t KeyType;
    uint8_t PartnerAddress[8];
} __attribute__((__packed__))APSMELinkKeyIndication_t;

/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/
/*******************************************************************************
 APS_changeState
 *   Return Value:
 *       Success
 *
 *  Parameters:
 *
 *  Input parameter:
 *       state - gives the state to which the APS state should be changed.
 *  Output parameter:
 *       None
 *
 * Scope of the function:
 * 	 This API is called when the change of APS state is required.Once the
 *   network is formed or joined successfully, this API should be called
 *   to change the APS state to ready state. This is also invoked when
 *   the APS should be reset to initial state. This API is used within the
 *   stack and not exposed to the Application.
 *
 *
 *  Purpose of the function:
 *   This function changes the current state to the required state mentioned
 *   in the input parameter. There is a global variable maintained for
 *   APS state - APS_Current_State. This state is updated to the mentioned
 *   state.
 *
 *
 *  Before the function is called:
 *   None
 *
 *  After the function is called:
 *     None
 *
 *******************************************************************************/
void APS_changeState(uint8_t state);

/*******************************************************************************
 TBD
 *   This function is issued by the ZDO module to transport key to another
 *   device. This request is issued when the (trust center)device knows that
 *   some chile device is joined successfully and the key needs to be
 *   transported.Also when an update device indication is received, this request
 *   is issued to transport the  key.
 *   Return Value:
 *       None
 *
 *   Parameters:
 *   Input parameter:
 *       APSME_Transport_Key_Request
 *   Output parameter:
 *       None
 *
 *   Scope of the function:
 *   This function is invoked by the ZDO module to transport the key
 *   ( Trust center master key or network key or link key or the Application
 *   master key) to another device.
 *
 *   Purpose of the function:
 *   This function uses the input parameter that holds the type of key and
 *   the key data along with the device address to whom the key is to be
 *   transmitted.Using the above set of inputs, this function creates the
 *   transport key request command and puts it into NLME queue for
 *   further processing.
 *
 *   Before the function is called:
 *   p_Transport_Key_Request should be allocated
 *
 *   After the function is called:
 *   p_Transport_Key_Request should be de-allocated
 *
 *******************************************************************************/
void APSME_handleTransportKeyRequest(Transport_Key_Request_t *pKeyData,
        uint16_t destAddress);

/*******************************************************************************
 ZDO_SEC_Handle_Transport_NWKKey_Indication
 *   This primitive is issued to the ZDO of the device that it received a
 *   transport key command.
 *   Return Value:
 *       None
 *
 * Parameters:
 * Input parameter:
 *
 * src_short_address - the address of device from whom transport
 *   key command is received.
 *
 *   p_Transport_Key_Indication - holds the parameter from whom the
 *   transport key initiated along with the type of key transferred.
 * Output parameter:
 *       None
 *
 * Scope of the function:
 *   This function is invoked by the APSME to the ZDO module to inform ZDO.
 *
 *  Purpose of the function:
 *   Here, the ZDO module is informed about the transfer of key from
 * the Trust center(device) and the type of the key
 *
 *  Before the function is called:
 *  p_Transport_Key_Indication should be allocated
 *
 *  After the function is called:
 *  p_Transport_Key_Indication should be de-allocated
 *
 *******************************************************************************/
void ZDO_SEC_Handle_Transport_NWKKey_Indication(uint16_t src_short_address,
        Transport_Key_Indication_t* p_Transport_Key_Indication);

/*******************************************************************************
 APSME_Update_Device_Request
 *   This primitive is issued by a device(ZR)to inform the device(TC) that
 *   a second device(say ZED) is joined to the network or left the network based
 *   on the status of the device.
 *   Return Value:
 *       None
 *
 *  Parameters:
 *  Input parameter:
 *       p_Update_Device_Key_Request  - holds the address info of the device
 *   which joined the network or left the network. This information is mentioned
 *   in the status parameter of the structure p_Update_Device_Key_Request
 *  Output parameter:
 *       None
 *
 * Scope of the function:
 *   This function is invoked when the second device joins the device and the
 *   Trust center needs to be updated with the status of the second device.
 *
 *  Purpose of the function:
 *    This function creates the update device key request command. This uses
 *   the address information and status of the device in the input parameter
 *   and places it in the NLME queue for further processing.
 *
 *  Before the function is called:
 *   p_Update_Device_Key_Request should be allocated.
 *
 *  After the function is called:
 *   p_Update_Device_Key_Request should be de-allocated.
 *
 *******************************************************************************/
void APSME_handleUpdateDeviceRequest(uint8_t* device_address, uint8_t status,
        uint16_t device_short_address, uint16_t dest_short_address);

/*******************************************************************************
 APS_SEChandleUpdateDeviceIndication
 *   This primitive is used to inform the ZDO module of the Trust center that a
 *   status of the device needs to be updated.
 *   Return Value:
 *       None
 *
 *   Parameters:
 *   Input parameter:
 *       p_Update_Device_Key_Indication
 *   Output parameter:
 *       None
 *
 *   Scope of the function:
 *   This function is just to inform the ZDO module of the Trust center that the
 *   status of the device needs to be updated.
 *
 *   Purpose of the function:
 *   This function informs the ZDO of the Trust center that a device with the
 *   src address sent an update device command along with the status of the
 *   device. The status gives information if the device joined the network or
 *   left the network.
 *
 *   Before the function is called:
 *   p_Update_Device_Key_Indication should be allocated
 *
 *   After the function is called:
 *   p_Update_Device_Key_Indication should be de-allocated
 *
 *******************************************************************************/
void APS_SEChandleUpdateDeviceIndication(
        APSME_Update_Device_Indication_t* p_Update_Device_Key_Indication);

/*******************************************************************************
 APSME_Remove_Device_Request
 *   This primitive is issued by the ZDO module when the device(Trust center)
 *   wishes to remove a particular device from the network.since it couldn't
 *   authenticate properly
 *   Return Value:
 *       None
 *
 *   Parameters:
 *   Input parameter:
 *       p_Remove_Device_Key_Request - is a pointer that holds the address info
 *       of the device that is to be removed from the network.
 *   Output parameter:
 *       None
 *
 *   Scope of the function:
 *       This function is issued only by the ZDO module of the trust center
 *   to remove a particular device from the network.
 *
 *   Purpose of the function:
 *   This function creates the remove device request command for a device
 *   which could not authenticate properly.This queues the message for the
 *   NLME to process it further.
 *
 *   Before the function is called:
 *   p_Remove_Device_Key_Request should be allocated
 *
 *  After the function is called:
 *   p_Remove_Device_Key_Request should be de- allocated
 *
 *******************************************************************************/
void APSME_SEChandleRemoveDeviceRequest(uint16_t parent_short_address,
        uint8_t* child_address);

/*******************************************************************************
 APS_SEChandleRemoveDeviceIndication
 *   This primitive is issued to inform the ZDO module of a device by the
 *   Trust center that it wants to remove a particular child device from the Nwk.
 *   Return Value:
 *       None
 *
 *  Parameters:
 *  Input parameter:
 *       p_Remove_Device_Key_Indication - holds the address that initiated the
 *       remove device command and the child device address who should be
 *       removed from the network.
 *   Output parameter:
 *   None
 *
 *   Scope of the function:
 *   This function is used only to inform the ZDO module to inform a child
 *   device should be removed from the network.
 *
 *   Purpose of the function:
 *   This function informs the ZDO module of the device to remove its child
 *   device as mentioned by the trust center because the child device could
 *   not authenticate properly.
 *
 *   Before the function is called:
 *
 *
 *   After the function is called:
 *
 *
 *******************************************************************************/
void APS_SEChandleRemoveDeviceIndication(
        APSME_Remove_Device_Indication_t* p_Remove_Device_Key_Indication);

/*******************************************************************************
 DOUBT
 APSME_Switch_Key_Request
 *   This primitive is issued by the ZDO module of the Trust center to inform
 *   other devices to change the network key.
 *   Return Value:
 *       None
 *
 *   Parameters:
 *   Input parameter:
 *       p_Switch_Key_Request - hold the key to which the devices need to change
 *   Output parameter:
 *       None
 *
 *   Scope of the function:
 *   This function is called only by the TC to change a particular device's
 *   network key.
 *
 *   Purpose of the function:
 *   This function creates the switch key request command to the destined
 *   address who needs to change the network key to a new key. This is then
 *   placed in the NLME queue for further processing.
 *
 *   Before the function is called:
 *   p_Switch_Key_Request should be allocated
 *
 *   After the function is called:
 *   p_Switch_Key_Request should be de-allocated
 *
 *******************************************************************************/
void APSME_SEChandleSwitchKeyRequest(uint8_t key_sequence_number,
        uint16_t dst_short_address);

/*******************************************************************************
 ZDO_SEChandleSwitchKeyIndication
 *   This function is issued to the ZDO module of the device on receipt of
 *   switch key request command.
 *   Return Value:
 *       None
 *
 *   Parameters:
 *   Input parameter:
 *       p_Switch_Key_Indication - is a pointer that holds the source address of
 *       the switch key command, the key sequence number that indicates to which
 *       network key the device should get updated.
 *   Output parameter:
 *       None
 *
 *   Scope of the function:
 *   This function is invoked by the APSME module on receipt of the switch key
 *   request command from the device(Trust center) to switch the key.
 *
 *   Purpose of the function:
 *   This function informs the ZDO, that a switch key request command is
 *   received,the key sequence number to which the network key needs to be
 *   updated. On receipt of this indication, ZDO module changes the current
 *   network key to as per the sequence number mentioned in the input parameter.
 *
 *   Before the function is called:
 *   p_Switch_Key_Indication pointer should be allocated
 *
 *   After the function is called:
 *   p_Switch_Key_Indication pointer should be de-allocated
 *
 *******************************************************************************/
void ZDO_SEChandleSwitchKeyIndication(
        APSME_Switch_Key_Indication_t* p_Switch_Key_Indication);

/*******************************************************************************
 ZDO_SEChandleRequestKeyIndication
 *   This function is issued to the ZDO module of the device on receipt of
 *   request key command.
 *   Return Value:
 *       None
 *
 *   Parameters:
 *   Input parameter:
 *       pLinkKeyIndication - is a pointer that holds the request key command payload.
 *   Output parameter:
 *       None
 *
 *   Scope of the function:
 *   This function is invoked by the APSME module on receipt of the request key
 *   command from the device to get the link key.
 *
 *   Purpose of the function:
 *   This function informs the ZDO, that a request key command is
 *   received, and appropriate key to be sent.
 *
 *   Before the function is called:
 *   none
 *
 *   After the function is called:
 *   none
 *
 *******************************************************************************/
void ZDO_SEChandleRequestKeyIndication(
        APSMELinkKeyIndication_t * pLinkKeyIndication);

/*******************************************************************************
 ZDO_SEC_Handle_LinkKey_Indication
 *   This function is issued to the ZDO module of the device on receipt of
 *   link key command.
 *   Return Value:
 *       None
 *
 *   Parameters:
 *   Input parameter:
 *       pIndication - is a pointer that holds the link key command payload.
 *       srcAddress - src address of the device for which link key is received.
 *   Output parameter:
 *       None
 *
 *   Scope of the function:
 *   This function is invoked by the APSME module on receipt of the link key
 *   command from the device and this key will be used for further communication.
 *
 *   Purpose of the function:
 *   This function informs the ZDO, that a link key command is
 *   received, and this key will be used for further communication.
 *
 *   Before the function is called:
 *   none
 *
 *   After the function is called:
 *   none
 *
 *******************************************************************************/
void ZDO_SEC_Handle_LinkKey_Indication(Transport_Key_Indication_t* pIndication,
        uint16_t srcAddress, uint8_t key_type);

/*******************************************************************************
 APSMEHandleRequestKeyRequest
 *   This function is called after the device joins and required to send link key
 *
 *   Return Value:
 *       None
 *
 *   Parameters:
 *   Input parameter:
 *       request key - is a pointer that holds the request key data.
 *       security -  type of security needs to be applied.
 *       dstShortAddress - To whom this command to be sent
 *   Output parameter:
 *       None
 *
 *   Scope of the function:
 *   This function is invoked by the APSME module
 *
 *
 *   Purpose of the function:
 *   This function informs the ZDO, that a link key command link key command needs to send
 *
 *   Before the function is called:
 *   none
 *
 *   After the function is called:
 *   none
 *
 *******************************************************************************/
void APSMEHandleRequestKeyRequest(requestKeyRequest_t * requestkey,
        uint8_t security, uint16_t dstShortAddress);

/*******************************************************************************
 ZDO_SEC_Send_Tunnel_Command_Request
 *   This function is called after the device joins  in Pre-config Link Key mode
 *
 *   Return Value:
 *       None
 *
 *   Parameters:
 *   Input parameter:
 *       UpdateDeviceKeyIndication - is a pointer that holds update device indication.
 *
 *   Output parameter:
 *       None
 *
 *   Scope of the function:
 *   This function is invoked by the ZDO module
 *
 *
 *   Purpose of the function:
 *   This function create tunnel command.
 *
 *   Before the function is called:
 *   none
 *
 *   After the function is called:
 *   none
 *
 *******************************************************************************/

void ZDO_SEC_Send_Tunnel_Command_Request(
        APSME_Update_Device_Indication_t*UpdateDeviceKeyIndication);

/*-----------------------------------------------------------------------------
 APSME_Bind_Unbind_Request
 *	This primitive is issued to bind /unbind two devices on a unique cluster id
 *  based on action_to_be_taken.
 *	Return Value:
 *		ILLEGAL_REQUEST - if the device is not yet joined to a network and if a
 *							bind request is issued.
 *		SUCCESS -		  if binding is performed successfully on the given
 *							cluster id.
 * 		ILLEGAL_DEVICE -  if the address is not present in the neighbor table
 *							or address map table
 *		TABLE_FULL - 	  If the binding table is full with maxm entries.
 *		NOT_SUPPORTED -   Binding/unbinding is an optional primitive, if source
 *                        binding not supported on a device, a status of
 *							NOT_SUPPORTED is issued in the confirmation.
 *
 *  Parameters:
 *  Input parameter:
 *		p_bind_request - is a pointer to a structure of type
 *		Binding_Table.This pointer holds the cluster id, dest addr info
 *		over which the binding should be performed.
 *  Output parameter:
 *		None
 *
 * Scope of the function:
 *	This function is called by two modules. One, the NHLE issues the APS layer
 *	to bind two devices. Two, ZDP issues this request to APS layer on receipt
 *	of end device bind request.
 *
 *  Purpose of the function:
 *	This function takes the src addr as the current network address of the
 *	device since source binding is performed and the dest address info is
 *	present in the i/p parameter. The two devices are bound by adding an
 *	entry into the binding table.This is as good as setting the Bind table
 *	Returns the status as explained in the field return value.
 *
 *  Before the function is called:
 *	p_APSME_Bind_Request pointer should be allocated.
 *
 *
 *  After the function is called:
 *   p_APSME_Bind_Request pointer should be Freed.
 *
 *----------------------------------------------------------------------------*/
//#if ( g_APS_BINDING_CAPACITY_c == 1 )
uint8_t APSME_Bind_Unbind_Request ( ZDP_Bind_Request_t *p_bind_request,
        uint8_t action_to_be_taken );
//#endif /*#if ( g_APS_BINDING_CAPACITY_c == 1 )*/

/*-----------------------------------------------------------------------------
 APSME_Get_Binding_Table_Entry
 *	This primitive is issued to the APS layer from the NHLE when it wishes to
 get a particular entry from the Binding table
 *	Return Value:
 *		gSuccess_c - gives when the index specified is found in the binding
 *			table and getting the entry is successful
 *		g_Invalid_Parameter_c - gives when the index is beyond max binding
 *			table entries.
 *  Parameters:
 *  Input parameter:
 *		bind_index - gives index in the binding table whose entry is to be
 *				retrieved
 *
 *  Output parameter:
 *		p_Binding_Table - gives the pointer that holds the binding table
 *			This pointer gets updated in this function call.
 *
 * Scope of the function:
 *   This function is invoked by two modules either by the NHLE or the ZDP
 *	module.
 *
 *  Purpose of the function:
 *	This function checks if the index is valid and within the max binding table
 *	entries.If yes, it reads the binding entry corresponding to that index and
 *	the output pointer p_Binding_Table is made to hold this binding table entry
 *	returns success.If the index is invalid, returns invalid index.
 *
 *  Before the function is called:
 *    p_Binding_Table pointer should be allocated
 *
 *  After the function is called:
 *    p_Binding_Table pointer should be de-allocated
 *
 *----------------------------------------------------------------------------*/
uint8_t APSME_Get_Binding_Table_Entry(uint8_t bind_index,
        ZDP_Bind_Request_t* p_Binding_Table_entry);
/*******************************************************************************
 ZigBeeGetBindingEntry
 * Return Value:
 *	returns SUCCESS if the command is successful else failure
 * Parameters:
 *   Input parameter:
 *	    BindIndex - indicates the index from which the binding entry needs
 *       to be copied
 *
 * Output parameter:
 *	pGetBindingEntry indicates the pointer to which the binding entry
 *   need to be copied for the given index
 *
 * Scope of the function:
 *   This function is invoked by Application to get the binding table entry
 *
 * Purpose of the function:
 *   This function allows the Application to get an entry from the binding table
 *   from the given index of the binding table

 * Before the function is called:
 *   None
 * After the function is called:
 *   None
 *
 *******************************************************************************/
uint8_t ZigBeeGetBindingEntry(uint8_t bindIndex,
        ZDP_Bind_Request_t * pGetBindingEntry);

/*******************************************************************************
 ZigBeeDeleteBinding
 * Return Value:
 *	returns SUCCESS if the command is successful else failure
 * Parameters:
 *   Input parameter:
 *	    BindIndex indicates the index which need to be deleted
 *
 * Output parameter:
 *	None
 * Scope of the function:
 *   This function is invoked by Application to delete the binding table entry
 *
 * Purpose of the function:
 *   This function allows the Application to delete an entry in the binding
 *   table for the specified index

 * Before the function is called:
 *   None
 * After the function is called:
 *   None
 *
 *******************************************************************************/
uint8_t ZigBeeDeleteBinding(uint8_t bindIndex);
/*******************************************************************************
 ZigBeeClearBindingTable
 * Return Value:
 *	returns SUCCESS if the command is successful else failure
 * Parameters:
 *   Input parameter:
 *	    None
 *
 * Output parameter:
 *	None
 * Scope of the function:
 *   This function is invoked by Application to delete all the binding table entries
 *
 * Purpose of the function:
 *   This function allows the Application to clear all the binding table entries

 * Before the function is called:
 *   None
 * After the function is called:
 *   None
 *
 *******************************************************************************/
uint8_t ZigBeeClearBindingTable(void);

/*******************************************************************************
 * ZigBeeSetBindingEntry
 * Return Value:
 *	returns SUCCESS if the command is successful else failure
 * Parameters:
 *   Input parameter:
 *		pSetBindingEntry indicates the pointer to the binding entry 
 *
 * Output parameter:
 *	None
 * Scope of the function:
 *   This function is invoked by Application to Set the Binding entry in the
 *   Binding  table
 *
 * Purpose of the function:
 *   This function allows the Application to set an entry in the binding table by
 *   copying the structure pointed to by pSetBindingEntry into the binding table.
 * Before the function is called:
 *   None
 * After the function is called:
 *   None
 *
 *******************************************************************************/
uint8_t ZigBeeSetBindingEntry(ZDP_Bind_Request_t * pSetBindingEntry);

/*******************************************************************************
 * ZigBeeIsBindingEntryActive
 * Return Value:
 *	returns SUCCESS if the command is successful else failure
 * Parameters:
 *   Input parameter:
*	BindIndex : index to the binding entry
 * Output parameter:
 *	None
 * Scope of the function:
 *   This function is invoked by Application to check whether Binding entry
 *   is active or not
 *
 * Purpose of the function:
 *   This function checks whether the binding entry is active or not
 *
 * Before the function is called:
 *   None
 * After the function is called:
 *   None
 *
 *******************************************************************************/
uint8_t ZigBeeIsBindingEntryActive(uint8_t BindIndex);

/*******************************************************************************
 * ZigBeeGetBindingIndices
 * Return Value:
 *	returns length = 0x00, if ther is no active binding entry else return 
 *              length no zero value if ther is an active entry
 * Parameters:
 *   Input parameter:
 *	
 *       be set in the given index
 * Output parameter:
*	noOfActiveIndices :  Pointer 
 * Scope of the function:
 *   Application calls this api
 *
 * Purpose of the function:
 *   This function is invoked by Application to get binding indices
 *
 * Before the function is called:
 *   None
 * After the function is called:
 *   None
 *
 *******************************************************************************/

uint8_t ZigBeeGetBindingIndices( uint8_t *noOfActiveIndices);


uint8_t ZigBeeSetAddressMapEntry(uint8_t * pExtAddress ,uint16_t * pShortAddress);

/*******************************************************************************
 * ZigBeeGetAddressMapEntry
 * Return Value:
 *	returns SUCCESS if the command is successful else failure
 * Parameters:
 *   Input parameter:
 *		pExtAddress :pointers to Extended address
 * Output parameter:
 *
 *      pShortAddress :pointer to short address
 * Scope of the function:
 *   This function is invoked by Application to get address map table entry
 *
 * Purpose of the function:
 *
 * Before the function is called:
 *   None
 * After the function is called:
 *   None
 *
 *******************************************************************************/

uint8_t ZigBeeGetAddressMapEntry(uint8_t * pExtAddress ,uint16_t * pShortAddress);


APSME_Address_Map_Table_t ZigBeeAddressMapTableEntry(uint8_t index);

ZigBeeStatus_t ZigBeeSetAddressTableRemoteEui64(uint8_t addressTableIndex ,
                                         uint8_t* pExtAddress);

void ZigBeeSetAddressTableRemoteNodeId(uint8_t addressTableIndex ,
                                         uint16_t *pShortAddress);

void ZigBeeGetAddressTableRemoteEui64 ( uint8_t addressTableIndex,
                                            uint8_t* pExtendedAddress);

uint16_t ZigBeeGetAddressTableRemoteNodeId ( uint8_t addressTableIndex );

/* moved from aps_linkkey_security_manager.h, because these are used for cbke*/
uint8_t getLinkKeyEntryIndex(uint8_t *address);
void MathyasMeyerAlgorithm(uint8_t *message, uint8_t length, uint8_t *output);

void HMACAlgorithm(uint8_t *key, uint8_t *text, uint8_t *transformedKey,
        uint8_t dataLength);
bool APSAddLinkKey(uint8_t * dstAddress, uint8_t * linkKey);

/*This is specifically added for ZLL security to use ECB mode. However, this function
is generic for any module to use ECB mode*/
uint8_t Encrypt_Decrypt_Using_ECB_Mode(  uint8_t *pPldLen, uint8_t *pPld,
                                       uint8_t *pEncryptedPld,bool encrypt );
/*******************************************************************************
 * End Of File
 *******************************************************************************/

#endif                                                  /*APSME_INTERFACE_H*/
