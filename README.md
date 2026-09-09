## pHttp
### Http protocol realization in C

HTTP/1.0 HTTP/1.1

Pet-Project for educate C, idea was in realization one of simpliest protocol at OSI-> AppLayer

Project contains rulesets, structs ...etc content of HTTP documentation, raw tcp server on sockets for request/response
python parser

**Directories**
1. /src - Source of project, C files
2. /protocol - python parser files // Should be renamed later
3. /include - C Headers

**About sources**
src/main.c - Will be entry point for tests, where we gonna init all required components, run auto tests
src/parser.c - C source that used to run python parsers, call data from them and convert data to C-Type for compability to
http.h structures. Parsing httpRequests (from client) implemented at python code
src/server.c - WinSock2 (for today only Win32 || Win64 > Windows95) server, used as common http server for tests, later could be
finished as production-ready http/https server library
phttp.c - Http handlers, proceed (r/w) requests, comparing received types

**Parser**

**C-Entry point function**
*Used by C Library to parse server-received data to Dict -> C-Structure*
```python
def parse_http_request(parse) -> dict[str, Any]:
```
C-Structure to resolve from python result
```c
// Request
typedef struct {
	HTTP_CORE_MODEL model; // 1-st line of http
	HTTP_CLIENT_HEADER headers;
	char *body;
} HTTP_CLIENT_REQUEST;

// Headers
typedef struct {
	char name;
	char value;
} HTTP_CORE_HEADER;

// Model, 1st line of request, only client
typedef struct {
	http_addr addr;
	http_method method;
	http_proto proto;
} HTTP_CORE_MODEL;
```
**is_http_method**
*If is method e.g. GET return true*
```python
is_http_method(method) -> bool
```
**resolve_first_line**
*Returns HTTP_CORE_MODEL from 1st line raw string*
```python
def resolve_first_line(line: str) -> Dict[str, str]
```
**parse_http_header**
*Parse http headers from raw header array, returning HTTP_CORE_HEADER[]*
```python
def parse_http_header(raw_headers: tuple[str]) -> tuple[dict[str, str]]
```

## TODO:
1. Finish handling server requests from client
2. Create handling server responses
3. Finish protocol rulecheck
4. Create autotests in main.c, to test server, handlers
5. Create standalone .c to make responses
6. Normalize types, enums

## TO_LONG_DO:
1. Update server security, implement Secured at HTTP
2. Make requests more safe by checksum
3. Add MIME support
4. Add UNIX systems support

### library
1. https://ru.wikipedia.org/wiki/TCP/IP
2. https://metanit.com/c/tutorial
3. https://ru.wikipedia.org/wiki/HTTP
4. https://datatracker.ietf.org/doc/html/rfc2616#autoid-42

[![Most useful visualisation](https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcS7wuEVkmEl-Aqnb7m5_9n0JPTC2y1XIe3B9tokm9GQPOvu6jO8)](#)
