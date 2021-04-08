#include "world.h"
#include "login.h"
#include "map.h"
#include "hum.h"

namespace game {

	Map* map;
	Hum* hum;

	bool fillmode = false;
	float x = 295, y = 308;
	bool World::Initiate()
	{
		std::cout << "World::Initiate()" << std::endl;

		map = new Map(pUge);
		hum = new Hum(pUge);

		return true;
	}

	bool World::Update()
	{
		map->Update();
		hum->Update();
		return true;
	}

	bool World::Show()
	{
		map->Show(fillmode);
		hum->Show();
		return true;
	}
}
