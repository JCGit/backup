#ifndef __FRIEND_HELP_ITEM_H__
#define __FRIEND_HELP_ITEM_H__

#include "basic/BaseLayout.h"
#include "CSFriendMsg.pb.h"


class FriendHelpItem: public BaseLayout
{
public:
    FriendHelpItem();
    virtual ~FriendHelpItem();

    static FriendHelpItem* create();

    // override start
    virtual bool init();

    virtual std::string getDescription() const;

    bool getIsSelected() const;

    void setSelected(bool bSelected);

    const vmsg::CSOneFriend& getInfo(); 

    void setInfo(const vmsg::CSOneFriend& stFriend);

    void onItemTouched(const CCPoint& pt);
private:
private:
    UIImageView *m_pHeadImg;

    UILabel *m_pNameTxt;
    UILabel *m_pFpTxt;
    UIImageView* m_pSelIcon; //

    UIImageView* m_pHelpBtnImg;
    UILabel* m_pHelpBtnImgCoinLabel;
    UIImageView* m_pHelpBtnImgCoinIcon;
    UILabel* m_pHelpBtnImgCoinTxt;

    vmsg::CSOneFriend m_stFriendInfo;

};

#endif // __FRIEND_HELP_ITEM_H__
