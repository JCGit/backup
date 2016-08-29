#pragma once
#include <string>
#include <list>
#include "Singleton.h"

class RestrictedWord : public Singleton<RestrictedWord>
{
private:
	std::string mFilename;
	std::list<std::string> mRestrictList;

public:
	RestrictedWord(void);
	~RestrictedWord(void);

	static RestrictedWord* getInstance(){return RestrictedWord::Get();}

	void init(const std::string& filename);
	void reload(){init(mFilename);}
	//这里专门用来取名字的时候判断
	bool isStringOK(const std::string& str);
	//这里是聊天的时候检测是否有屏蔽字
	bool isChatWorldStringOK( const std::string& str );

	std::string filterWordSentence( const std::string& str );
	
	bool isSecretOK(const std::string& str);
};

