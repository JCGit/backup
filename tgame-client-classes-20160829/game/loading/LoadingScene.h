#ifndef LoadingScene_h__
#define LoadingScene_h__

#include "BaseMapScene.h"

class LoadingScene: public BaseScene
{
public:
	static LoadingScene *sharedLoadingScene();

	// override start

	virtual void onEnter();

	virtual void onExit();

	void setPercentage(float fPercentage);

	void setNoticeWords(const char *pszNoticeWords);

private:
	LoadingScene();
	~LoadingScene();
};

#endif // LoadingScene_h__
