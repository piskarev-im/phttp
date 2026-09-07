#pragma once

#include <stddef.h>
#include "http.h"

#define ARRAY_SIZE(a) ((sizeof(a) / sizeof((a)[0])) - 1)

/// Extern section
// MB we need to move it to utils.c
extern void *split_array(char *str, char tok[4], char *result);
// TODO:
// HTTP_CORE_HEADER we need to change later to request structure!!
// Parsing full request from client here
__attribute__((deprecated))
extern HTTP_CORE_HEADER *parse_request(char *body);

/// Static section

// Parsing entry line of http request
__attribute__((deprecated))
static HTTP_CORE_MODEL parse_entry_line(char *parsed);

// Make row from raw http -> HTTP_CORE_HEADER like Key:Value pair
// Returns pointer to header structure
__attribute__((deprecated))
static HTTP_CORE_HEADER *parse_http_header(char *parsed[]);

__attribute__((deprecated))
static void parse_http_payload(char *parsed[]);