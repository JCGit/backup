#ifndef __MGAME_PVPROOM_H__
#define __MGAME_PVPROOM_H__

#include "ui/system//Window.h"
#include "MessageManager.h"


class PvpRoomItem :Node
{
public:
	virtual bool init();
	CREATE_FUNC(PvpRoomItem);

	Node *		mPlayer;
	Node *		mNone;
	Node *		mClose;
	Button *	mIcon;
	Sprite *	mIconBg;
	Text *		mName;
	Text *		mRank;
	Text *		mWin;
	Text *      mChat;
	Button *    mkillPlayer;
	Sprite *	mCrown;
	Sprite *	mTitle;
	Node *		mVipNode;

private:
	Node *		mRoot;

	void killoffCallBack(cocos2d::Ref* sender);
	void detailCallback(cocos2d::Ref* sender);

	
};



class PvpRoom : public Window
{
public:
	PvpRoom();
	~PvpRoom();

	CREATE_FUNC(PvpRoom);

	virtual bool init();

	virtual void onShow();

	virtual void onClose();
	
	virtual void updateUI();

	void showPlayer();//��ʾ���

	void showChat(int uin, int id );	//��ʾ��Ϣ

	void changeMode();	//�ı�ģʽ
	
	Text *getTimeNode(){ return mTime; };
private:
	int			m_iTime2Start;			//�Զ���ʼʱ��	
	vector <PvpRoomItem*>m_vPlayer;		//4����ҽڵ�

	Button * mBtnStart;					//��ʼ��ť			
	Button * mBtn2;						//���˰�ť
	Button * mBtn3;						//���˰�ť
	Button * mBtn4;						//���˰�ť
	Sprite * mSpriteBtnBg;				//�л�ģʽ��ť�ı���	
	Text   * mTime;						//time 
	Node   * mQuicChat;					//��ݷ���
	Button * mChallenge;				//��սȫ��
	Text   * mChallengeTime;			//��սȫ����ȴʱ��
	Window * mInvitePage;				//������Ѵ���

private:
	virtual void onMenuItemAction(std::string, cocos2d::Ref*);


	void removeChat(Node * pSender);

	void update(float dt);


};


#endif //__MGAME_PVPROOM_H__