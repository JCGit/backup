#pragma once
#include "cocos2d.h"
#include "Constant.h"
#include "SyncProxy.h" 
#include "net/proto/SaveData.pb.h"
#include "Message.h"
class SyncControl
{
public:
	SyncControl(void);
	~SyncControl(void); 
	void init();	
	void update();

	void saveData(); 



	//ͬ�������Ϣ

	void sendSyncPlayerInfoRqst();													//ͬ�������Ϣ---����		
	void doSyncPlayerInfoRsp(const fgame::CSSycPlayerInfoResp& resp);				//ͬ�������Ϣ---����


	//ͬ���ؿ���Ϣ						

	/////////////////////////////////////////


	void doGmCmdRsp(const fgame::CSGMCmdReq&);  
	
	SyncProxy* getSyncProxy();

	void createLoginTimer();
	void removeLoginTimer();

	void createSyncTimer();
	void removeSyncTimer();

	bool checkSyncSaveDataOver();
	void setSyncSaveDataOver(bool value);
private:	
	bool mUploadSaveData;
	bool mSendSavaData;
	bool mSyncSaveDataOver;

	

	void loadSyncData();				//���ļ�����ͬ������
	void refreshSyncUsePropData();		//��������ʹ�õ����������д��protobuf��
 

	void updateLoginTimer();

	SyncProxy m_SyncProxy;

	 
};

#define sgSyncControl() Singleton<SyncControl>::Get()