#include "world.h"
#include "login.h"
#include "map.h"
#include "hum.h"

namespace game {

	Map* map;
	Hum* hum;

	bool kDownSpace = false;
	bool kDown3 = false;
	bool kDown4 = false;
	bool kDown5 = false;
	bool fillmode = false;
	int x = 295, y = 308;
	bool World::Initiate()
	{
		std::cout << "World::Initiate()" << std::endl;

		map = new Map(pUge);
		map->Load("map\\0.map");
		map->LoadMap(x,y);

		hum = new Hum(pUge);
		hum->Load();

		return true;
	}

	bool World::Update()
	{
		return true;
	}

	bool World::Show()
	{
		map->Show(fillmode);
		hum->Show();
		return true;
	}
}
