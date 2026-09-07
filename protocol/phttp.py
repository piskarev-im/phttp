import re
from multiprocessing.managers import Array
from typing import Dict, Any
from wsgiref import headers

# Test string (is a GET request from client
http_req = """
GET /wiki/страница HTTP/1.1\n
Host: ru.wikipedia.org\n
User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\n
Accept: text/html\n
Connection: close\n
\r\n\r\n
<Body>
"""

# Output format | unused
http_client_req = {
	"Method": "GET",
	"Path": "ru.wikipedia.org",
	"Protocol": "HTTP/1.1",
	"Headers" : [
		{ "Content-Type": "text/html" },
		{ "Content-Length": 1002 },
	],
	"Payload" : [ "Buffer" ]
}

http_req_pattern = re.compile(r'^https?:\/\/(?:www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,63}\b(?:[-a-zA-Z0-9()@:%_\+.~#?&\/=]*)$')
http_protov_pattern = re.compile(r'HTTP+\/+\d{1}.(\d{1}||[x])$')

def is_http_method(method) -> bool:
	return method.upper() == 'GET' or method.upper() == 'POST' or  method.upper() == 'PUT' or method.upper() == 'DELETE' or method.upper() == 'HEAD'

def is_http_path(path):
	return re.match(http_req_pattern, path)

def is_http_protov(path):
	return re.match(http_protov_pattern, path)

# Parsing HTTP Headers from [1] : {Body}
def parse_http_header(raw_headers: tuple[str]) -> tuple[dict[str, str]]:
	headers = []
	print(raw_headers)
	for line in raw_headers:
		line = line.split(':') #Must be 2
		if len(line) >= 2:
			header = dict(header_name=line[0], header_val=line[1]) # Header
			headers.append(header)
		else:
			continue

	return headers

def build_client_request(method: str, path: str, protov: str, headers: Dict[str, str], payload: str) -> Dict[str, Any]:
	return {
		"Method": method,
		"Path": path,
		"Protocol": protov,
		"Headers": headers,
		"Payload": payload
	}

def resolve_first_line(line: str) -> Dict[str, str]:
	return {
		"Method": line[0],
		"Path": line[1],
		"Protocol": line[2],
	}

def build_server_request():
	return {}

# Parsing full http request
def parse_http_request(parse) -> dict[str, Any]:
	temp = http_req.split('\r\n\r\n')
	req_info = temp[0]
	req_type = 0 # 0 - Client ; 1 - Server
	req_data = temp[1]
	req_info = req_info.split('\n')
	req_info = [x for x in req_info if x != ''] # Filter array from \n \0 character
	method, path, protov = '', '', ''; # 1st line, method, proto, path
	print(req_info)
	request_main_data = req_info[0]
	request_main_data = request_main_data.split(' ')

	if is_http_method(request_main_data[0]):
		req_type = 0
		d = resolve_first_line(request_main_data)
		method = d['Method']
		path = d['Path']
		protov = d['Protocol']
	else:
		req_type = 1

	headers = parse_http_header(req_info)

	out = {}
	if req_type == 0:
		out = build_client_request(method, path, protov, headers, req_data)
	elif req_type == 1:
		out = build_client_request(method, path, protov, headers, req_data) # change

	print(out)

	return out