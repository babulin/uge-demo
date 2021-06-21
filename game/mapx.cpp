#include "mapx.h"

namespace game {

	ugeLine map3, map4;

	MapX::MapX(UGE* pUge)
	{
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

		_Load("map\\nga0.map");

		centerX = 150;
		centerY = 40;
		_LoadMap(centerX, centerY);
	}

	void MapX::_Load(const char* path)
	{
		int size = 0;
		void* data = open_file(path, &size);

		//头部
		byte* btData = reinterpret_cast<byte*>(data);
		memcpy_s(&mapHead, size, btData, sizeof(MapHeaderX));

		//读取到信息中[地图从左往右，从上往下]
		btData = btData + sizeof(mapHead);//偏移头部
		const int infoSize = mapHead.width * mapHead.height;//结构体大小
		mapInfo = new MapInfo36[infoSize];
		memcpy_s(mapInfo, size, btData, infoSize * sizeof(MapInfo36));

		//释放源数据
		free(data);
	}

	void MapX::_LoadMap(int x, int y)
	{
		//每次初始化
		vBtLight.clear();
		vBtTileAni.clear();
		vUiTiles.clear();
		vUiSmTiles.clear();
		vUiObjects.clear();

		std::cout <<"地图坐标："<< x << ":" << y << std::endl;

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

				if (sort == 44)
				{
					int i = 0;
				}

				if (ty < y+ mapSizeH)
				{
					_Tiles(sx, sy, tx, ty, sort);
					_SmTiles(sx, sy, sort);
					_Animation(sx, sy, sort);
				}
				_Objects(sx, sy, sort);
			}
		}
	}

	float MapX::_GetWidthOffset(int val,int val2) {
		return static_cast<float>(val * bWidth + offsetX + val2);
	}

	float MapX::_GetHeightOffset(int val, int val2) {
		return static_cast<float>(val * bHeight + offsetY + val2);
	}

	void MapX::_Tiles(int sx, int sy,int tx,int ty,int sort)
	{

		if (sort < 0){
			return;
		}

		//Tiles序号
		int tiles = (mapInfo[sort].wBkImg & 32767) - 1;
		if (tiles >= 0 && (tx % 2 == 0 && ty % 2 == 0))
		{
			char szTilesFile[_MAX_DIR] = "data\\Tiles";
			//int area = mapInfo[sort].btArea + 1;//1-Tiles 2-Tiles2
			int area = mapInfo[sort].btBackIndex + 1;//1-Tiles 2-Tiles2
			if (area > 1)
			{
				sprintf_s(szTilesFile, "data\\Tiles%d", area);
			}

			wzl::ugeImage image;
			if (pWzlCache->LoadWzl(szTilesFile, tiles, &image)) {
				image.x = _GetWidthOffset(sx, -1 * image.px);
				image.y = _GetHeightOffset(sy, -1 * image.py);
				vUiTiles.push_back(image);
			}
		}
	}

	void MapX::_SmTiles(int sx, int sy, int sort)
	{
		if (sort < 0){
			return;
		}

		//SmTiles
		int smTiles = (mapInfo[sort].wMidImg & 32767) - 1;
		if (smTiles >= 0 && mapInfo[sort].wMidImg != 0XFFFF)
		{
			char szSmTilesFile[_MAX_DIR] = "data\\SmTiles";
			//int area = mapInfo[sort].btArea + 1;//1-SmTiles 2-SmTiles2
			int area = mapInfo[sort].btMidIndex + 1;//1-SmTiles 2-SmTiles2
			if (area > 1)
			{
				sprintf_s(szSmTilesFile, "data\\SmTiles%d", area);
			}
			wzl::ugeImage image;
			if (pWzlCache->LoadWzl(szSmTilesFile, smTiles, &image)) {
				image.x = _GetWidthOffset(sx, -1 * image.px);
				image.y = _GetHeightOffset(sy, -1 * image.py);
				vUiSmTiles.push_back(image);
			}
		}
	}

	void MapX::_Animation(int sx, int sy, int sort)
	{
		if (sort < 0) {
			return;
		}

		//地图上等的标识
		int tileAni = (mapInfo[sort].TileAnimationImage);
		int tileTotal = (mapInfo[sort].TileAnimationOffset);
		int tileFrame = (mapInfo[sort].TileAnimationFrame);
		if (tileAni > 0 && tileFrame > 0)
		{
			int offset = tileFrame ^ 0x2000;
			char szObjectFile[_MAX_DIR] = "data\\AniTiles1";
			wzl::ugeAnimation animation;
			pWzlCache->LoadWzl(szObjectFile, tileAni - 1, tileTotal, &animation, offset);
			animation.x = _GetWidthOffset(sx);
			animation.y = _GetHeightOffset(sy);
			vBtTileAni.push_back(animation);
		}
	}

	void MapX::_Objects(int sx, int sy, int sort)
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
			//int area = mapInfo[sort].btArea + 1;//1-Objects 2-Objects2
			int area = mapInfo[sort].btArea + 1;//1-Objects 2-Objects2

			if (area > 1)
			{
				sprintf_s(szObjectFile, "data\\Objects%d", area);
			}

			if (light > 0)
			{
				if (light > 0x7F)
				{
					light = light - 128;
				}

				wzl::ugeAnimation animation;
				pWzlCache->LoadWzl(szObjectFile, object, light, &animation);
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


	void MapX::Update()
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

	void MapX::Show(bool fillmode)
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

		//Light 动画水有点小问题
		for (std::vector<wzl::ugeAnimation>::iterator it = vBtTileAni.begin(); it != vBtTileAni.end(); it++)
		{
			pUge->DxRenderQuad(reinterpret_cast<uge::ugeAnimation*>(it._Ptr));
		}

		//Object
		for (std::vector<wzl::ugeImage>::iterator itImage = vUiObjects.begin(); itImage != vUiObjects.end(); itImage++)
		{
			pUge->DxRenderQuad(reinterpret_cast<uge::ugeImage*>(itImage._Ptr));
		}

		//Light 动画水有点小问题
		pUge->SetDrawMode(UGE_DW_LIGHT);
		for (std::vector<wzl::ugeAnimation>::iterator it = vBtLight.begin(); it != vBtLight.end(); it++)
		{
			pUge->DxRenderQuad(reinterpret_cast<uge::ugeAnimation*>(it._Ptr));
		}
		pUge->SetDrawMode(UGE_DW_NORMAL);
	}

}

