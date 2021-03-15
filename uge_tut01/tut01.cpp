#include <iostream>
#include <ugex.h>

class SandBox : public uge::Game {

public:
	SandBox() {

	}

	bool Config() {
		std::cout << "SandBox::Config()" << std::endl;
		//ÉèÖÃ²ÎÊý
		pUge->SetTitle("UGE Initiate - tut01 v1.0.0");
		pUge->SetWindowed(true);
		pUge->SetScreen(1024, 768);
		return true;
	}

	bool Initiate() {
		std::cout << "SandBox::Initiate()" << std::endl;
		return true;
	}

	bool Update() {
		//std::cout << "SandBox::Update()" << std::endl;
		return true;
	}

	bool Show() {
		//std::cout << "SandBox::Show()" << std::endl;
		return true;
	}
};

uge::Game* uge::gameCreate()
{
	std::cout << "createGame()" << std::endl;
	return new SandBox();
}