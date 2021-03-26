#pragma once
#include "uge.h"
using namespace uge;
namespace game {

	class Hum {
		UGE* pUge;
		ugeAnimation anima;
	public:
		Hum(UGE* pUge);
		void Load();
		void Show();
	};
}