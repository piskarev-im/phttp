/*
 * This used to compare any -> http protocol rulesets
 * e.g. If in header value is incorrect, return false ...etc
 */

// TODO: Fix import paths via cmake cfg
#include "../include/proto.h"

#include <stdarg.h>
#include <stdlib.h>

#include "../include/http.h"

#include <string.h>

// TODO: Try to make function smarter
// Mb should be used macro-programming
unsigned int compare_method(char method) {
	if (strncmp(&method, "GET", 3) == 0) return P_HTTP_METHOD_GET;
	if (strncmp(&method, "POST", 4) == 0) return P_HTTP_METHOD_POST;
	if (strncmp(&method, "PUT", 3) == 0) return P_HTTP_METHOD_PUT;
	if (strncmp(&method, "DELETE", 6) == 0) return P_HTTP_METHOD_DELETE;
	if (strncmp(&method, "HEAD", 4) == 0) return P_HTTP_METHOD_HEAD;
	if (strncmp(&method, "OPTIONS", 7) == 0) return P_HTTP_METHOD_OPTIONS;
	if (strncmp(&method, "TRACE", 6) == 0) return P_HTTP_METHOD_TRACE;
	if (strncmp(&method, "CONNECT", 7) == 0) return P_HTTP_METHOD_CONNECT;
	return 0;
}

// If returns 0 -> failed to get class => error
unsigned int get_status_class(unsigned int status_c) {
	int class = status_c / 100;
	if (class == 1) return P_HTTP_CODE_INFO;
	if (class == 2) return P_HTTP_CODE_SUCCESS;
	if (class == 3) return P_HTTP_CODE_REDIRECT;
	if (class == 4) return P_HTTP_CODE_CLIENT_ERROR;
	if (class == 5) return P_HTTP_CODE_SERVER_ERROR;
	return 0;
}

unsigned int resolve_header(HTTP_CORE_HEADER header) {
	if (strcmp(&header.name, "Content-Type") == 0) return P_HTTP_CLIENT_H_CT;
	if (strcmp(&header.name, "Content-Length") == 0) return P_HTTP_CLIENT_H_CL;
	if (strcmp(&header.name, "Accept") == 0) return P_HTTP_CLIENT_H_ACC;
	if (strcmp(&header.name, "Accept-Charset") == 0) return P_HTTP_CLIENT_H_ACCC;
	if (strcmp(&header.name, "Accept-Encoding") == 0) return P_HTTP_CLIENT_H_ACCE;
	if (strcmp(&header.name, "Accept-Language") == 0) return P_HTTP_CLIENT_H_ACCL;
	if (strcmp(&header.name, "Authorization") == 0) return P_HTTP_CLIENT_H_AUTH;
	if (strcmp(&header.name, "Content-Disposition") == 0) return P_HTTP_CLIENT_H_CD;
	if (strcmp(&header.name, "Expect") == 0) return P_HTTP_CLIENT_H_EXPECT;
	if (strcmp(&header.name, "Host") == 0) return P_HTTP_CLIENT_H_HOST;
	if (strcmp(&header.name, "User-Agent") == 0) return P_HTTP_CLIENT_H_UA;

	return 0;
}

// If resolved == 1; else return 0
// If notResolved it means client receives 4xx error, due bad request
// To resolve headers list should contain all required
unsigned int resolve_required_client_header(HTTP_CORE_HEADER header) {
	return 0;
}