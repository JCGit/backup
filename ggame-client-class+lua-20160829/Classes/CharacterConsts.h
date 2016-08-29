#pragma once
#include "Singleton.h"
#include <string>
#include <map>

struct CHARACTER
{
	enum 
	{
		MAX_YUANFEN_COUNT = 12
	};
	unsigned int id;
	std::string name;
	std::string describe;
	int stars;
	unsigned int bornSkillID;
	int contry;
	unsigned int yuanfens[MAX_YUANFEN_COUNT];
	std::string facePic;
	std::string bodyPic;
	std::string namePic;
	std::string linkPlist;
	int backPic;
};
struct YUANFEN
{
	unsigned int id;
	std::string describe;
};
class CharacterConsts : public Singleton<CharacterConsts>
{
private:
	typedef std::map<unsigned int,CHARACTER* > CHARACTER_LIST;
	CHARACTER_LIST mCharacterList;

	typedef std::map<unsigned int, YUANFEN* > YUANFEN_LIST;
	YUANFEN_LIST mYuanfenList;
public:
	
	void init(const std::string& filename);
	void cleanUp();

	const CHARACTER* getCharacter(unsigned int id);
	const YUANFEN* getYuanfen(unsigned int id);

};

