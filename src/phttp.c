/**
 * Main library sources, this .c file should export all lib methods
 */

#include "../include/phttp.h"

#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#ifndef P_PHTTP_MODULE
	#define P_PHTTP_MODULE
#endif

__attribute__((noreturn))
extern void p_http_server() {

}

extern void p_kill(int code, char *exit_str[], void *Memory[]) {
	if (Py_IsInitialized()) {
		Py_Finalize();
	}
	WSACleanup();

	for (register unsigned int i = 0; i < sizeof(void *); i++) {
		free(Memory[i]);
	}
	printf("%s; With code: %d", *exit_str, code);
	exit(code);
}