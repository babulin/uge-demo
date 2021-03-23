#pragma once
#include "wzlbmp.h"
#include <unordered_map>
#include <algorithm>

namespace uge {

	// wzl MapÈÝÆ÷
	typedef std::unordered_map<std::string, WzlBmp*> UWzlMap;

	class UgeResource {
	public:
		UgeResource();
		~UgeResource();
		WzlBmp* GetWzlCache(const char* path);
		bool ReleaseWzlCache(const char* path, int sort);
	private:
		UWzlMap _wzlMap;				//wzl×ÊÔ´ÈÝÆ÷
	};
}