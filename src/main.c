#include <Python.h>
#include <stdio.h>

int main() {
	Py_Initialize();

	if (!Py_IsInitialized()) {
		printf("Python initialization failed\n");
		return 1;
	}

	// Add PATH variable to let python use this directory
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('../protocol/')");

	return 0;
}
