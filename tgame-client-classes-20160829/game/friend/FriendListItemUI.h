#ifndef __FRIEND_LIST_ITEM_UI_H__
#define __FRIEND_LIST_ITEM_UI_H__

#include "basic/BaseLayout.h"
#include "CSFriendMsg.pb.h"

//�����б�ͺ������б�ȶ��õ�����ṹ
class FriendListItemUI: public BaseLayout
{
public:
    
    enum EFRIENDLISTSTAT
    {
        ESTAT_NULL = 0, //������
        ESTAT_FRIEND = 1, //�����б�
        ESTAT_MORE = 2, //�����Ƽ�
        ESTAT_ADMIN = 3, //��������
        ESTAT_BLACKLIST = 4, //������
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
    
    void setStat(const int iStat); //����item�Ǵ����ĸ�tabҳ��list��

    void setDelBtnVisible(bool isVisible); //��ʾɾ�����Ѱ�ť
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
    

	bool m_bIsSelected;				// �Ƿ�ѡ��
    vmsg::CSOneFriend m_stFriendInfo;

    int m_iStat; //��ǰitem��stat�������ĸ��б���
};

#endif // __FRIEND_LIST_ITEM_UI_H__
