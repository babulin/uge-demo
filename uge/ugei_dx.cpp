#include "ugei.h"

namespace uge {

	bool UGEI::_DxInit()
	{
		D3DADAPTER_IDENTIFIER9 ad_id;
		D3DDISPLAYMODE display_mode;
		D3DPRESENT_PARAMETERS d3dpp;

		//创建 D3D
		_d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if (_d3d == nullptr)
		{
			_SetErrMsg("初始化D3D失败!");
			return false;
		}

		// 获取适配器信息
		_d3d->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &ad_id);
		Log("D3D 驱动:%s", ad_id.Driver);
		Log("显卡描述: %s", ad_id.Description);
		Log("版本号: %d.%d.%d.%d",
			HIWORD(ad_id.DriverVersion.HighPart),
			LOWORD(ad_id.DriverVersion.HighPart),
			HIWORD(ad_id.DriverVersion.LowPart),
			LOWORD(ad_id.DriverVersion.LowPart));

		// 设置显示模式
		if (FAILED(_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode)) || display_mode.Format == D3DFMT_UNKNOWN)
		{
			_SetErrMsg("无法确定桌面视频模式");
			return false;
		}

		// 显示参数[窗口]
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.BackBufferWidth = screen_width;
		d3dpp.BackBufferHeight = screen_height;
		d3dpp.BackBufferFormat = display_mode.Format;   //D3DFMT_X8R8G8B8 (32位)
		d3dpp.BackBufferCount = 1;                      //后备缓冲区
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = true;
		d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		if (_z_buffer) {
			d3dpp.EnableAutoDepthStencil = TRUE;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		}

		// 创建D3D Device
		D3DCAPS9 caps;
		_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
		uint32_t vp;
		if ((caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
			|| !(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)) {
			Log("已选择软件顶点处理设备");
			vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}
		else {
			Log("已选择硬件顶点处理设备");
			vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		}

		if (FAILED(_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vp, &d3dpp, &_d3d_device)))
		{
			_SetErrMsg("无法创建D3D device!");
			return false;
		}

		Log("模式: %d * %d - %s", screen_width, screen_height, _FormatStr(display_mode.Format));

		// 初始化变换矩阵
		_SetProjectionMatrix();
		D3DXMatrixIdentity(&_view_matrix);

		//顶点缓存&索引缓存
		if (!_InitResource())
		{
			return false;
		}

		// 清屏
		//Clear(ugeColor32::WHITE());

		return true;
	}

	//+-----------------------------------
	//| 设置投影矩阵
	//+-----------------------------------
	void UGEI::_SetProjectionMatrix() {
		D3DXMATRIX tmp;
		D3DXMatrixScaling(&_proj_matrix, 1.0f, -1.0f, 1.0f);
		D3DXMatrixTranslation(&tmp, -0.5f, (float)screen_height + 0.5f, 0.0f);
		D3DXMatrixMultiply(&_proj_matrix, &_proj_matrix, &tmp);
		D3DXMatrixOrthoOffCenterLH(&tmp, 0, static_cast<float>(screen_width), 0, static_cast<float>(screen_height), 0.0f, 1.0f);
		D3DXMatrixMultiply(&_proj_matrix, &_proj_matrix, &tmp);
	}

    //+-----------------------------------
    //| 获取编码格式
    //+-----------------------------------
    const char* UGEI::_FormatStr(const D3DFORMAT fmt)
    {
        int type = 0;
        switch (fmt) {
        case D3DFMT_R5G6B5:
            type = 1;
            break;
        case D3DFMT_X1R5G5B5:
            type = 2;
            break;
        case D3DFMT_A1R5G5B5:
            type = 3;
            break;
        case D3DFMT_X8R8G8B8:
            type = 4;
            break;
        case D3DFMT_A8R8G8B8:
            type = 5;
            break;
        default:
            type = 0;
            break;
        }

        static const char* szFormats[] = {
        "FMT_UNKNOWN", "FMT_R5G6B5", "FMT_X1R5G5B5", "FMT_A1R5G5B5", "FMT_X8R8G8B8", "FMT_A8R8G8B8"
        };

        return szFormats[type];
    }

    //+-----------------------------------
    //| 初始化顶点缓存，索引缓存，渲染状态等
    //+-----------------------------------
    bool UGEI::_InitResource()
    {
        // 创建顶点缓存 [4个顶点一个正方形]
        if (FAILED(_d3d_device->CreateVertexBuffer(
            VERTEX_BUFFER_SIZE * sizeof(ugeVertex),
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            D3DFVF_UGEVERTEX,
            D3DPOOL_DEFAULT,
            &_vertex_buffer,
            nullptr
        )))
        {
            _SetErrMsg("Can't create D3D vertex buffer");
            return false;
        }
        // 顶点着色器
        _d3d_device->SetVertexShader(nullptr);
        // 设置灵活顶点格式
        _d3d_device->SetFVF(D3DFVF_UGEVERTEX);
        // 顶点数据输入源
        _d3d_device->SetStreamSource(0, _vertex_buffer, 0, sizeof(ugeVertex));


        // 创建顶点索引[4个顶点一个正方形需要6个索引]
        if (FAILED(_d3d_device->CreateIndexBuffer(
            VERTEX_BUFFER_SIZE / 4 * 6 * sizeof(uint16_t),
            D3DUSAGE_WRITEONLY,
            D3DFMT_INDEX16,
            D3DPOOL_DEFAULT,
            &_index_buffer,
            nullptr
        )))
        {
            _SetErrMsg("Can't create D3D index buffer");
            return false;
        }

        uint16_t* pIndices, n = 0;
        if (FAILED(_index_buffer->Lock(0, 0, (VOID**)&pIndices, 0))) {
            _SetErrMsg("Can't lock D3D index buffer");
            return false;
        }
        // 0 1  1 2
        // 3 2  0 3 
        for (int i = 0; i < VERTEX_BUFFER_SIZE / 4; i++) {
            *pIndices++ = n;        //0 4
            *pIndices++ = n + 1;    //1 5
            *pIndices++ = n + 2;    //2 6
            *pIndices++ = n + 2;    //2 6
            *pIndices++ = n + 3;    //3 7
            *pIndices++ = n;        //0 4
            n += 4;
        }

        _index_buffer->Unlock();
        _d3d_device->SetIndices(_index_buffer);

        // 设置渲染状态
        _d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);          //背面剔除 禁用
        _d3d_device->SetRenderState(D3DRS_LIGHTING, FALSE);                 //灯光 关闭

        _d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);          //打开Alpha渲染
        _d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);     //源混合要素
        _d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); //目标混合要素
                
        _d3d_device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);           //开启Alpha测试
        _d3d_device->SetRenderState(D3DRS_ALPHAREF, 0x01);                  //Alpha测试值
        _d3d_device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);  //大于等于成功

        _d3d_device->SetTransform(D3DTS_VIEW, &_view_matrix);
        _d3d_device->SetTransform(D3DTS_PROJECTION, &_proj_matrix);

        return true;
    }


    void UGEI::SetDrawMode(DrawMode dw) {

        if (dw == UGE_DW_NORMAL)
        {
            _d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);     //源混合要素
            _d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); //目标混合要素
        }
        else if (dw == UGE_DW_LIGHT) {
            //高光效果
            _d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
            _d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
        }
    }


    void UGEI::_BeginScene()
    {
        //创建三角形顶点
        _Clear(ugeColor32::GREY());
        _d3d_device->BeginScene();
        _n_prim = 0;

        //设置顶点
        _vertex_buffer->Lock(0, 0, reinterpret_cast<void**>(&_vert_array), D3DLOCK_DISCARD);
    }

    //+------------------------------------------
    //| 渲染图片
    //+------------------------------------------
    void UGEI::DxRenderQuad(ugeImage* image,bool fillMode)
    {
        ugeQuad quad;
        quad.blend = BLEND_COLORADD;
        quad.tex = image->tex;

        quad.v[0].x = image->x + image->px;
        quad.v[0].y = image->y + image->height + image->py;
        quad.v[0].z = 0.5f;
        quad.v[0].col = 0xFFFFFFFF;
        quad.v[0].tx = 0.0f;
        quad.v[0].ty = 1.0f;

        quad.v[1].x = image->x + image->px;
        quad.v[1].y = image->y + image->py;
        quad.v[1].z = 0.5f;
        quad.v[1].col = 0xFFFFFFFF;
        quad.v[1].tx = 0.0f;
        quad.v[1].ty = 0.0f;

        quad.v[2].x = image->x + image->width + image->px;
        quad.v[2].y = image->y + image->py;
        quad.v[2].z = 0.5f;
        quad.v[2].col = 0xFFFFFFFF;
        quad.v[2].tx = 1.0f;
        quad.v[2].ty = 0.0f;

        quad.v[3].x = image->x + image->width + image->px;
        quad.v[3].y = image->y + image->height + image->py;
        quad.v[3].z = 0.5f;
        quad.v[3].col = 0xFFFFFFFF;
        quad.v[3].tx = 1.0f;
        quad.v[3].ty = 1.0f;
        
        if (fillMode)
        {
            _d3d_device->SetTexture(0, nullptr);
            _d3d_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        }
        else {
            _d3d_device->SetTexture(0, reinterpret_cast<UgeTexture*>(quad.tex));
            _d3d_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        }

        memcpy(&_vert_array[_n_prim * UGEPRIM_QUADS], quad.v,sizeof(ugeVertex) * UGEPRIM_QUADS);
        _d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, _n_prim * 6, 2);
        _n_prim++;

    }

    //+------------------------------------------
    //| 渲染图片
    //+------------------------------------------
    void UGEI::DxRenderQuad(ugeAnimation* animation, bool fillMode)
    {
        if (_game_time_s - animation->time > animation->rate )
        {
            animation->curFrame++;
            animation->time = _game_time_s;
            if (animation->curFrame >= animation->total)
            {
                animation->curFrame = 0;
            }
        }
        animation->image[animation->curFrame].x = animation->x;
        animation->image[animation->curFrame].y = animation->y;

        DxRenderQuad(&animation->image[animation->curFrame], fillMode);
    }

    void UGEI::DxRenderQuad(ugeLine* line)
    {
        ugeVertex ver[] = {
            {line->x,line->y,0.5f,line->col,0.0f,0.0f},
            {line->x1,line->y1,0.5f,line->col,0.0f,0.0f},
        };

        _d3d_device->SetTexture(0, nullptr);
        memcpy(&_vert_array[_n_prim * UGEPRIM_QUADS], ver, sizeof(ugeVertex) * UGEPRIM_LINES);
        _d3d_device->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, 1, _n_prim * 6, 2);
        _n_prim++;
    }

    void UGEI::_EndScene()
    {
        _vertex_buffer->Unlock();
        _d3d_device->EndScene();
        _d3d_device->Present(nullptr, nullptr, nullptr, nullptr);
    }

    //+----------------------------
    //| 设置错误信息
    //+----------------------------
    void UGEI::_Clear(const ugeColor32 color)
    {
        if (_z_buffer) {
            _d3d_device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                color.argb, 1.0f, 0);
        }
        else {
            _d3d_device->Clear(0, nullptr, D3DCLEAR_TARGET, color.argb, 1.0f, 0);
        }
    }

    UTEXTURE UGEI::CreateWzlTexture(int tWidth, int tHeight, int pixelFormat, const RGBQUAD palette[256], byte* dstBuffer)
    {
        //纹理
        UgeTexture* p_tex;

        //变量
        D3DSURFACE_DESC _desc = {};//表面
        HRESULT hr = S_OK;

        ///创建贴图
        D3DFORMAT fmt = D3DFMT_A8R8G8B8;
        if (pixelFormat == 3)
        {
            fmt = D3DFMT_A8R8G8B8;
        }
        else if (pixelFormat == 5)
        {
            fmt = D3DFMT_R5G6B5;
        }

        _d3d_device->CreateTexture(tWidth, tHeight, 0, D3DUSAGE_DYNAMIC, fmt, D3DPOOL_DEFAULT, &p_tex, NULL);

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
        if (pixelFormat == 3)
        {
            imageData3 = (DWORD*)lockRect.pBits;
        }
        else if (pixelFormat == 5)
        {
            imageData5 = (SHORT*)lockRect.pBits;
        }

        for (UINT h = 0; h < height; h++)
        {
            for (UINT w = 0; w < width; w++)
            {
                if (pixelFormat == 3) {
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
                else if (pixelFormat == 5)
                {
                    //if (has16To32)
                    //{
                    //    //D3DFMT_R5G6B5
                    //    sort = ((h * width) + w) * 2;
                    //    BYTE sh1 = dstBuffer[sort];
                    //    BYTE sh2 = dstBuffer[sort + 1];
                    //    USHORT sVal = (sh2 << 8) | sh1;

                    //    //D3DFMT_A8R8G8B
                    //    BYTE r = (sVal & 0xF800) >> 8;
                    //    BYTE g = (sVal & 0x07E0) >> 3;
                    //    BYTE b = (sVal & 0x001F) << 3;
                    //    DWORD color = D3DCOLOR_ARGB(0xFF, r, g, b);

                    //    //图片数组 绘制从下往上
                    //    UINT index = (height - h) * lockRect.Pitch / 4 + w;
                    //    if (color != 0xFF000000)
                    //    {
                    //        imageData3[index] = D3DCOLOR_ARGB(0xFF, r, g, b);
                    //    }
                    //    else {
                    //        imageData3[index] = D3DCOLOR_ARGB(0x88, 0, 0, 0);
                    //    }
                    //}
                    {
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

        if (pixelFormat == 5) {

            //新建一个D3DFMT_A8R8G8B8
            IDirect3DSurface9* srcSurface;
            p_tex->GetSurfaceLevel(0, &srcSurface);

            UgeTexture* dstTexture;
            IDirect3DSurface9* dstSurface;
            _d3d_device->CreateTexture(tWidth, tHeight, 0, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &dstTexture, NULL);
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

    void UGE_CALL UGEI::ReleaseWzlTexture(UTEXTURE tex) {
        if (tex)
        {
            reinterpret_cast<UgeTexture*>(tex)->Release();
        }
    }
}