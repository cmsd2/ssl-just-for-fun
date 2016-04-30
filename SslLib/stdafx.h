// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <stdio.h>
#include <tchar.h>

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <stdarg.h>
#include <assert.h>
#ifdef WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif
