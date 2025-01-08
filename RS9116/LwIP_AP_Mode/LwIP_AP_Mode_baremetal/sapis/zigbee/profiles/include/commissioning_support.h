/*******************************************************************************
 * commissioning_support.h
 ********************************************************************************
 *  Program Description:
 *
 *******************************************************************************/
#ifndef COMMISSIONING_SUPPORT
#define COMMISSIONING_SUPPORT

/*******************************************************************************
 * Includes
 *******************************************************************************/

/* None */

/*******************************************************************************
 * Global Constants
 *******************************************************************************/

#if ( g_COMMISSIONING_CAPABILITIES_d == 1 )
#define m_DEFAULT_SIZE_ZDO_SAS_CONFIGURATION_c  	    0x100

#define m_SIZE_CONFIGURATION_c				            0x80	
#define m_SET_SAS_INDEX_c				                0x01
#define m_SET_ZDO_CONFIG_c				                0x00
/*The below two macros are used for storing ZDO and SAS Configurations.*/
/*Only one sector will be active at any time.*/
#define m_NVM_SECTOR_ONE_FOR_CONFIG_c                   ((uint32_t)0x0801F400)
#define m_NVM_SECTOR_TWO_FOR_CONFIG_c		            ((uint32_t)0x0801F800)
#define m_INDEX_TO_ZDO_CONFIG_c			                ((uint32_t)0x0801F402)
#endif                           /* ( g_COMMISSIONING_CAPABILITIES_d == 1 ) */

/*******************************************************************************
 * Type Definitions
 *******************************************************************************/

/* None */

/*******************************************************************************
 * Memory Declarations
 *******************************************************************************/

/* None */

/**----------------------------------------------------------------------------
 * Function Prototypes
 *******************************************************************************/

#if( g_COMMISSIONING_CAPABILITIES_d == 1 )
/*******************************************************************************
 *  ZDOupdateSAS
 ********************************************************************************
 *
 * Return Value:
 *           NONE.
 *
 * Input parameters:
 *
 *  	p_RAMSAS
 *           -	It is a pointer to the Startup attribute set structure giving by
 *           the application.
 *
 *		SAS_index
 *           - startup attribute set index(0 to 5)giving by the application.
 *
 * Output Parameters:
 * 				NONE.
 *
 * Scope of the function:
 *           This is a global function and is called by the NHLE when it wants
 *           to store the Data from the RAM into the NVM.
 *
 * Detailed Description:
 *           This is a synchronous function call and NHLE calls  this function
 *           to store startup attribute set into NVM from the RAM.Upon issuing
 *           this command the network will switch off the receiver ,disable all
 *           global interrupts, and Read the data from NVM into the RAM.
 *
 * Before function is called:
 *           The application has to ensure that the global interrupts are
 *           disabled and receiver is switched OFF.
 *
 * After function is called:
 * 				NONE.
 *
 * Error Handling:
 * 				NONE.
 *
 *******************************************************************************/
void ZDOupdateSAS (uint8_t* p_RAM_SAS, uint8_t SAS_index);

/*******************************************************************************
 *  ZDOupdateConfiguration
 ********************************************************************************
 *
 * Return Value:
 *  			NONE.
 *
 * Input parameters:
 *
 *     p_ZDO_config
 *           -	It is a pointer to the ZDO Default configuration structure
 *              giving by the application.
 *
 *		action_to_be_taken
 *           - if it is false it will points consider the given ZDO
 *           Configuration structure(RAM data).
 *
 * Output Parameters
 * 				NONE.
 *
 * Scope of the function:
 *           This is a global function and is called by the NHLE when it wants
 *           to store the Data from the RAM into the NVM.
 *
 * Detailed Description:
 *           This is a synchronous function call and NHLE calls  this function
 *           to store ZDO Default Configuration into NVM from the RAM.Upon
 *           issuing this command the network will switch off the receiver,
 *           disable all global interrupts, and read the data from NVM into the
 *           RAM.
 *
 * Before function is called:
 *           The application has to ensure global interrupts are disabled and
 *           receiver is switched OFF.
 *
 * After function is called:
 * 				NONE.
 *
 * Error Handling:
 * 				NONE.
 *
 *******************************************************************************/

void ZDOupdateConfiguration (uint8_t* p_zdo_config, uint8_t action_to_be_taken);

/*******************************************************************************
 *  ZDOreadNVMdefaultSAS
 ********************************************************************************
 *
 * Return Value :
 *  	      NONE.
 *
 * Input parameters:
 *  			NONE.
 *
 * Output Parameters:
 * 				p_default_SAS
 *              - It holds the default SAS from NVM.
 *
 * Scope of the function:
 *           This is a global function and is called by the NHLE when it wants
 *           to restore the data from the NVM into the RAM.
 *
 * Detailed Description:
 *           This is a synchronous function call and NHLE calls this function
 *           to Restore Default SAS into RAM from the NVM.Upon issuing this
 *           command the network will switch off the receiver,disable all global
 *           interrupts,and Read the data from NVM into the RAM.
 *
 * Before function is called:
 *           The application has to ensure that the global interrupts are
 *           disabled and receiver is switched OFF.
 *
 * After function is called:
 * 				NONE.
 *
 * Error Handling:
 * 			   NONE.
 *
 *******************************************************************************/
void ZDOreadNVMdefaultSAS (uint8_t *p_default_SAS);

/*******************************************************************************
 *  ZDOactivateSASindex
 ********************************************************************************
 *
 * Return Value:
 *  			NONE.
 *
 * Input parameters:
 *
 *     index
 *           -	SAS index
 *
 * Output Parameters
 * 				NONE.
 *
 * Scope of the function:
 *           This is a global function and is called by the NHLE when it wants
 *           to store the Data from the RAM into the NVM.
 *
 * Detailed Description:
 *           This is a synchronous function call and NHLE calls  this function
 *           to store ZDO Default Configuration into NVM from the RAM.Upon
 *           issuing this command the network will switch off the receiver,
 *           disable all global interrupts, and read the data from NVM into the
 *           RAM.
 *
 * Before function is called:
 *           The application has to ensure global interrupts are disabled and
 *           receiver is switched OFF.
 *
 * After function is called:
 * 				NONE.
 *
 * Error Handling:
 * 				NONE.
 *
 *******************************************************************************/
void ZDOactivateSASindex(uint8_t index);

#endif

/*******************************************************************************
 * End Of File
 *******************************************************************************/
#endif                                              /* COMMISSIONING_SUPPORT */
