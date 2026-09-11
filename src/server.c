#include "../include/server.h"
#include "../include/parser.h"
#include "parser.c"

#include <stdio.h>
#include <winsock2.h>
#include <pthread.h>
#include <unistd.h>

WSADATA wsa_data;

static unsigned char* buf_str(char *buffer) {
	size_t buffer_t = sizeof(buffer);
	char *res = malloc(++buffer_t);
	memcpy(res, buffer, buffer_t);
	res[buffer_t] = '\0';
	free(res);
	return (unsigned char *)res;
}

static char* hex_dump(unsigned char *buffer, size_t size) {
	const char hex[] = "0123456789ABCDEF";
	size_t cap = size * 3 + size / 16 + 2;
	char *res = malloc(cap);
	if (!res) return NULL;

	size_t pos = 0;
	for (size_t i = 0; i < size; i++) {
		if (i % 16 == 0 && i != 0) res[pos++] = '\n';
		res[pos++] = hex[buffer[i] >> 4];
		res[pos++] = hex[buffer[i] & 0x0f];
		res[pos++] = ' ';
		pos += sprintf(res + pos, "%02X ", buffer[i]);
	}
	res[pos] = '\0';
	return res;
}

pthread_t* new_cl_sock_thread(SOCKET client, void* thread_p) {
	pthread_t listen_thread;
	pthread_create(&listen_thread, NULL, thread_p, &client);
	return &listen_thread;
}

static char* buff_str(const char *buffer, size_t size) {
	char *res = malloc(size + 1);
	if (!res) return NULL;

	for (size_t i = 0; i < size; i++) {
		unsigned char c = (unsigned char)buffer[i];
		if (c == '\r' || c == '\n' || c == '\t') {
			res[i] = c;
		} else if (c >= 0x20 && c < 0x7F) {
			res[i] = c;
		} else {
			res[i] = '.';
		}
	}
	res[size] = '\0';
	return res;
}

// Reading winsock2 data from SOCKET addr, then use result in future HTTP function
void *read_cl_sock(SOCKET client) {
	char buf[1024];
	size_t sbytes;
	for (;;) {
		if ((sbytes = recv(client, buf, 1024, 0)) == 0) {
			printf("Client sent no data, or we can't read it; ErrorCode: %i", WSAGetLastError());
		} else {
			char* result = buff_str(buf, sbytes);
			if (sbytes == 0xffffffffffffffff) printf("WSASocket error: %d \n", WSAGetLastError());

			unsigned char *d = hex_dump(result, sbytes);
			// Test log
			printf("Socket sent:\n%s; Size: %llu ; WSAError Code: %i \n", result, sbytes, WSAGetLastError());
			printf("\n-------------\n Socket (size): 0x%x Full dump:\n%s\n-------------\n", sbytes, d);

			// Cleanup here
			free(d);
			free(result);
			WSACleanup();
		}
	}
}

// Default winsock2 server
// Used for me to remember what we do here: winsock2 ...func returns 0 if no error else error code
// AF_INET is used for IPv4, AF_INET6 (|| 2) is used for IPv6 SOCK_STREAM == tcp // SOCK_DRAM == udp, protocol idk, mb we need it )
// (struct sockaddr*)&addr -> sockaddr we use as socket address but sockaddr_in used to simply init ipv4 addr
extern void serve(const unsigned int port, http_server_conf config) {
	int err = WSAStartup(MAKEWORD(2, 2), &wsa_data); // Startup WSA // INDEF In UNIX
	if (err != 0) printf("WSAStartup failed with error: %d\n", err);

	//server socket
	SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addr; // socket-address
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	// Bind local addr to global
	int status = bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
	if (status != 0) {
		printf("Server addr bind error\n; WSAGetLastErrorCode: %d", WSAGetLastError());
		kill(1);
	}

	SOCKET accepted;

	int addr_len = sizeof(struct sockaddr_in);
	accepted = accept(server_fd, (struct sockaddr*)&addr, &addr_len); // Accepted : int

	if (listen(server_fd, SOMAXCONN) == 0) {
		int client;
		if ((client = accept(server_fd, (struct sockaddr*)&addr, &addr_len)) == SOCKET_ERROR) {
			close(server_fd);
		} else {
			// Conn established

			pthread_t* th = new_cl_sock_thread(client, &read_cl_sock);
			pthread_join(*th, NULL);
			printf("Client handled successfully.\n Thread closed");
		}
	}
	WSACleanup();
};

extern unsigned int send_data(SOCKET s, SOCKET r, char *buffer) {
	int result = sendto(s, buffer, sizeof(buffer), 0, (struct sockaddr*)&r, sizeof(r));
	if (result == SOCKET_ERROR || result != 0) {
		printf("Send to %d data failed", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	return 0;
}

// Unfinished, using threads for several functions
void server() {
	pthread_t listen_thread;
	int *signal_lir = 0;

	// Listening thread
	// Exit from all threads when them gonna finish
	pthread_exit(NULL);
}

// TODO: Add error text, pointers to clean
void kill(int code) {
	WSACleanup(); // WinSock2 Cleaning up
	exit(code);
}

boolean cors(char domain, CORS_OPTIONS options) {
	if ((options.origin && options.allowed) && strcmp(&options.origin, &domain) == 0) {
		return 1;
	} return 0;
}

// Currently validate only FAMILYIFNET; for production need to resolve Ethernet, wifi ...etc
// ByteByByte iteration | Big-Endian
void read_buff(unsigned int* buffer, size_t size, char* result) {
	for (unsigned int i = 0; i < size; i++) {
		result[i] = (char)buffer[i];
	}
}

