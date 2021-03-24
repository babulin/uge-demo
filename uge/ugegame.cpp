#include "ugegame.h"
#include <iostream>

namespace uge {
	ugeGame::ugeGame(UGE* pUge)
	{
		this->pUge = pUge;
	}

	ugeGame::~ugeGame()
	{
	}

	bool ugeGame::Initiate()
	{
		std::cout << "ugeGame::Initiate()" << std::endl;
		return true;
	}

	bool ugeGame::Update()
	{
		return true;
	}

	bool ugeGame::Show()
	{
		return true;
	}
}

