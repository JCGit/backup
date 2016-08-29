#pragma once
#include <vector>
#include <string>
#include <map>
using namespace std;
#include "ServerDateManager.h"
class BlackBoard : public Singleton<BlackBoard>
{
public:
	BlackBoard( void );
	/*{
		mVaribleMap.clear();
	}*/


	bool isSamSungi9100Audio;
	unsigned int PLATFORM_TYPE_FOR_LUA;
	bool hasVarible(const std::string& key);

	bool addVarible(const std::string& key, const std::string& value);
	bool addVarible(const std::string& key, bool value);
	bool addVarible(const std::string& key, unsigned int value);

	void delVarible(const std::string& key);

	bool setVarible(const std::string& key, const std::string& value);
	bool setVarible(const std::string& key, bool value);
	bool setVarible(const std::string& key, unsigned int value);

	std::string getVarible(const std::string& key);
	unsigned int getVaribleAsUint(const std::string& key);
	bool getVaribleAsBool(const std::string& key);
	
	std::string fetchVarible(const std::string& key);
	unsigned int fetchVaribleAsUint(const std::string& key);
	bool fetchVaribleAsBool(const std::string& key);

	void clearMap();

	static BlackBoard* getInstance();

private:
		std::map<std::string,std::string> mVaribleMap;
};

#define HAS_V(key)			BlackBoard::Get()->hasVarible(key)
#define ADD_V(key, value)	BlackBoard::Get()->addVarible(key, value)
#define SET_V(key, value)	BlackBoard::Get()->setVarible(key, value)
#define GET_V_S(key)		BlackBoard::Get()->getVarible(key)
#define GET_V_UINT(key)		BlackBoard::Get()->getVaribleAsUint(key)
#define GET_V_B(key)		BlackBoard::Get()->getVaribleAsBool(key)
#define FETCH_V_S(key)		BlackBoard::Get()->fetchVarible(key)
#define FETCH_V_UINT(key)	BlackBoard::Get()->fetchVaribleAsUint(key)
#define FETCH_V_B(key)		BlackBoard::Get()->fetchVaribleAsBool(key)
#define DEL_V(key)			BlackBoard::Get()->delVarible(key)

#define ADD_OR_SET_V(key, value)	if(HAS_V(key)) SET_V(key, value); else ADD_V(key, value)