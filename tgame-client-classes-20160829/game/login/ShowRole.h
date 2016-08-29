#ifndef ShowRole_h__
#define ShowRole_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CMainHero;

class ShowRole: public CCNode
{
public:
	ShowRole();
	virtual ~ShowRole();
	
	static ShowRole* create();

	// override start

	virtual bool init();

	virtual void onEnter();

	virtual void onExit();

	// override end

	unsigned int getPlayerID();

	void setPlayerID(unsigned int uPlayerID);

	void play();

	void stop();

private:
	void updateAnim(float dt);

	void onAnimPlayEvent(CCArmature* pArmature, MovementEventType type, const char *pszActionName);

private:
	unsigned int m_uPlayerID;

	CCArmature *m_pRoleArmature;

	CMainHero *m_pRole;

	bool m_bIsPlaying;

	std::vector<int> m_stShowIdxs; // 用于登录展示的动作索引

};

#endif // ShowRole_h__