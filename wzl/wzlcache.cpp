#include "wzlcache.h"
#include "help.h"

namespace wzl {

	WzlCache::WzlCache()
	{
		_wzlMap = new UWzlMap();
	}

	WzlCache::~WzlCache()
	{

		if (!_wzlMap->empty())
		{
			for (UWzlMap::iterator it = _wzlMap->begin(); it != _wzlMap->end(); it++)
			{
				delete it->second;
			}
		}

		if (_wzlMap)
		{
			delete _wzlMap;
			_wzlMap = nullptr;
		}
	}

	Wzl* WzlCache::GetWzlCache(const char* path)
	{
		//获取名称
		std::string name = get_file_name(path);

		//加载wzl资源
		UWzlMap::const_iterator curWzlMap = _wzlMap->find(name);
		if (curWzlMap == _wzlMap->end())
		{
			//加载资源到内存
			Wzl* bmp = new Wzl(path);

			_wzlMap->insert(std::pair<std::string, Wzl*>(name, bmp));
			curWzlMap = _wzlMap->find(name);
		}

		return curWzlMap->second;
	}

	bool WzlCache::ReleaseWzlCache(const char* path, int sort)
	{
		//获取名称
		std::string name = get_file_name(path);

		//寻找元素
		UWzlMap::const_iterator t_CurWzl = _wzlMap->find(name);
		if (t_CurWzl == _wzlMap->end())
		{
			return false;
		}

		//找到
		bool erase = false;
		if (!_wzlMap->at(name)->ReleaseTexture(sort, &erase)) {
			return false;
		}

		//是否需要移除资源
		if (erase)
		{
			//删除缓存
			_wzlMap->erase(name);
		}

		return true;
	}
}