#ifndef CityProxy_h__
#define CityProxy_h__

#include "cocos2d.h"

#include "QBaseProxy.h"

USING_NS_CC;

class OSDSceneInf;

class CityProxy: public QBaseProxy
{
public:
	CityProxy();
	~CityProxy();

	// override rsp handler
	virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);

	virtual void fault(const vmsg::CSCommonErrMsg &err);

	void sendGetPlayersRqst();

	void sendPlayerLeaveRqst();

	void sendPlayerMoveRqst(const CCPoint &direct, const CCPoint &pos);

	void sendPlayerSetPosRqst(const CCPoint &direct, const CCPoint &pos);

	void sendPlayerChangeSceneRqst(unsigned int uSceneID);

    //切换场景时更新
    void setOSDSceneInf(OSDSceneInf *pOSDSceneInf);
    void stopOSD();

    void sendPlayerAtkRqst(bool bIsShooting, const CCPoint& direct, const CCPoint& pos); //上报射击状态
    void sendPlayerSkillInSceneRqst(const int iSkillID, const CCPoint& direct, const CCPoint& pos); //上报放技能

private:
    void doSyncPlayerOSDInfo(vmsg::VLogicSvr2CliMsg &stMsg);

private:
    OSDSceneInf *m_pOSDSceneInf; //更新该指针，在回包处理时根据此指针来更新同屏角色
};

#endif // CityProxy_h__
