#include <iostream>
#include <ugex.h>

class SandBox : public uge::Game {

	ugeImage image;
	ugeImage image1;
	bool kDown = false, kDown1 = false;
	float x = 100.0f, y = 100.0f;
public:
	SandBox() {
	}

	~SandBox() {

	}

	bool Config() {
		std::cout << "SandBox::Config()" << std::endl;
		//设置参数
		pUge->SetTitle("UGE Initiate - tut01 v1.0.0");
		pUge->SetWindowed(true);
		pUge->SetScreen(1024, 768);
		return true;
	}

	bool Initiate() {
		std::cout << "SandBox::Initiate()" << std::endl;

		//quad.texture = pUge->LoadTexture("image\\zk.png");
		//quad1.texture = pUge->LoadTexture("image\\cat.jpg");

		pUge->LoadWzl("data\\hum3",0, &image);
		pUge->LoadWzl("data\\hum3",0, &image1);

		return true;
	}

	bool Update() {
		//std::cout << "SandBox::Update()" << std::endl;
		//按下1
		if (GetAsyncKeyState(49) & 0x8000 && kDown == false)
		{
			kDown = true;
			pUge->ReleaseWzl(&image);
			pUge->LoadWzl("data\\hum", 0, &image);
		}

		if (GetAsyncKeyState(50) & 0x8000 && kDown == true)
		{
			kDown = false;
			pUge->ReleaseWzl(&image);
			pUge->LoadWzl("data\\hum3", 0, &image);
		}

		if (GetAsyncKeyState(51) & 0x8000 && kDown1 == false)
		{
			kDown1 = true;
			pUge->ReleaseWzl(&image1);
			pUge->LoadWzl("data\\hum", 0, &image1);
		}
		if (GetAsyncKeyState(52) & 0x8000 && kDown1 == true)
		{
			kDown1 = false;
			pUge->ReleaseWzl(&image1);
			pUge->LoadWzl("data\\hum3", 0, &image1);
		}
		return true;
	}

	bool Show() {
		//std::cout << "SandBox::Show()" << std::endl;

		for (size_t j = 0; j < 4; j++)
		{
			image.x = 100.0f + j * image.width;
			image.y = 100.0f;
			pUge->DxRenderQuad(&image);
		}		
		
		for (size_t j = 0; j < 4; j++)
		{
			image1.x = 100.0f + j * image1.width;
			image1.y = 200.0f;
			pUge->DxRenderQuad(&image1);
		}

		return true;
	}
};

uge::Game* uge::gameCreate()
{
	std::cout << "createGame()" << std::endl;
	return new SandBox();
}