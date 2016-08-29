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

    //�л�����ʱ����
    void setOSDSceneInf(OSDSceneInf *pOSDSceneInf);
    void stopOSD();

    void sendPlayerAtkRqst(bool bIsShooting, const CCPoint& direct, const CCPoint& pos); //�ϱ����״̬
    void sendPlayerSkillInSceneRqst(const int iSkillID, const CCPoint& direct, const CCPoint& pos); //�ϱ��ż���

private:
    void doSyncPlayerOSDInfo(vmsg::VLogicSvr2CliMsg &stMsg);

private:
    OSDSceneInf *m_pOSDSceneInf; //���¸�ָ�룬�ڻذ�����ʱ���ݴ�ָ��������ͬ����ɫ
};

#endif // CityProxy_h__
