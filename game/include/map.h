#pragma once
#include <uge.h>
#include <wzlcache.h>
#include <vector>
#include <algorithm>
#include "hum.h"

using namespace uge;
using namespace wzl;
namespace game {

	struct MapHeader {
		short width;	//地图宽
		short height;	//地图高
		char title[16];
		int updatetime[2];
		char Reserved[24];
	};

	struct MapInfo {
		unsigned short wBkImg;	//第一层
		unsigned short wMidImg; //第二层
		unsigned short wFrImg;	//第三层
		byte btDoorIndex;
		byte btDoorOffset;
		byte btAniFrame;
		byte btAniTick;
		byte btArea;
		//byte btLight;			//12字节
		//byte btBackIndex;
		//byte btMidIndex;		//14字节
		//unsigned short TileAnimationImage;//15-16字节
		//byte reserved1[5];//保留17-21字节
		//byte TileAnimationOffset;	//22字节
		//unsigned short TileAnimationFrame;//23-24字节
		//byte reserved2[12];//保留25-36字节
	};

	class Map {

		WzlCache* pWzlCache;					//wzl缓存对象
		UGE* pUge;								//引擎对象
		MapHeader mapHead;						//map头部结构体
		MapInfo* mapInfo;						//map数据结构体

		int bWidth = 48;						//坐标像素宽度
		int bHeight = 32;						//坐标像素高度
		int offsetX = -7;						//地图绘制中心点X偏移
		int offsetY = 44;						//地图绘制中心点Y偏移
		static const int mapSizeW = 25;			//地图绘制尺寸21x21
		static const int mapSizeH = 25;			//地图绘制尺寸21x21

		std::vector<wzl::ugeImage> vUiTiles;		//大瓷砖
		std::vector<wzl::ugeImage> vUiSmTiles;		//小瓷砖
		std::vector<wzl::ugeImage> vUiObjects;		//物体对象
		std::vector<wzl::ugeAnimation> vBtLight;//地图灯动画

		int screen_width;
		int screen_height;

		Hum** _hum;
		const char* mapName;
		float x = 0;
		float y = 0;
	public:
		Map(UGE* pUge,Hum** hum);
		void SetMap(const char * mapName);
		void Load();
		void Update();
		void Show(bool fillmode);
	private:
		float _GetWidthOffset(int val, int val2 = 0);
		float _GetHeightOffset(int val,int val2 = 0);
		void _Load(const char* path);
		void _LoadMap(int x, int y);
		void _Tiles(int sx, int sy, int tx, int ty, int sort);
		void _SmTiles(int sx, int sy,int sort);
		void _Objects(int sx, int sy, int sort);
	};
}