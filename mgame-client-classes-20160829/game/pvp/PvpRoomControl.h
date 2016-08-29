#pragma once
#include "Constant.h"
#include "Singleton.h"
#include "net/NetPack.h"
#include "game/ui/dialogs/PvpRoom.h"
#include "PvpRoomProxy.h"
#include "game/ui/dialogs/PvpInviteDialog.h"
#define DEFAULT_TIME_TO_START 10
#define DEFAULT_TIME_TO_DEFAULTSTART 60
#define  CREATE_PRICE 8
#define  ChallengeTime 20
enum eRoomMode
{
	ROOMMODE_TWO = 2,		//2��ģʽ
	ROOMMODE_THREE = 3,		//3��ģʽ
	ROOMMODE_FOUR = 4,		//4��ģʽ
};

class PvpRoomControl
{
public:
	PvpRoomControl();
	~PvpRoomControl();
	
	//������������
	void sendCreateRoomRqst();
	//��������ذ�
	void doCreateRoomRsp(const fgame::CSPersonalRoomCreateResp& resp);

	//���뷿��ذ�
	void doEnterRoomRsp(const fgame::CSPersonalRoomEnterResp& resp);
	
	//�ı䷿��ģʽ����
	void semdChangeRoomModeRqst(eRoomMode);
	//�ı䷿��ģʽ�ذ�
	void doChangeRoomModeRsp(const fgame::CSPersonalRoomModeResp &resp);

	//�뿪��������
	void sendLeaveRoomRqst();
	//�뿪����ذ�
	void doLeaveRoomRsp(const fgame::CSPersonalRoomLeaveResp &resp);

	//�߳��������
	void sendKilloffPlayer(int uin);
	//�߳���һذ�
	void doKilloffPlayerRsp(const fgame::CSPersonalRoomKickResp & resp);

	//�����������
	void sendInvitePlayer(int uin);
	//������һذ�
	void doInvitePlayerRsp(const fgame::CSPersonalRoomInviteResp &resp);

	//��սȫ������
	void sendChallengeRequest();
	//��սȫ���ذ�
	void doChallengeRsp(const fgame::CSPersonalRoomBroadcastResp&resp);

	//��������	
	void sendChatRequest(int id );
	//���Իذ�
	void doChatRsp(const fgame::CSPersonalRoomChatResp& resp);
	
	//��ʼս������
	void sendBeginFightRequest();
	//��ʼս���ذ�
	void doBeginFightRsp(const fgame::CSPersonalRoomBeginFightResp &resp);
	
	//���������ս
	void sendEnterRoomRequest(int roomid,std::string serverNode);

	//����ʱ�Զ�����
	void doCountDownRsp(const fgame::CSPersonalRoomCountDownResp &resp);

	vector <FriendSystem::PlayerData>  mPlayerList;

	void showTip(std::string str);//��ʾ��Ϣ

	void deletePlayer(int uin);	
private:
	PvpRoomProxy g_PvpRoomProxy;
	CC_SYNTHESIZE(PvpRoom*,mPvpRoom,PvpRoom);				
	CC_SYNTHESIZE(PvpInviteDialog*, mPvpInvieDialog, PvpInviteDilog);
	CC_SYNTHESIZE(eRoomMode, mRoomMode, RoomMode);			//��ǰ����ģʽ
	CC_SYNTHESIZE(int,mRoomID,RoomId);						//����ID
	CC_SYNTHESIZE(bool,mIsHourse,IsHourse);					//�Ƿ��Ƿ���
	CC_SYNTHESIZE(int, mPlayerNum,PlayerNum);				//��������			
};

#define  sgPvpRoomControl()  Singleton<PvpRoomControl>::Get()

