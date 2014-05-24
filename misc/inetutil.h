/*
 *   Copyright 2014, University Corporation for Atmospheric Research
 *   See file ../COPYRIGHT for copying and redistribution conditions.
 */

/* 
 * Miscellaneous functions to make dealing with Internet addresses easier.
 */

#ifndef _INETUTIL_H_
#define _INETUTIL_H_

#include <netinet/in.h>
#include "error.h"
#include "rpc/types.h"
#include "rpc/xdr.h"

typedef struct ServAddr ServAddr;

#ifdef IPPROTO_IP /* we included netinet/in.h, so struct sockaddr_in is */
extern const char*    hostbyaddr(
    const struct sockaddr_in* const paddr);
extern int            addrbyhost(
    const char* const               id,
    struct sockaddr_in* const       paddr);
extern ErrorObj*      hostHasIpAddress(
    const char* const	            hostname,
    const in_addr_t	            targetAddr,
    int* const		            hasAddress);
extern char*          s_sockaddr_in(struct sockaddr_in *paddr);
extern int            gethostaddr_in(struct sockaddr_in *paddr);
#endif
extern int            getservport(const char *servicename, const char *proto);
extern char*          ghostname(void);
extern int            usopen(const char *name);
extern int            udpopen(const char *hostname, const char *servicename);
extern int            isMe(const char *remote);
extern int            local_sockaddr_in(struct sockaddr_in* addr);
extern int            sockbind(const char *type, unsigned short port);
extern void           sa_free(ServAddr* const sa);
extern ServAddr*      sa_clone(const ServAddr* const sa);
extern const char*    sa_getHostId(const ServAddr* const sa);
extern unsigned short sa_getPort(const ServAddr* const sa);
extern int            sa_format(const ServAddr* restrict sa, char* restrict buf,
                            size_t len);
extern bool_t         xdr_ServAddr(XDR* const xdrs, ServAddr* sa);

#endif /* !_INETUTIL_H_ */
