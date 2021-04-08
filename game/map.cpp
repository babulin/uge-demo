#include "map.h"

namespace game {

	ugeLine map1, map2;

	Map::Map(UGE* pUge)
	{
		this->pUge = pUge;
		pWzlCache = new WzlCache(pUge);

		//获取屏幕
		pUge->GetScreen(&screen_width, &screen_height);

		bWidth = 48;
		bHeight = 32;
		offsetX = 0;
		offsetY = 0;

		int mapw = ((mapSizeW - 1) / 2) * 48 + 48;
		offsetX += screen_width/2 - mapw;

		int maph = ((mapSizeH - 1) / 2) * 32 + 32;
		offsetY += screen_height/2 - maph;

		_Load("map\\2.map");

		centerX = 0;
		centerY = 0;
		_LoadMap(centerX, centerY);
	}

	void Map::_Load(const char* path)
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

	void Map::_LoadMap(int x, int y)
	{
		x = x - 12;
		y = y - 12;

		//获取xy周围的点
		int sort = 0;
		int tiles = 0;
		int smTiles = 0;
		int object = 0;
		int sx = 0;
		int sy = 0;
		int area = 0;

		std::cout << x + 12 <<":" << y + 12 << std::endl;

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
					_Tiles(sx, sy, tx, ty, sort);
					_SmTiles(sx, sy, sort);
				}
				_Objects(sx, sy, sort);
			}
		}
	}

	float Map::_GetWidthOffset(int val,int val2) {
		return static_cast<float>(val * bWidth + offsetX + val2);
	}

	float Map::_GetHeightOffset(int val, int val2) {
		return static_cast<float>(val * bHeight + offsetY + val2);
	}

	void Map::_Tiles(int sx, int sy,int tx,int ty,int sort)
	{
		if (sort < 0)
		{
			memset(&uiTiles[sx][sy], 0, sizeof(uiTiles[sx][sy]));
			return;
		}
		//Tiles序号
		int tiles = (mapInfo[sort].wBkImg & 32767) - 1;
		if (tiles >= 0 && (tx % 2 == 0 && ty % 2 == 0))
		{
			char szTilesFile[_MAX_DIR] = "data\\Tiles";
			int area = mapInfo[sort].btArea + 1;//1-Tiles 2-Tiles2
			if (area > 1)
			{
				//sprintf_s(szTilesFile, "data\\Tiles%d", area);
			}

			pWzlCache->LoadWzl(szTilesFile, tiles, &uiTiles[sx][sy]);
			uiTiles[sx][sy].x = _GetWidthOffset(sx, -1*uiTiles[sx][sy].px);
			uiTiles[sx][sy].y = _GetHeightOffset(sy, -1*uiTiles[sx][sy].py);
		}
		else {
			memset(&uiTiles[sx][sy], 0, sizeof(uiTiles[sx][sy]));
		}
	}

	void Map::_SmTiles(int sx, int sy, int sort)
	{
		if (sort < 0)
		{
			memset(&uiSmTiles[sx][sy], 0, sizeof(uiSmTiles[sx][sy]));
			return;
		}
		//SmTiles
		int smTiles = (mapInfo[sort].wMidImg & 32767) - 1;
		if (smTiles >= 0)
		{
			char szSmTilesFile[_MAX_DIR] = "data\\SmTiles";
			int area = mapInfo[sort].btArea + 1;//1-SmTiles 2-SmTiles2
			if (area > 1)
			{
				//sprintf_s(szSmTilesFile, "data\\SmTiles%d", area);
			}

			pWzlCache->LoadWzl(szSmTilesFile, smTiles, &uiSmTiles[sx][sy]);
			uiSmTiles[sx][sy].x = _GetWidthOffset(sx, -1 * uiSmTiles[sx][sy].px);
			uiSmTiles[sx][sy].y = _GetHeightOffset(sy, - 1 * uiSmTiles[sx][sy].py);
		}
		else {
			memset(&uiSmTiles[sx][sy], 0, sizeof(uiSmTiles[sx][sy]));
		}
	}

	void Map::_Objects(int sx, int sy, int sort)
	{
		if (sort < 0)
		{
			memset(&uiObjects[sx][sy], 0, sizeof(uiObjects[sx][sy]));
			return;
		}
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

			pWzlCache->LoadWzl(szObjectFile, object, &uiObjects[sx][sy]);
			uiObjects[sx][sy].x = _GetWidthOffset(sx, -1*uiObjects[sx][sy].px);
			uiObjects[sx][sy].y = _GetHeightOffset(sy, bHeight - uiObjects[sx][sy].height - uiObjects[sx][sy].py);
		}
		else {
			memset(&uiObjects[sx][sy], 0, sizeof(wzl::ugeImage));
		}
	}


	void Map::Update()
	{
		if (pUge->KeyState(UGEK_UP))
		{
			_LoadMap(centerX, --centerY);
		}
		if (pUge->KeyState(UGEK_DOWN))
		{
			_LoadMap(centerX, ++centerY);
		}
		if (pUge->KeyState(UGEK_LEFT))
		{
			_LoadMap(--centerX, centerY);
		}
		if (pUge->KeyState(UGEK_RIGHT))
		{
			_LoadMap(++centerX, centerY);
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
					pUge->DxRenderQuad(reinterpret_cast<uge::ugeImage*>(&uiTiles[tx][ty]));
					pUge->DxRenderQuad(reinterpret_cast<uge::ugeImage*>(&uiSmTiles[tx][ty]));
				}
				pUge->DxRenderQuad(reinterpret_cast<uge::ugeImage*>(&uiObjects[tx][ty]), fillmode);
			}
		}
	}

}

