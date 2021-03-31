#pragma once
#include "uge.h"
using namespace uge;
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
		byte btLight;
	};

	class Map {
		UGE* pUge;								//引擎对象
		MapHeader mapHead;						//map头部结构体
		MapInfo* mapInfo;						//map数据结构体

		int bWidth = 48;						//坐标像素宽度
		int bHeight = 32;						//坐标像素高度
		int offsetX = -7;						//地图绘制中心点X偏移
		int offsetY = 44;						//地图绘制中心点Y偏移
		static const int mapSizeW = 25;			//地图绘制尺寸21x21
		static const int mapSizeH = 25;			//地图绘制尺寸21x21
		ugeImage uiTiles[mapSizeW][mapSizeH];		//大瓷砖
		ugeImage uiSmTiles[mapSizeW][mapSizeH];	//小瓷砖
		ugeImage uiObjects[mapSizeW][mapSizeH*2];	//物体对象
	public:
		Map(UGE* pUge);
		void Load(const char* path);
		void LoadMap(int x, int y);
		void Show(bool fillmode);
		void Tiles(int sx, int sy, int tx, int ty, int sort);
		void SmTiles(int sx, int sy,int sort);
		void Objects(int sx, int sy, int sort);
	};
}