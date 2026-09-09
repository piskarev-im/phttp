#include "../include/server.h"
#include "../include/parser.h"
#include "parser.c"

#include <stdio.h>
#include <winsock2.h>
#include <pthread.h>
#include <unistd.h>

// Default winsock2 server
// Used for me to remember what we do here: winsock2 ...func returns 0 if no error else error code
// AF_INET is used for IPv4, AF_INET6 (|| 2) is used for IPv6 SOCK_STREAM == tcp // SOCK_DRAM == udp, protocol idk, mb we need it )
// (struct sockaddr*)&addr -> sockaddr we use as socket address but sockaddr_in used to simply init ipv4 addr
extern void serve(const unsigned int port, http_server_conf config) {
	SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	// Bind local addr to global
	int status = bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
	if (status != 0) {
		printf("Server addr bind error\n; WSAGetLastErrorCode: %d", WSAGetLastError());
		kill(1);
	}
	listen(server_fd, SOMAXCONN);

	SOCKET accepted;

	accepted = accept(server_fd, (struct sockaddr*)&addr, (int *)sizeof(addr)); // Accepted : int
	if (accepted == INVALID_SOCKET) {
		printf("Accepted socket is invalid\n; WSAGetLastError: %d", WSAGetLastError());
		closesocket(accepted);
		WSACleanup();
	} else {
		// Connect to socket
		int success;
		success = connect(accepted, (struct sockaddr*)&accepted, sizeof(accepted));
		if (success != 0) {
			printf("Client socket connect error; WSAGetLastError: %d", WSAGetLastError());
		}
	}
	WSACleanup();
};

void *listen_incoming_request(void *arg) { // SIGEXIT pointer to INT that says what to do here, like, should we exit??
	int *void_signal = &arg[0];

	for (;;) {
		if (void_signal == SIG_HTTP_LIR_LISTEN || void_signal == NULL) {
			read_received_data(*void_signal);
		}
		if (void_signal == (int *)SIG_HTTP_LIR_KILL) { // WHY?? >_< | Error that int* == int
			kill(1); // Here we should kill application
		}
		if (void_signal == (int *)SIG_HTTP_LIR_SLEEP) {
			sleep(1);
		}
	}
};

static char *buff_str(const char *buffer) {
	size_t buffer_t = sizeof(buffer);
	char *res = malloc(++buffer_t);
	memcpy(res, buffer, buffer_t);
	res[buffer_t] = '\0';
	free(res);
	return res;
}

extern unsigned int send_data(SOCKET s, SOCKET r, char *buffer) {
	int result = sendto(s, buffer, sizeof(buffer), 0, (struct sockaddr*)&r, sizeof(r));
	if (result == SOCKET_ERROR || result != 0) {
		printf("Send to %d data failed", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	return 0;
}

// Reading winsock2 data from SOCKET addr, then use result in future HTTP function
void *read_received_data(SOCKET s) { // CR, LF, CR, LF
	char *buffer = malloc(1024);
	char *res = "";
	int result = recv(s, buffer, 1024, 0);
	if (result == 0) {
		HTTP_CLIENT_REQUEST *parsed = malloc(sizeof(HTTP_CLIENT_REQUEST));
		// No errors, proceed data
		// So, now we can read received data as request
		res = buff_str(buffer); // Convert to str
		parse_request_client(res, parsed);

		// Temp cleanup
		free(buffer);
		free(parsed);
	}
}

// Unfinished, using threads for several functions
void server() {
	pthread_t listen_thread;
	int *signal_lir = 0;

	// Listening thread
	pthread_create(&listen_thread, NULL, listen_incoming_request, signal_lir);
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