#ifndef LoginScene_h__
#define LoginScene_h__

#include "BaseScene.h"
class UpdateResScreen;
class LoginScene: public BaseScene
{
public:
	LoginScene();
	virtual ~LoginScene();

	static LoginScene *create();

	// override start

	virtual bool init();

	virtual void loadRes();

	virtual void build();

	virtual void onEnterTransitionDidFinish();

	virtual void onExit();

	// override end
    void doUpdateResDone();
private:
    CCSprite *m_pBgStart;
    UpdateResScreen *m_pUpdateResScreen; //¼ì²é¸üÐÂ

};

#endif // LoginScene_h__
