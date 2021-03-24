#pragma once
#include <uge.h>
using namespace uge;
namespace game {
	class World : public ugeGame {
	public:
		World(UGE* pUge) :ugeGame(pUge) {}
		// Í¨¹ý Game ¼Ì³Ð
		virtual bool Initiate() override;
		virtual bool Update() override;
		virtual bool Show() override;
	};
}