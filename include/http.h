#pragma once

#ifndef HTTP_PROTOTYPE_HTTP_H
	#define HTTP_PROTOTYPE_HTTP_H
#endif

#ifndef P_HTTP_STATUS
	#define P_HTTP_STATUS
	// After defining code we can use increment or decrement to use any code
	#define P_HTTP_CODE_INFO 100
	#define P_HTTP_CODE_SUCCESS 200
	#define P_HTTP_CODE_REDIRECT 300
	#define P_HTTP_CODE_NOT_FOUND 400
	#define P_HTTP_CODE_ERROR 500
#endif

#ifndef P_HTTP_METHOD
	#define P_HTTP_METHOD_GET 1 >> 2
	#define P_HTTP_METHOD_POST 2 >> 2
	#define P_HTTP_METHOD_PUT 3 >> 2
	#define P_HTTP_METHOD_DELETE 4 >> 2
	#define P_HTTP_METHOD_HEAD 5 >> 2
	#define P_HTTP_METHOD_OPTIONS 6 >> 2
	#define P_HTTP_METHOD_TRACE 7 >> 2
	#define P_HTTP_METHOD_CONNECT 8 >> 2
#endif

// Http Headers from http/1.0 - http/1.1
#ifndef P_HTTP_HEADER
// Content-Type
	#define P_HTTP_HEADER_CT "%s/%s" // text/html
// Content-Length
	#define P_HTTP_HEADER_CL "%s" // Content len as int,
// Accept
	#define P_HTTP_HEADER_ACCEPT "%s/%s" // text/html
// Accept-Charset
	#define P_HTTP_HEADER_CS "%s" // utf-8
// Accept-Encoding
	#define P_HTTP_HEADER_AE "%s" // compress
// Accept-Language
	#define P_HTTP_HEADER_AL "%s" // ru
// Authorization
	#define P_HTTP_HEADER_AUTH "%s" // Bearer ...asdasd
// Content-Disposition
	#define P_HTTP_HEADER_CD "%s" // Content-Disposition: form-data; ...
// Expect
	#define P_HTTP_HEADER_EXPECT "%s" // 100-continue
// Host
	#define P_HTTP_HEADER_HOST "%s" // Uri
//User-Agent
	#define P_HTTP_HEADER_UA "%s"
#endif

typedef enum  {
	HTTP_INFO,
	HTTP_SUCCESS,
	HTTP_REDIRECT,
	HTTP_ERROR_CLIENT,
	HTTP_ERROR_SERVER,
} HTTP_STATUS_CLASS;

typedef enum  { // i gonna use only 6 for start, later we can create 4xx class struct with enums
	HTTP_CODE_SUCCESS,
	HTTP_CODE_OK,
	HTTP_CODE_REDIRECT,
	HTTP_CODE_ERROR,
	HTTP_CODE_ERROR_SERVER,
	HTTP_CODE_NOT_FOUND,
} HTTP_STATUS_CODE;

enum HTTP_SERVICE_HEADERS {
	CACHE_CONTROL,
	CONNECTION,
	DATE,
	MIME_VERSION,
	PRAGMA,
};

// not all :(
typedef enum {
	ACCEPT,
	AUTHORIZATION,
	HOST,
	USER_AGENT,
	CONTENT_LENGTH,
	CONTENT_TYPE,
} E_HTTP_CLIENT_HEADERS;

// HTTP message GET /uri/ HTTP/1.1
typedef char *http_method;
typedef char *http_proto;
typedef char *http_addr;

typedef struct {
	char name;
	char value;
} HTTP_CORE_HEADER;

typedef struct {
	http_addr addr;
	http_method method;
	http_proto proto;
} HTTP_CORE_MODEL;

typedef struct {
	char *accept;
	char *authorization;
	char *host;
	char *date;
	char *user_agent;
	char *content_type;
	size_t *content_length;
	char *content_language;
} HTTP_CLIENT_HEADERS;

typedef struct {
	HTTP_CORE_MODEL model; // 1-st line of http
	HTTP_CLIENT_HEADERS headers;
	// buffer
	char *body;
} HTTP_CLIENT_REQUEST;
