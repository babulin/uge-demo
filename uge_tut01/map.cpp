#include "map.h"

namespace game {

	ugeLine map1, map2;

	Map::Map(UGE* pUge)
	{
		this->pUge = pUge;

		bWidth = 48;
		bHeight = 32;
		offsetX = -7;
		offsetY = 44;

		int mapw = ((mapSizeW - 1) / 2) * 48 + 48;
		offsetX += 512 - mapw;

		int maph = ((mapSizeH - 1) / 2) * 32 + 32;
		offsetY += 384 - maph;
	}

	void Map::Load(const char* path)
	{
		int size = 0;
		void* data = open_file(path, &size);

		//头部
		byte* btData = reinterpret_cast<byte*>(data);
		memcpy_s(&mapHead, size, btData, sizeof(MapHeader));

		//读取到信息中[地图从左往右，从上往下]
		btData = btData + sizeof(mapHead);//偏移头部
		const int infoSize = mapHead.width * mapHead.height;//结构体大小
		mapInfo = new MapInfo[infoSize];
		memcpy_s(mapInfo, size, btData, infoSize * sizeof(MapInfo));

		//释放源数据
		free(data);
	}

	void Map::LoadMap(int x,int y)
	{
		//获取xy周围的点
		int sort = 0;
		int tiles = 0;
		int smTiles = 0;
		int object = 0;
		int sx = 0;
		int sy = 0;
		int area = 0;

		for (int tx = x; tx < x + mapSizeW; tx++)
		{
			sx = tx - x;
			for (int ty = y; ty < y + mapSizeH * 2; ty++)
			{
				sy = ty - y;

				//超过地图不绘制
				if (tx >= mapHead.width || ty >= mapHead.height)
				{
					break;
				}
				
				int sort = tx * mapHead.height + ty;

				if (ty < y+ mapSizeH)
				{
					Tiles(sx, sy, tx, ty, sort);
					SmTiles(sx, sy, sort);
				}
				Objects(sx, sy, sort);
			}
		}
	}

	void Map::Tiles(int sx, int sy,int tx,int ty,int sort)
	{
		//Tiles序号
		int tiles = (mapInfo[sort].wBkImg & 32767) - 1;
		if (tiles > 0 && (tx % 2 == 0 && ty % 2 == 0))
		{
			pUge->LoadWzl("data\\tiles", tiles, &uiTiles[sx][sy]);
			uiTiles[sx][sy].x = static_cast<float>(sx * bWidth + offsetX);
			uiTiles[sx][sy].y = static_cast<float>(sy * bHeight + offsetY);
		}
		else {
			memset(&uiTiles[sx][sy], 0, sizeof(uiTiles[sx][sy]));
		}
	}

	void Map::SmTiles(int sx, int sy, int sort)
	{
		//SmTiles
		int smTiles = (mapInfo[sort].wMidImg & 32767) - 1;
		if (smTiles > 0)
		{
			char szMapFile[_MAX_DIR] = "data\\SmTiles";

			pUge->LoadWzl(szMapFile, smTiles, &uiSmTiles[sx][sy]);
			uiSmTiles[sx][sy].x = static_cast<float>(sx * bWidth + offsetX);
			uiSmTiles[sx][sy].y = static_cast<float>(sy * bHeight + offsetY);
		}
		else {
			memset(&uiSmTiles[sx][sy], 0, sizeof(uiSmTiles[sx][sy]));
		}
	}

	void Map::Objects(int sx, int sy, int sort)
	{
		//object
		int object = (mapInfo[sort].wFrImg & 32767) - 1;
		if (object > 0)
		{
			char szObjectFile[_MAX_DIR] = "data\\Objects";
			int area = mapInfo[sort].btArea + 1;//1-Objects 2-Objects2
			if (area > 1)
			{
				sprintf_s(szObjectFile, "data\\Objects%d", area);
			}

			pUge->LoadWzl(szObjectFile, object, &uiObjects[sx][sy]);
			uiObjects[sx][sy].x = static_cast<float>(sx * bWidth + offsetX);
			uiObjects[sx][sy].y = static_cast<float>(sy * bHeight - uiObjects[sx][sy].height + bHeight + offsetY);
		}
		else {
			memset(&uiObjects[sx][sy], 0, sizeof(uiObjects[sx][sy]));
		}
	}

	void Map::Show(bool fillmode)
	{
		for (int tx = 0; tx < mapSizeW; tx++)
		{
			for (int ty = 0; ty < mapSizeH * 2; ty++)
			{
				if (ty < mapSizeH)
				{
					pUge->DxRenderQuad(&uiTiles[tx][ty]);
					pUge->DxRenderQuad(&uiSmTiles[tx][ty]);
				}
				pUge->DxRenderQuad(&uiObjects[tx][ty], fillmode);
			}
		}
	}
}

