#pragma once
#include "defs.h"
#include <stdarg.h>
#include <cstdio>
namespace wzl {
	void Log(const char* format, ...);
	void* open_file(const char* filename, int* size);
	const char* resoure_path(const char* filename);
	std::string get_file_name(const char* filename);
}