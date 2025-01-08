/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Common.h"
#include "common_includes.h"
#include "memory_utility.h"
#include "stack_common.h"
#include "AppZoneTableUtilities.h"
#include "ZCL_Stack_Utility.h"
#include "stack_util.h"
#include "ApplicationThinLayer.h"
#include "ZCL_Interface.h"
#include "ZCL_Security_And_Safety_IAS_Zone_Cluster.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

extern uint8_t a_IEEE_Address_Found[8];
//uint8_t ga_Zone_Id_Map_Section[32] = { 0x00 };
uint8_t ga_Zone_Id_Map_Section[32];

/* Enumerations for Security and Safety Commands */

/*Table maintained for zone type*/
uint16_t ma_Zone_Type_List[] =
{
   /*Macros for different zone types*/
   g_ZONE_TYPE_CIE_c,
   g_ZONE_TYPE_MOTION_SENSOR_c,
   g_ZONE_TYPE_CONTACT_SWITCH_c,
   g_ZONE_TYPE_FIRE_SENSOR_c,
   g_ZONE_TYPE_WATER_SENSOR_c,
   g_ZONE_TYPE_GAS_SENSOR_c,
   g_ZONE_TYPE_PERSONAL_EMERGENCY_DEVICE_c,
   g_ZONE_TYPE_VIBRATION_MOVEMENT_SENSOR_c,
   g_ZONE_TYPE_REMOTE_CONTROL_c ,
   g_ZONE_TYPE_KEY_FOB_c,
   g_ZONE_TYPE_KEP_PAD_c ,
   g_ZONE_TYPE_STANDARD_WD_c
};

/*IAS Zone Table maintained by IAS ACE device*/
IAS_Zone_Table_t g_IAS_Zone_Table[ m_MAXIMUM_ZONE_TABLE_ENTRIES_c ] =
{
   /*Zone type*/
   {{0xFF,
   0xFF},
   /*64- bit Zone Address*/
   {0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00}} ,
   /*Second entry*/
   /*Zone type*/
   {{0xFF,
   0xFF},
   /*64- bit Zone Address*/
   {0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00}} ,
   /*Third entry*/
   /*Zone type*/
   {{0xFF,
   0xFF},
   /*64- bit Zone Address*/
   {0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00}} ,
};

Zone_Enroll_Response_Payload_t g_ZCL_data;

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void App_Reset_Zone_Table( void )
{
   uint8_t count;
   for ( count = 0x00; count < m_MAXIMUM_ZONE_TABLE_ENTRIES_c; count++ ) {
      memUtils_memSet ( g_IAS_Zone_Table[count].a_zone_type, 0xFF, 0x02 );
      memUtils_memSet ( g_IAS_Zone_Table[count].a_zone_address, 0x00,
			   									g_EXTENDED_ADDRESS_LENGTH_c );
   }
   /*reset this array to zero initial value*/
   memUtils_memSet (  ga_Zone_Id_Map_Section,  0x00,  0x20 );
}

/*---------------------------------------------------------------------------*/
uint8_t App_Add_Zone_Address ( uint8_t *p_IEEE_Address )
{
   uint8_t count;
   for ( count = 0x00; count < m_MAXIMUM_ZONE_TABLE_ENTRIES_c; count++ ) {
      if ( ! ( Is_Address_Null( g_IAS_Zone_Table[count].a_zone_address ) ) ) {
         /*The IEEE address is received in little endian format*/
         memUtils_memCopy ((uint8_t*)g_IAS_Zone_Table[count].a_zone_address,
		            								(uint8_t*)p_IEEE_Address,8);
         return g_ZCL_Success_c;
      }
   }
   return g_ZCL_Failure_c;
}

/*---------------------------------------------------------------------------*/
void App_Create_Enroll_Response_Command_Payload (   uint8_t *p_dest_addr,
												   uint8_t *p_payload,
												   uint8_t *p_src_endpoint,
												   uint8_t *p_dest_endpoint )
{
   /*update the zone index to invalid zone index*/
   uint8_t zoneIndex;
   /*assigne the pointer that holds the data indication received*/
   /*copy the destination short address*/
   uint16_t shortAddress = ZCL_Callback_Get_Device_ShortAddress();
   memUtils_memCopy (p_dest_addr,(uint8_t*)&shortAddress,2 );

   /*update the src endpoint and destination endpoint*/

   *p_src_endpoint = g_ZCL_data.dest_endpoint;
   *p_dest_endpoint = g_ZCL_data.dest_endpoint;
   *p_payload++ = ( g_Cluster_Specific_Cmd_c | (Frame_Control)g_Client_To_Server_c )
                                             |  g_Disable_Default_Response_c;
   /*update the transaction sequence number*/
   *p_payload++  =  g_ZCL_data.trans_seq_num;
   *p_payload++ = g_Zone_Enroll_Response_Command_c;
   /*Validate the IEEE address and update the enroll response code*/
#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
   *p_payload++ = ZCL_Callback_Zone_Validate_IEEE_Address (  a_IEEE_Address_Found, &zoneIndex,
										 			 (uint8_t*)&( g_ZCL_data.zone_type));
#endif
   /*update the zone index*/
   *p_payload = zoneIndex;
}

/*---------------------------------------------------------------------------*/

uint8_t ZCL_Callback_Get_Zone_Information (  uint8_t *p_data,uint8_t *p_respData, uint8_t mode )
{
   /*count to go through the zone table*/
   uint8_t count;
   /*fetch the zone id for get zone information command*/
   uint8_t zoneId= *( p_data + 0x03);
   /*loop through the zone table*/
   for ( count = 0x00; count < m_MAXIMUM_ZONE_TABLE_ENTRIES_c; count++ ) {
      /*if the command is get zone id map, then check if the
      zone index is allocated, if so set the corresponding bit to 1*/
      if ( g_Get_Zone_Id_Map_c == mode )  {
         if( g_IAS_Zone_Table[count].a_zone_type[0] != m_INVALID_ZONE_INDEX_c ){
            *p_respData |= ( 0x01 << count );
            /*if 0 to 7th bit is set considering 0x00 to 0x07, then
            increment the p_data to set the bit map of next byte*/
            if ( ( count > 0x07 ) && ( count != 0x00 ) && ( 0x00 ==
														   ( count % 0x07 ) ) ){
               p_respData++;
            }
         }
      }
      else  {
         /*if the mode is get zone information for a given zone*/
         if( count == zoneId ) {
            /*update the zone index*/
            *p_respData = count;
            p_respData++;
            /*if the zone type is not invalid zone type or unallocated, then
            copy the zone type and the zone address*/
            if( m_INVALID_ZONE_INDEX_c !=
                  g_IAS_Zone_Table[count].a_zone_type[0] )  {

               memUtils_memCopy ((uint8_t*)p_respData ,
                  		(uint8_t*)g_IAS_Zone_Table[count].a_zone_type,
                 		 sizeof(IAS_Zone_Table_t));

            }
            /*set the zone type and zone address to 0xFF indicating that
            the zone index is not allocated*/
            else  {
               memUtils_memSet ( p_respData ,  m_INVALID_ZONE_INDEX_c,  0x0A  );
            }
            /*considering the size of zone table that
            consists of zone type, zone address, 0x01 is
            added considering the zone index*/
            return ( sizeof(IAS_Zone_Table_t) + 0x01 );
         }
      }
   }
   if ( ( g_Get_Zone_Id_Map_c != mode ) &&
            ( count == m_MAXIMUM_ZONE_TABLE_ENTRIES_c ) ) {
      /*update the zone index*/
      *p_respData = *( p_data + 0x03);
      memUtils_memSet  (  p_respData + 0x01 , m_INVALID_ZONE_INDEX_c, 0x0A );
      return ( sizeof(IAS_Zone_Table_t) + 0x01 );
   }
   return 0x00;
}


/*----------------------------------------------------------------------------*/
uint8_t ZCL_Callback_Zone_Check_For_Valid_Zone_Type (  uint8_t *p_zone_type )
{
   uint8_t count;
   /*loop through the zone type table to find
   if the zone type is part of the table, if present returns success or
   not supported*/
   for ( count = 0x00; count < sizeof(ma_Zone_Type_List)/sizeof(uint16_t);
       count++ )  {
	/* since the zone type is stored in integer format in local and the zone
	type received in the command is little endian the zone type is reverse
	compared*/
      if ( g_Memory_Matches_c == memUtils_memCompare (p_zone_type,
								               (uint8_t*)&(ma_Zone_Type_List[count]),
								               sizeof(uint16_t) ) ) {
         return g_Enroll_Response_Success_c;
      }
   }
   return g_Enroll_Not_Supported_c;
}

/*---------------------------------------------------------------------------*/
uint8_t ZCL_Callback_Zone_Validate_IEEE_Address ( uint8_t *p_IEEE_address,
									   uint8_t *p_zone_index,
									   uint8_t *p_zone_type )
{
   uint8_t count;
   /*loop through the zone table and validate the configured zone address*/
   for ( count = 0x00; count < m_MAXIMUM_ZONE_TABLE_ENTRIES_c; count++ ) {
      if(  g_Memory_Matches_c == ( memUtils_memCompare( p_IEEE_address,
              							 g_IAS_Zone_Table[count].a_zone_address,
               							 0x08 ) ) ) {
         /*if the address is present copy the index.*/
         *p_zone_index = count;
         /*copy the zone type*/
         memUtils_memCopy  ( (uint8_t*)g_IAS_Zone_Table[count].a_zone_type,
            		(uint8_t*)p_zone_type, sizeof(uint16_t) );
         return g_Enroll_Response_Success_c;
      }
   }
   *p_zone_index  = m_INVALID_ZONE_INDEX_c;

   /*if the IEEE address is not configured, then return the status as
   No permit*/
   return g_No_Enroll_Permit_c;
}

/*---------------------------------------------------------------------------*/
void ZCL_Callback_Backup_Zone_Enroll_Resp_Data( uint8_t *p_payload)
{
	g_ZCL_data.dest_endpoint = ZCL_Callback_Get_Device_EndPoint();
	g_ZCL_data.src_endpoint =ZCL_Callback_Get_Device_Src_EndPoint();
	g_ZCL_data.trans_seq_num = p_payload[0x01];
	memUtils_memCopy  ( (uint8_t*)&g_ZCL_data.zone_type,
            		(p_payload+0x03), sizeof(uint16_t) );
}
/*---------------------------------------------------------------------------*/
void ZCL_Callback_Get_ZoneId_Map_Information(uint8_t *p_zone_Id_Map_Section ,uint8_t *p_respData,
										   	uint8_t length)
{
    /*update the zone id map*/
	ZCL_Callback_Get_Zone_Information ( ga_Zone_Id_Map_Section, p_respData,g_Get_Zone_Id_Map_c);
	 /*copy the zone id map to the payload*/
	memUtils_memCopy(p_respData,
				(uint8_t *)ga_Zone_Id_Map_Section, length);
}

/*---------------------------------------------------------------------------*/
