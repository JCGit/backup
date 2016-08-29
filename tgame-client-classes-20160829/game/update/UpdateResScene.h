#ifndef UpdateResScene_h__
#define UpdateResScene_h__
#include "BaseScene.h"

class UpdateResScreen;

class UpdateResScene: public BaseScene
{
public:
    // override start
    CREATE_FUNC(UpdateResScene);

    bool init();
	virtual void onEnter();

	virtual void onExit();

    virtual void loadRes();

	void setPercentage(float fPercentage);

	void setNoticeWords(const char *pszNoticeWords);

private:
	UpdateResScene();
	~UpdateResScene();

    UpdateResScreen *m_pScreen;
};

#endif // UpdateResScene_h__
