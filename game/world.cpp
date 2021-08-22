#include "world.h"
#include "login.h"
#include "map.h"
#include "hum.h"
#include "mapx.h"

namespace game {

	Map* map;
	MapX* mapx;
	Hum* hum;

	bool fillmode = false;
	float x = 295, y = 308;
	bool World::Initiate()
	{
		std::cout << "World::Initiate()" << std::endl;

		hum = new Hum(pUge);
		map = new Map(pUge, &hum);
		map->SetMap("0");
		map->Load();
		//mapx = new MapX(pUge);

		return true;
	}

	bool World::Update()
	{
		if (pUge->KeyDown(UGEK_0)) {
			map->SetMap("0");
			map->Load();
			hum->x = 333;
			hum->y = 263;
		}

		if (pUge->KeyDown(UGEK_1)) {
			map->SetMap("1");
			map->Load();
			hum->x = 333;
			hum->y = 263;
		}

		if (pUge->KeyDown(UGEK_2)) {
			map->SetMap("2");
			map->Load();
		}

		if (pUge->KeyDown(UGEK_3)) {
			map->SetMap("3");
			map->Load();
		}

		//mapx->Update();
		map->Update();
		hum->Update();
		return true;
	}

	bool World::Show()
	{
		//mapx->Show(fillmode);
		map->Show(fillmode);
		hum->Show();
		return true;
	}
}
