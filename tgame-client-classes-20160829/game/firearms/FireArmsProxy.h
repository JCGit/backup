#ifndef FireArmsProxy_h__
#define FireArmsProxy_h__

#include "QBaseProxy.h"
#include "CSWeaponMsg.pb.h"
#include "BasicTypes.h"

class CFireArmsProxy : public QBaseProxy
{
public:
	CFireArmsProxy();
	virtual ~CFireArmsProxy();

    virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);
    virtual void fault(const vmsg::CSCommonErrMsg &err);

	//≤È—Ø«π–µ¡–±Ì
	void sendWeaponBagRqst();

	//ªª«π
	void sendWeaponChangeRqst(unsigned int bagIdx, UINT64_t weaponGUID);

	//…˝º∂
	void sendWeaponLvUpRqst(unsigned int bagIdx, UINT64_t weaponGUID, vector<vmsg::CSWeaponLvUpConsume> stConsume);

	//…˝–«
	void sendWeaponStarStepRqst(unsigned int bagIdx, UINT64_t weaponGUID, vector<UINT64_t> stNeedWeaponGUIDs);

    void sendWeaponRefitRqst(unsigned int bagIdx, UINT64_t weaponGUID);

	//≤È—Ø…œ’Û«π–µ
	void sendQueryWeaponInBattleRqst();

	//–ﬁ∏ƒ…œ’Û«π–µ
	void sendChangeWeaponInBattleRqst(const vmsg::CSInbattleWeaponElmtBrief& stOldWeapon, const vmsg::CSInbattleWeaponElmtBrief& stNewWeapon);
protected:
private:
};

#endif // FireArmsProxy_h__
