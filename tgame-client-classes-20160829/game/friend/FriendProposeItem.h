#ifndef __FRIEND_PROPOSE_ITEM_H__
#define __FRIEND_PROPOSE_ITEM_H__

#include "basic/BaseLayout.h"
#include "CSFriendMsg.pb.h"

//好友列表和黑名单列表等都用的这个结构
class FriendProposeItem: public BaseLayout
{
public:
	FriendProposeItem();
	virtual ~FriendProposeItem();

	static FriendProposeItem* create();

	// override start
	virtual bool init();

	virtual std::string getDescription() const;

	bool getIsSelected() const;

	void selected(bool bSelected);

	const vmsg::CSOneFriend& getInfo(); 

	void setInfo(const vmsg::CSOneFriend& stFriend);

private:
private:
	UIImageView *m_pHeadImg;

	UILabel *m_pNameTxt;
    UILabel *m_pFpTxt;
    vmsg::CSOneFriend m_stFriendInfo;

};

#endif // __FRIEND_LIST_ITEM_H__
