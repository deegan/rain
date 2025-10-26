/*--------------------------------------------------*
 * $Header: /usr/src/Projects/rain/RCS/rain_common.h,v 1.1 2001/06/11 20:44:09 root Exp root $
 * $Author: root $
 * rain_common.h
 * rain - by Evil (mystic@tenebrous.com)
 * A flexible packet flooder for testing stability.
 * Copyright(c) 2001
 * Licensed under the GNU General Public License
 *
 * $Log: rain_common.h,v $
 * Revision 1.1  2001/06/11 20:44:09  root
 * Initial revision
 *
 * Revision 1.1  2001/06/11 02:59:47  root
 * Initial revision
 *
 *-------------------------------------------------*/
#ifndef _RAIN_COMMON_H
#define _RAIN_COMMON_H



#if (HAVE_CONFIG_H)
  #include "config.h"
#endif

#if (HAVE_STDIO_H)
  #include <stdio.h>
#endif

#if (HAVE_STDLIB_H)
  #include <stdlib.h>
#endif

#if (HAVE_UNISTD_H)
  #include <unistd.h>
#endif

#if (HAVE_STRING_H)
  #include <string.h>
#endif

#if (HAVE_ERRNO_H)
  #include <errno.h>
#endif

#if (HAVE_SIGNAL_H)
  #include <signal.h>
#endif 

#if (HAVE_TIME_H)
  #include <time.h>
#endif

#if (HAVE_FCNTL_H)
  #include <fcntl.h>
#endif

#if (HAVE_SYS_STAT_H)
  #include <sys/stat.h>
#endif

#if (HAVE_SYS_TIME_H)
  #include <sys/time.h>
#endif

#if (HAVE_SYS_TYPES_H)
  #include <sys/types.h>
#endif

#if (HAVE_SYS_SOCKET_H)
  #include <sys/socket.h>
#endif

#if (HAVE_NETINET_IN_H)
  #include <netinet/in.h>
#endif

#if (HAVE_ARPA_INET_H)
  #include <arpa/inet.h>
#endif

#if (HAVE_NETDB_H)
  #include <netdb.h>
#endif



/*
 * Linux defines these macros in order to use
 * BSD style internet headers.
 * We'll go ahead and define them if they aren't
 * already defined.
 */
#ifndef __USE_BSD
  #define __USE_BSD
#endif
#ifndef __FAVOR_BSD
  #define __FAVOR_BSD
#endif
#ifndef __BSD_SOURCE
  #define __BSD_SOURCE
#endif

/*
 * Proprietary headers
 */
#include "config.h"
#include "rain_inet_common.h"
#include "rain_command_line.h"
#include "rain_tcp.h"
#include "rain_udp.h"
#include "rain_igmp.h"
#include "rain_die.h"
#include "rain_resolv.h"
#include "rain_signal.h"
#include "rain_string.h"
#include "rain_mem.h"
#include "rain_defaults.h"
#include "rain_services.h"
#include "rain_icmp.h"
#include "rain_errno.h"


#define TRUE  (1)
#define FALSE (0)

#define SUCCESS (0)
#define FAILURE (1)


/* 
 * BYTE_FIX:
 *
 * The byte order in BSD internet headers
 * vary from that of Linux internet headers
 * 
 * This macro will be used when constructing the headers
 * to ensure compatibility on both OS types.
 */
#ifndef BYTE_FIX
  #if (RAIN_LINUX) 
    /* 
     * Linux style (network order) 
     */
    #define BYTE_FIX(x) htons(x)
    #define BYTE_UFIX(x) ntohs(x)
  #elif (RAIN_BSD) 
    /* 
     * BSD style (host order) 
     */
    #define BYTE_FIX(x) x
    #define BYTE_UFIX(x) x
    /*
     * Cygwin - just like Linux
     */
  #elif defined(__CYGWIN__)
    #define BYTE_FIX(x) htons(x)
    #define BYTE_UFIX(x) ntohs(x)

  #else
    #error "Unknown OS type (rain is currently only supported on Linux and *BSD)"
  #endif
#endif /* BYTE_FIX */


#ifdef __CYGWIN__
  #define UID_CHECK(uid,opt) { fprintf(stdout,""); }
#else
#define UID_CHECK(uid,opt) { \
  switch(uid) { \
    case  0: { break; } \
    default:{ \
      fprintf(stderr,"\n- - Error: uid %d is not permitted to use %s (must be root)\n",uid,opt); \
      exit(1); \
    } \
  } \
}
#endif /* __CYGWIN__ */


#endif /* _RAIN_H */
