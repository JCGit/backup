#ifndef GuildPanel_h__
#define GuildPanel_h__

#include "basic/BasePanel.h"
#include "CSGuildMsg.pb.h"

class GuildPanel: public BasePanel
{
public:
    enum {
        MAX_GUILD_IMPEACH_TIME = 604800,  //发起弹劾后七天弹劾成功,3600*24*7=604800
        MAX_GUILD_CAN_IMPEACH_TIME = 432000, //酋长离线时间超过5天可发起弹劾,3600*24*5=
    };
public:
	GuildPanel();
	virtual ~GuildPanel();

	static GuildPanel* create();

	// override start

    virtual bool init();

    virtual void open();

    virtual void close();

	// override end

	void updateGuildInfo();

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	void textFieldEventHandler(CCObject *pSender, TextFiledEventType type);

    //void onUpdateImpeachTime(float dt);

private:
	UIButton *m_pBackBtn;

	UILabel *m_pGuildNameTxt;

	UIImageView *m_pGuildLevelImg;

	UILabel *m_pGuildActivenessValTxt;

	UITextField *m_pGuildNoticeInputTxt;

	UIListView *m_pMemberListView;

	UIButton *m_pDailyActBtn;

	UIButton *m_pGuildAdminBtn;

    UILabel *m_pImpeachTipTxt; //弹劾倒计时文本
    int m_iImpeachTime; //弹劾倒计时时间

};

#endif // GuildPanel_h__
