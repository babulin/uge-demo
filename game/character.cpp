#include "character.h"
#include "login.h"

namespace game {
	bool Character::Initiate()
	{
		std::cout << "Character::Initiate()" << std::endl;
		return true;
	}

	bool Character::Update()
	{
		if (GetAsyncKeyState(50) & 0x8000 && kDownSpace == false)
		{
			kDownSpace = true;
			Login* login = new Login(pUge);
			login->Initiate();
			login->Update();
			pUge->SetGame(login);
			delete this;
		}
		return true;
	}

	bool Character::Show()
	{
		return true;
	}
}
