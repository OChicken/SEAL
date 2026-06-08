#include "seal.h"
#include "sealconfig.h"

#include <stdint.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#ifndef _EMULATOR_H
#define _EMULATOR_H

#define SEAL_IO_WAITCONN (0)
#define SEAL_IO_READY (1)
#define SEAL_IO_EOF (-2)
#define SEAL_IO_ERROR (0xFF)
#define SEAL_IO_SERVER (0)
#define SEAL_IO_CLIENT (1)

#ifdef __cplusplus
extern "C" {
#endif
    //APIs for Emulator.
    //Bind a variable.
    int SealBind(Seal * seal, const char *comp, const void *var);

    //Sync all bound variables.
    void SealSync(Seal * seal);

    //APIs for SealIO.
    //Data structure for SealIO.
    typedef struct _SealIO_ {
	struct sockaddr_un addr;
	int sockfd;		//FD for socket.
	int commfd;		//FD for communication.
	int stat;		//State of SealIO.
	int type;
    } SealIO;

    //Initiate a new SealIO.
    SealIO *SioOpen(const char *);
    void SioClose(SealIO *);
    SealIO *SioConnect(const char *path);
    int SioWaitConn(SealIO *);

    //Basic IO.
    int SioGetchar(SealIO *);
    int SioPutchar(SealIO *, const int);

    //==========================================================================
    // Compatibility shim: maps the newer Seal "VirtualPort" IO API (used by
    // uELMO, which tracks the private sca-research/Seal) onto this release's
    // SealIO / Sio* API. Header-only (static inline) -> no .so rebuild needed.
    //==========================================================================
#ifndef SEAL_VIRTUALPORT_COMPAT
#define SEAL_VIRTUALPORT_COMPAT
    typedef SealIO SealVirtualPort;

    // Remove a stale socket file. SioOpen() also unlinks; kept for API parity.
    static inline void SealCleanPort(const char *path)
    {
	unlink(path);
    }

    // Open a server port and block until the peer connects (READY state).
    static inline SealVirtualPort *SealOpenPort(const char *path)
    {
	SealIO *io = SioOpen(path);
	if (NULL == io)
	    return NULL;
	if (SEAL_SUCCESS != SioWaitConn(io))
	    {
		SioClose(io);
		return NULL;
	    }
	return io;
    }

    // Close the port and NULL the caller's handle.
    static inline void SealClosePort(SealVirtualPort **pp)
    {
	if (NULL == pp || NULL == *pp)
	    return;
	SioClose(*pp);
	*pp = NULL;
    }

    static inline int SealGetchar(SealVirtualPort *io)
    {
	return SioGetchar(io);
    }

    static inline int SealPutchar(SealVirtualPort *io, const int c)
    {
	return SioPutchar(io, c);
    }
#endif				/* SEAL_VIRTUALPORT_COMPAT */

#ifdef __cplusplus
}
#endif
#endif
