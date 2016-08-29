#include "FragListItem.h"
#include "defines.h"
#include "IconCell.h"
#include "FragControl.h"
#include "DebrisCfgMaster.h"
#include "CCColorUtil.h"

FragListItem::FragListItem()
{
    m_iShowCellNum = 0;
    m_bIsMyFrag = false;
}

FragListItem::~FragListItem()
{
}

FragListItem* FragListItem::create() {
	FragListItem *pRet = new FragListItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool FragListItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

        initLayoutFromJsonFile("ui_v2/frag_weapon_list_item.ExportJson");
        addChild(m_pRootLayout);

        UIImageView* pCell;
        IconCell* pIconCell;
        for(int i = 0; i < EQUIP_CELL_TYPE_NUM; ++i) {
            pCell = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName( string("cell_" + intToString(i)).c_str() ) ) ;
            m_stEquipCell[i] = pCell;
            pIconCell = IconCell::create();
            pIconCell->setAnchorPoint(ccp(0.0f, 0.0f));
            pIconCell->getIconImg()->setAnchorPoint(ccp(0.0f, 0.0f));
            pIconCell->setPosition(ccp(10.0f, 10.0f) );
            pIconCell->showNumTxt();

            pCell->addChild(pIconCell, 0, ICON_CELL_TAG);
            m_stIconCellLst[i] = pIconCell;
        }
        m_iCellOriX = m_stEquipCell[1]->getPositionX(); //记下第2个cell的x

        m_iShowCellNum = EQUIP_CELL_TYPE_NUM; //default is 4 cell

		bRet = true;
	} while(0);

	return bRet;
}


void FragListItem::setCellTypeNum(const int iNum) {
    if(m_iShowCellNum == iNum) {
        return;
    }
    if(WEAPON_CELL_TYPE_NUM == iNum) {
        m_iShowCellNum = iNum;

        UIImageView* pCell;
        for(int i = 0; i < EQUIP_CELL_TYPE_NUM; ++i) {
            pCell = m_stEquipCell[i];
            if( i < WEAPON_CELL_TYPE_NUM) {
                //weapon cell
                pCell->setSize(CCSize(180, 100) );
                pCell->loadTexture("share/weapon_frame.png", UI_TEX_TYPE_PLIST);
            }
            pCell->setVisible(false); //默认全部不可见，当设置Info时才可见
        }
        m_stEquipCell[1]->setPositionX(m_iCellOriX + 100);

    } else if(EQUIP_CELL_TYPE_NUM == iNum) {
        m_iShowCellNum = iNum;

        UIImageView* pCell;
        for(int i = 0; i < EQUIP_CELL_TYPE_NUM; ++i) {
            pCell = m_stEquipCell[i];
            if( (i < WEAPON_CELL_TYPE_NUM)) {
                //weapon cell
                pCell->setSize(CCSize(100, 100) );
                pCell->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
            }
            pCell->setVisible(false);
        }
        m_stEquipCell[1]->setPositionX(m_iCellOriX);
    } else {
        CCLOG("[warn]FragListItem::setCellNum, invalid cell num %d", iNum);
    }
}



void FragListItem::onTouched(const CCPoint& stGlobalPt) {
    UIImageView* pCell;
    int iFindIdx = -1;
    for(int i = 0; i < EQUIP_CELL_TYPE_NUM; ++i) {
        pCell = m_stEquipCell[i];
        if(!pCell->isVisible()) {
            continue;
        }
        if(pCell->hitTest(stGlobalPt) ) {
            //find
            iFindIdx = i;
            break;
        }
    }
    if(iFindIdx < 0) {
        return;
    }

    //CCLOG("[trace]FragListItem::onTouched,touch cell %d, tag %d", iFindIdx, pCell->getTag() );
    if(m_bIsMyFrag) {
        //点了配置的
        P_FRAG_CTRL->getFragPanel()->onSelBagIdx(pCell->getTag());
    } else {
        P_FRAG_CTRL->getFragPanel()->onSelCfgID(pCell->getTag());
    }
    
}

void FragListItem::setInfoByIdx(const int iLogicIdx, const int iBagIdx) {
    m_bIsMyFrag = true;
    UIImageView* pCellBg = NULL;
    if(EQUIP_CELL_TYPE_NUM == m_iShowCellNum) {
        if(iLogicIdx >= 0 && iLogicIdx < EQUIP_CELL_TYPE_NUM) {
            pCellBg = m_stEquipCell[iLogicIdx];
        }
    } else if(WEAPON_CELL_TYPE_NUM == m_iShowCellNum) {
        if(iLogicIdx >= 0 && iLogicIdx < WEAPON_CELL_TYPE_NUM) {
            pCellBg = m_stEquipCell[ (iLogicIdx) ];
        }
    }
    if(NULL == pCellBg) {
        return;
    }
    IconCell* pIconCell = (IconCell*) (pCellBg->getChildByTag(ICON_CELL_TAG));
    if(NULL == pIconCell) {
        return;
    }
    pCellBg->setVisible(true);
    const CSDebrisInfo* pstOneInfo = P_FRAG_CTRL->getOneFragByIdx(iBagIdx);
    if(NULL == pstOneInfo) {
        return;
    }
    const DebrisCfg* pstCfg = DEBRIS_CFG_MASTER->GetDebrisCfgByID(pstOneInfo->debriscfgid());
    if(NULL == pstCfg) {
        return;
    }

    //const string strIcon = DEBRIS_CFG_MASTER->getIconPath(pstOneInfo->debriscfgid());
    //pIconCell->getIconImg()->loadTexture(strIcon.c_str());
    
    pIconCell->setInfo(vmsg::THING_TYPE_DEBRIS//pstCfg->composetype()
		, 0, pstCfg->debrisid()//pstCfg->composecfgid() 
		);

    CCColorUtil::removeGray(pIconCell->getIconImg()->getVirtualRenderer());

    if(vmsg::THING_TYPE_WEAPON == pstCfg->composetype() ) {
        pIconCell->getNumTxt()->setPosition(ccp(160, 12) );
    } else {
        pIconCell->getNumTxt()->setPosition(ccp(80, 12) );
    }
    
    /*
    char txt[30] = {0};
    snprintf(txt, 30, "%d/%d", pstOneInfo->debriscnt(), pstCfg->composeneedcnt());
    pIconCell->getNumTxt()->setText(txt);
    
    bool bIsEnough = (pstOneInfo->debriscnt() >= pstCfg->composeneedcnt());
    if(bIsEnough) {
        CCColorUtil::setLabelColor(pIconCell->getNumTxt(), ccGREEN);
    } else {
        CCColorUtil::setLabelColor(pIconCell->getNumTxt(), ccRED);
    }
    */
    pIconCell->getNumTxt()->setText(intToString(pstOneInfo->debriscnt()));

    pCellBg->setTag(iBagIdx); //以tag来记录吧
}


void FragListItem::resetCellByIdx(const int iLogicIdx) {
    UIImageView* pCellBg = NULL;
    if(iLogicIdx >= 0 && iLogicIdx < EQUIP_CELL_TYPE_NUM) {
        pCellBg = m_stEquipCell[iLogicIdx];
        pCellBg->setVisible(false);
    }
}


void FragListItem::setNotOwnTip(bool bIsShow) {
    const int iTipTag = 83;
    UIImageView* pInUseTipImg = dynamic_cast<UIImageView*> (getChildByTag(iTipTag));
    if(bIsShow){
        //显示为当前装备
        if(NULL == pInUseTipImg) {
            pInUseTipImg = UIImageView::create();
            pInUseTipImg->setPosition(ccp(this->getSize().width/2, getSize().height + 5));
            pInUseTipImg->loadTexture("other/not_own_tip.png", UI_TEX_TYPE_PLIST);

        }
        if(NULL == pInUseTipImg->getParent()) {
            addChild(pInUseTipImg, 1, iTipTag);
        }
    } else {
        if(pInUseTipImg && pInUseTipImg->getParent()) {
            pInUseTipImg->removeFromParentAndCleanup(true);
        }
    }
}


void FragListItem::setInfoByCfgID(const int iLogicIdx, const int iCfgID) {
    m_bIsMyFrag = false;
    UIImageView* pCellBg = NULL;
    if(EQUIP_CELL_TYPE_NUM == m_iShowCellNum) {
        if(iLogicIdx >= 0 && iLogicIdx < EQUIP_CELL_TYPE_NUM) {
            pCellBg = m_stEquipCell[iLogicIdx];
        }
    } else if(WEAPON_CELL_TYPE_NUM == m_iShowCellNum) {
        if(iLogicIdx >= 0 && iLogicIdx < WEAPON_CELL_TYPE_NUM) {
            pCellBg = m_stEquipCell[ (iLogicIdx) ];
        }
    }
    if(NULL == pCellBg) {
        return;
    }
    IconCell* pIconCell = (IconCell*) (pCellBg->getChildByTag(ICON_CELL_TAG));
    if(NULL == pIconCell) {
        return;
    }
    pCellBg->setVisible(true);

    const DebrisCfg* pstCfg = DEBRIS_CFG_MASTER->GetDebrisCfgByID(iCfgID);
    if(NULL == pstCfg) {
        return;
    }

    
    pIconCell->setInfo(vmsg::THING_TYPE_DEBRIS//pstCfg->composetype()
		, 0,pstCfg->debrisid()
		);
    CCColorUtil::addGray(pIconCell->getIconImg()->getVirtualRenderer()); //灰调

    if(vmsg::THING_TYPE_WEAPON == pstCfg->composetype() ) {
        pIconCell->getNumTxt()->setPosition(ccp(160, 12) );
    } else {
        pIconCell->getNumTxt()->setPosition(ccp(80, 12) );
    }
    
    pIconCell->getNumTxt()->setText("");

    pCellBg->setTag(iCfgID); //tag 是cfg ID
}


