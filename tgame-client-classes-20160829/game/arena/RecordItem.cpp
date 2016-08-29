#include "RecordItem.h"
#include "TimeEx.h"
#include "TextHelper.h"
#include "ToolUtils.h"
#include "ArenaControl.h"
#include "Constant.h"
#include "CommonDialog.h"
#include "NounHelper.h"
#include "defines.h"

using namespace std;

CRecordItem::CRecordItem(){

}

CRecordItem::~CRecordItem(){

}

bool CRecordItem::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/record_item_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pFightBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("fight_btn"));
		m_pRecordTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("record_txt"));
		m_pTimeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("time_txt"));

		m_pFightBtn->addTouchEventListener(this, toucheventselector(CRecordItem::touchEventHandler));

		bRet = true;
	} while (0);

	return bRet;
}

void CRecordItem::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "fight_btn") == 0) {
			const CSArenaQueryRsp& stArenaInfo = P_ARENA_CTRL->getArenaObj()->getArenaInfo();

			if(stArenaInfo.remainfightcd() > 0){  //冷却中
				COMMON_DIALOG->show(TextStr(ARENA_FIGHT_CD_TXT));
				return;
			}

			unsigned int lastNum = MAX_ARENA_FREE_NUM + stArenaInfo.todaybuynum() - stArenaInfo.todayfightnum();
			if(lastNum <= 0){
				COMMON_DIALOG->show(TextStr(ARENA_FIGHT_NUM_NOT_ENOUGH_TXT), NULL, NounStr(OK));
				return;
			}		

            P_ARENA_CTRL->getArenaObj()->setFightBackInfo(m_stInfo.time());
			P_ARENA_CTRL->getArenaObj()->setOpponentUin(m_stInfo.opponentuin());
			P_ARENA_CTRL->sendArenaStartFightRqst(P_ARENA_CTRL->getArenaObj()->getOpponentUin());
			P_ARENA_CTRL->getRecordPanel()->loading();
		}
		break;
	default:

		break;
	}
}

void CRecordItem::setInfo(const CSArenaOneRecord& stInfo){
	m_stInfo = stInfo;
	updateUI();
}

void CRecordItem::updateUI(){
	string timeStr = TimeEx::parseSecondToDate(m_stInfo.time(), "YMD");

	char str[200];
	unsigned int idx;

    bool bShowCoin = true;
    bool bCanFightBack = false;
    if(m_stInfo.isfightback()) {
        //反击
        if(m_stInfo.ismechallenger()) {
            if(m_stInfo.ismewin()){
                idx = ARENA_RD_WIN;
            }else{
                idx = ARENA_RD_LOSE;
            } 
        } else {
            if(m_stInfo.ismewin()) {
                idx = ARENA_RD_WIN_BACK; //无奖励文本
                bShowCoin = false;
            } else {
                idx = ARENA_RD_LOSE_BACK;
            }
        }
        
    } else {
        if(m_stInfo.ismewin()){
            idx = ARENA_RD_WIN;
        }else{
            if(m_stInfo.ismechallenger()) {
                idx = ARENA_RD_LOSE;
            } else {
                idx = ARENA_RD_LOSE_OTHER;
                bCanFightBack = true;
            }
        }
    }
    m_pFightBtn->setVisible(bCanFightBack);
    m_pFightBtn->setEnabled(bCanFightBack);
    if(bShowCoin) {
        int iScore = m_stInfo.scorechange();
        string strScore;
        if(iScore >= 0) {
            strScore = strScore + "+" + intToString(iScore); 
        } else {
            strScore = strScore + "-" + intToString(-iScore); 
        }
        int iCoin = m_stInfo.coinchange();
        if(iCoin <= 0) {
            iCoin = -iCoin;
        }
        snprintf(str, 200, TextStr(idx), m_stInfo.opponentname().c_str(), iCoin, strScore.c_str() ); 
        
    } else {
        snprintf(str, 200, TextStr(idx), m_stInfo.opponentname().c_str()); 
    }

	string recordStr = str;
	m_pRecordTxt->setText(recordStr);
	//m_pRecordTxt->setText(CToolUtils::splitUTFStr(recordStr, 21));
	m_pTimeTxt->setText(timeStr);
}