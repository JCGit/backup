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

	//��ѯǹе�б�
	void sendWeaponBagRqst();

	//��ǹ
	void sendWeaponChangeRqst(unsigned int bagIdx, UINT64_t weaponGUID);

	//����
	void sendWeaponLvUpRqst(unsigned int bagIdx, UINT64_t weaponGUID, vector<vmsg::CSWeaponLvUpConsume> stConsume);

	//����
	void sendWeaponStarStepRqst(unsigned int bagIdx, UINT64_t weaponGUID, vector<UINT64_t> stNeedWeaponGUIDs);

    void sendWeaponRefitRqst(unsigned int bagIdx, UINT64_t weaponGUID);

	//��ѯ����ǹе
	void sendQueryWeaponInBattleRqst();

	//�޸�����ǹе
	void sendChangeWeaponInBattleRqst(const vmsg::CSInbattleWeaponElmtBrief& stOldWeapon, const vmsg::CSInbattleWeaponElmtBrief& stNewWeapon);
protected:
private:
};

#endif // FireArmsProxy_h__
