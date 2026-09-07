#pragma once

#ifndef HTTP_PROTOTYPE_HTTP_H
	#define HTTP_PROTOTYPE_HTTP_H
#endif

// (\n\n || \n) || (\r\n\r\n || \r\n)
#ifdef __WIN32__ || __WIN64__ || (_WIN32 || _WIN64)
	#define CLRF '\r\n\r\n'
#else
	#define CLRF '\n\n'
#endif

#ifndef P_HTTP_STATUS
	#define P_HTTP_STATUS
	// After defining code we can use increment or decrement to use any code
	#define P_HTTP_CODE_INFO 100
	#define P_HTTP_CODE_SUCCESS 200
	#define P_HTTP_CODE_REDIRECT 300
	#define P_HTTP_CODE_CLIENT_ERROR 400
	#define P_HTTP_CODE_SERVER_ERROR 500
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

typedef enum {
	CACHE_CONTROL,
	CONNECTION,
	HDATE, // Common DATE not allowed
	MIME_VERSION,
	PRAGMA,
} HTTP_SERVICE_HEADERS;

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
	HTTP_CORE_HEADER headers;
	// buffer
	char *body;
} HTTP_CLIENT_REQUEST;

// Headers
// Content-Type
typedef struct {
	char content_type1;
	char content_type2;
	char charset;
	char boundary;
} P_HTTP_HEADER_CT;

typedef struct {
	unsigned int length;
} P_HTTP_HEADER_CL;
// Structure for Accept / Accept-Encoding
typedef struct {
	char accept;
	float q; // Q param, .0 - 1.0, priority of element
} P_HTTP_HEADER_SUB;

// Accept
typedef struct {
	char allowed_types;
} P_HTTP_HEADER_ACCEPT;

// Accept-Charset
typedef struct {
	char charset;
} P_HTTP_HEADER_CS;

typedef struct {
	P_HTTP_HEADER_SUB params;
} P_HTTP_HEADER_AE;

typedef struct {
	char lang;
} P_HTTP_HEADER_AL;

typedef struct {
	char auth_type; // Bearer or any else
	char token; // Token / Hash
} P_HTTP_HEADER_AUTH;

typedef struct {
	char key;
	char value;
} P_HTTP_HEADER_UNIFIED_FIELD;

typedef struct {
	char type;
	P_HTTP_HEADER_UNIFIED_FIELD fields;
} P_HTTP_HEADER_CD;

// So, here we could have only 1 value
// 100-continue
typedef struct {
	char expect;
} P_HTTP_HEADER_EXPECT;

typedef struct {
	char uri;
} P_HTTP_HEADER_HOST;

// User-Agent: <product> / <product-version> <comment>
typedef struct {
	char product;
	char version;
	char comment; // Here should be a billion of arguments :(
} P_HTTP_HEADER_UA;