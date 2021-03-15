#pragma once

#include <Windows.h>

#define	UGE_VERSION 0x100
#define DX9_VERSION 0x09;

#include "ugedefs.h"
#include "ugehelp.h"
#include "ugecolor.h"

namespace uge {

	// 顶点结构体
	struct ugeVertex {
		float x, y; // screen position
		float z; // Z-buffer depth 0..1
		//float rhw;
		//uint32_t col; // color
		float tx, ty; // texture coordinates
	};

	//wzx头部结构
	struct WzxHeader {
		char description[20];	//www.shandagames.com
		int x1[6];				//占位
		int imageCount;			//图片数量
	};

	//Wzl头部结构
	struct WzlHeader {
		char description[44];	//www.shandagames.com
		int imageCount;			//图片数量
		int x3[4];				//占位
	};

	//Wzl图片信息结构
	struct WzlBmpInfo {
		BYTE pixelFormat;	//图片位深
		BYTE compressed;	//表示图片数据是否经过gzip压缩
		BYTE reserve;		//占位
		BYTE compressLevel; //如果图片数据是压缩过，这个就表示压缩的等级
		short width;		//图片宽度
		short height;		//图片高度
		short x;			//偏移x
		short y;			//偏移y
		int size;			//图片数据长度[压缩后的]
	};

	// 回调函数类型
	typedef bool (*ugeCallback)();

	// 应用程序
	class Game {
	public:
		Game() {}
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
	};

	// 图像接口
	class Graphics {
	public:
		virtual ~Graphics() = default;
		virtual bool UGE_CALL Initiate() = 0;
		virtual void UGE_CALL Clear(const ugeColor32 color) = 0;
		virtual void UGE_CALL Render() = 0;
		virtual std::string GetErrMsg() = 0;

	};

	extern "C" {
		UGE_EXPORT UGE* UGE_CALL ugeCreate(const int ver);
	}
}

