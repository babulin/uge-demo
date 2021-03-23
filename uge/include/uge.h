#pragma once

#include <Windows.h>

#define	UGE_VERSION 0x100
#define DX9_VERSION 0x09;

#include "ugedefs.h"
#include "ugehelp.h"
#include "ugecolor.h"

namespace uge {

	// 纹理资源
	struct ugeImage{
		float x;		//坐标x
		float y;		//坐标y
		int width;		//图片宽
		int height;		//图片高
		UTEXTURE tex;	//纹理地址
		char path[_MAX_DIR];
		int sort;
	};

	// 回调函数类型
	typedef bool (*ugeCallback)();

	// 应用程序
	class Game {
	public:
		Game() {}
		virtual ~Game() = default;
		virtual bool Config() = 0;
		virtual bool Initiate() = 0;
		virtual bool Update() = 0;
		virtual bool Show() = 0;
	};

	Game* gameCreate();

	// 引擎接口
	class UGE {
	public:
		virtual ~UGE() = default;
		virtual void UGE_CALL SetFrameCallback(const ugeCallback value) = 0;
		virtual void UGE_CALL SetUpdateCallback(const ugeCallback value) = 0;
		virtual void UGE_CALL SetGame(Game* game) = 0;
		virtual void UGE_CALL SetScreen(int width,int height) = 0;
		virtual void UGE_CALL SetTitle(const char* title) = 0;
		virtual void UGE_CALL SetWindowed(bool value) = 0;
		virtual bool UGE_CALL Initiate() = 0;
		virtual bool UGE_CALL Start() = 0;
		virtual void UGE_CALL Release() = 0;
		virtual std::string GetErrMsg() = 0;

		virtual bool LoadTexture(const char* filename, bool bMipmap = false) = 0;
		virtual bool LoadWzl(const char* path, int sort, ugeImage* image) = 0;
		virtual bool ReleaseWzl(ugeImage* image) = 0;
		virtual void DxRenderQuad(ugeImage* image) = 0;
	};

	extern "C" {
		UGE_EXPORT UGE* UGE_CALL ugeCreate(const int ver);
	}
}

