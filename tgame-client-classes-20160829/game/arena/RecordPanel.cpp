#include "RecordPanel.h"
#include "RecordItem.h"
#include "ArenaControl.h"

using namespace vmsg;

CRecordPanel::CRecordPanel(){

}

CRecordPanel::~CRecordPanel(){

}

bool CRecordPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/record_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pList = dynamic_cast<ListView*>(m_pRootLayout->getChildByName("record_list"));
		m_pTipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("tip_txt"));
		m_pTipTxt->setVisible(false);

		m_pCloseBtn->addTouchEventListener(this, toucheventselector(CRecordPanel::touchEventHandler));

		bRet = true;
	} while (0);

	return bRet;
}

void CRecordPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			this->close();			
		}
		break;
	default:

		break;
	}
}

void CRecordPanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	const CSArenaQueryRecordRsp& stRecord = P_ARENA_CTRL->getArenaObj()->getRecordInfo();

	m_pTipTxt->setVisible(false);
    
	if(stRecord.recordlst_size() <= 0){
		m_pList->removeAllItems();
		m_pTipTxt->setVisible(true);
		return;
	}
    m_pList->removeAllItems();
    /*
	//删除多余的元素
	while(int(m_pList->getItems()->count()) > stRecord.recordlst_size() ){
		m_pList->removeLastItem(); 
	}
    */
	CRecordItem* pItem = NULL;
    const int iMaxNum = stRecord.recordlst_size();
	for(int i = 0; i < iMaxNum; ++i){
		pItem = dynamic_cast<CRecordItem*>(m_pList->getItem(i));
		if(pItem == NULL){
			pItem = CRecordItem::create();
			m_pList->pushBackCustomItem(pItem);
        }
        int iIdx = iMaxNum - 1 - i;
		pItem->setInfo(stRecord.recordlst(iIdx));
	}
}

void CRecordPanel::loading(){
	this->showLoading();
}

void CRecordPanel::open(){
	BasePanel::open();

	P_ARENA_CTRL->sendArenaQueryRecordRqst();

	this->showLoading();
}

void CRecordPanel::close(){
	this->closeLoading();

	BasePanel::close();
	P_ARENA_CTRL->setHasNewRecord(false); // 查看过了
	m_pList->removeAllItems();
}