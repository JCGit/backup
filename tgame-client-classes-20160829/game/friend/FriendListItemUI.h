#ifndef __FRIEND_LIST_ITEM_UI_H__
#define __FRIEND_LIST_ITEM_UI_H__

#include "basic/BaseLayout.h"
#include "CSFriendMsg.pb.h"

//好友列表和黑名单列表等都用的这个结构
class FriendListItemUI: public BaseLayout
{
public:
    
    enum EFRIENDLISTSTAT
    {
        ESTAT_NULL = 0, //黑名单
        ESTAT_FRIEND = 1, //好友列表
        ESTAT_MORE = 2, //好友推荐
        ESTAT_ADMIN = 3, //好友审批
        ESTAT_BLACKLIST = 4, //黑名单
    };
    
	FriendListItemUI();
	virtual ~FriendListItemUI();

	static FriendListItemUI* create();

	// override start
	virtual bool init();

	virtual std::string getDescription() const;

	bool getIsSelected() const;

	void selected(bool bSelected);

	const vmsg::CSOneFriend& getInfo(); 

	void setInfo(const vmsg::CSOneFriend& stFriend);
    
    void setStat(const int iStat); //设置item是处于哪个tab页的list中

    void setDelBtnVisible(bool isVisible); //显示删除好友按钮
    void updateManualBtnStat();

    void onItemTouched(const CCPoint& pt);

	UIButton* getPresentManualBtn();

	UIButton* getAddFriendBtn();

private:
    void doBtnOneClick();
    void doBtnTwoClick();
private:
	UIImageView *m_pHeadImg;

	UILabel *m_pNameTxt;
    UILabel *m_pLvTxt;
    UILabel *m_pFpTxt;
    //UILabel *m_pIsOnlieTxt;

    UIButton *m_pBtnOne;
    UIButton *m_pBtnTwo;

    UIButton *m_pBtnSingle; 
    

	bool m_bIsSelected;				// 是否选中
    vmsg::CSOneFriend m_stFriendInfo;

    int m_iStat; //当前item的stat，是在哪个列表里
};

#endif // __FRIEND_LIST_ITEM_UI_H__
