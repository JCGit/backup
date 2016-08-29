#include "EquipSelMatPanel.h"
#include "EquipSelMatItem.h"
#include "UserObj.h"
#include "UserInfo.h"
#include "bag/BagControl.h"
#include "EquipmentCfgMaster.h"
#include "EquipLibControl.h"
#include "EquipExpItemCfgMaster.h"
#include "EquipmentCfgMaster.h"



#include <algorithm>

EquipSelMatPanel::EquipSelMatPanel()
:m_pCloseBtn(NULL)
,m_pTableView(NULL)
,m_pEquipDemo(NULL)
{
}

EquipSelMatPanel::~EquipSelMatPanel()
{
    CC_SAFE_RELEASE_NULL(m_pEquipDemo);
}

bool EquipSelMatPanel::init() 
{
	bool bRet = false;

	do 
    {
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/equip_sel_mat_panel.ExportJson");
		addChild(m_pRootLayout);
		
		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
        m_pCloseBtn->addTouchEventListener(this, toucheventselector(EquipSelMatPanel::touchEventHandler));
        UIButton* pOKBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("ok_btn"));
        pOKBtn->addTouchEventListener(this, toucheventselector(EquipSelMatPanel::touchEventHandler));

		EquipSelMatItem* pItem = EquipSelMatItem::create();
		m_pEquipDemo = pItem->getRootLayout();
		m_pEquipDemo->retain();

        m_pTableView = CCTableView::create(this, CCSizeMake(690, 390));
        m_pTableView->setDirection(kCCScrollViewDirectionVertical);
        m_pTableView->setAnchorPoint(CCPointZero);
        m_pTableView->setPosition(ccp(20, 40));
        m_pTableView->setDelegate(this);
        m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);

        UIImageView* pImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_inner"));
        pImg->addNode(m_pTableView);

		bRet = true;
	} 
    while(0);

	return bRet;
}

CCSize EquipSelMatPanel::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width, 130);
}

unsigned int EquipSelMatPanel::numberOfCellsInTableView(CCTableView *table){
	return m_stMatInfo.size();
}

CCTableViewCell* EquipSelMatPanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
	CCTableViewCell* cell = table->dequeueCell();
	EquipSelMatItem *pItem = NULL;

	if(!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pItem = EquipSelMatItem::create(m_pEquipDemo);
		cell->addChild(pItem, 0, EQUIP_SEL_ITEM_TAG);
	}else{
		pItem = dynamic_cast<EquipSelMatItem*>(cell->getChildByTag(EQUIP_SEL_ITEM_TAG));
	}

    if(idx < m_stMatInfo.size()) {
        pItem->setInfo(m_stMatInfo[idx], idx);
        if(m_stNowSelMatIdx.count(idx) > 0) {
            pItem->setSelect(true);
        } else {
            pItem->setSelect(false);
        }
    }

	return cell;
}

void EquipSelMatPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
	EquipSelMatItem *pItem = dynamic_cast<EquipSelMatItem*>(cell->getChildByTag(EQUIP_SEL_ITEM_TAG));
    if(NULL == pItem) {
        return;
    }
    if(pItem->isSelect()) {
        pItem->setSelect(false);
        m_stNowSelMatIdx.erase(pItem->getListIdx());
    } else {
        if(m_stNowSelMatIdx.size() < EQUIP_SEL_MAT_MAX_NUM) {
            pItem->setSelect(true);
            m_stNowSelMatIdx.insert(pItem->getListIdx());
        }
    }

}

void EquipSelMatPanel::open()
{
    BasePanel::open();
    updateUI();
}


void EquipSelMatPanel::close()
{
    BasePanel::close();

    //关闭时更新一下
    P_EQUIP_LIB_CTRL->getObj()->clearLvUpMat();

    int iTotalAddExp = 0;
    bool bIsAddSuccess = false;
    for(auto iter = m_stNowSelMatIdx.begin(); iter != m_stNowSelMatIdx.end(); ++iter) {
        const int iListIdx = *iter;
        bIsAddSuccess = P_EQUIP_LIB_CTRL->getObj()->addOneLvUpMat( m_stMatInfo[iListIdx] );
        if(bIsAddSuccess) {
            
        }
    }
    
    m_stMatInfo.clear(); //reset
    m_stNowSelMatIdx.clear(); 
    P_EQUIP_LIB_CTRL->getEquipLibPanel()->updateSelMat();
}

void EquipSelMatPanel::updateUI(){

    /*
    //show role equip
    const std::vector<vmsg::CSEquipInfo>& stRoleEquipLst = P_USER_OBJ->getUserInfo()->stEquipLst;
    for (int i = 0; i < (int)stRoleEquipLst.size(); ++i) {
        addOneEquip(stRoleEquipLst[i], true);
    }

    //备用防具
    const vmsg::CSEquipInfo& stSpareEquip = P_USER_OBJ->getUserInfo()->spareEquip;
    if(0 != stSpareEquip.equipguid()) {
		addOneEquip(stSpareEquip, false);
    }
    */

    m_stMatInfo.clear();
    m_stNowSelMatIdx.clear();
    findMat();

    //把上次选择的材料过滤出来
    m_stNowSelMatIdx.clear();
    map<UINT64_t, vmsg::CSEquipLvUpConsume> stMat = P_EQUIP_LIB_CTRL->getObj()->getLvUpMat();
    UINT64_t uGuid;
    for(unsigned int i = 0; i < m_stMatInfo.size(); ++i) {
        const EquipSelMatOneInfo& stOneInfo = m_stMatInfo[i];

        if(stOneInfo.itemCfgID > 0) {
            uGuid = stOneInfo.itemCfgID;
        } else {
            uGuid = stOneInfo.stEquip.equipInfo.equipguid();
        }
        if(stMat.count(uGuid) > 0 ) {
            m_stNowSelMatIdx.insert(i); //add one
            const int remainCnt = stMat[uGuid].cnt() - 1;
            if(remainCnt <= 0) {
                stMat.erase(uGuid);
            } else {
                stMat[uGuid].set_cnt(remainCnt );
            }
        }
    }

	m_pTableView->reloadData();
}

const vector<EquipSelMatOneInfo>& EquipSelMatPanel::findMat() {
    //材料选择仅仅过滤背包中的装备和材料,把当前选择的装备给过滤掉,只能同部位
    m_stMatInfo.clear();
    const UINT64_t uNowSelEquipGuid = P_EQUIP_LIB_CTRL->getObj()->getSelEquipGuid();
    const EquipLibOneInfo stNowSelEquipInfo = P_EQUIP_LIB_CTRL->getObj()->getEquipInfo(uNowSelEquipGuid);
    const EquipmentCfg* pNowSelEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stNowSelEquipInfo.equipInfo.equipid());
    if(NULL == pNowSelEquipCfg) {

        return m_stMatInfo ;
    }
    const int iEquipPart = pNowSelEquipCfg->part();
    const vmsg::CSBagInfo& stBag = P_BAG_CTRL->getBagInfo();
    for (int i = 0; i < stBag.celllst_size(); ++i) {
        if (stBag.celllst(i).has_equipinfo()) {
            
            if(uNowSelEquipGuid != stBag.celllst(i).equipinfo().equipguid()  ){
                const EquipmentCfg* pOneEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stBag.celllst(i).equipinfo().equipid());
                if(pOneEquipCfg && pOneEquipCfg->part() == iEquipPart) {
                    addOneEquip(stBag.celllst(i).equipinfo(), false);
                }
               
            }
        } else if(stBag.celllst(i).has_iteminfo() ) {
            //过滤出升级材料
            const EquipExpItemCfg* pExpCfg = EQUIPEXPITEM_CFG_MASTER->GetEquipExpItemCfg( stBag.celllst(i).iteminfo().itemcfgid() );
            if(pExpCfg) {
                addSomeItem(stBag.celllst(i).iteminfo().itemcfgid(), stBag.celllst(i).iteminfo().itemcnt(), stBag.celllst(i).index() );
            }
        }

    }

    //sort,当前装备排列前面，等级越高越靠前
    sort(m_stMatInfo.begin(), m_stMatInfo.end(), compareMat );

    return m_stMatInfo;
}

void EquipSelMatPanel::addOneEquip(const vmsg::CSEquipInfo& stInfo, const int bagIdx) {
    EquipSelMatOneInfo stOne;
    stOne.stEquip.equipInfo = stInfo;
    stOne.stEquip.bIsWear = false;
    stOne.stEquip.iBagIdx = bagIdx;
    stOne.itemCfgID = -1;
    m_stMatInfo.push_back( stOne );
}
void EquipSelMatPanel::addSomeItem(const int itemCfgID, const int cnt, const int bagIdx) {
    if(cnt <= 0) {
        return;
    }
    EquipSelMatOneInfo stOne;
    stOne.stEquip.equipInfo.set_equipguid(0);
    stOne.stEquip.bIsWear = false;
    stOne.stEquip.iBagIdx = bagIdx; //暂时这样处理吧
    stOne.itemCfgID = itemCfgID;
    m_stMatInfo.insert(m_stMatInfo.end(), cnt, stOne);
}


void EquipSelMatPanel::touchEventHandler(CCObject *pSender, TouchEventType type) 
{
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();

    switch(type) {
    case TOUCH_EVENT_ENDED:
        if(std::strcmp(pszWidgetName, "close_btn") == 0) {
            close();
        } else if(std::strcmp(pszWidgetName, "ok_btn") == 0) {
            close(); //ok_btn
        }
        break;
    default:
        break;
    }
}


bool EquipSelMatPanel::compareMat(const EquipSelMatOneInfo& stInfoA, const EquipSelMatOneInfo& stInfoB) {
    //返回A是否靠前排列
    //sort,当前装备排列后面，等级越高越靠后！
    //选择材料面板。排列为材料靠前，其次为低等级装备
    if(stInfoA.itemCfgID > 0) {
        if(stInfoB.itemCfgID > 0) {
            return false;
        } else {
            return true;
        }
        return false;
    } else if(stInfoB.itemCfgID > 0) {
        return false;
    } else {
        //低等级装备在前,低品级在前面吧
        const int aColor = EQUIPMENT_CFG_MASTER->GetEquipColor(stInfoA.stEquip.equipInfo.equipid());
        const int bColor = EQUIPMENT_CFG_MASTER->GetEquipColor(stInfoB.stEquip.equipInfo.equipid());
        if(aColor == bColor) {
            return (stInfoA.stEquip.equipInfo.level() < stInfoB.stEquip.equipInfo.level() );
        } else {
            return aColor < bColor;
        }
        
    }

    return true;
}



