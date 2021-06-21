#pragma once
#include "defs.h"
#include "wzl.h"
namespace wzl {

	// wzl MapÈÝÆ÷
	typedef std::unordered_map<std::string, Wzl*> UWzlMap;

	class WZL_EXPORT WzlCache{
	public:
		WzlCache(uge::UGE* pUge);
		~WzlCache();
		bool LoadWzl(const char* path, int sort, ugeImage* image);
		bool LoadWzl(const char* path, int sort, int total, ugeAnimation* animation,int offset = 1);
		//UTEXTURE CreateWzlTexture(wzl::WzlBmpInfo wzlBmp, byte* dstBuffer, bool has16To32 = false);
		bool ReleaseWzl(ugeImage* image);
		bool ReleaseWzl(ugeAnimation* animation);
	private:
		Wzl* _GetWzlCache(const char* path);
		bool _ReleaseWzlCache(const char* path, int sort);
	private:
		UWzlMap* _wzlMap;				//wzl×ÊÔ´ÈÝÆ÷
		uge::UGE* pUge;
	};
}