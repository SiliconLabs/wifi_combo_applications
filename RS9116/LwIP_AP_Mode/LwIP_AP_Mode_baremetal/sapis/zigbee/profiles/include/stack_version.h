/*******************************************************************************
* stack_version.h
********************************************************************************
* Program Description:
*     This file contains the macros that is used to define the stack build.
*
********************************************************************************
 * Includes
*******************************************************************************/
#ifndef STACK_VERSION_H_
#define STACK_VERSION_H_
/* None */

/*******************************************************************************
 * Global Constants
*******************************************************************************/

/* {@ Major_Version
 *   @brief This macro specifys the version of the ZigBee specification
 *          supported. eg: r17, r18, ..r20, ..etc */
#ifdef  APPLY_R_20_CHANGE
#define STACK_BUILD_MAJOR_VERSION                        4u
#endif

#ifdef  APPLY_R_19_CHANGE
#ifndef  STACK_BUILD_MAJOR_VERSION
#define STACK_BUILD_MAJOR_VERSION                        3u
#endif
#endif

#ifdef  APPLY_R_18_CHANGE
#ifndef  STACK_BUILD_MAJOR_VERSION
#define STACK_BUILD_MAJOR_VERSION                        2u
#endif
#endif

#ifndef  STACK_BUILD_MAJOR_VERSION /* r17 */
#define STACK_BUILD_MAJOR_VERSION                        1u
#endif
/* Note: Updating values to this macro should be done as it signifies the stack
 *       itself is changed to a different version of the specification */
/* @} Major_Version*/

/* {@ Minor_Version
 *   @brief This macro signifies the common changes which is reflected in all
 *          the stack versions */
#define STACK_BUILD_MINOR_VERSION                        2u
/* Note: Updating this macro value should be done only if the change
 *       affects all versions of the stack */
/* @} Minor_Version*/

/* {@ Sub_Version
 *   @brief This macro signifies the specific changes the stack */
#ifdef  APPLY_R_20_CHANGE
#define STACK_BUILD_SUB_VERSION                          0u
#endif
#ifdef  APPLY_R_19_CHANGE
#ifndef STACK_BUILD_SUB_VERSION
#define STACK_BUILD_SUB_VERSION                          0u
#endif
#endif
#ifdef  APPLY_R_18_CHANGE
#ifndef STACK_BUILD_SUB_VERSION
#define STACK_BUILD_SUB_VERSION                          0u
#endif
#endif
#ifndef STACK_BUILD_SUB_VERSION   /* r17 */
#define STACK_BUILD_SUB_VERSION                          4u
#endif
/* Note: If the changes made affect all version of the stack then
 *      'STACK_BUILD_MINOR_VERSION' macro should be changed */
/* @} Sub_Version*/

/*******************************************************************************
 * Type Definitions
 *******************************************************************************/
/* None */

/*******************************************************************************
 * Memory Declarations
 *******************************************************************************/

/* None */

/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/* None */

/*******************************************************************************
 * End Of File
 *******************************************************************************/

#endif  /* STACK_VERSION_H_ */