#pragma once
#include "defs.h"
#include "wzl.h"

namespace wzl {

	// wzl MapÈÝÆ÷
	typedef std::unordered_map<std::string, Wzl*> UWzlMap;

	class WZL_EXPORT WzlCache{
	public:
		WzlCache();
		~WzlCache();
		Wzl* GetWzlCache(const char* path);
		bool ReleaseWzlCache(const char* path, int sort);
	private:
		UWzlMap* _wzlMap;				//wzl×ÊÔ´ÈÝÆ÷
	};
}