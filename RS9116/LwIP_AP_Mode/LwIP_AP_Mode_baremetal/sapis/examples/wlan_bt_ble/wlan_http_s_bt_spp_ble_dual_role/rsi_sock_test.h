/********************************************************************/
/** \file rsi_sock_test.h
*
* \brief Test code to run HTTP/HTTPS download test using Redpine
*        implementation of BSD socket API
*
*   \par MODULE NAME:
*       redpine_sock_test.h - Redpine sockets test header
*
*   \par DESCRIPTION:
*       This module exercises Redpine BSD socket API implementation
*       by connecting to an HTTP/S server and downloading a test file
*       with a known pattern. A few runtime knobs are provided via
*       a few variables which control certain aspects of the test.
*
* Copyright 2019 by Garmin Ltd. or its subsidiaries.
*
* NOTICE:
*   This code is intended for Redpine internal use only. The code is
*   considered Confidential Information under our NDA, and is not
*   considered to be Feedback. This code may not be put into a
*   public SDK.
*
*********************************************************************/

#ifndef REDPINE_SOCK_TEST
#define REDPINE_SOCK_TEST

#ifdef FRDM_K28F
#include "rsi_common_app.h"
#endif
#include <rsi_socket.h>

/*DEFINES*/
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif
void init_redpine_sock_test  (void);

void perform_redpine_sock_test(void* instance_ctx);

#define http_buffer_sz      ( 1500 )
#define ERRORTRACK_SIZE     ( 17 )

struct redpine_socktest_ctx_struct
    {
    int                     sock_fd;
    struct rsi_sockaddr_in      sa;
    int                     bytes_sent;
    int                     bytes_received;
    int                     headers_received;
    int                     res;
    char                    http_buffer[ http_buffer_sz + 1 ];
    int                     http_buffer_bytes_saved;
    int                     even_odd;
    uint32_t                verification_val;
    int                     selrdfd_ret_cnt;
    int                     selrdfd_ready_cnt;
    int                     selrdfd_notready_cnt;
    int                     selwrfd_ret_cnt;
    int                     selwrfd_ready_cnt;
    int                     selwrfd_notready_cnt;
    char                    sock_shutdown_in_progress;
    int                     num_successful_test;
    int                     num_failed_test;
    uint8_t                 errortrack[ERRORTRACK_SIZE];
    int                     threadid;
    rsi_semaphore_handle_t  http_soc_wait_sem;
    int                     high_performance_socket;
    int 					t_start;
    int 					t_end;
    };
typedef struct redpine_socktest_ctx_struct redpine_socktest_ctx_t;

#endif

 
