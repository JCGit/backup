#include "EquipLibListItem.h"
#include "UserObj.h"
#include "CCColorUtil.h"
#include "Resources.h"
#include "defines.h"
#include "firearms/StarProgressBar.h"
#include "CCColorUtil.h"

#include "ItemUtils.h"
#include "ClientSvrMsg.pb.h"
#include "EquipmentCfgMaster.h"
#include "game/guide/GuideControl.h"
#include "EquipLibControl.h"
#include "EquipStarStepCfgMaster.h"
#include "EquipRefitCfgMaster.h"
#include "bag/BagControl.h"
#include "ToolUtils.h"

EquipLibListItem::EquipLibListItem()
{
   
}

EquipLibListItem::~EquipLibListItem()
{

}

EquipLibListItem* EquipLibListItem::create(UILayout *pRefLayout) {
	EquipLibListItem *pRet = new EquipLibListItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EquipLibListItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/equip_lib_list_item.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

bool EquipLibListItem::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void EquipLibListItem::initUI(){
    m_stOne.initWithLayout(dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("item_0")));
    m_stTwo.initWithLayout(dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("item_1")));

}

void EquipLibListItem::setEquipInfo(const int iItemIdx, const EquipLibOneInfo& stInfo){
    if(0 == iItemIdx) {
        m_stOne.setEquipInfo(stInfo);
    } else if(1 == iItemIdx) {
        m_stTwo.setEquipInfo(stInfo);
    }
}
const vmsg::CSEquipInfo& EquipLibListItem::getEquipInfo(){
	return m_stOne.getEquipLibOneInfo().equipInfo;
}

void EquipLibListItem::hideSecondItem() {
    m_stTwo.hide();
}

bool EquipLibListItem::onItemTouched(const CCPoint& stGlobalTouchPt){
    UIImageView* pLayout = m_stOne.getRefLayoutImg();
    if(pLayout && pLayout->hitTest(stGlobalTouchPt)) {
        //CCLOG("click one");
		GUIDE_TOUCH(m_stOne.getRefLayoutImg());
        m_stOne.onTouch();
        return true;
    }
    pLayout = m_stTwo.getRefLayoutImg();
    if(pLayout && pLayout->isVisible() && pLayout->hitTest(stGlobalTouchPt)) {
        //CCLOG("click two");
        m_stTwo.onTouch();
        return true;
    }
    return false;
}
CCNode* EquipLibListItem::getFirstItemNode() {
    return m_stOne.getRefLayoutImg();
}

//////////
EquipLibListItemOne::EquipLibListItemOne() {
    m_pNameTxt = NULL;
    m_pRefLayoutImg = NULL;
    m_pEquipImg = NULL;
}


void EquipLibListItemOne::initWithLayout(UIImageView *pRefLayout) {
    m_pRefLayoutImg = pRefLayout;
    if(NULL == pRefLayout) {
        return;
    }
    m_pCellBg = dynamic_cast<UIImageView*>(pRefLayout->getChildByName("cell_bg"));
    m_pNameTxt = dynamic_cast<UILabel*>(pRefLayout->getChildByName("name_txt"));
    m_pLvTxt = dynamic_cast<UILabel*>(pRefLayout->getChildByName("lv_txt"));
    
    m_pEquipImg = UIImageView::create();
    m_pEquipImg->setAnchorPoint(ccp(0.5f, 0.5f));
    m_pEquipImg->setPosition(m_pCellBg->getPosition());
    m_pRefLayoutImg->addChild(m_pEquipImg);
    m_pCellBg->setZOrder(1);

    //level star
    m_pStarBar = CStarProgressBar::create("share/little_star_dark.png", "share/little_star_light.png", "ui_v2/share.png", 6);
    m_pStarBar->setAnchorPoint(ccp(0.5f, 1.0f));
    float xx = m_pLvTxt->getPositionX() + 10; 
    float yy = m_pLvTxt->getPositionY() - 90;
    m_pStarBar->setPosition(ccp(xx, yy));
    m_pStarBar->setStarMargin(1);
    m_pStarBar->setStarNum(1, 6); //装备星级
    
    //m_pStarBar->resizeStars();
    m_pCellBg->addChild(m_pStarBar);
}


void EquipLibListItemOne::setEquipInfo(const EquipLibOneInfo& stInfo){
    m_stEquipInfo = stInfo;
    if(!m_pRefLayoutImg->isVisible()) {
        m_pRefLayoutImg->setVisible(true);
    }
    m_pEquipImg->loadTexture(ItemUtils::getItemIcon(vmsg::THING_TYPE_EQUIP, stInfo.equipInfo.equipid()).c_str());
    
    m_pLvTxt->setText("Lv." + intToString(stInfo.equipInfo.level()));
    const EquipmentCfg* pstCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stInfo.equipInfo.equipid());
    if(pstCfg) {
        m_pNameTxt->setText(pstCfg->equipname());
        m_pStarBar->setStarNum(pstCfg->initstar()); //最高6星
        m_pCellBg->loadTexture(ItemUtils::getColorBgImg(pstCfg->color()).c_str(),  UI_TEX_TYPE_PLIST );
    }

    const int iTipTag = 87;
    UIImageView* pInUseTipImg = dynamic_cast<UIImageView*> (m_pRefLayoutImg->getChildByTag(iTipTag));
    if(stInfo.bIsWear){
        //显示为当前装备
        if(NULL == pInUseTipImg) {
            pInUseTipImg = UIImageView::create();
            pInUseTipImg->setPosition(ccp(270, 130));
            pInUseTipImg->loadTexture("other/now_use_img.png", UI_TEX_TYPE_PLIST);

        }
        if(NULL == pInUseTipImg->getParent()) {
            m_pRefLayoutImg->addChild(pInUseTipImg, 1, iTipTag);
        }
    } else {
        if(pInUseTipImg && pInUseTipImg->getParent()) {
            pInUseTipImg->removeFromParentAndCleanup(true);
        }
    }

	bool bAdvanceOrRefit = false;

	vmsg::CSEquipInfo stEquipInfo = stInfo.equipInfo;

	if(stEquipInfo.equipguid() > 0) {
		const EquipmentCfg *pEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquipInfo.equipid());
		const EquipStarStepCfg *pEquipAdvanceCfg = EQUIPSTARSTEP_CFG_MASTER->GetCfg(pEquipCfg->initstar(), pEquipCfg->initstep());
		unsigned int uMaxLv = P_EQUIP_LIB_CTRL->getObj()->getEquipMaxLvUpLv(stEquipInfo.equipid());

		if(stEquipInfo.level() >= uMaxLv
			&& pEquipAdvanceCfg != NULL 
			&& (pEquipAdvanceCfg->star() != pEquipAdvanceCfg->nextstar() || pEquipAdvanceCfg->step() != pEquipAdvanceCfg->nextstep())) {
				bool isMaterialEnough = true;

				for(int i = 0; i < pEquipAdvanceCfg->needs_size(); i++) {
					const EquipNeeds &stEquipNeeds = pEquipAdvanceCfg->needs(i);

					if(P_BAG_CTRL->getItemCnt(stEquipNeeds.cfgid()) < stEquipNeeds.cfgcnt()) {
						isMaterialEnough = false;
						break;
					}
				}

				if(isMaterialEnough && pEquipAdvanceCfg->needequipcnt() > 0) {
					if(P_BAG_CTRL->getEquipCnt(stEquipInfo.equipid(), stEquipInfo.equipguid()) < (int)pEquipAdvanceCfg->needequipcnt()) {
						isMaterialEnough = false;
					}
				}

				if(isMaterialEnough && P_USER_OBJ->checkCoin(pEquipAdvanceCfg->costcoin()) == 0) {
					bAdvanceOrRefit = true;
				}	
		}

		const EquipRefitCfg *pEquipRefitCfg = EQUIPREFIT_CFG_MASTER->GetCfg(pEquipCfg->part(), stEquipInfo.quality() + 1);

		if(!bAdvanceOrRefit && pEquipRefitCfg != NULL) {
			bool isMaterialEnough = true;

			for(int i = 0; i < pEquipRefitCfg->elmtlist_size(); i++) {
				const EquipRefitConsumeElement &stElmt = pEquipRefitCfg->elmtlist(i);

				if(P_BAG_CTRL->getItemCnt(stElmt.elementid()) < stElmt.elementcnt()) {
					isMaterialEnough = false;
					break;
				}
			}

			if(isMaterialEnough && P_USER_OBJ->checkCoin(pEquipRefitCfg->costcoin()) == 0) {
				bAdvanceOrRefit = true;
			}
		}
	}
	
	float offsetX = m_pRefLayoutImg->getSize().width * 0.96f;
	float offsetY = m_pRefLayoutImg->getSize().height * 0.91f;

	if(bAdvanceOrRefit) {
		CToolUtils::badgeWidgetWithPoint(m_pRefLayoutImg, 1, offsetX, offsetY);
	} else {
		CToolUtils::badgeWidgetWithPoint(m_pRefLayoutImg, 0, offsetX, offsetY);
	}
}

void EquipLibListItemOne::hide() {
    if(m_pRefLayoutImg) {
        m_pRefLayoutImg->setVisible(false);
    }
}

void EquipLibListItemOne::onTouch() {
    CCLOG("[trace]EquipLibListItemOne::onTouch, sel equip %llu", m_stEquipInfo.equipInfo.equipguid() );
    P_EQUIP_LIB_CTRL->getObj()->onSelEquip(m_stEquipInfo.equipInfo.equipguid() );

}

