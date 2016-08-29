#ifndef FightControl_h__
#define FightControl_h__

#include "FightProxy.h"
#include "FightHeroReliveLayer.h"
#include "FightMedicalLayout.h"

class CFightControl
{
public:
	CFightControl();
	~CFightControl();

	CFightProxy* getFightProxy();

	void doNormalFightEnd();

	void doEliteFightEnd();

	void doDailyFightEnd();

	void doBuyBulletRsp(const vmsg::CSBuyBulletRsp& stRsp);

	void doReliveRsp(const vmsg::CSReliveRsp& stRsp);

	bool isWarMapFight();

	void setWarMapFight(bool isWarMapFight);

	bool isLastFightFailed();

	void setLastFightFailed(bool isFailed);

	CFightHeroReliveLayer* getReliveLayout();
	CFightMedicalLayout* getMedicalLayout();
protected:
private:
	CFightProxy* m_pFightProxy;
	CFightHeroReliveLayer* m_pReliveLayout;
	CFightMedicalLayout* m_pMedicalLayout;

	bool m_bLastFightFailed;

	bool m_bWarMapFight;

};

#define P_FIGHT_CTRL CVLib::CVSingleton<CFightControl>::Instance()

#endif // FightControl_h__
