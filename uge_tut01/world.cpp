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
		map->Show(x, y);

		hum = new Hum(pUge);
		hum->Load();

		return true;
	}

	bool World::Update()
	{
		if (GetAsyncKeyState(49) & 0x8000 && fillmode == false)
		{
			fillmode = true;
		}
		if (GetAsyncKeyState(50) & 0x8000 && fillmode == true)
		{
			fillmode = false;
		}
		if (GetAsyncKeyState(52) & 0x8000 && kDown4 == false)
		{
			kDown4 = true;
			x++;
			map->Show(x, y);
		}
		if (GetAsyncKeyState(53) & 0x8000 && kDown4 == true)
		{
			kDown4 = false;
			x++;
			map->Show(x, y);
		}

		if (GetAsyncKeyState(54) & 0x8000 && kDown5 == false)
		{
			kDown5 = true;
			y++;
			map->Show(x, y);
		}
		if (GetAsyncKeyState(55) & 0x8000 && kDown5 == true)
		{
			kDown5 = false;
			y++;
			map->Show(x, y);
		}

		return true;
	}

	bool World::Show()
	{
		map->Draw(fillmode);
		hum->Show();
		return true;
	}
}
