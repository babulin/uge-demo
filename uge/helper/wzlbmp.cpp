#include "wzlbmp.h"

namespace uge {

    WzlBmp::WzlBmp(const char* filename, UgeDevice* d3dDevice)
	{
        this->d3d_device = d3dDevice;
        this->filename = filename;
        wzxData = nullptr;
        wzxSize = 0;
        wzxHead = {};

        wzlData = nullptr;
        wzxSize = 0;
        wzlHead = {};

        _LoadWzx();
        _LoadWzl();
	}

    WzlBmp::~WzlBmp()
    {
        if (wzxData)
        {
            free(wzxData);
        }

        if (wzlData)
        {
            free(wzlData);
        }

        Log("wzlbmp:free 数据");
    }

    int WzlBmp::_GetOffset(int sort)
    {
        //读取第一个
        int offset = 0;
        memcpy_s(&offset, wzxSize, reinterpret_cast<byte*>(wzxData) + sizeof(wzxHead) + (sort * sizeof(int)), sizeof(offset));

        return offset;
    }

    byte* WzlBmp::_GetBmp(int sort,WzlBmpInfo* wzlBmp, int* dstSize)
    {
        //获取图片序号sort的偏移值offset
        int offset = _GetOffset(sort);

        //读取第一个
        WzlBmpInfo* _wzlBmp = new WzlBmpInfo();
        memcpy_s(_wzlBmp, wzlSize, reinterpret_cast<byte*>(wzlData) + offset, sizeof(WzlBmpInfo));

        //读取数据
        byte* srcBuffer = new byte[_wzlBmp->size];
        memcpy_s(srcBuffer, wzlSize, reinterpret_cast<byte*>(wzlData) + offset + sizeof(WzlBmpInfo), _wzlBmp->size);

        //图片实际大小 (5 = 16位图 D3DFMT_R5G6B5[2倍数据大小对齐])(3 = 32位图 D3DFMT_A8R8G8B8)
        ULONG _dstSize = _wzlBmp->width * _wzlBmp->height * (_wzlBmp->pixelFormat == 5 ? 2 : 1);

        //解压数据
        byte* dstBuffer = new byte[_dstSize];

        //获得位图数据[与调色板对应的位置数据] //这句是，对909 的所有数据进行解压，得到的位图数据
        uncompress(dstBuffer, &_dstSize, srcBuffer, _wzlBmp->size);

        //清除源数据缓冲区
        delete[] srcBuffer;

        if (wzlBmp)
        {
            *dstSize = _dstSize;
            *wzlBmp = *_wzlBmp;
        }

        return dstBuffer;
    }

    //+-----------------------------------
    //| 加载wzx
    //+-----------------------------------
    bool WzlBmp::_LoadWzx()
    {
        // wzx路径
        static char wzx_path[MAX_PATH];
        strcpy_s(wzx_path, filename);
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
    bool WzlBmp::_LoadWzl()
    {
        // wzl路径
        static char wzl_path[MAX_PATH];
        strcpy_s(wzl_path, filename);
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



    //+-----------------------------------
    //| 通过wzl创建纹理
    //+-----------------------------------
    UTEXTURE WzlBmp::LoadTexture(WzlBmpInfo wzlBmp,byte* dstBuffer, bool has16To32)
    {
        //获取纹理数据
        UgeTexture* p_tex;

        //变量
        D3DSURFACE_DESC _desc = {};//表面
        HRESULT hr = S_OK;

        ///创建贴图
        D3DFORMAT fmt = D3DFMT_A8R8G8B8;
        if (wzlBmp.pixelFormat == 3)
        {
            fmt = D3DFMT_A8R8G8B8;
        }
        else if (wzlBmp.pixelFormat == 5)
        {
            fmt = D3DFMT_R5G6B5;
            if (has16To32)
            {
                fmt = D3DFMT_A8R8G8B8;
            }
        }

        d3d_device->CreateTexture(wzlBmp.width, wzlBmp.height, 0, D3DUSAGE_DYNAMIC, fmt, D3DPOOL_DEFAULT, &p_tex, NULL);

        //获取表面信息
        p_tex->GetLevelDesc(0, &_desc);

        ///表面数据
        D3DLOCKED_RECT lockRect;
        p_tex->LockRect(0, &lockRect, 0, 0);

        int sort = 0;
        int index = 0;
        UINT width = _desc.Width;
        UINT height = _desc.Height;

        DWORD* imageData3 = nullptr;
        SHORT* imageData5 = nullptr;
        if (wzlBmp.pixelFormat == 3)
        {
            imageData3 = (DWORD*)lockRect.pBits;
        }
        else if (wzlBmp.pixelFormat == 5)
        {
            imageData5 = (SHORT*)lockRect.pBits;
            if (has16To32)
            {
                imageData3 = (DWORD*)lockRect.pBits;
                if (width > height)
                {
                    width = width + 1;
                    height = height - 1;
                }
            }
        }

        //获取调色板
        //OpenRGB();

        for (UINT h = 0; h < height; h++)
        {
            for (UINT w = 0; w < width; w++)
            {
                if (wzlBmp.pixelFormat == 3) {
                    //数据 第一行 是图片的最后一行 数据从上往下读取
                    //D3DFMT_A8R8G8B8
                    sort = h * width + w;
                    BYTE  r = palette[dstBuffer[sort]].rgbRed;
                    BYTE  g = palette[dstBuffer[sort]].rgbGreen;
                    BYTE  b = palette[dstBuffer[sort]].rgbBlue;
                    DWORD color = D3DCOLOR_ARGB(0xff, r, g, b);
                    //index = (height - h) * width + w;

                    //图片数组 绘制从下往上
                    UINT index = (height - 1 - h) * lockRect.Pitch / 4 + w;
                    if (color != 0xff000000)
                    {
                        imageData3[index] = D3DCOLOR_ARGB(0xff, r, g, b);
                    }
                    else {
                        imageData3[index] = D3DCOLOR_ARGB(0, 0, 0, 0);
                    }
                }
                else if (wzlBmp.pixelFormat == 5)
                {
                    if (has16To32)
                    {
                        //D3DFMT_R5G6B5
                        sort = ((h * width) + w) * 2;
                        BYTE sh1 = dstBuffer[sort];
                        BYTE sh2 = dstBuffer[sort + 1];
                        USHORT sVal = (sh2 << 8) | sh1;

                        //D3DFMT_A8R8G8B
                        BYTE r = (sVal & 0xF800) >> 8;
                        BYTE g = (sVal & 0x07E0) >> 3;
                        BYTE b = (sVal & 0x001F) << 3;
                        DWORD color = D3DCOLOR_ARGB(0xFF, r, g, b);

                        //图片数组 绘制从下往上
                        UINT index = (height - h) * lockRect.Pitch / 4 + w;
                        if (color != 0xFF000000)
                        {
                            imageData3[index] = D3DCOLOR_ARGB(0xFF, r, g, b);
                        }
                        else {
                            imageData3[index] = D3DCOLOR_ARGB(0x88, 0, 0, 0);
                        }
                    }
                    else {
                        //D3DFMT_R5G6B5
                        sort = (h * width + w) * 2;
                        BYTE sh1 = dstBuffer[sort];
                        BYTE sh2 = dstBuffer[sort + 1];
                        USHORT sVal = (sh2 << 8) | sh1;

                        index = (height - 1 - h) * lockRect.Pitch / 2 + w;
                        imageData5[index] = sVal;
                    }

                }
            }
        }

        //解锁
        p_tex->UnlockRect(0);


        if (wzlBmp.pixelFormat == 5 && !has16To32) {

            //新建一个D3DFMT_A8R8G8B8
            IDirect3DSurface9* srcSurface;
            p_tex->GetSurfaceLevel(0, &srcSurface);

            UgeTexture* dstTexture = nullptr;
            IDirect3DSurface9* dstSurface;
            hr = d3d_device->CreateTexture(wzlBmp.width, wzlBmp.height, 0, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &dstTexture, NULL);
            dstTexture->GetSurfaceLevel(0, &dstSurface);

            D3DXLoadSurfaceFromSurface(
                dstSurface,
                NULL, NULL,
                srcSurface,
                NULL, NULL,
                D3DX_FILTER_LINEAR,
                D3DCOLOR_ARGB(0xFF, 0, 0, 0)
            );
            D3DSURFACE_DESC dstDesc;
            dstSurface->GetDesc(&dstDesc);

            //释放之前的
            p_tex->Release();
            p_tex = dstTexture;
        }

        return reinterpret_cast<UTEXTURE>(p_tex);
    }
}

