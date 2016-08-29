
#include "stdafx.h"

#include "GameMessages.h"

#define MESSAGE_FACTORY_BEGIN(msg) \
struct Factory##msg : public GameMessageFactory{ \
	virtual GameMessage* create(const std::string& paras = ""){ \
		msg* ret = new msg; \

#define MESSAGE_FACTORY_END(msg,opcode) \
		return ret;	}}; \
		bool fac_##msg = MessageManager::Get()->registerFactory(opcode,new Factory##msg);


MESSAGE_FACTORY_BEGIN(MsgLoadingFrameEnterGame)
MESSAGE_FACTORY_END(MsgLoadingFrameEnterGame,MSG_LOADINGFRAME_ENTERGAME)



MESSAGE_FACTORY_BEGIN(MsgMainFrameChangePage)
char pagename[128];
int boolret;
sscanf(paras.c_str(), "%128[^\t]\t%d",pagename,&boolret);
ret->pageName = pagename;
ret->needPopAllPage = (bool)boolret;
MESSAGE_FACTORY_END(MsgMainFrameChangePage,MSG_MAINFRAME_CHANGEPAGE);



MESSAGE_FACTORY_BEGIN(MsgMainFramePushPage)
char pagename[128];
int boolret;
sscanf(paras.c_str(), "%128[^\t]",pagename);
ret->pageName = pagename;
MESSAGE_FACTORY_END(MsgMainFramePushPage,MSG_MAINFRAME_PUSHPAGE);



MESSAGE_FACTORY_BEGIN(MsgMainFramePopPage)
	char pagename[128];
sscanf(paras.c_str(), "%128[^\t]",pagename);
ret->pageName = pagename;
MESSAGE_FACTORY_END(MsgMainFramePopPage,MSG_MAINFRAME_POPPAGE);


MESSAGE_FACTORY_BEGIN(MsgMainFrameMSGShow)
MESSAGE_FACTORY_END(MsgMainFrameMSGShow,MSG_MAINFRAME_MSGSHOW)



MESSAGE_FACTORY_BEGIN(MsgMainFrameCoverShow)
char pagename[128];
sscanf(paras.c_str(), "%128[^\t]",pagename);
ret->pageName = pagename;
MESSAGE_FACTORY_END(MsgMainFrameCoverShow,MSG_MAINFRAME_COVERSHOW);



MESSAGE_FACTORY_BEGIN(MsgMainFrameCoverHide)
char pagename[128];
sscanf(paras.c_str(), "%128[^\t]",pagename);
ret->pageName = pagename;
MESSAGE_FACTORY_END(MsgMainFrameCoverHide,MSG_MAINFRAME_COVERHIDE)



MESSAGE_FACTORY_BEGIN(MsgCareerChangePage)
int pageID;
sscanf(paras.c_str(), "%d",&pageID);
ret->pageID = pageID;
MESSAGE_FACTORY_END(MsgCareerChangePage,MSG_CAREER_CHANGEPAGE);



MESSAGE_FACTORY_BEGIN(MsgFriendsPageChangePage)
int index;
int oprType;
sscanf(paras.c_str(), "%d\t%d",&index,&oprType);
ret->index = index;
ret->oprType = oprType;
MESSAGE_FACTORY_END(MsgFriendsPageChangePage,MSG_FRIENDSPAGE_CHANGEPAGE);




MESSAGE_FACTORY_BEGIN(MsgLeaveMsgReceivePage)
char uName[128];
int uID;
sscanf(paras.c_str(), "%128[^\t]\t&d",uName,&uID);
ret->uName = uName;
ret->uID = uID;
MESSAGE_FACTORY_END(MsgLeaveMsgReceivePage,MSG_LEAVEMSG_RECIEVEMSG);


MESSAGE_FACTORY_BEGIN(MsgFriendsDetailReceivePage)
int index;
int uID;
int type;
int isAttention;
char uName[128];
sscanf(paras.c_str(), "%d\t%d\t%d\t%d\t128%[^\t\n\0]",&index,&uID,&type,&isAttention,uName);
ret->uName = uName;
ret->index = index;
ret->uID = uID;
ret->type = type;
ret->isAttention = isAttention;
MESSAGE_FACTORY_END(MsgFriendsDetailReceivePage,MSG_FRIEND_DETAIL_RECICEVEMSG);



MESSAGE_FACTORY_BEGIN(MsgFriendsDetailDisposePage)
	int index;
int oprType;
sscanf(paras.c_str(), "%d\t%d",&index,&oprType);
ret->index = index;
ret->oprType = oprType;
MESSAGE_FACTORY_END(MsgFriendsDetailDisposePage,MSG_FRIEND_DETAIL_DISPOSE);



MESSAGE_FACTORY_BEGIN(MsgBuyPropPopupPage)
int toolId;
int panelIndex;
sscanf(paras.c_str(), "%d\t%d",&toolId,&panelIndex);
ret->toolId = toolId;
ret->panelIndex = panelIndex;
MESSAGE_FACTORY_END(MsgBuyPropPopupPage,MSG_BUY_PROP_POPUPMSG);


MESSAGE_FACTORY_BEGIN(MsgSeverInfoUpdate)
	int opcode;
sscanf(paras.c_str(), "%d",&opcode);
ret->opcode = opcode;
MESSAGE_FACTORY_END(MsgSeverInfoUpdate,MSG_SEVERINFO_UPDATE);

MESSAGE_FACTORY_BEGIN(MsgAdventurePageChange)
	int index;
sscanf(paras.c_str(), "%d",&index);
ret->index = index;
MESSAGE_FACTORY_END(MsgAdventurePageChange,MSG_ADVENTURE_PAGECHANGE);


MESSAGE_FACTORY_BEGIN(MsgMainFrameRefreshPage)
	char pagename[128];
sscanf(paras.c_str(), "%128[^\t]",pagename);
ret->pageName = pagename;
MESSAGE_FACTORY_END(MsgMainFrameRefreshPage,MSG_MAINFRAME_REFRESH);

MESSAGE_FACTORY_BEGIN(MsgMainFrameGetNewInfo)
	int type;
sscanf(paras.c_str(), "%d",type);
ret->type = type;
MESSAGE_FACTORY_END(MsgMainFrameGetNewInfo,MSG_GET_NEW_INFO);

MESSAGE_FACTORY_BEGIN(MsgAdventureRemoveItem)
	int index;
sscanf(paras.c_str(), "%d",&index);
ret->index = index;
MESSAGE_FACTORY_END(MsgAdventureRemoveItem,MSG_ADVENTURE_REMOVEITEM);


MESSAGE_FACTORY_BEGIN(MsgInstructionNext)
	int tag;
sscanf(paras.c_str(), "%d",&tag);
ret->tag = tag;
MESSAGE_FACTORY_END(MsgInstructionNext,MSG_INSTRUCTION_NEXT);

MESSAGE_FACTORY_BEGIN(MsgLevelInstructionNext)
	int tag;
sscanf(paras.c_str(), "%d",&tag);
ret->tag = tag;
MESSAGE_FACTORY_END(MsgLevelInstructionNext,MSG_LEVEL_INSTRUCTION_NEXT);

MESSAGE_FACTORY_BEGIN(MsgFightEnd)
int gotItem;
sscanf(paras.c_str(), "%d",&gotItem);
ret->gotItem = gotItem;
MESSAGE_FACTORY_END(MsgFightEnd,MSG_FIGHT_END);


MESSAGE_FACTORY_BEGIN(MsgButtonPressed)
char ccb[128];
char func[128];
int tag;
sscanf(paras.c_str(), "%128[^\t]\t%128[^\t]\t%d",ccb,func,&tag);
ret->ccb = ccb;
ret->func = func;
ret->tag = tag;
MESSAGE_FACTORY_END(MsgButtonPressed,MSG_BUTTON_PRESSED);




MESSAGE_FACTORY_BEGIN(MsgToTab)
int tabIndex;
sscanf(paras.c_str(), "%d",&tabIndex);
ret->tabIndex = tabIndex;
MESSAGE_FACTORY_END(MsgToTab,MSG_TO_TAB);


MESSAGE_FACTORY_BEGIN(MsgFightRoundLoadSkill)
MESSAGE_FACTORY_END(MsgFightRoundLoadSkill,MSG_FIGHTROUND_LOADSKILL)


MESSAGE_FACTORY_BEGIN(MsgRechargeSuccess)
MESSAGE_FACTORY_END(MsgRechargeSuccess,MSG_RECHARGE_SUCCESS)