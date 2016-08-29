#pragma once
#include "MessageManager.h"
#include "GameConsts.h"
#include "StringConverter.h"
#include "DataTableManager.h"

enum
{
	MSG_LOADINGFRAME_ENTERGAME,
	MSG_MAINFRAME_CHANGEPAGE,
	MSG_MAINFRAME_PUSHPAGE,
	MSG_MAINFRAME_POPPAGE,
	MSG_MAINFRAME_COVERSHOW,
	MSG_MAINFRAME_COVERHIDE,
	MSG_MAINFRAME_MSGSHOW,
	MSG_CAREER_CHANGEPAGE,
	MSG_FRIENDSPAGE_CHANGEPAGE,
	MSG_LEAVEMSG_RECIEVEMSG,
	MSG_FRIEND_DETAIL_RECICEVEMSG,
	MSG_BUY_PROP_POPUPMSG,
	MSG_FRIEND_DETAIL_DISPOSE,
	MSG_CONTINUE_LOGIN_FLIPCARD,
	MSG_ADVENTURE_PAGECHANGE,
	MSG_SEVERINFO_UPDATE,
	MSG_MAINFRAME_REFRESH,
	MSG_GET_NEW_INFO,
	MSG_ADVENTURE_REMOVEITEM,
	MSG_ADVENTURE_TEACHEXPINIT,
	MSG_PACKPREVIEW_SOURCE_MSG,
	MSG_INSTRUCTION_NEXT,
	MSG_LEVEL_INSTRUCTION_NEXT,
	MSG_FIGHT_END,
	MSG_BUTTON_PRESSED,
	MSG_TO_TAB,
	MSG_FIGHTROUND_LOADSKILL,
	MSG_RECHARGE_SUCCESS,
	MSG_FIGHTPAGE_EXIT,
	MSG_TITLESTATUS_CHANGE,
	MSG_ADVENTUREPAGE_HASCHANGE,
	MSG_UPDATE_LEAGUA_CONTRIBUTION,
	MSG_CHAT_WINDOW_ONSWITCH,
	MSG_REFRESH_DISCIPLEPAGE,
	MSG_GOTOPRIVATECHATPAGE,
	MSG_GOTOPRIVATECHATPAGEFROMLEAGUE
};
#define RETURN_TRUE_TYPE(classname) \
	static classname* getTrueType(GameMessage * msg){ return dynamic_cast<classname *>(msg);};

struct MsgLoadingFrameEnterGame : public GameMessage
{
	RETURN_TRUE_TYPE(MsgLoadingFrameEnterGame)
	MsgLoadingFrameEnterGame(){};
	virtual const unsigned int getTypeId() const{return MSG_LOADINGFRAME_ENTERGAME;}
	virtual GameMessage * clone() const {return new MsgLoadingFrameEnterGame();}
};
struct MsgMainFrameChangePage : public GameMessage
{
	RETURN_TRUE_TYPE(MsgMainFrameChangePage)
	MsgMainFrameChangePage(): needPopAllPage(true){};
	std::string pageName;
	bool needPopAllPage;
	virtual const unsigned int getTypeId() const{return MSG_MAINFRAME_CHANGEPAGE;}
	//virtual std::string getParameters() const{return pageName+(needPopAllPage?"\t1":"\t0");}
	virtual GameMessage * clone() const{MsgMainFrameChangePage*ret = new MsgMainFrameChangePage;ret->pageName = pageName;return ret;}
};

struct MsgMainFramePushPage : public GameMessage
{
	RETURN_TRUE_TYPE(MsgMainFramePushPage)
	MsgMainFramePushPage(){}
	std::string pageName;
	virtual const unsigned int getTypeId() const{return MSG_MAINFRAME_PUSHPAGE;}
	//virtual std::string getParameters() const{return pageName;}
	virtual GameMessage * clone() const{MsgMainFramePushPage*ret = new MsgMainFramePushPage;ret->pageName = pageName; return ret;}
};

struct MsgMainFramePopPage : public GameMessage
{
	RETURN_TRUE_TYPE(MsgMainFramePopPage)
	MsgMainFramePopPage(){};
	std::string pageName;
	virtual const unsigned int getTypeId() const{return MSG_MAINFRAME_POPPAGE;}
	//virtual std::string getParameters() const{return pageName;}
	virtual GameMessage * clone() const{MsgMainFramePopPage*ret = new MsgMainFramePopPage;ret->pageName = pageName;return ret;}
};
struct MsgMainFrameMSGShow : public GameMessage
{
	RETURN_TRUE_TYPE(MsgMainFrameMSGShow)
	MsgMainFrameMSGShow(){};
	virtual const unsigned int getTypeId() const{return MSG_MAINFRAME_MSGSHOW;}
	virtual GameMessage * clone() const{MsgMainFrameMSGShow*ret = new MsgMainFrameMSGShow;return ret;}
};
struct MsgMainFrameCoverShow : public GameMessage
{
	RETURN_TRUE_TYPE(MsgMainFrameCoverShow)
	MsgMainFrameCoverShow(){};
	virtual const unsigned int getTypeId() const{return MSG_MAINFRAME_COVERSHOW;}
	std::string pageName;
	//virtual std::string getParameters() const{return pageName;}
	virtual GameMessage * clone() const{MsgMainFrameCoverShow*ret = new MsgMainFrameCoverShow;ret->pageName = pageName;return ret;}
};

struct MsgChatWindowOnSwitch : public GameMessage
{
	RETURN_TRUE_TYPE(MsgChatWindowOnSwitch)
		MsgChatWindowOnSwitch(){};
	virtual const unsigned int getTypeId() const{return MSG_CHAT_WINDOW_ONSWITCH;}
	virtual GameMessage * clone() const {return new MsgChatWindowOnSwitch();}
};

struct MsgRefreshDisciplePage : public GameMessage
{
	RETURN_TRUE_TYPE(MsgRefreshDisciplePage)
		MsgRefreshDisciplePage(){};
	virtual const unsigned int getTypeId() const{return MSG_REFRESH_DISCIPLEPAGE;}
	virtual GameMessage * clone() const {return new MsgRefreshDisciplePage();}
};

struct MsgGoToPrivateChatPage : public GameMessage
{
	RETURN_TRUE_TYPE(MsgGoToPrivateChatPage)
		MsgGoToPrivateChatPage(){};
	virtual const unsigned int getTypeId() const{return MSG_GOTOPRIVATECHATPAGE;}
	virtual GameMessage * clone() const {return new MsgGoToPrivateChatPage();}
};

struct MsgGoToPrivateChatPageFromLeague : public GameMessage
{
	RETURN_TRUE_TYPE(MsgGoToPrivateChatPageFromLeague)
		MsgGoToPrivateChatPageFromLeague(){};
	virtual const unsigned int getTypeId() const{return MSG_GOTOPRIVATECHATPAGEFROMLEAGUE;}
	virtual GameMessage * clone() const {return new MsgGoToPrivateChatPageFromLeague();}
};

struct MsgMainFrameCoverHide : public GameMessage
{
	RETURN_TRUE_TYPE(MsgMainFrameCoverHide)
	MsgMainFrameCoverHide(){};
	virtual const unsigned int getTypeId() const{return MSG_MAINFRAME_COVERHIDE;}
	std::string pageName;
	virtual GameMessage * clone() const{MsgMainFrameCoverHide*ret = new MsgMainFrameCoverHide;ret->pageName = pageName;return ret;}
};
struct MsgCareerChangePage : public GameMessage
{
	RETURN_TRUE_TYPE(MsgCareerChangePage)
	MsgCareerChangePage(){};
	int pageID;
	virtual std::string getParameters() const{return StringConverter::toString(pageID);}
	virtual const unsigned int getTypeId() const{return MSG_CAREER_CHANGEPAGE;}
	virtual GameMessage * clone() const{MsgCareerChangePage*ret = new MsgCareerChangePage;ret->pageID = pageID;return ret;}
};

struct MsgFriendsPageChangePage : public GameMessage
{
	RETURN_TRUE_TYPE(MsgFriendsPageChangePage)
	MsgFriendsPageChangePage(){};
	int index;
	int oprType;
	virtual const unsigned int getTypeId() const{return MSG_FRIENDSPAGE_CHANGEPAGE;}
	//virtual std::string getParameters() const{return StringConverter::toString(index)+"\t"+StringConverter::toString(oprType);}
	virtual GameMessage * clone() const{MsgFriendsPageChangePage*ret = new MsgFriendsPageChangePage;ret->index = index;ret->oprType=oprType;return ret;}
};

struct MsgLeaveMsgReceivePage : public GameMessage
{
	RETURN_TRUE_TYPE(MsgLeaveMsgReceivePage)
	MsgLeaveMsgReceivePage(){};
	int uID;
	std::string uName;
	virtual const unsigned int getTypeId() const{return MSG_LEAVEMSG_RECIEVEMSG;}
	//virtual std::string getParameters() const{return StringConverter::toString(uID)+"\t"+uName;}
	virtual GameMessage * clone() const{MsgLeaveMsgReceivePage*ret = new MsgLeaveMsgReceivePage;ret->uID = uID;ret->uName=uName;return ret;}
};

struct MsgFriendsDetailReceivePage : public GameMessage
{
	RETURN_TRUE_TYPE(MsgFriendsDetailReceivePage)
	MsgFriendsDetailReceivePage(){};
	int index;
	int uID;
	int type;
	bool isAttention;
	std::string uName;
// 	virtual std::string getParameters() const{
// 		return StringConverter::toString(index)+ "\t"+
// 		StringConverter::toString(uID)+ "\t"+
// 		StringConverter::toString(type) + 
// 		(isAttention?"\t1\t":"\t0\t")+
// 		uName;
// 	}
	virtual const unsigned int getTypeId() const{return MSG_FRIEND_DETAIL_RECICEVEMSG;}
	virtual GameMessage * clone() const{MsgFriendsDetailReceivePage*ret = new MsgFriendsDetailReceivePage;ret->index=index;ret->uID = uID;ret->uName=uName;ret->type=type;ret->isAttention=isAttention;return ret;}
};

struct MsgFriendsDetailDisposePage : public GameMessage
{
	RETURN_TRUE_TYPE(MsgFriendsDetailDisposePage)
	MsgFriendsDetailDisposePage(){};
	int index;
	int oprType;
	//virtual std::string getParameters() const{return StringConverter::toString(index)+"\t"+StringConverter::toString(oprType);}
	virtual const unsigned int getTypeId() const{return MSG_FRIEND_DETAIL_DISPOSE;}
	virtual GameMessage * clone() const{MsgFriendsDetailDisposePage*ret = new MsgFriendsDetailDisposePage;ret->index=index;ret->oprType = oprType;return ret;}
};


struct MsgBuyPropPopupPage : public GameMessage
{
	RETURN_TRUE_TYPE(MsgBuyPropPopupPage)
	MsgBuyPropPopupPage(){};
	unsigned int toolId;
	unsigned int panelIndex;
	//virtual std::string getParameters() const{return StringConverter::toString(toolId)+"\t"+StringConverter::toString(panelIndex);}
	virtual const unsigned int getTypeId() const{return MSG_BUY_PROP_POPUPMSG;}
	virtual GameMessage * clone() const{MsgBuyPropPopupPage*ret = new MsgBuyPropPopupPage;ret->toolId=toolId;ret->panelIndex=panelIndex;return ret;}
};

struct MsgSeverInfoUpdate : public GameMessage
{
	RETURN_TRUE_TYPE(MsgSeverInfoUpdate)
	MsgSeverInfoUpdate(){};
	unsigned int opcode;
	//virtual std::string getParameters() const{return StringConverter::toString(opcode);}
	virtual const unsigned int getTypeId() const{return MSG_SEVERINFO_UPDATE;}
	virtual GameMessage * clone() const{MsgSeverInfoUpdate*ret = new MsgSeverInfoUpdate;ret->opcode=opcode;return ret;}
};

struct MsgAdventurePageChange:public GameMessage
{	
	RETURN_TRUE_TYPE(MsgAdventurePageChange)
	MsgAdventurePageChange(){};
	unsigned int index;
	virtual const unsigned int getTypeId() const{return MSG_ADVENTURE_PAGECHANGE;}
	//virtual std::string getParameters() const{return StringConverter::toString(index);}
	virtual GameMessage * clone() const{MsgAdventurePageChange*ret = new MsgAdventurePageChange;ret->index=index;return ret;}
};
struct MsgMainFrameRefreshPage : public GameMessage
{
	RETURN_TRUE_TYPE(MsgMainFrameRefreshPage)
	MsgMainFrameRefreshPage(){pageName = "";extraParam = "";};
	std::string pageName;
	std::string extraParam;
	//virtual std::string getParameters(){return pageName;}
	virtual const unsigned int getTypeId() const{return MSG_MAINFRAME_REFRESH;}
	virtual GameMessage * clone() const{MsgMainFrameRefreshPage*ret = new MsgMainFrameRefreshPage;ret->pageName = pageName;ret->extraParam = extraParam;return ret;}
};

struct MsgMainFrameGetNewInfo : public GameMessage
{
	RETURN_TRUE_TYPE(MsgMainFrameGetNewInfo)
		MsgMainFrameGetNewInfo(){};
	int type;
	//virtual std::string getParameters(){return pageName;}
	virtual const unsigned int getTypeId() const{return MSG_GET_NEW_INFO;}
	virtual GameMessage * clone() const{MsgMainFrameGetNewInfo*ret = new MsgMainFrameGetNewInfo;ret->type = type;return ret;}
};

struct MsgAdventureRemoveItem : public GameMessage
{
	RETURN_TRUE_TYPE(MsgAdventureRemoveItem)
	MsgAdventureRemoveItem(){};
	unsigned int index;
	//virtual std::string getParameters() const{return StringConverter::toString(index);}
	virtual const unsigned int getTypeId() const{return MSG_ADVENTURE_REMOVEITEM;}
	virtual GameMessage * clone() const{MsgAdventureRemoveItem*ret = new MsgAdventureRemoveItem;ret->index = index;return ret;}
};

struct MsgInstructionNext : public GameMessage
{
	RETURN_TRUE_TYPE(MsgInstructionNext)
	MsgInstructionNext(){};
	unsigned int tag;
	//virtual std::string getParameters() const{return StringConverter::toString(tag);}
	virtual const unsigned int getTypeId() const{return MSG_INSTRUCTION_NEXT;}
	virtual GameMessage * clone() const{MsgInstructionNext*ret = new MsgInstructionNext;ret->tag = tag;return ret;}
};

struct MsgLevelInstructionNext : public GameMessage
{
	RETURN_TRUE_TYPE(MsgLevelInstructionNext)
		MsgLevelInstructionNext(){};
	unsigned int tag;
	//virtual std::string getParameters() const{return StringConverter::toString(tag);}
	virtual const unsigned int getTypeId() const{return MSG_INSTRUCTION_NEXT;}
	virtual GameMessage * clone() const{MsgLevelInstructionNext*ret = new MsgLevelInstructionNext;ret->tag = tag;return ret;}
};

struct MsgFightEnd : public GameMessage
{
	RETURN_TRUE_TYPE(MsgFightEnd)
	MsgFightEnd():gotItem(false) {};
	bool gotItem;
	//virtual std::string getParameters() const{return gotItem?"1":"0";}
	virtual const unsigned int getTypeId() const{return MSG_FIGHT_END;}
	virtual GameMessage * clone() const{MsgFightEnd*ret = new MsgFightEnd;ret->gotItem = gotItem;return ret;}
};

struct MsgButtonPressed : public GameMessage
{
	RETURN_TRUE_TYPE(MsgButtonPressed)
	MsgButtonPressed(){};
	std::string ccb;
	std::string func;
	int tag;
// 	virtual std::string getParameters() const{
// 		return ccb+ "\t"+
// 			func+ "\t"+
// 			StringConverter::toString(tag);
// 	}
	virtual const unsigned int getTypeId() const{return MSG_BUTTON_PRESSED;}
	virtual GameMessage * clone() const{
		MsgButtonPressed*ret = new MsgButtonPressed;
		ret->ccb = ccb;ret->func = func;ret->tag = tag;
		return ret;}
};


struct MsgToTab:public GameMessage
{	
	RETURN_TRUE_TYPE(MsgToTab)
	MsgToTab(){};
	unsigned int tabIndex;
	//virtual std::string getParameters() const{return StringConverter::toString(tabIndex);}
	virtual const unsigned int getTypeId() const{return MSG_TO_TAB;}
	virtual GameMessage * clone() const{MsgToTab*ret = new MsgToTab;ret->tabIndex=tabIndex;return ret;}
};

struct MsgFightRoundLoadSkill : public GameMessage
{
	RETURN_TRUE_TYPE(MsgFightRoundLoadSkill)
	MsgFightRoundLoadSkill(){};
	virtual const unsigned int getTypeId() const{return MSG_FIGHTROUND_LOADSKILL;}
	virtual GameMessage * clone() const {return new MsgFightRoundLoadSkill();}
};

struct MsgRechargeSuccess : public GameMessage
{
	RETURN_TRUE_TYPE(MsgRechargeSuccess)
	MsgRechargeSuccess(){};
	virtual const unsigned int getTypeId() const{return MSG_RECHARGE_SUCCESS;}
	virtual GameMessage * clone() const {return new MsgRechargeSuccess();}
};

struct MsgFightPageExit : public GameMessage
{
	RETURN_TRUE_TYPE(MsgFightPageExit)
	MsgFightPageExit(){};
	virtual const unsigned int getTypeId() const{return MSG_FIGHTPAGE_EXIT;}
	virtual GameMessage * clone() const {return new MsgFightPageExit();}
};

struct MsgTitleStatusChange : public GameMessage
{
	RETURN_TRUE_TYPE(MsgTitleStatusChange)
	MsgTitleStatusChange(){};
	virtual const unsigned int getTypeId() const{return MSG_TITLESTATUS_CHANGE;}
	virtual GameMessage * clone() const {return new MsgTitleStatusChange();}
};

struct MsgAdventurePageHasChange:public GameMessage
{	
	RETURN_TRUE_TYPE(MsgAdventurePageHasChange)
		MsgAdventurePageHasChange(){};
	unsigned int index;
	virtual const unsigned int getTypeId() const{return MSG_ADVENTUREPAGE_HASCHANGE;}
	//virtual std::string getParameters() const{return StringConverter::toString(index);}
	virtual GameMessage * clone() const{MsgAdventurePageHasChange*ret = new MsgAdventurePageHasChange;ret->index=index;return ret;}
};

struct MsgUpdateLeaguaContribution: public GameMessage
{
	RETURN_TRUE_TYPE(MsgUpdateLeaguaContribution);
	MsgUpdateLeaguaContribution()
	{
		leftContribution = 0;
		totalContribution = 0;
	};
	unsigned int leftContribution;
	unsigned int totalContribution;
	virtual const unsigned int getTypeId() const { return MSG_UPDATE_LEAGUA_CONTRIBUTION; }
	virtual GameMessage* clone() const 
	{
		MsgUpdateLeaguaContribution* msg = new MsgUpdateLeaguaContribution;
		msg->leftContribution = leftContribution;
		msg->totalContribution = totalContribution;
		return msg;
	}
};