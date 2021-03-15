#include <iostream>
#include <ugex.h>

class SandBox : public uge::Game {

	ugeQuad quad;
	ugeQuad quad1;
	ugeQuad quad2[4];
	float x = 100.0f, y = 100.0f;
public:
	SandBox() {
		quad = {};
		quad1 = {};
	}

	~SandBox() {

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

		quad.texture = pUge->LoadTexture("image\\zk.png");
		quad1.texture = pUge->LoadTexture("image\\cat.jpg");

		quad2[0].texture = pUge->LoadWzl("data\\hum3",0);
		quad2[1].texture = pUge->LoadWzl("data\\hum3", 1);
		quad2[2].texture = pUge->LoadWzl("data\\hum3", 2);
		quad2[3].texture = pUge->LoadWzl("data\\hum3", 3);

		return true;
	}

	bool Update() {
		//std::cout << "SandBox::Update()" << std::endl;
		quad.x = 10.0f;
		quad.y = 10.0f;

		quad1.x = 100.0f;
		quad1.y = 100.0f;

		for (size_t i = 0; i < 4; i++)
		{
			quad2[i].x = 200.0f;
			quad2[i].y = 200.0f;
		}

		return true;
	}

	bool Show() {
		//std::cout << "SandBox::Show()" << std::endl;

		pUge->DxRenderQuad(&quad);

		pUge->DxRenderQuad(&quad1);

		static int frame = 0;
		if (frame >= 4)
		{
			frame = 0;
		}
		pUge->DxRenderQuad(&quad2[frame]);
		frame++;

		return true;
	}
};

uge::Game* uge::gameCreate()
{
	std::cout << "createGame()" << std::endl;
	return new SandBox();
}