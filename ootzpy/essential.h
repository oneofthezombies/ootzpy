#pragma once

/* Python.h must before any standard headers */
#ifdef _DEBUG // to use pythonxx.lib in debug mode
#undef _DEBUG   
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif


#pragma comment(lib, "python36.lib")


#include <memory> // for std::unique_ptr
