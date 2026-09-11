#pragma once

#include <pthread.h>
#include <stdbool.h>
#include <winsock2.h>
#include <unistd.h>

#ifndef HTTP_PROTOTYPE_SERVER_H
	#define HTTP_PROTOTYPE_SERVER_H
#endif

#ifndef SOMAXCONN
	#define SOMAXCONN 200
#endif

typedef struct {} http_server_conf;

typedef struct {
	char origin;
	boolean allowed;
} CORS_OPTIONS;

typedef struct {} http_server_handler;

void receive();

void call_method();

void *read_received_data(SOCKET s);

void serve_static();

boolean cors(char domain, CORS_OPTIONS options);

__attribute__((noreturn)) extern void serve(unsigned int port, http_server_conf config);

__attribute__((noreturn)) extern void kill(int code);

__attribute__((noreturn))
void *read_received_data(SOCKET s);

extern unsigned int send_data(SOCKET s, SOCKET r, char *buffer);

void server();

pthread_t* new_cl_sock_thread(SOCKET client, void* thread_p);