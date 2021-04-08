#pragma once
#include <uge.h>
using namespace uge;
namespace game{
	class Character : public ugeGame {
		bool kDownSpace = false;
	public:
		Character(UGE* pUge) :ugeGame(pUge) {}
		// Í¨¹ý Game ¼Ì³Ð
		virtual bool Initiate() override;
		virtual bool Update() override;
		virtual bool Show() override;
	};
}