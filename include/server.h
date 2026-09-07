#pragma once

#include <stdio.h>
#include <winsock2.h>
#include <pthread.h>
#include <unistd.h>

#ifndef HTTP_PROTOTYPE_SERVER_H
	#define HTTP_PROTOTYPE_SERVER_H
#endif

#ifndef SOMAXCONN
	#define SOMAXCONN 200
#endif

// Signal for LIR function (listen_incoming_request)
// void *arg is a arg pointer arg[0] uint8 == what function we need to call
#define SIG_HTTP_LIR_LISTEN 0
#define SIG_HTTP_LIR_KILL 1
#define SIG_HTTP_LIR_SLEEP 2

typedef struct {

} http_server_conf;

typedef struct {
	char origin;
	boolean allowed;
} CORS_OPTIONS;

typedef struct {

} http_server_handler;

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

