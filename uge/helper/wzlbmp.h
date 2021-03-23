#pragma once
#include <uge.h>
#include "ugeapi.h"
#include "zlibwapi.h"
#include <unordered_map>
#include <algorithm>

namespace uge {

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

	//8位图 256调色板
	const RGBQUAD palette[256] = {
			RGBQUAD{0, 0, 0},
			RGBQUAD{0, 0, 128},
			RGBQUAD{0, 128, 0},
			RGBQUAD{0, 128, 128},
			RGBQUAD{128, 0, 0},
			RGBQUAD{128, 0, 128},
			RGBQUAD{128, 128, 0},
			RGBQUAD{192, 192, 192},
			RGBQUAD{151, 128, 85},
			RGBQUAD{200, 185, 157},
			RGBQUAD{115, 115, 123},
			RGBQUAD{41, 41, 45},
			RGBQUAD{82, 82, 90},
			RGBQUAD{90, 90, 99},
			RGBQUAD{57, 57, 66},
			RGBQUAD{24, 24, 29},
			RGBQUAD{16, 16, 24},
			RGBQUAD{24, 24, 41},
			RGBQUAD{8, 8, 16},
			RGBQUAD{113, 121, 242},
			RGBQUAD{95, 103, 225},
			RGBQUAD{90, 90, 255},
			RGBQUAD{49, 49, 255},
			RGBQUAD{82, 90, 214},
			RGBQUAD{0, 16, 148},
			RGBQUAD{24, 41, 148},
			RGBQUAD{0, 8, 57},
			RGBQUAD{0, 16, 115},
			RGBQUAD{0, 24, 181},
			RGBQUAD{82, 99, 189},
			RGBQUAD{16, 24, 66},
			RGBQUAD{153, 170, 255},
			RGBQUAD{0, 16, 90},
			RGBQUAD{41, 57, 115},
			RGBQUAD{49, 74, 165},
			RGBQUAD{115, 123, 148},
			RGBQUAD{49, 82, 189},
			RGBQUAD{16, 33, 82},
			RGBQUAD{24, 49, 123},
			RGBQUAD{16, 24, 45},
			RGBQUAD{49, 74, 140},
			RGBQUAD{0, 41, 148},
			RGBQUAD{0, 49, 189},
			RGBQUAD{82, 115, 198},
			RGBQUAD{24, 49, 107},
			RGBQUAD{66, 107, 198},
			RGBQUAD{0, 74, 206},
			RGBQUAD{57, 99, 165},
			RGBQUAD{24, 49, 90},
			RGBQUAD{0, 16, 42},
			RGBQUAD{0, 8, 21},
			RGBQUAD{0, 24, 58},
			RGBQUAD{0, 0, 8},
			RGBQUAD{0, 0, 41},
			RGBQUAD{0, 0, 74},
			RGBQUAD{0, 0, 157},
			RGBQUAD{0, 0, 220},
			RGBQUAD{0, 0, 222},
			RGBQUAD{0, 0, 251},
			RGBQUAD{82, 115, 156},
			RGBQUAD{74, 107, 148},
			RGBQUAD{41, 74, 115},
			RGBQUAD{24, 49, 82},
			RGBQUAD{24, 74, 140},
			RGBQUAD{17, 68, 136},
			RGBQUAD{0, 33, 74},
			RGBQUAD{16, 24, 33},
			RGBQUAD{90, 148, 214},
			RGBQUAD{33, 107, 198},
			RGBQUAD{0, 107, 239},
			RGBQUAD{0, 119, 255},
			RGBQUAD{132, 148, 165},
			RGBQUAD{33, 49, 66},
			RGBQUAD{8, 16, 24},
			RGBQUAD{8, 24, 41},
			RGBQUAD{0, 16, 33},
			RGBQUAD{24, 41, 57},
			RGBQUAD{57, 99, 140},
			RGBQUAD{16, 41, 66},
			RGBQUAD{24, 66, 107},
			RGBQUAD{24, 74, 123},
			RGBQUAD{0, 74, 148},
			RGBQUAD{123, 132, 140},
			RGBQUAD{90, 99, 107},
			RGBQUAD{57, 66, 74},
			RGBQUAD{24, 33, 41},
			RGBQUAD{41, 57, 70},
			RGBQUAD{148, 165, 181},
			RGBQUAD{90, 107, 123},
			RGBQUAD{148, 177, 206},
			RGBQUAD{115, 140, 165},
			RGBQUAD{90, 115, 140},
			RGBQUAD{115, 148, 181},
			RGBQUAD{115, 165, 214},
			RGBQUAD{74, 165, 239},
			RGBQUAD{140, 198, 239},
			RGBQUAD{66, 99, 123},
			RGBQUAD{57, 86, 107},
			RGBQUAD{90, 148, 189},
			RGBQUAD{0, 57, 99},
			RGBQUAD{173, 198, 214},
			RGBQUAD{41, 66, 82},
			RGBQUAD{24, 99, 148},
			RGBQUAD{173, 214, 239},
			RGBQUAD{99, 140, 165},
			RGBQUAD{74, 90, 99},
			RGBQUAD{123, 165, 189},
			RGBQUAD{24, 66, 90},
			RGBQUAD{49, 140, 189},
			RGBQUAD{41, 49, 53},
			RGBQUAD{99, 132, 148},
			RGBQUAD{74, 107, 123},
			RGBQUAD{90, 140, 165},
			RGBQUAD{41, 74, 90},
			RGBQUAD{57, 123, 156},
			RGBQUAD{16, 49, 66},
			RGBQUAD{33, 173, 239},
			RGBQUAD{0, 16, 24},
			RGBQUAD{0, 33, 41},
			RGBQUAD{0, 107, 156},
			RGBQUAD{90, 132, 148},
			RGBQUAD{24, 66, 82},
			RGBQUAD{41, 90, 107},
			RGBQUAD{33, 99, 123},
			RGBQUAD{33, 123, 156},
			RGBQUAD{0, 165, 222},
			RGBQUAD{57, 82, 90},
			RGBQUAD{16, 41, 49},
			RGBQUAD{123, 189, 206},
			RGBQUAD{57, 90, 99},
			RGBQUAD{74, 132, 148},
			RGBQUAD{41, 165, 198},
			RGBQUAD{24, 156, 16},
			RGBQUAD{74, 140, 66},
			RGBQUAD{66, 140, 49},
			RGBQUAD{41, 148, 16},
			RGBQUAD{16, 24, 8},
			RGBQUAD{24, 24, 8},
			RGBQUAD{16, 41, 8},
			RGBQUAD{41, 66, 24},
			RGBQUAD{173, 181, 165},
			RGBQUAD{115, 115, 107},
			RGBQUAD{41, 41, 24},
			RGBQUAD{74, 66, 24},
			RGBQUAD{74, 66, 49},
			RGBQUAD{222, 198, 99},
			RGBQUAD{255, 221, 68},
			RGBQUAD{239, 214, 140},
			RGBQUAD{57, 107, 115},
			RGBQUAD{57, 222, 247},
			RGBQUAD{140, 239, 247},
			RGBQUAD{0, 231, 247},
			RGBQUAD{90, 107, 107},
			RGBQUAD{165, 140, 90},
			RGBQUAD{239, 181, 57},
			RGBQUAD{206, 156, 74},
			RGBQUAD{181, 132, 49},
			RGBQUAD{107, 82, 49},
			RGBQUAD{214, 222, 222},
			RGBQUAD{181, 189, 189},
			RGBQUAD{132, 140, 140},
			RGBQUAD{222, 247, 247},
			RGBQUAD{24, 8, 0},
			RGBQUAD{57, 24, 8},
			RGBQUAD{41, 16, 8},
			RGBQUAD{0, 24, 8},
			RGBQUAD{0, 41, 8},
			RGBQUAD{165, 82, 0},
			RGBQUAD{222, 123, 0},
			RGBQUAD{74, 41, 16},
			RGBQUAD{107, 57, 16},
			RGBQUAD{140, 82, 16},
			RGBQUAD{165, 90, 33},
			RGBQUAD{90, 49, 16},
			RGBQUAD{132, 66, 16},
			RGBQUAD{132, 82, 49},
			RGBQUAD{49, 33, 24},
			RGBQUAD{123, 90, 74},
			RGBQUAD{165, 107, 82},
			RGBQUAD{99, 57, 41},
			RGBQUAD{222, 74, 16},
			RGBQUAD{33, 41, 41},
			RGBQUAD{57, 74, 74},
			RGBQUAD{24, 41, 41},
			RGBQUAD{41, 74, 74},
			RGBQUAD{66, 123, 123},
			RGBQUAD{74, 156, 156},
			RGBQUAD{41, 90, 90},
			RGBQUAD{20, 66, 66},
			RGBQUAD{0, 57, 57},
			RGBQUAD{0, 89, 89},
			RGBQUAD{44, 53, 202},
			RGBQUAD{33, 115, 107},
			RGBQUAD{0, 49, 41},
			RGBQUAD{16, 57, 49},
			RGBQUAD{24, 57, 49},
			RGBQUAD{0, 74, 66},
			RGBQUAD{24, 99, 82},
			RGBQUAD{41, 115, 90},
			RGBQUAD{24, 74, 49},
			RGBQUAD{0, 33, 24},
			RGBQUAD{0, 49, 24},
			RGBQUAD{16, 57, 24},
			RGBQUAD{74, 132, 99},
			RGBQUAD{74, 189, 107},
			RGBQUAD{74, 181, 99},
			RGBQUAD{74, 189, 99},
			RGBQUAD{74, 156, 90},
			RGBQUAD{57, 140, 74},
			RGBQUAD{74, 198, 99},
			RGBQUAD{74, 214, 99},
			RGBQUAD{74, 132, 82},
			RGBQUAD{41, 115, 49},
			RGBQUAD{90, 198, 99},
			RGBQUAD{74, 189, 82},
			RGBQUAD{0, 255, 16},
			RGBQUAD{24, 41, 24},
			RGBQUAD{74, 136, 74},
			RGBQUAD{74, 231, 74},
			RGBQUAD{0, 90, 0},
			RGBQUAD{0, 136, 0},
			RGBQUAD{0, 148, 0},
			RGBQUAD{0, 222, 0},
			RGBQUAD{0, 238, 0},
			RGBQUAD{0, 251, 0},
			RGBQUAD{148, 90, 74},
			RGBQUAD{181, 115, 99},
			RGBQUAD{214, 140, 123},
			RGBQUAD{214, 123, 107},
			RGBQUAD{255, 136, 119},
			RGBQUAD{206, 198, 198},
			RGBQUAD{156, 148, 148},
			RGBQUAD{198, 148, 156},
			RGBQUAD{57, 49, 49},
			RGBQUAD{132, 24, 41},
			RGBQUAD{132, 0, 24},
			RGBQUAD{82, 66, 74},
			RGBQUAD{123, 66, 82},
			RGBQUAD{115, 90, 99},
			RGBQUAD{247, 181, 206},
			RGBQUAD{156, 123, 140},
			RGBQUAD{204, 34, 119},
			RGBQUAD{255, 170, 221},
			RGBQUAD{42, 180, 240},
			RGBQUAD{159, 0, 223},
			RGBQUAD{179, 23, 227},
			RGBQUAD{240, 251, 255},
			RGBQUAD{164, 160, 160},
			RGBQUAD{128, 128, 128},
			RGBQUAD{0, 0, 255},
			RGBQUAD{0, 255, 0},
			RGBQUAD{0, 255, 255},
			RGBQUAD{255, 0, 0},
			RGBQUAD{255, 0, 255},
			RGBQUAD{255, 255, 0},
			RGBQUAD{255, 255, 255},
	};

	// wzl 
	struct WzlTexture {
		int sort;				//序号
		int quote;				//引用次数
		UTEXTURE tex;			//纹理地址
		WzlBmpInfo wzlBmpInfo;	//纹理信息
	};

	//wzl Map容器
	typedef std::unordered_map<int, WzlTexture*> UWzlTexMap;

	class WzlBmp {
	private:
		//UgeDevice* d3d_device;
		const char* path;

		void *wzxData;
		int wzxSize;
		WzxHeader wzxHead;

		void* wzlData;
		int wzlSize;
		WzlHeader wzlHead;
		
		UWzlTexMap _wzlTexMap;
	public:
		WzlBmp(const char* path);
		~WzlBmp();
		byte* GetBmp(int sort, WzlBmpInfo* wzlBmp, int* dstSize);
		WzlTexture* GetTextureCache(int sort);
		bool SetTextureCache(WzlTexture* tex);
		bool ReleaseTexture(int sort,bool* hasErase);
	private:
		int _GetOffset(int sort);
		bool _LoadWzl();
		bool _LoadWzx();
	};
}