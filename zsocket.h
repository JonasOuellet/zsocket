#pragma once

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the ZSOCKET_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// ZSOCKET_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ZSOCKET_EXPORTS
#define ZSOCKET_API __declspec(dllexport)
#else
#define ZSOCKET_API __declspec(dllimport)
#endif


#ifdef __cplusplus
EXTERN_C
{
#endif

ZSOCKET_API float GetSocketAddr(char* text, double number, char* memBlockIn, int memBlockSizeIn,
	char* memBlockOut, int memBlockSizeOut, char** zData);

ZSOCKET_API float SetSocketAddr(char* text, double number, char* memBlockIn, int memBlockSizeIn,
	char* memBlockOut, int memBlockSizeOut, char** zData);

ZSOCKET_API float SocketSend(char* text, double number, char* memBlockIn, int memBlockSizeIn,
	char* memBlockOut, int memBlockSizeOut, char** zData);

ZSOCKET_API float Version(char* pDontCare, double optValue, char* pOptBuffer1, int optBuffer1Size,
	char* pOptBuffer2, int optBuffer2Size, char** zData);

#ifdef __cplusplus
}
#endif