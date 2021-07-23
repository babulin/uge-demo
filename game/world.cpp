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
		//mapx = new MapX(pUge);

		return true;
	}

	bool World::Update()
	{
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
