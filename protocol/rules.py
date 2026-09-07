import re
from typing import Dict, Any, List
from wsgiref import headers

#Here we will parse http headers, and will return result as array (tuple)

def filter_arr(strg: list[str]) -> list[str]:
	return [x for x in strg if x != '']

def content_type_allowed(params: str) -> List:
  params = params.split(';')
	result = []
	content_type = params[0]
	additional_params = params[1:]
	content_types = content_type.split('/')
	for i in range(--len(content_types) + --len(additional_params)):
		if i > len(content_type):
			result.append(additional_params[i])
		else:
			result.append(content_types[i])
	return result

def content_disposition_allowed(p: str) -> List:
	params = p.split(';')
	result = []
	for i in range(len(params) - 1):
		if params[i].__contains__('='):
			params[i] = params[i].split('=')
			result.append(params[i])
		else:
			result.append(params[i])
	return result

def