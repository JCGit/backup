#include "ItemDropTipListPanel.h"
#include "IconCell.h"
#include "NounHelper.h"
#include "ToolUtils.h"
#include "ClientSvrMsg.pb.h"
#include "ItemUtils.h"
#include "IconCell.h"
#include "ItemUtils.h"
#include "fight/LevelControl.h"
#include "fight/LevelObj.h"
#include "PopTipLayer.h"
#include "TextHelper.h"
#include "defines.h"

#include "ItemCfgMaster.h"
#include "WarMapCfgMaster.h"
#include "ItemDropTipCfgMaster.h"
#include "EliteWarMapCfgMaster.h"
#include "../warmap/WarMapControl.h"
#include "../city/EntryID.h"
#include "EntryCfgMaster.h"
#include "../city/EntryControl.h"
#include "../fight/FightControl.h"

ItemDropTipListPanel::ItemDropTipListPanel()
{
    m_iType = 0;
    m_iCfgID = 0;
    m_pItemCell = NULL;
}

ItemDropTipListPanel::~ItemDropTipListPanel(){
    //m_pTipList->removeAllItems();
}

bool ItemDropTipListPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());
        
        setClickOutToClose(false);
		
        initLayoutFromJsonFile("ui_v2/drop_way_panel_ui.ExportJson");
        addChild(m_pRootLayout);

        m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
        m_pCloseBtn->addTouchEventListener(this, toucheventselector(ItemDropTipListPanel::touchEventHandler));
		

		m_pItemLayout = dynamic_cast<Layout*>(m_pRootLayout->getChildByName("item_layout"));
		m_pDropLayout = dynamic_cast<Layout*>(m_pRootLayout->getChildByName("drop_layout"));

		m_pSureBtn = dynamic_cast<UIButton*>(m_pItemLayout->getChildByName("sure_btn"));
		m_pSureBtn->addTouchEventListener(this, toucheventselector(ItemDropTipListPanel::touchEventHandler));

		m_pNameTxt = dynamic_cast<Label*>(m_pItemLayout->getChildByName("item_txt"));
		m_pDescTxt = dynamic_cast<Label*>(m_pItemLayout->getChildByName("desc_txt"));

		m_pItemBg = dynamic_cast<ImageView*>(m_pItemLayout->getChildByName("icon_bg"));
		//m_pItemBg->setTouchEnabled(true);
		//m_pItemBg->addTouchEventListener(this, toucheventselector(ItemDropTipListPanel::touchEventHandler));

		m_pItemCell = IconCell::create();
		m_pItemBg->addChild(m_pItemCell);


		UILayout* pdrop = dynamic_cast<UILayout*>(m_pDropLayout->getChildByName("drop_bg"));
		
		m_pTipTable = CCTableView::create(this, pdrop->getContentSize());
		m_pTipTable->setDirection(kCCScrollViewDirectionVertical);
		m_pTipTable->setAnchorPoint(CCPointZero);
		m_pTipTable->setPosition(ccp(5, 5));
		m_pTipTable->setDelegate(this);
		m_pTipTable->setScaleY(1.0f);
		m_pTipTable->setTouchPriority(INT_MIN);
		m_pTipTable->setVerticalFillOrder(kCCTableViewFillTopDown);
		pdrop->addNode(m_pTipTable);
		
        m_pTipTxt = dynamic_cast<UILabel*>(m_pDropLayout->getChildByName("tip_txt"));
        m_pTipTxt->setVisible(false);

		
		bRet = true;
	} while (0);

	return bRet;
}

CCSize ItemDropTipListPanel::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(table->getContentSize().width - 40, 120);
}

unsigned int ItemDropTipListPanel::numberOfCellsInTableView(CCTableView *table)
{
	int pRet = m_pDropInfoVec.size();
	return pRet;
}

void ItemDropTipListPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	ItemDropTipListItem *pCell = dynamic_cast<ItemDropTipListItem*>(cell->getChildByTag(LIST_ITEM_TAG));
	if (NULL == pCell) {
		return;
	}
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

	const unsigned int iWarID = pCell->getWarID();
	const int iWarType = pCell->getWarType();
	const unsigned int iType = pCell->getDropType();
	//检查是否已经通关了

	if (iType == DROP_WAR)
	{
		if(ItemDropTipCfgMaster::WAR_TYPE_NORMAL == iWarType) {
			if(iWarID <= P_LEVEL_OBJ->getMapInfo().newestwarmapid()) {
				P_LEVEL_CTL->getEnterPanel()->open();
				P_LEVEL_CTL->getEnterPanel()->setLevelID(iWarID);
			} else {
				//还没通关
				POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_WAR_NOT_CLEAR_TIP));
			}

		} else if(ItemDropTipCfgMaster::WAR_TYPE_ELITE == iWarType) {
			int pMaxElite = P_WAR_MAP_CTRL->getMaxEliteWarChapterID();//精英副本开放的章节数
			const EliteWarMapCfg* iEliteMap = ELITE_WAR_MAP_CFG_MASTER	->getEliteWarMapCfg(iWarID);

			if( pMaxElite >= iEliteMap->warid())
			{
				P_ENTRY_CTRL->enter(ENTRY_ELITE_WAR, iEliteMap->warid());
			}else
			{
				POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_WAR_ELITE_NO_OPEN));
			}


		}
	}else
	{
		unsigned int pentryID = 0;
		switch (iType)
		{
		case DROP_NIHILITY:
			pentryID = ENTRY_NOTHING_LAND;
			break;
		case DROP_LOTTERY:
			pentryID = ENTRY_LOTTERY;
			break;
		case DROP_BLACK_MARKET:
			pentryID = ENTRY_BLACK_MARKET;
			break;
		case DROP_WEAPON_BOOK:
			pentryID = ENTRY_WEAPON_BOOK;
			break;
		case DROP_ARENA_SHOOP:
			pentryID = ENTRY_ARENA;
			break;
		default:
			break;
		}
		if (P_ENTRY_CTRL->isEntryOpen(pentryID))
		{
			P_ENTRY_CTRL->enter(pentryID);
		}else
		{
			POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_MODULE_NOOPEN));
		}

	}
}

CCTableViewCell* ItemDropTipListPanel::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();
	ItemDropTipListItem *pRewardItem = NULL;

	if (!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pRewardItem = ItemDropTipListItem::create();

		cell->addChild(pRewardItem, 0, LIST_ITEM_TAG);
	}
	else{
		cell->removeAllChildren();
		pRewardItem = ItemDropTipListItem::create();

		cell->addChild(pRewardItem, 0, LIST_ITEM_TAG);
	}

	
	if (m_pDropInfoVec.size() == 0)
	{
		return cell;
	}
	Drop_Info pDropInfo = m_pDropInfoVec[idx];
	pRewardItem->setWarInfo(pDropInfo.idroptype,pDropInfo.iWarID,pDropInfo.iWarType,pDropInfo.iThingType,pDropInfo.iThingID);

	return cell;
}


void ItemDropTipListPanel::touchEventHandler(CCObject* pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			close();			
		}else if(strcmp(pszWidgetName, "sure_btn") == 0) {
			close();			
		}
		break;
	default:

		break;
	}
}

void ItemDropTipListPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
    UIListView *pListView = dynamic_cast<UIListView*>(pSender);

    if(LISTVIEW_ONSELECTEDITEM_END != type) {
        return;
    }


    int selectedIdx = pListView->getCurSelectedIndex();
    ItemDropTipListItem *pCell = dynamic_cast<ItemDropTipListItem*>(pListView->getItem(selectedIdx));
    if(NULL == pCell) {
        return;
    }
    //CCLOG("click %d", selectedIdx);

    const unsigned int iWarID = pCell->getWarID();
    const int iWarType = pCell->getWarType();
	const unsigned int iType = pCell->getDropType();
    //检查是否已经通关了
	
	if (iType == DROP_WAR)
	{
		if(ItemDropTipCfgMaster::WAR_TYPE_NORMAL == iWarType) {
			if(iWarID <= P_LEVEL_OBJ->getMapInfo().newestwarmapid()) {
				P_LEVEL_CTL->getEnterPanel()->open();
				P_LEVEL_CTL->getEnterPanel()->setLevelID(iWarID);
			} else {
				//还没通关
				POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_WAR_NOT_CLEAR_TIP));
			}

		} else if(ItemDropTipCfgMaster::WAR_TYPE_ELITE == iWarType) {
			int pMaxElite = P_WAR_MAP_CTRL->getMaxEliteWarChapterID();//精英副本开放的章节数
			const EliteWarMapCfg* iEliteMap = ELITE_WAR_MAP_CFG_MASTER	->getEliteWarMapCfg(iWarID);

			if( pMaxElite >= iEliteMap->warid())
			{
				P_ENTRY_CTRL->enter(ENTRY_ELITE_WAR, iEliteMap->warid());
			}else
			{
				POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_WAR_ELITE_NO_OPEN));
			}

			
		}
	}else
	{
		unsigned int pentryID = 0;
		switch (iType)
		{
		case DROP_NIHILITY:
			pentryID = ENTRY_NOTHING_LAND;
			break;
		case DROP_LOTTERY:
			pentryID = ENTRY_LOTTERY;
			break;
		case DROP_BLACK_MARKET:
			pentryID = ENTRY_BLACK_MARKET;
			break;
		case DROP_WEAPON_BOOK:
			pentryID = ENTRY_WEAPON_BOOK;
			break;
		case DROP_ARENA_SHOOP:
			pentryID = ENTRY_ARENA;
			break;
		default:
			break;
		}
		if (P_ENTRY_CTRL->isEntryOpen(pentryID))
		{
			P_ENTRY_CTRL->enter(pentryID);
		}else
		{
			POP_TIP_LAYER_MASTER->ShowErrorStr(TextStr(TEXT_MODULE_NOOPEN));
		}

	}
    
    

}

bool ItemDropTipListPanel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(m_pCloseBtn->onTouchBegan(pTouch,pEvent))
	{
		return false;
	}else if (m_pSureBtn->onTouchBegan(pTouch,pEvent))
	{
		return false;
	}
	return true;
}

void ItemDropTipListPanel::open(){
	CToolUtils::loadSpriteFrameRGBA4444("ui_v2/key_stages.plist");
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN, true);
    BasePanel::open();
    updateUI();
	
	
}


void ItemDropTipListPanel::close(){
    BasePanel::close();
    //reset
    //m_pTipList->removeAllItems();
    m_pTipTxt->setVisible(false);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	//CToolUtils::removePlistTexFile("ui_v2/key_stages.plist");
}


void ItemDropTipListPanel::updateUI(){
    const ItemDropTipCfg* pstCfg = ITEM_DROP_TIP_MASTER->GetOneCfg(m_iCfgID, m_iType);
    if(NULL == pstCfg) {
        close();
        CCLOG("[error]ItemDropTipListPanel::updateUI, fail to get cfg!");
        return;
    }
    m_pDropInfoVec.clear();
    //m_pTipList->removeAllItems();
	m_pItemCell->clearCell();
	
	if (pstCfg->droptype_size()>0)
	{
		m_pTipTxt->setVisible(false);
		//m_pTipList->setVisible(true);
		//ItemDropTipListItem* pItem = NULL;

		for (int idx = 0; idx<pstCfg->droptype_size(); idx++)
		{
			unsigned ptype = pstCfg->droptype(idx).type();
			if (ptype == 0)
			{
				continue;
			}
			if (ptype ==DROP_WAR )
			{
				for(int i = 0; i < pstCfg->warlist_size(); ++i) {
					//pItem = ItemDropTipListItem::create();
					//pItem->setWarInfo(ptype ,pstCfg->warlist(i).warid(), pstCfg->warlist(i).wartype(),
						//pstCfg->thingtype(), pstCfg->thingid() );
					//m_pTipList->pushBackCustomItem(pItem);
					Drop_Info pInfo(ptype ,pstCfg->warlist(i).warid(), pstCfg->warlist(i).wartype(),
						pstCfg->thingtype(), pstCfg->thingid());
					m_pDropInfoVec.push_back(pInfo);
				}
			}else
			{
				/*
				pItem = ItemDropTipListItem::create();
				pItem->setWarInfo(ptype ,0, 0,
					pstCfg->thingtype(), pstCfg->thingid() );
				m_pTipList->pushBackCustomItem(pItem);
				*/
				Drop_Info pInfo(ptype ,0, 0,
					pstCfg->thingtype(), pstCfg->thingid());
				m_pDropInfoVec.push_back(pInfo);
			}
			
		}	
	}else
	{
		m_pTipTxt->setText(CToolUtils::splitUTFStr(pstCfg->tiptxt(), 10));
		m_pTipTxt->setVisible(true);
		//m_pTipList->setVisible(false);
	}
    
	m_pNameTxt->setText(ItemUtils::getItemName(m_iType, m_iCfgID));
	m_pDescTxt->setText(ItemUtils::getItemDesc(m_iType, m_iCfgID));
	m_pItemCell->setInfo(m_iType, 1, m_iCfgID);

	CToolUtils::adjustIconBgSize(m_iType, m_iCfgID, m_pItemBg);
	CToolUtils::adjustFragIcon(m_iType,  m_pItemBg);
	
	m_pTipTable->reloadData();
}





////////////////////////////////

ItemDropTipListItem::ItemDropTipListItem()
{

}

ItemDropTipListItem::~ItemDropTipListItem()
{
}

ItemDropTipListItem* ItemDropTipListItem::create() {
    ItemDropTipListItem* pitem = new ItemDropTipListItem();

    if(pitem != NULL && pitem->init()) {
        pitem->autorelease();
    } else {
        CC_SAFE_DELETE(pitem);
    }

    return pitem;
}

bool ItemDropTipListItem::init() {
    bool bRet = false;

    do {
        CC_BREAK_IF(!BaseLayout::init());

        initLayoutFromJsonFile("ui_v2/drop_layout_ui.ExportJson");
        addChild(m_pRootLayout);

        m_pMapImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("drop_map_img"));
        m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("war_name_txt"));
		m_pLvTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("lev_txt"));

        bRet = true;
    } while(false);

    return bRet;
}

std::string ItemDropTipListItem::getDescription() const {
    return "ItemDropTipListItem";
}

void ItemDropTipListItem::setWarInfo(unsigned int idroptype,const int iWarID, const int iWarType, const int iThingType, const int iThingID) {
    m_iWarID = iWarID;
    m_iWarType = iWarType;
    m_iThingType = iThingType;
    m_iThingID = iThingID;
	m_dropType = idroptype;
    updateUI();
}
void ItemDropTipListItem::updateUI() {

	if(m_dropType == DROP_WAR)
	{
		if(m_iWarType == ItemDropTipCfgMaster::WAR_TYPE_NORMAL) {
			string mapRes = string("key_stages/stage_") + intToString(m_iWarID) + string("_opened.png");
			m_pMapImg->loadTexture(mapRes.c_str(), UI_TEX_TYPE_PLIST);

			const WarMapCfg* pCfg = WAR_MAP_CFG->GetWarMapCfgByID(m_iWarID);
			m_pNameTxt->setText(pCfg->name());
			string lvStr = "Lv." + intToString(pCfg->level());
			m_pLvTxt->setText(lvStr);

		}else
		{
			const EliteWarMapCfg* pElitecfg = ELITE_WAR_MAP_CFG_MASTER->getEliteWarMapCfg(m_iWarID);
			string mapRes = string("key_stages/stage_") + intToString(30) + string("_opened.png");
			m_pMapImg->loadTexture(mapRes.c_str(), UI_TEX_TYPE_PLIST);

			char szTime[64] = { 0, };
			snprintf(szTime, 64, TextStr(TEXT_WAR_ELITE_CHAPTER), pElitecfg->warid());
			m_pNameTxt->setText(szTime);
			string lvStr = "Lv." + intToString(pElitecfg->level());
			m_pLvTxt->setText(lvStr);
		}
	}else
	{
		unsigned int pentryID = 0;
		switch (m_dropType)
		{
		case DROP_NIHILITY:
			pentryID = ENTRY_NOTHING_LAND;
			break;
		case DROP_LOTTERY:
			pentryID = ENTRY_LOTTERY;
			break;
		case DROP_BLACK_MARKET:
			pentryID = ENTRY_BLACK_MARKET;
			break;
		case DROP_WEAPON_BOOK:
			pentryID = ENTRY_WEAPON_BOOK;
			break;
		case DROP_ARENA_SHOOP:
			pentryID = ENTRY_ARENA;
			break;
		default:
			break;
		}

		string url = "";
		const EntryCfg *pEntryCfg = ENTRY_CFG_MASTER->getEntryCfg(pentryID);
		url = string("icon/entry/") + pEntryCfg->entryicon() + ".png";
		m_pMapImg->loadTexture(url.c_str());
		m_pMapImg->getParent()->setPositionX(m_pMapImg->getParent()->getPositionX()+70);
		m_pMapImg->getParent()->setScale(1.1);
		
		m_pNameTxt->setVisible(false);
		m_pLvTxt->setVisible(false);
	}
    
}

