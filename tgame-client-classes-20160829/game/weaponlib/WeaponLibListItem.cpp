#include "WeaponLibListItem.h"
#include "UserObj.h"
#include "firearms/FireArmsObj.h"
#include "CCColorUtil.h"
#include "Resources.h"
#include "defines.h"
#include "WeaponCfgMaster.h"
#include "firearms/StarProgressBar.h"
#include "WeaponLibControl.h"
#include "CCColorUtil.h"
#include "WeaponStarStepCfgMaster.h"
#include "WeaponRefitCfgMaster.h"
#include "basic/ToolUtils.h"
#include "game/bag/BagControl.h"

#include "game/guide/GuideControl.h"

CWeaponLibListItem::CWeaponLibListItem()
{
   
}

CWeaponLibListItem::~CWeaponLibListItem()
{

}

CWeaponLibListItem* CWeaponLibListItem::create(UILayout *pRefLayout) {
	CWeaponLibListItem *pRet = new CWeaponLibListItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CWeaponLibListItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/weapon_lib_list_item.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

bool CWeaponLibListItem::initWithLayout(UILayout *pRefLayout) {
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

void CWeaponLibListItem::initUI(){
    m_stOne.initWithLayout(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("item_0")));
    m_stTwo.initWithLayout(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("item_1")));

}

void CWeaponLibListItem::setWeaponInfo(const int iItemIdx, const unsigned int cfgID, const UINT64_t weaponGUID, const int iLv){
    if(0 == iItemIdx) {
        m_stOne.setWeaponInfo(cfgID, weaponGUID, iLv);
    } else if(1 == iItemIdx) {
        m_stTwo.setWeaponInfo(cfgID, weaponGUID, iLv);
    }
}

void CWeaponLibListItem::hideSecondItem() {
    m_stTwo.hide();
}

void CWeaponLibListItem::setShowNowOwnTip(const bool bIsShow){
    const int iTipTag = 83;
    UIImageView* pInUseTipImg = dynamic_cast<UIImageView*> (getChildByTag(iTipTag));
    if(bIsShow){
        //显示为当前装备
        if(NULL == pInUseTipImg) {
            pInUseTipImg = UIImageView::create();
            pInUseTipImg->setPosition(ccp(this->getSize().width/2, this->getSize().height + 5));
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

void CWeaponLibListItem::onItemTouched(const CCPoint& stGlobalTouchPt){
    UILayout* pLayout = m_stOne.getRefLayout();
    if(pLayout && pLayout->hitTest(stGlobalTouchPt)) {
        //CCLOG("click one");
		GUIDE_TOUCH(m_stOne.getRefLayout());
        m_stOne.onTouch();
        return;
    }
    pLayout = m_stTwo.getRefLayout();
    if(pLayout && pLayout->isVisible() && pLayout->hitTest(stGlobalTouchPt)) {
        //CCLOG("click two");
        m_stTwo.onTouch();
        return;
    }

}
CCNode* CWeaponLibListItem::getFirstItemNode() {
    return m_stOne.getRefLayout();
}

//////////
CWeaponLibListItemOne::CWeaponLibListItemOne() {
    m_pNameTxt = NULL;
    m_pRefLayout = NULL;
    m_pLvImg = NULL;
    m_pWeaponImg = NULL;
}


void CWeaponLibListItemOne::initWithLayout(UILayout *pRefLayout) {
    m_pRefLayout = pRefLayout;
    if(NULL == pRefLayout) {
        return;
    }
    UIImageView* pCellBg = dynamic_cast<UIImageView*>(pRefLayout->getChildByName("cell_bg"));
    m_pNameTxt = dynamic_cast<UILabel*>(pRefLayout->getChildByName("name_txt"));
    m_pLvTxt = dynamic_cast<UILabel*>(pRefLayout->getChildByName("lv_txt"));
    //m_pBgImg = dynamic_cast<ImageView*>(pRefLayout->getChildByName("bg_img"));
    //m_pIconBg = dynamic_cast<ImageView*>(pRefLayout->getChildByName("weapon_icon_bg"));

    m_pLvImg = UIImageView::create();
    m_pLvImg->setPosition(ccp(60, pCellBg->getPositionY() ));
    pRefLayout->addChild(m_pLvImg);

    m_pWeaponImg = UIImageView::create();
    m_pWeaponImg->setAnchorPoint(ccp(0.5f, 0.5f));
    m_pWeaponImg->setPosition(ccp(0, 0));
    pCellBg->addChild(m_pWeaponImg);

    //level star
    m_pStarBar = CStarProgressBar::create("share/little_star_dark.png", "share/little_star_light.png", "ui_v2/share.png");
    m_pStarBar->setAnchorPoint(ccp(0.5f, 0.0f));
    float xx = m_pWeaponImg->getPositionX(); 
    float yy = m_pWeaponImg->getPositionY() + 30;
    m_pStarBar->setPosition(ccp(xx, yy));
    m_pStarBar->setStarMargin(1);
    m_pStarBar->setStarNum(7);
    //m_pStarBar->resizeStars();
    pCellBg->addChild(m_pStarBar);
}

void CWeaponLibListItemOne::setWeaponInfo(const unsigned int cfgID, const UINT64_t weaponGUID, const int iLv){
    _weaponGUID = weaponGUID;
    _cfgID = cfgID;
    _lv = iLv;
    const WeaponCfg* stCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(cfgID);
    if(NULL == stCfg) {
        return;
    }

    m_pNameTxt->setText(stCfg->weaponname());
    m_pLvTxt->setText("Lv." + intToString(iLv) );
    setWeaponLv(stCfg->weaponlv());

    string iconStr = ICON_WEAPON_PATH + stCfg->weaponicon() + PNG_TYPE_NAME;
    m_pWeaponImg->loadTexture(iconStr.c_str());

    if(weaponGUID > 0) {
        //my weapon
        CCColorUtil::removeGray(m_pWeaponImg->getVirtualRenderer());
    } else {
        CCColorUtil::addGray(m_pWeaponImg->getVirtualRenderer());
    }

    if(!m_pRefLayout->isVisible()) {
        m_pRefLayout->setVisible(true);
    }
    m_pStarBar->setStarNum(stCfg->initstar());
    
    const int iTipTag = 87;
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/other.plist");
    UIImageView* pInUseTipImg = dynamic_cast<UIImageView*> (m_pRefLayout->getChildByTag(iTipTag));
    if(weaponGUID > 0){
        //枪械背包中的
		bool bIsShowImg = false;
		string urlStr = "";
		if(weaponGUID == P_ARM_OBJ->getWeaponBag().playerweapon().weaponguid()){   //主武器
			bIsShowImg = true;
			urlStr = "other/main_weapon_use_img.png";
		}else{
			const vmsg::CSWeapon& stWeaponInfo = P_ARM_OBJ->getOneWeaponByGUID(weaponGUID);
			if(stWeaponInfo.state() == vmsg::WEAPON_IN_BATTLE){  //武器处于上阵状态
				bIsShowImg = true;
				urlStr = "other/deputy_weapon_use_img.png";
			}else{
				bIsShowImg = false;
				urlStr = "";
			}
		}

		if(bIsShowImg){
			if(NULL == pInUseTipImg) {
				pInUseTipImg = UIImageView::create();
				pInUseTipImg->setPosition(ccp(270, 60));
			}

			if(urlStr != ""){
				pInUseTipImg->loadTexture(urlStr.c_str(), UI_TEX_TYPE_PLIST);
			}

			if(NULL == pInUseTipImg->getParent()) {
				m_pRefLayout->addChild(pInUseTipImg, 1, iTipTag);
			}
		}else{
			if(pInUseTipImg && pInUseTipImg->getParent()) {
				pInUseTipImg->removeFromParentAndCleanup(true);
			}
		}
    } else {
        if(pInUseTipImg && pInUseTipImg->getParent()) {
            pInUseTipImg->removeFromParentAndCleanup(true);
        }
    }

	bool bAdvanceOrRefit = false;

	if(weaponGUID > 0) {
		const vmsg::CSWeapon& stWeapon = P_ARM_OBJ->getOneWeaponByGUID(weaponGUID);
		const WeaponCfg* pWeaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeapon.cfgid());
		
		const WeaponStarStepCfg* pWeaponAdvanceCfg = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(pWeaponCfg->initstar(), pWeaponCfg->initstep());

		if(pWeaponAdvanceCfg != NULL
		&& (pWeaponAdvanceCfg->star() != pWeaponAdvanceCfg->nextstar() || pWeaponAdvanceCfg->step() != pWeaponAdvanceCfg->nextstep())) {
			if(stWeapon.level() >= pWeaponAdvanceCfg->needlevel()
			&& P_USER_OBJ->checkCoin(pWeaponAdvanceCfg->costcoin()) == 0
			&& P_ARM_OBJ->checkCanStarWeapon(stWeapon.cfgid(), stWeapon.weaponguid())) {
				bAdvanceOrRefit = true;
			}
		}

		const WeaponRefitCfg *pWeaponRefitCfg = WEAPONREFIT_CFG_MASTER->GetCfg(pWeaponCfg->weapontype(), stWeapon.quality() + 1);

		if(!bAdvanceOrRefit && pWeaponRefitCfg != NULL) {
			bool isMaterialEnough = true;

			for(int i = 0; i < pWeaponRefitCfg->elmtlist_size(); i++) {
				const RefitConsumeElement &stElmt = pWeaponRefitCfg->elmtlist(i);

				if(P_BAG_CTRL->getItemCnt(stElmt.elementid()) < stElmt.elementcnt()) {
					isMaterialEnough = false;
					break;
				}
			}

			if(isMaterialEnough) {
				bAdvanceOrRefit = true;
			}
		}
	}

	float offsetX = m_pRefLayout->getSize().width * 0.96f;
	float offsetY = m_pRefLayout->getSize().height * 0.91f;

	if(bAdvanceOrRefit) {
		CToolUtils::badgeWidgetWithPoint(m_pRefLayout, 1, offsetX, offsetY);
	} else {
		CToolUtils::badgeWidgetWithPoint(m_pRefLayout, 0, offsetX, offsetY);
	}
}

void CWeaponLibListItemOne::hide() {
    if(m_pRefLayout) {
        m_pRefLayout->setVisible(false);
    }
}

void CWeaponLibListItemOne::onTouch() {
    //P_WEAPON_CTRL->getWeaponLibPanel()->close();
    //P_WEAPON_CTRL->getWeaponInfoPanel()->open();

    P_WEAPON_CTRL->getWeaponInfoPanel()->openInSeq(P_WEAPON_CTRL->getWeaponLibPanel());
    P_WEAPON_CTRL->getWeaponInfoPanel()->updateWeaponInfo(_cfgID, _weaponGUID);
}


void CWeaponLibListItemOne::setWeaponLv(const unsigned int lev){
    string levUrl = "";
    switch (lev)
    {
    case 1:
		levUrl = "weapon_lib/f_level_img.png";
        break;
    case 2:
		levUrl = "weapon_lib/e_level_img.png";
        break;
    case 3:
        levUrl = "weapon_lib/d_level_img.png";
        break;
    case 4:
        levUrl = "weapon_lib/c_level_img.png";
        break;
    case 5:
        levUrl = "weapon_lib/b_level_img.png";
        break;
	case 6:
		levUrl = "weapon_lib/a_level_img.png";
		break;
	case 7:
		levUrl = "weapon_lib/s_level_img.png";
		break;
	case 8:
		levUrl = "weapon_lib/ss_level_img.png";
		break;
	case 9:
		levUrl = "weapon_lib/sss_level_img.png";
		break;
    default:
        levUrl = "weapon_lib/a_level_img.png";
        break;
    }

    m_pLvImg->loadTexture(levUrl.c_str(), UI_TEX_TYPE_PLIST);
}