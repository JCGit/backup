#ifndef __MGAME_PVPINVITEDIALOG_H__
#define __MGAME_PVPINVITEDIALOG_H__

#include "ui/system//ActionWindow.h"
#include "ui/UIListView.h"
#include "Constant.h"

class  PvpInviteItem :public Node
{
public:
	CREATE_FUNC(PvpInviteItem);
	virtual bool init();

	Button *  mBtnIcon;
	Button *  mBtnInvite;
	Text   *  mName;
	Text   *  mWin;
	Text   *  mRank;
	Sprite *  mPlayerTitle;
	Text   *  mLimitLevel;       //���Ƶȼ�

private:
	Node * mRoot;

	void inviteFirendCallback(Ref * pSender);
	
	
};
class PvpInviteDialog : public ActionWindow
{
public:
	PvpInviteDialog();
	~PvpInviteDialog();

	CREATE_FUNC(PvpInviteDialog);

	virtual bool init();

	virtual void onShow();

	virtual void updateUI();
	void detailCallback(Ref * pSender);


private:
	virtual void onMenuItemAction(std::string, cocos2d::Ref*);
	void pushListItem(PvpInviteItem* item,FriendSystem::PlayerData *playerData);
private:
	Text	 * mTime;				//��սȫ��ʣ��ʱ��
	ListView * mListView;			//����list
	int		   mFriendNum;			//������
	vector<FriendSystem::PlayerData*> mPlayerInfo; //������ĺ����б�
};


#endif //__MGAME_PVPINVITEDIALOG_H__