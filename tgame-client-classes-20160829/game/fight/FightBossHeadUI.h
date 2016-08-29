#ifndef FightBossHeadUI_h__
#define FightBossHeadUI_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BaseLayout.h"
#include "BasicTypes.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace gui;
using namespace std;

const unsigned int BIG_BOSS_HPNUM = 3;
const unsigned int SMALL_BOSS_HPNUM = 3;

class CFightBossHeadUI : public BaseLayout
{
public:
	CFightBossHeadUI();
	virtual ~CFightBossHeadUI();

	bool init();

	CREATE_FUNC(CFightBossHeadUI);

	void setBossType(unsigned int type, string headRes);
	unsigned int getBossType();

	void setHPPercent(UINT64_t nowHP, UINT64_t maxHP);

	void setParent(CCNode* parent);

	void setVisible(bool visible);

	void setAllChildOpacity(GLubyte opacity);
protected:
private:
	UILoadingBar* bossHp;		//上层血条
	ImageView* bossHpBg;		//下层血条
	LabelAtlas* hpNum;
	ImageView* m_pBossHead;		//boss头像

	unsigned int m_uType;		//boss类型
	unsigned int m_uHpNum;		//血条数量
};

#endif // FightBossHeadUI_h__
