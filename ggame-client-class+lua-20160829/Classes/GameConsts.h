#pragma once
#include "cocos2d.h"
#include "Singleton.h"

class GameConst : public Singleton<GameConst>
{
private:
	GameConst(){};
public:

	void init(const std::string& filename);
	void cleanUp();

	const std::string& getVersonFileAddress();
	
};