#include "IconCell.h"
#include "ItemCfgMaster.h"
#include "EquipmentCfgMaster.h"
#include "ClientSvrMsg.pb.h"
#include "defines.h"
#include "ItemUtils.h"
#include "CCColorUtil.h"
#include "Resources.h"
#include "DebrisCfgMaster.h"
#include "firearms/StarProgressBar.h"
#include "CfgMaster/WeaponCfgMaster.h"

IconCell::IconCell() 
:m_pIconImg(NULL)
,m_pNumTxt(NULL)
,m_stColor(ccWHITE)
,m_bIsShowNumTxt(false) 
,m_iType(0)
,m_iNum(0)
,m_iCfgID(0)
,m_pStarBar(NULL)
{

}
bool IconCell::init() {
    bool bRet = false;
    do {
        CC_BREAK_IF(!BaseLayout::init());
        this->ignoreContentAdaptWithSize(false);

        m_pIconImg = UIImageView::create();
        m_pIconImg->setTouchEnabled(false);
        addChild(m_pIconImg);

		m_pStarBar = CStarProgressBar::create("share/little_star_dark.png", "share/little_star_light.png", "ui_v2/share.png");
		m_pStarBar->setAnchorPoint(ccp(0.5f, 0.0f));
		float xx = m_pIconImg->getPositionX(); 
		float yy = m_pIconImg->getPositionY() + 30;
		m_pStarBar->setPosition(ccp(xx, yy));
		m_pStarBar->setStarMargin(1);
		m_pStarBar->setStarNum(7);
		addChild(m_pStarBar);
		m_pStarBar->setVisible(false);
		m_pStarBar->setScale(0.8);

        bRet = true;
    } while(false);

    return bRet;
}

void IconCell::setOpacity(GLubyte opacity){
    m_pIconImg->setOpacity(opacity);

    if(m_pNumTxt){
        m_pNumTxt->setOpacity(opacity);
    }
}

void IconCell::setInfo(const int iType, const int iNum, const int iCfgID, ccColor3B color) {
    m_iType = iType;
    m_iNum = iNum;
    m_iCfgID = iCfgID;
    m_pIconImg->setVisible(true);
    const std::string strPath = ItemUtils::getItemIcon(iType, iCfgID);
    m_pIconImg->loadTexture(strPath.c_str());
	
	if (iType == vmsg::THING_TYPE_WEAPON)
	{
		const WeaponCfg* stCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(iCfgID);
		m_pStarBar->setVisible(true);
		m_pStarBar->setStarNum(stCfg->initstar());
	}else
	{
		m_pStarBar->setVisible(false);
	}
	
    setNum(iNum, color);
}


void IconCell::setNum(const int iNum, ccColor3B color){
    if(NULL == m_pNumTxt) {
        return;
    }
    m_pNumTxt->setColor(color);
    if(iNum <= 0){
        m_pNumTxt->setText("");
        return;
    }
    m_pNumTxt->setText(ItemUtils::numbFormateW(iNum));
    if(vmsg::THING_TYPE_WEAPON == m_iType) {
        m_pNumTxt->setPosition(ccp(75, -30));
    } else if(vmsg::THING_TYPE_DEBRIS == m_iType) {
        if(DEBRIS_CFG_MASTER->GetDebrisCfgByID(m_iCfgID)) {
            if(DEBRIS_CFG_MASTER->GetDebrisCfgByID(m_iCfgID)->composetype() == vmsg::THING_TYPE_WEAPON ) {
                m_pNumTxt->setPosition(ccp(75, -30));
            } else {
                m_pNumTxt->setPosition(ccp(40, -30));
            }
        } else {
            m_pNumTxt->setPosition(ccp(40, -30));
        }
    } else {
        m_pNumTxt->setPosition(ccp(40, -30));
    }
}

void IconCell::clearCell() {
    //Çå¿ÕÏÔÊ¾
    if(m_pIconImg) {
        m_pIconImg->setVisible(false);
        //m_pIconImg->loadTexture(DEFAULT_ITEM.c_str());
    }
    if(m_pNumTxt) {
        m_pNumTxt->setText("");
    }
    m_iType = 0;
    m_iNum = 0;
    m_iCfgID = 0;
}


void IconCell::showNumTxt() {
    m_bIsShowNumTxt = true;
    m_pNumTxt = UILabel::create();
    m_pNumTxt->setColor(m_stColor);
    m_pNumTxt->setFontName("fonts/04b_03b.ttf");
    m_pNumTxt->setFontSize(24);
    m_pNumTxt->setTextHorizontalAlignment(kCCTextAlignmentRight); //ÓÒ¶ÔÆë
    m_pNumTxt->setText("");
    m_pNumTxt->setAnchorPoint(ccp(1, 0.5));
    m_pNumTxt->setPosition(ccp(40, -30));
    CCColorUtil::addStroke(m_pNumTxt);
    addChild(m_pNumTxt);
}


void IconCell::showNumTxt(bool bIsShow) {
    m_bIsShowNumTxt = bIsShow;

    if(m_pNumTxt == NULL) {
        if(m_bIsShowNumTxt) {
            showNumTxt();
        }
    } else {
        m_pNumTxt->setVisible(m_bIsShowNumTxt);
    }
}


