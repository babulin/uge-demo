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

        DxRenderQuad(&animation->image[animation->curFrame]);
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
}