#pragma once

#include "BaseProxy.h"
class SyncProxy: public BaseProxy
{
public:
	SyncProxy(void);
	~SyncProxy(void);
	void DoRsp(NetPack& stRsp);
	bool fault(const fgame::CSErrorMsg &err);

	void sendOpenStageRqst(int stageId); 
	void sendChangeName(std::string name, int areaId);
	void sendChangePlayerIcon(int iconId,std::string iconUrl = "");
	
	void sendChangePlayerTitle(int iTitle);			//���õ�ǰ����

	void sendSyncPlayerInfoRqst();														//ͬ����Ҹ�����Ϣ---����
	void doSyncPlayerInfoRsp(const fgame::CSSycPlayerInfoResp& resp);					//ͬ����Ҹ�����Ϣ---����

	void sendChangeSignRqst(std::string data);                                            //����ǩ��
	void sendChangeInforVisibleRqst(bool isVisible);                                       //��������Ƿ�ɼ�
	void sendExtraInfoRqst(int uin);                                                      //��ȡ���������Ϣ

};

