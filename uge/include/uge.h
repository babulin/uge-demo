#pragma once

#include <Windows.h>

#define	UGE_VERSION 0x100
#define DX9_VERSION 0x09;

#include "ugedefs.h"
#include "ugehelp.h"
#include "ugecolor.h"
#include "ugekeys.h"
#include "ugegame.h"

namespace uge {

	// 纹理
	using UTEXTURE = uintptr_t;

	// 划线
	struct ugeLine {
		float x;		//坐标x
		float y;		//坐标y
		float x1;		//偏移x
		float y1;		//偏移y
		uint32_t col;	//颜色
	};

	// 纹理资源
	struct ugeImage{
		float x;		//坐标x
		float y;		//坐标y
		int px;		//偏移x
		int py;		//偏移y
		int width;		//图片宽
		int height;		//图片高
		UTEXTURE tex;	//纹理地址
		char path[_MAX_DIR];
		int sort;
	};

	// 动画资源
	struct ugeAnimation {
		float x;			//坐标x
		float y;			//坐标y
		int total;			//总帧数
		int curFrame;		//当前帧
		float rate;			//播放帧率
		float time;			//前帧时间
		ugeImage image[8];	//图片集合
	};

	// 回调函数类型
	typedef bool (*ugeCallback)();

	// 引擎接口
	class UGE {
	public:
		virtual ~UGE() = default;
		virtual void UGE_CALL SetFrameCallback(const ugeCallback value) = 0;
		virtual void UGE_CALL SetUpdateCallback(const ugeCallback value) = 0;
		virtual void UGE_CALL SetGame(ugeGame* game) = 0;
		virtual void UGE_CALL SetScreen(int width,int height) = 0;
		virtual void UGE_CALL GetScreen(int* width,int* height) = 0;
		virtual void UGE_CALL SetTitle(const char* title) = 0;
		virtual void UGE_CALL SetWindowed(bool value) = 0;
		virtual bool UGE_CALL Initiate() = 0;
		virtual bool UGE_CALL Start() = 0;
		virtual void UGE_CALL Release() = 0;
		virtual std::string GetErrMsg() = 0;
		
		//提供给wzl.dll
		virtual UTEXTURE UGE_CALL CreateWzlTexture(int width,int height,int pixelFormat,const RGBQUAD palette[256], byte* dstBuffer) = 0;
		virtual void UGE_CALL ReleaseWzlTexture(UTEXTURE tex) = 0;

		//渲染
		virtual bool LoadTexture(const char* filename, bool bMipmap = false) = 0;
		virtual void DxRenderQuad(ugeImage* image, bool fillMode = false) = 0;
		virtual void DxRenderQuad(ugeAnimation* animation, bool fillMode = false) = 0;
		virtual void DxRenderQuad(ugeLine* line) = 0;

		//按键
		virtual void GetMousePos(float *x,float *y) = 0;
		virtual bool KeyDown(const int key) = 0;
		virtual bool KeyUp(const int key) = 0;
		virtual bool KeyState(const int key) = 0;
	};

	ugeGame* gameCreate(UGE* _uge);

	extern "C" {
		UGE_EXPORT UGE* UGE_CALL ugeCreate(const int ver);
	}

}


