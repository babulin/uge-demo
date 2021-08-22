#include "map.h"


namespace game {

	ugeLine map1, map2;

	Map::Map(UGE* pUge,Hum** hum)
	{
		mapName = nullptr;

		this->pUge = pUge;
		pWzlCache = new WzlCache(pUge);

		//获取屏幕
		pUge->GetScreen(&screen_width, &screen_height);

		offsetX = 0;
		offsetY = 0;

		int mapw = ((mapSizeW - 1) / 2) * 48 + 48;
		offsetX += screen_width/2 - mapw;

		int maph = ((mapSizeH - 1) / 2) * 32 + 32;
		offsetY += screen_height/2 - maph;

		//_Load("map\\0.map");

		//保存角色
		this->_hum = hum;
		x = (*hum)->x;
		y = (*hum)->y;

	}

	void Map::SetMap(const char* mapName)
	{
		this->mapName = mapName;
	}

	void Map::Load()
	{
		//地图路径
		char map[20] = "map\\";
		strcat_s(map,sizeof(map), mapName);
		strcat_s(map, sizeof(map), ".map");
		_Load(map);
		_LoadMap(x, y);
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
		//每次初始化
		vBtLight.clear();
		vUiTiles.clear();
		vUiSmTiles.clear();
		vUiObjects.clear();

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
			for (int ty = y; ty < y + mapSizeH * 1.5; ty++)
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

		if (sort < 0){
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
			if (tx == 336 && ty == 358)
			{
				int i = 0;
			}

			wzl::ugeImage image;
			if (pWzlCache->LoadWzl(szTilesFile, tiles, &image)) {
				image.x = _GetWidthOffset(sx, -1 * image.px);
				image.y = _GetHeightOffset(sy, -1 * image.py);
				vUiTiles.push_back(image);
			}
		}
	}

	void Map::_SmTiles(int sx, int sy, int sort)
	{
		if (sort < 0){
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

			wzl::ugeImage image;
			if (pWzlCache->LoadWzl(szSmTilesFile, smTiles, &image)) {
				image.x = _GetWidthOffset(sx, -1 * image.px);
				image.y = _GetHeightOffset(sy, -1 * image.py);
				vUiSmTiles.push_back(image);
			}
		}
	}

	void Map::_Objects(int sx, int sy, int sort)
	{
		if (sort < 0){
			return;
		}
		//object
		int object = (mapInfo[sort].wFrImg & 32767) - 1;

		//地图上等的标识
		int light = (mapInfo[sort].btAniFrame);

		if (object > 0)
		{
			char szObjectFile[_MAX_DIR] = "data\\Objects";
			int area = mapInfo[sort].btArea + 1;//1-Objects 2-Objects2
			if (area > 1)
			{
				sprintf_s(szObjectFile, "data\\Objects%d", area);
			}

			if (light > 0)
			{
				wzl::ugeAnimation animation;
				pWzlCache->LoadWzl(szObjectFile, object + 1, 9, &animation);
				animation.x = _GetWidthOffset(sx);
				animation.y = _GetHeightOffset(sy, bHeight - animation.image[0].height);
				vBtLight.push_back(animation);
			}
			else {
				wzl::ugeImage image;
				if (pWzlCache->LoadWzl(szObjectFile, object, &image)) {
					image.x = _GetWidthOffset(sx, -1 * image.px);
					image.y = _GetHeightOffset(sy, bHeight - image.height - image.py);
					vUiObjects.push_back(image);
				}
			}
		}
	}


	void Map::Update()
	{
		//Log("x:%.2f", (*_hum)->x);

		if (x != (*_hum)->x || y != (*_hum)->y)
		{
			x = (*_hum)->x;
			y = (*_hum)->y;
			_LoadMap(x, y);
		}
	}

	void Map::Show(bool fillmode)
	{

		//Tiles
		for (std::vector<wzl::ugeImage>::iterator itTiles = vUiTiles.begin();  itTiles != vUiTiles.end(); itTiles++)
		{
			pUge->DxRenderQuad(reinterpret_cast<uge::ugeImage*>(itTiles._Ptr));
		}	

		//SmTiles
		for (std::vector<wzl::ugeImage>::iterator itSmTiles = vUiSmTiles.begin(); itSmTiles != vUiSmTiles.end(); itSmTiles++)
		{
			pUge->DxRenderQuad(reinterpret_cast<uge::ugeImage*>(itSmTiles._Ptr));
		}

		//Object
		for (std::vector<wzl::ugeImage>::iterator itImage = vUiObjects.begin(); itImage != vUiObjects.end(); itImage++)
		{
			pUge->DxRenderQuad(reinterpret_cast<uge::ugeImage*>(itImage._Ptr));
		}

		//Light
		pUge->SetDrawMode(UGE_DW_LIGHT);
		for (std::vector<wzl::ugeAnimation>::iterator it = vBtLight.begin(); it != vBtLight.end(); it++)
		{
			pUge->DxRenderQuad(reinterpret_cast<uge::ugeAnimation*>(it._Ptr));
		}
		pUge->SetDrawMode(UGE_DW_NORMAL);
	}

}

