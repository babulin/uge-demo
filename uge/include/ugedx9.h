#pragma once
#include "uge.h"
#include "ugeapi.h"
#include <string>

namespace uge {

	//灵活顶点格式
	//const int D3DFVF_UGEVERTEX = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	const int D3DFVF_UGEVERTEX = (D3DFVF_XYZ | D3DFVF_TEX1);
	const int VERTEX_BUFFER_SIZE = 40;

	class UGEDX9 : public Graphics {
		public:
			virtual bool Initiate() override;						//初始化
			virtual void Clear(const ugeColor32 color) override;	//清屏
			virtual void Render() override;							//渲染
			virtual std::string GetErrMsg() override;				//获取错误信息
		public:
			UGEDX9(HWND hwnd, int width, int height);
			~UGEDX9();
		private:
			void SetErrMsg(const char* error);						//设置错误信息
			void SetProjectionMatrix();								//设置矩阵
			bool InitResource();									//初始化资源
			const char* format_str(const D3DFORMAT fmt);			//获取FMT格式文案
		private:
			std::string err_msg;			//错误信息

			HWND _hwnd;						//窗口句柄
			int _width;						//屏幕宽
			int _height;					//屏幕高
			bool _z_buffer;					//是否开启z缓冲

			UgeIDirect3D *_d3d;				//d3d对象
			UgeDevice * _d3d_device;		//d3d设备

			D3DXMATRIX _proj_matrix;		//投影矩阵
			D3DXMATRIX _view_matrix;		//视口矩阵

			UgeVertexBuffer* _vertex_buffer;//顶点缓存
			UgeIndexBuffer* _index_buffer;	//索引缓存
			ugeVertex* _vert_array;			//顶点缓冲数组

			
			UgeTexture* _texture1;			//纹理
			UgeTexture* _texture2;			//纹理
			UgeTexture* _texture3;			//纹理
			UgeTexture* _texture4;			//纹理
	};
}