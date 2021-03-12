#include <iostream>
#include <ugex.h>

class SandBox : public uge::Game {
public:
	SandBox() {

	}
	bool Initiate() {
		std::cout << "SandBox::Initiate()" << std::endl;

		//设置参数
		pUge->SetTitle("UGE Initiate - tut01");
		pUge->SetWindowed(true);
		pUge->SetScreen(800, 600);
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

uge::Game* uge::CreateGame()
{
	std::cout << "调试输出 CreateApplication" << std::endl;
	return new SandBox();
}