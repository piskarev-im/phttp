#include "../include/parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Python.h>

extern void *split_array(char *str, char tok[4], char *result) {
	char tokens[512];
	memset(tokens,'\0', sizeof(tokens));
	for(register int i=0, j=0; i < sizeof(tokens); i++)
	{
		if (str[i] == '\n') {
			// If we need split by \n
		}

		if (str[i] == tok[0] && str[++i] == tok[1] && str[i+2] == tok[2] && str[i+3] == tok[3]) { // LF || CRLF
			printf("%s\n", tokens); // For tests
			memset(tokens,'\0', sizeof(tokens));
			j=0;
		} else {
			tokens[j] = str[i];
			j++;
		}

		if (str[i] == '\0') {
			result = tokens;
			break;
		}
	}
	return 0;
}

// We need to use ' ' split to parse 1st line
static HTTP_CORE_MODEL parse_entry_line(char *parsed) {
	HTTP_CORE_MODEL result = {
		.method = &parsed[0],
		.addr = &parsed[1],
		.proto = &parsed[2],
	};
	return result;
}

static HTTP_CORE_HEADER *parse_http_header(char *parsed[]) {
	if (*parsed[0] == '\0') return NULL;
	HTTP_CORE_HEADER header = {
		.name = *parsed[0],
		.value = *parsed[1],
	};
	return &header;
}

//TODO: Save payload, we can use header is not equal parsed!e
extern HTTP_CORE_HEADER *parse_request(char *body) {
	char *parsed = malloc(sizeof(body));
	char terminator[4] = "\r\n\r\n";
	HTTP_CORE_HEADER *headers = malloc(sizeof(HTTP_CORE_HEADER));
	char *t_head = malloc(sizeof(char) * 256); // Max HeadOfRequest 256 symbols
	char t[4] = {'\n'};

	split_array(body, t, t_head);
	split_array(body, terminator, parsed); // Windows CLRF

	HTTP_CORE_MODEL head = parse_entry_line(&t_head[0]); // Head of request (1st line)

	return headers;
}

int initialize_python() {
	Py_Initialize();

	if (!Py_IsInitialized()) {
		printf("Python initialization failed\n");
		return 1;
	}
	return 0;
};

__attribute__((noreturn))
static void parse_request_headers(PyObject *p_tuple, size_t tuple_size, size_t total, HTTP_CORE_HEADER **headers, size_t *outc) { // -> Dict[key, tuple[Dict[str,str]] >> HTTP_HEADER[]
	HTTP_CORE_HEADER *t_headers = malloc(sizeof(HTTP_CORE_HEADER) * total);
	if (!t_headers) {
		printf("Mem for headers not allocated\n");
	}

	size_t i1 = 0;
	for (Py_ssize_t i = 0; i < tuple_size; i++) {
		PyObject *dict = PyTuple_GetItem(p_tuple, i);
		PyObject *p_key, *p_value = malloc(sizeof(PyObject *));
		Py_ssize_t pos = 0;
		while (PyDict_Next(dict, &pos, &p_key, &p_value)) {
			if (!PyUnicode_Check(p_key) || !PyUnicode_Check(p_value)) { // Values we can not check i think
				// Free for all temp headers object && keys
				printf("Dict Key ||& Value Is not string");
				goto T_ALLOCATED_MEM_FREE;
			}
			const char *t_key = PyUnicode_AsUTF8(p_key);
			const char *t_value = PyUnicode_AsUTF8(p_value);

			if (!t_key || !t_value) {
				printf("Convert to UTF_8 failed! \n");
				goto T_ALLOCATED_MEM_FREE;
			}

			t_headers[i1].name = *strdup(t_key);
			t_headers[i1].value = *strdup(t_value);
			if (!t_headers[i1].name || !t_headers[i1].value) {
				printf("Failed to get Dict name or value");
				goto T_ALLOCATED_MEM_FREE;
			}

			i1++;
		}
	}
	headers = &t_headers;
	outc = &total;

	T_ALLOCATED_MEM_FREE:
	for (size_t j = 0; j < i1; j++) {
		free(&t_headers[j].name);
		free(&t_headers[j].value);
	}
	free(t_headers);
}

static void parse_request_dictionary(PyObject *result, HTTP_CLIENT_REQUEST *request) {
	HTTP_CLIENT_REQUEST *temp = malloc(sizeof(HTTP_CLIENT_REQUEST));
	size_t count;
	PyObject *p_Method = PyDict_GetItemString(result, "Method");
	PyObject *p_Path = PyDict_GetItemString(result, "Path");
	PyObject *p_Protocol = PyDict_GetItemString(result, "Protocol");
	PyObject *p_Payload = PyDict_GetItemString(result, "Payload");

#pragma GCC diagnostic ignored "-Wno-write-strings"
	if (temp) {
		char *path = PyUnicode_AsUTF8(p_Path); // URL
		if (path) temp->model.addr = path;

		char *method = PyUnicode_AsUTF8(p_Method); // METHOD
		if (method) temp->model.method = method;

		char *protocol = PyUnicode_AsUTF8(p_Protocol); // PROTO_V
		if (protocol) temp->model.proto = protocol;

		char *body = PyUnicode_AsUTF8(p_Payload); // BODY
		if (body) temp->body = body;

		if (!result || PyDict_Check(result)) {
			printf("parse_request_headers failed; Value is NOT Dict\n");
		}

		PyObject *p_tuple = PyDict_GetItemString(result, "headers");
		if (!p_tuple) { // Check Header is exists
			PyErr_Print();
			printf("Header key not found");
		}

		if (!PyTuple_Check(p_tuple)) { // Check Header is equal pyTuple
			printf("Header value is not a tuple\n");
		}

		Py_ssize_t tuple_size = PyTuple_Size(p_tuple);
		size_t total = 0;
		for (Py_ssize_t i = 0; i < tuple_size; i++) {
			PyObject *dict = PyTuple_GetItem(p_tuple, i);
			if (!dict) {
				printf("Item is not a dict!\n");
			}
			total += PyDict_Size(dict);
		}

		HTTP_CORE_HEADER *headers = malloc(sizeof(HTTP_CORE_HEADER) * total);
		parse_request_headers(p_tuple, tuple_size, total, &headers, &count);

		// TODO: Finish return result via pointer
	}
}

__attribute__((noreturn))
extern void parse_request_client(char *body, HTTP_CLIENT_REQUEST *result) {
	// Firstly we should try initialize python in main.c
	// But if !init-ed we will try to initialize it manually
	if (!Py_IsInitialized()) {
		if (initialize_python() == 0) {
			// All good, we can proceed parsing request
		} else printf("Python initialization failed\n");
	}

	PyObject *p_module_name = PyUnicode_FromString("protocol/phttp");
	PyObject *p_module = PyImport_Import(p_module_name);

	Py_DECREF(p_module_name);

	if (p_module == NULL) {
		PyErr_Print();
		printf("Python module not found\n");
		Py_Finalize();
	}

	PyObject *p_parse_function = PyObject_GetAttrString(p_module, "parse_http_request");
	if (p_parse_function == NULL || !PyCallable_Check(p_parse_function)) {
		PyErr_Print();
		fprintf(stderr, "Python module not found\n");
		Py_XDECREF(p_parse_function);
		Py_DECREF(p_module);
		Py_Finalize();
	}

	PyObject *p_parse_str = PyUnicode_FromString(body);
	Py_DECREF(p_parse_str);

	PyObject *p_result = PyObject_CallObject(p_parse_function, p_parse_str);
	Py_DECREF(p_parse_str);

	if (p_result == NULL) {
		PyErr_Print();
		printf("Python module not found\n");
		Py_XDECREF(p_parse_function);
	}

	char *res = malloc(sizeof(char *));// PyStruct;
	// result = res; // TODO: We need to create Unified struct for Python and C types compability
	Py_DECREF(result);
	Py_Finalize();
}