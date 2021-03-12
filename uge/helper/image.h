#pragma once
#include <ugei.h>

namespace uge {

	class image {
	private:
		const char* file;
	public:
		image(const char* filename);
	};
}