#include "hum.h"

namespace game {
	ugeLine line1, line2;

	Hum::Hum(UGE* pUge):wzlCache(pUge)
	{
		current_dir = DOWN;
		this->pUge = pUge;
		pUge->GetScreen(&screen_width, &screen_height);
		_Load(current_dir * 8,4);
	}

	void Hum::_Load(int sort, int num)
	{
		wzlCache.ReleaseWzl(&anima);
		wzlCache.LoadWzl("data\\hum", sort, num, &anima);

		float lw = static_cast<float>(screen_width);
		float lh = static_cast<float>(screen_height);
		float lx = lw / 2;
		float ly = lh / 2;
		line1 = { lx,0.0f,lx,lh,0xFFFFFFFF };
		line2 = { 0.0f ,ly,lw,ly,0xFFFFFFFF };
	}

	void Hum::Update()
	{
		float _x{ 0 }, _y{ 0 };
		pUge->GetMousePos(&_x,&_y);
		if (pUge->KeyDown(UGEK_LBUTTON))
		{
			_GetDir(_x, _y);
			_Load(current_dir * 8, 4);
		}

		if (pUge->KeyDown(UGEK_UP))
		{
			y = --y;
		}
		if (pUge->KeyDown(UGEK_RIGHT))
		{
			x = ++x;
		}
		if (pUge->KeyDown(UGEK_DOWN))
		{
			y = ++y;
		}
		if (pUge->KeyDown(UGEK_LEFT))
		{
			x = --x;
		}
	}

	void Hum::_GetDir(float x, float y)
	{
		float px = x - screen_width / 2;
		float py = y - screen_height / 2;

		//反正切(已知y/x求角度) 正切(已知角度求y/x)
		double rad = std::atan2(py, px);
		double angle = 180 * rad / PI;//弧度制转角度

		if (angle >= -112.5 && angle < -67.5) {
			current_dir = UP;
		}
		else if (angle >= -67.5 && angle < -22.5) {
			current_dir = RIGHT_UP;
		}
		else if (angle >= -22.5 && angle < 22.5) {
			current_dir = RIGHT;
		}
		else if (angle >= 22.5 && angle < 67.5) {
			current_dir = RIGHT_DOWN;
		}
		else if (angle >= 67.5 && angle < 112.5) {
			current_dir = DOWN;
		}
		else if (angle >= 112.5 && angle < 157.5) {
			current_dir = LEFT_DOWN;
		}
		else if ((angle >= 157.5 && angle <= 179) || (angle >= -179 && angle < -157.5)) {
			current_dir = LEFT;
		}
		else if (angle >= -157.5 && angle <= -112.5) {
			current_dir = LEFT_UP;
		}

		Log("当前点击方向:%d", current_dir);
	}

	void Hum::Show()
	{
		pUge->DxRenderQuad(&line1);
		pUge->DxRenderQuad(&line2);

		anima.x = static_cast<float>(screen_width/2 - 25);
		anima.y = static_cast<float>(screen_height/2 - 22);
		pUge->DxRenderQuad(reinterpret_cast<uge::ugeAnimation*>(&anima));
	}
}