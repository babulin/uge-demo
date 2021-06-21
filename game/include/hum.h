#pragma once
#include "uge.h"
#include <wzlcache.h>
using namespace uge;
using namespace wzl;

namespace game {

	const double PI = 3.1415927;

	//人物方向枚举 0~7
	enum DIRECTION
	{
		UP,
		RIGHT_UP,
		RIGHT,
		RIGHT_DOWN,
		DOWN,
		LEFT_DOWN,
		LEFT,
		LEFT_UP
	};

	class Hum {
		UGE* pUge;
		WzlCache wzlCache;
		wzl::ugeAnimation anima;
		int screen_width;
		int screen_height;
		DIRECTION current_dir;
	public:
		float x = 310.0f;
		float y = 271.0f;
	public:
		Hum(UGE* pUge);
		void Update();
		void Show();

	private:
		void _Load(int sort,int num);
		void _GetDir(float x, float y);
	};
}