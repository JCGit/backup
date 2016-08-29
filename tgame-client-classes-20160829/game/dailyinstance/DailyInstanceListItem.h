#ifndef __FRIEND_LIST_ITEM_UI_H__
#define __FRIEND_LIST_ITEM_UI_H__

#include "basic/BaseLayout.h"


class DailyInstanceListItem: public BaseLayout
{
public:
    enum 
    {
        EFF_NODE_TAG = 21,
    };
	DailyInstanceListItem();
	virtual ~DailyInstanceListItem();

	static DailyInstanceListItem* create();

	// override start
	virtual bool init();

	virtual std::string getDescription() const;

    void setCfgKey(const int iCfgKey);
    void setSelect(bool bIsSel);
    int getCfgKey();
private:
    UIImageView* m_pBgSel; //ѡ��ʱ�ı���
    UIImageView* m_pIconCell; //������Сͼ��
    UILabel* m_pNameTxt;
    UILabel* m_pDayTxt;

    int m_iCfgKey;


    CCArmature* m_pstArmature;
};

#endif // __FRIEND_LIST_ITEM_UI_H__
