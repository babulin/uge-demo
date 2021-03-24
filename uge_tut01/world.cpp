#include "world.h"
#include "login.h"

namespace game {

	ugeImage hum;
	ugeAnimation animation;
	ugeAnimation animation2;
	bool kDownSpace = false;
	bool kDown3 = false;
	bool World::Initiate()
	{
		std::cout << "World::Initiate()" << std::endl;

		//¼ÓÔØÈËÎï
		//pUge->LoadWzl("data\\hum3", 7360, &hum);
		//pUge->LoadWzl("data\\hum3", 7296, &hum);
		//pUge->LoadWzl("data\\ui", 1813, &hum);
		pUge->LoadWzl("data\\hum", 7200, &hum);
		pUge->LoadWzl("data\\hum", 11552, 6, &animation);

		return true;
	}

	bool World::Update()
	{
		hum.x = 400.0f;
		hum.y = 300.0f;

		animation.x = 100.0f;
		animation.y = 100.0f;

		animation2.x = 100.0f;
		animation2.y = 200.0f;

		if (GetAsyncKeyState(49) & 0x8000 && kDownSpace == false)
		{
			kDownSpace = true;
			pUge->ReleaseWzl(&animation);
			pUge->LoadWzl("data\\hum", 11544, 6, &animation);
			animation2.rate = 8.0f / 60.0f;
		}

		if (GetAsyncKeyState(50) & 0x8000 && kDownSpace == true)
		{
			kDownSpace = false;
			pUge->ReleaseWzl(&animation);
			pUge->LoadWzl("data\\hum", 11552, 6, &animation);
			animation.rate = 5.0f / 60.0f;
		}

		if (GetAsyncKeyState(51) & 0x8000 && kDown3 == false)
		{
			kDown3 = true;
			pUge->ReleaseWzl(&animation);
			pUge->LoadWzl("data\\hum3", 11552, 6, &animation);
			animation.rate = 5.0f / 60.0f;
		}

		return true;
	}

	bool World::Show()
	{
		pUge->DxRenderQuad(&hum);

		pUge->DxRenderQuad(&animation);

		
		return true;
	}
}
