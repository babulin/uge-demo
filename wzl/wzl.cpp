#include "wzl.h"
#include "help.h"

namespace wzl {

    Wzl::Wzl(const char* path)
    {
        this->path = path;
        wzxData = nullptr;
        wzxSize = 0;
        wzxHead = {};

        wzlData = nullptr;
        wzxSize = 0;
        wzlHead = {};

        _LoadWzx();
        _LoadWzl();

        _wzlTexMap = new UWzlTexMap();
    }

    Wzl::~Wzl()
    {
        if (wzxData)
        {
            free(wzxData);
        }

        if (wzlData)
        {
            free(wzlData);
        }

        if (!_wzlTexMap->empty())
        {
            for (UWzlTexMap::iterator it = _wzlTexMap->begin(); it != _wzlTexMap->end(); it++)
            {
                delete it->second;
            }
        }
        if (_wzlTexMap)
        {
            delete _wzlTexMap;
            _wzlTexMap = nullptr;
        }
    }

    int Wzl::_GetOffset(int sort)
    {
        //读取第一个
        int offset = 0;
        memcpy_s(&offset, wzxSize, reinterpret_cast<byte*>(wzxData) + sizeof(wzxHead) + (sort * sizeof(int)), sizeof(offset));
        if (offset == 48 || offset == 0)
        {
            return 0;
        }
        return offset;
    }

    byte* Wzl::GetBmp(int sort, WzlBmpInfo* Wzl, int* dstSize)
    {
        //获取图片序号sort的偏移值offset
        int offset = _GetOffset(sort);
        if (!offset)
        {
            return nullptr;
        }
        //读取第一个
        WzlBmpInfo* _Wzl = new WzlBmpInfo();
        memcpy_s(_Wzl, wzlSize, reinterpret_cast<byte*>(wzlData) + offset, sizeof(WzlBmpInfo));

        //读取数据
        byte* srcBuffer = new byte[_Wzl->size];
        memcpy_s(srcBuffer, wzlSize, reinterpret_cast<byte*>(wzlData) + offset + sizeof(WzlBmpInfo), _Wzl->size);

        //图片实际大小 (5 = 16位图 D3DFMT_R5G6B5[2倍数据大小对齐])(3 = 32位图 D3DFMT_A8R8G8B8)
        ULONG _dstSize = _Wzl->width * _Wzl->height * (_Wzl->pixelFormat == 5 ? 2 : 1);

        //解压数据
        byte* dstBuffer = new byte[_dstSize];

        //获得位图数据[与调色板对应的位置数据] //这句是，对909 的所有数据进行解压，得到的位图数据
        uncompress(dstBuffer, &_dstSize, srcBuffer, _Wzl->size);

        //清除源数据缓冲区
        delete[] srcBuffer;

        if (Wzl)
        {
            *dstSize = _dstSize;
            *Wzl = *_Wzl;
        }

        return dstBuffer;
    }

    WzlTexture* Wzl::GetTextureCache(int sort)
    {
        //加载纹理
        UWzlTexMap::const_iterator curWzlTexMap = _wzlTexMap->find(sort);
        if (curWzlTexMap == _wzlTexMap->end())
        {
            return nullptr;
        }

        //引用次数加1
        _wzlTexMap->at(sort)->quote++;

        return curWzlTexMap->second;
    }

    bool Wzl::SetTextureCache(WzlTexture* tex)
    {
        _wzlTexMap->insert(std::pair<int, WzlTexture*>(tex->sort, tex));
        return true;
    }

    bool Wzl::ReleaseTexture(int sort, bool* hasErase)
    {
        UWzlTexMap::const_iterator t_CurTex = _wzlTexMap->find(sort);
        if (t_CurTex == _wzlTexMap->end())
        {
            return false;
        }

        //找到
        t_CurTex->second->quote--;
        if (t_CurTex->second->quote <= 0)
        {
            //reinterpret_cast<UgeTexture*>(t_CurTex->second->tex)->Release();
            delete t_CurTex->second;
            _wzlTexMap->erase(sort);
        }

        //检查是否无纹理
        if (_wzlTexMap->empty())
        {
            delete this;
            *hasErase = true;
        }

        return true;
    }

    //+-----------------------------------
    //| 加载wzx
    //+-----------------------------------
    bool Wzl::_LoadWzx()
    {
        // wzx路径
        static char wzx_path[MAX_PATH];
        strcpy_s(wzx_path, path);
        strcat_s(wzx_path, ".wzx");

        //读取wzx
        wzxData = open_file(wzx_path, &wzxSize);
        if (wzxData == nullptr) {
            return false;
        }

        //读取头部
        memcpy_s(&wzxHead, wzxSize, wzxData, sizeof(wzxHead));

        return false;
    }


    //+-----------------------------------
    //| 加载wzl
    //+-----------------------------------
    bool Wzl::_LoadWzl()
    {
        // wzl路径
        static char wzl_path[MAX_PATH];
        strcpy_s(wzl_path, path);
        strcat_s(wzl_path, ".wzl");

        //读取wzl
        wzlData = open_file(wzl_path, &wzlSize);
        if (wzlData == nullptr) {
            return false;
        }

        //读取头部
        memcpy_s(&wzlHead, wzlSize, wzlData, sizeof(wzlHead));

        return true;
    }

}

