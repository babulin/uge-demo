#include "wzlcache.h"
#include "help.h"

namespace wzl {

	WzlCache::WzlCache(uge::UGE *pUge)
	{
        this->pUge = pUge;
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

	Wzl* WzlCache::_GetWzlCache(const char* path)
	{
		//获取名称
		std::string name = get_file_name(path);

		//查找缓存
		UWzlMap::const_iterator curWzlMap = _wzlMap->find(name);

        //是否已加载
		if (curWzlMap == _wzlMap->end())
		{
			//加载文件
			Wzl* t_wzl = new Wzl(path,pUge);
			
			if ( ! t_wzl->Init()) {
				return nullptr;
			}

            //缓存
			_wzlMap->insert(std::pair<std::string, Wzl*>(name, t_wzl));
			curWzlMap = _wzlMap->find(name);
		}

		return curWzlMap->second;
	}

	bool WzlCache::LoadWzl(const char* path, int sort, ugeImage* image)
	{
		// 从资源获取缓存
		Wzl* t_wzl = _GetWzlCache(path);
		if (t_wzl == nullptr)
		{
			return false;
		}

		// 从缓存获取纹理
		WzlBmpTex* wzlTex = t_wzl->GetTextureCache(sort);
		if (wzlTex == nullptr)
		{
            return false;
		}

		// 传出参数
		strcpy_s(image->path, path);
		image->sort = sort;
		image->x = 0;
		image->y = 0;
		image->px = wzlTex->wzlBmpInfo.x;
		image->py = wzlTex->wzlBmpInfo.y;
		image->width = wzlTex->wzlBmpInfo.width;
		image->height = wzlTex->wzlBmpInfo.height;
		image->tex = wzlTex->tex;

		return true;
	}

    bool WzlCache::LoadWzl(const char* path, int sort, int total, ugeAnimation* animation)
    {
        // 从缓存获取纹理
        for (int i = 0; i < total; i++)
        {
            int t_sort = sort + i;
			if (LoadWzl(path, t_sort, &animation->image[i])) {

			}
        }

        animation->total = total;
        animation->curFrame = 0;
        animation->time = 0;
        animation->rate = 7.0f / 60.0f;

        return true;
    }

	bool WzlCache::_ReleaseWzlCache(const char* path, int sort)
	{
		//获取名称
		std::string name = get_file_name(path);

		//寻找元素
		UWzlMap::const_iterator t_CurWzl = _wzlMap->find(name);
		if (t_CurWzl == _wzlMap->end())
		{
			return true;
		}

		//找到
		bool erase = false;
		if (!_wzlMap->at(name)->ReleaseTexture(sort, &erase)) {
			return true;
		}

		//是否需要移除资源
		if (erase)
		{
			//删除缓存
			delete _wzlMap->at(name);
			//移除
			_wzlMap->erase(name);
		}

		return true;
	}

    bool WzlCache::ReleaseWzl(ugeImage* image) {
        if (_ReleaseWzlCache(image->path, image->sort)) {

        }
        return true;
    }

    bool WzlCache::ReleaseWzl(ugeAnimation* animation) {
        for (int i = 0; i < animation->total; i++)
        {
			if (ReleaseWzl(&animation->image[i]))
			{

            }
        }
        return true;
    }
}