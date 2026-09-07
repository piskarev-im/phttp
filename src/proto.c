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
int compare_method(char method) {
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
int get_status_class(int status_c) {
	int class = status_c / 100;
	if (class == 1) return P_HTTP_CODE_INFO;
	if (class == 2) return P_HTTP_CODE_SUCCESS;
	if (class == 3) return P_HTTP_CODE_REDIRECT;
	if (class == 4) return P_HTTP_CODE_CLIENT_ERROR;
	if (class == 5) return P_HTTP_CODE_SERVER_ERROR;
	return 0;
}

int resolve_header(HTTP_CORE_HEADER header) {
	if (header.name == (char)"Content-Type") {

	}
	if (header.name == (char)"Content-Length") {

	}
	if (header.name == (char)"Accept") {

	}
	if (header.name == (char)"Accept-Charset") {

	}
	if (header.name == (char)"Accept-Encoding") {

	}
	if (header.name == (char)"Accept-Language") {

	}
	if (header.name == (char)"Authorization") {

	}
	if (header.name == (char)"Content-Disposition") {

	}
	if (header.name == (char)"Expect") {

	}
	if (header.name == (char)"Host") {

	}
	if (header.name == (char)"User-Agent") {

	}
}

// If resolved == 1; else return 0
// If notResolved it means client receives 4xx error, due bad request
// To resolve headers list should contain all required
int resolve_required_client_header(HTTP_CORE_HEADER header) {

}

// Get parameters of header from python, but resolve here
int client_content_type(HTTP_CORE_HEADER header) {
	if (header.name != (char)"Content-Type" || !header.name) return 0;
	// header -> python -> tuple[reuslt]
	// Change result, it is bullshit
	char result[2] = { };
	char params[8] = { };
	char ct_1 = result[0]; // COntentType 1st
	char ct_2 = result[1]; // ContentType 2 nd

}