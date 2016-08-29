#include "WeaponLotResultPanel.h"
#include "IconCell.h"
#include "CSEquipItemMsg.pb.h"
#include "bag/BagControl.h"
#include "bag/ItemInfoPanel.h"
#include "WeaponLotResultCell.h"
#include "HBox.h"
#include "PopTipLayer.h"
#include "WeaponLotteryCfgMaster.h"
#include "ToolUtils.h"
#include "ItemUtils.h"
#include "Global.h"
#include "WeaponCfgMaster.h"
#include "ItemCfgMaster.h"
#include "DebrisCfgMaster.h"
#include "game/control/AudioManager.h"

#include "LotteryControl.h"

using namespace vmsg;

bool cmpElmtFunc(const CSAwardElement& elmtA, const CSAwardElement& elmtB) {
    if(elmtA.elementtype() == elmtB.elementtype()) {
        if(THING_TYPE_WEAPON == elmtA.elementtype()) {
            return (WEAPON_CFG_MASTER->getWeaponLv(elmtA.elementid()) > WEAPON_CFG_MASTER->getWeaponLv(elmtB.elementid()));
        } else if(THING_TYPE_EQUIP == elmtA.elementtype()) {
            return (ItemUtils::getItemColor(THING_TYPE_EQUIP, elmtA.elementid()) ) > (ItemUtils::getItemColor(THING_TYPE_EQUIP, elmtB.elementid()) );
        } else if(THING_TYPE_ITEM == elmtA.elementtype()) {
            return (ITEM_CFG_MASTER->GetItemColor(elmtA.elementid()) > ITEM_CFG_MASTER->GetItemColor(elmtB.elementid()));
        } else {
            return 0;
        }
    } else {
        if(THING_TYPE_WEAPON == elmtA.elementtype()) {
            return true;
        } else if(THING_TYPE_WEAPON == elmtB.elementtype()) {
            return false;
        } else {
            return elmtA.elementtype() > elmtB.elementtype();
        }
        
    }
}

WeaponLotResultPanel::WeaponLotResultPanel()
{
    m_pSingleCell = NULL;
    m_pBaoxiangAni = NULL;
    m_pCurCellAni = NULL;
    m_pFlyBallAni = NULL;
	m_uLotteryType = 0;
}

WeaponLotResultPanel::~WeaponLotResultPanel()
{
	CToolUtils::removePlistTexFile("ui_bg/bg_lottery.plist");
}

bool WeaponLotResultPanel::init() 
{
	bool bRet = false;

	do 
    {
		CC_BREAK_IF(!BasePanel::init());
        
        CCDirector *pDirector = CCDirector::sharedDirector();
        CCSize screenSize = pDirector->getVisibleSize();
        CCPoint screenOrigin = pDirector->getVisibleOrigin();

		initLayoutFromJsonFile("ui_v2/weapon_lottery_result_ui.ExportJson");
		addChild(m_pRootLayout);
		setSize(m_pRootLayout->getSize());

		CToolUtils::loadSpriteFrameRGBA4444("ui_bg/bg_lottery.plist");
        m_pBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_img"));
		m_pBg->loadTexture("bg_lottery.png", UI_TEX_TYPE_PLIST);
		float scaleX = screenSize.width / m_pBg->getSize().width;
		float scaleY = screenSize.height / m_pBg->getSize().height;
		float scale = scaleX > scaleY ? scaleX : scaleY;
		m_pBg->setScale(scale);

        m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
        m_pLotAgainBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("again_btn"));
		
        m_pCloseBtn->addTouchEventListener(this, toucheventselector(WeaponLotResultPanel::touchEventHandler));
        m_pLotAgainBtn->addTouchEventListener(this, toucheventselector(WeaponLotResultPanel::touchEventHandler));

        m_pCellHBox = HBox::create();
        m_pCellHBox->initUI(CCSize(1150, 200), 5, 60); 
		float posX = (m_pRootLayout->getSize().width - 1150) / 2;
		float posY = 0;
        m_pCellHBox->setPosition(ccp(posX, posY));
        m_pCellHBox->setTouchEnabled(false);
        m_pRootLayout->addChild(m_pCellHBox);

		bRet = true;
	} 
    while(0);

	return bRet;
}

void WeaponLotResultPanel::open()
{
    BasePanel::open();
    
    resetUI(); //reset一下，重复Lot时会用

    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00018/ut00018.ExportJson");

    //new eff
    CToolUtils::loadArmatureRGBA4444("effect/ut00025/ut00025.ExportJson");
    CToolUtils::loadArmatureRGBA4444("effect/ut00067/ut00067.ExportJson");
    CToolUtils::loadArmatureRGBA4444("effect/ut00068/ut00068.ExportJson");
}


void WeaponLotResultPanel::close()
{
    BasePanel::close();
    
    resetUI();

    CToolUtils::removeArmatureFile("effect/ut00018/ut00018.ExportJson");
    CToolUtils::removeArmatureFile("effect/ut00025/ut00025.ExportJson");
    CToolUtils::removeArmatureFile("effect/ut00067/ut00067.ExportJson");
    CToolUtils::removeArmatureFile("effect/ut00068/ut00068.ExportJson");
}

void WeaponLotResultPanel::resetUI() {
	P_LOTTERY_CTRL->sendQueryLotteryRqst();
    m_stElmtDataLst.clear();

    m_pCellHBox->clearBox();
    if(m_pSingleCell) {
        m_pSingleCell->removeFromParentAndCleanup(true);
    }
    m_pSingleCell = NULL;

    if(m_pBaoxiangAni) {
        m_pBaoxiangAni->removeFromParentAndCleanup(true);
    }
    m_pBaoxiangAni = NULL;

    if(m_pFlyBallAni) {
        m_pFlyBallAni->removeFromParentAndCleanup(true);
    }
    m_pFlyBallAni = NULL;

    P_GLOBAL->clearAdvItemEff();
    POP_TIP_LAYER_MASTER->ClearAllTips();

	if(m_pBg != NULL) {
		//m_pBg->setVisible(false);
	}

    //m_pCloseBtn->setEnabled(false);
    //m_pLotAgainBtn->setEnabled(false);
}

void WeaponLotResultPanel::touchEventHandler(CCObject *pSender, TouchEventType type) 
{
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
                close();
            } else if(std::strcmp(pszWidgetName, m_pLotAgainBtn->getName()) == 0 ) {
                P_LOTTERY_CTRL->lottery();
            }
		default:
			break;
	}
}



void WeaponLotResultPanel::showSingleResult(const CSWeaponLotRsp& stRsp){
    if(1 == stRsp.elmtlst_size()) {
        const CSAwardElement& elmt = stRsp.elmtlst(0);
        CCLOG("[trace]WeaponLotResultPanel::showSingleResult,type=%d,cnt=%d,id=%d", 
            elmt.elementtype(), elmt.elementcnt(), elmt.elementid());

        POP_TIP_LAYER_MASTER->ShowAwardTip(elmt.elementtype(), elmt.elementid(), elmt.elementcnt());

		if(elmt.elementtype() == vmsg::THING_TYPE_WEAPON) {
			string soundURL = "ut00025.ogg";
			AudioManager::playEffect(soundURL.c_str());
		}
        return;
    }
}

void WeaponLotResultPanel::showResult(const CSWeaponLotRsp& stRsp){
    if(!isOpen()) {
        return;
    }

	if(stRsp.has_type()) {
		m_uLotteryType = stRsp.type();
	}
    /*
    //固定奖励暂时不展示？TODO:
    const int iPreLotType = P_WEAPON_LOT_CTRL->getProxy()->m_iLotType;
    if(CS_LOTTERY_TYPE_NORMAL == iPreLotType || CS_LOTTERY_TYPE_ADVANCE == iPreLotType) {
        
        const WeaponLotteryCfg* pstNormalLotCfg = WEAPON_LOT_CFG_MASTER->GetWeaponLotteryCfg(iPreLotType);
        if(pstNormalLotCfg) {
            POP_TIP_LAYER_MASTER->ShowAwardTip(pstNormalLotCfg->fixedelmt().type(), 
                pstNormalLotCfg->fixedelmt().cfgid(), 
                pstNormalLotCfg->fixedelmt().cnt());
        }
    }
    */

    /* //2014-8-9,一个物品不展示这个面板了
    //奖励只有一个时单独展示
    if(1 == stRsp.elmtlst_size()) {
        //show single one elmt
        const CCSize globalSize = CCDirector::sharedDirector()->getVisibleSize(); 

        m_pSingleCell = WeaponLotResultCell::create();
        CCPoint pt;
        pt.x = (globalSize.width - m_pSingleCell->getSize().width) / 2;
        pt.y = (globalSize.height - m_pSingleCell->getSize().height) / 2;
        m_pSingleCell->setPosition(pt);

        //tip txt pos
        //pt.x = globalSize.width / 2;
        //pt.y += 300;
        //m_pTipTxt->setPosition(pt);

        const CSAwardElement& elmt = stRsp.elmtlst(0);
        CCLOG("[trace]WeaponLotResultPanel::showResult,type=%d,cnt=%d,id=%d", elmt.elementtype(), elmt.elementcnt(), elmt.elementid());
        m_pSingleCell->setInfo(elmt.elementtype(), elmt.elementcnt(), elmt.elementid());

        addChild(m_pSingleCell);
        m_pCellHBox->setVisible(false);

        //POP_TIP_LAYER_MASTER->ShowAwardTip(elmt.elementtype(), elmt.elementid(), elmt.elementcnt());
        return;
    }
    */

    //m_pCellHBox->setVisible(true);
    //m_pTipTxt->setPosition(m_stTipTxtInitPt);
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCSize screenSize = pDirector->getVisibleSize();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();
	CCPoint centerPos = ccp(screenOrigin.x + screenSize.width / 2, screenOrigin.y + screenSize.height / 2);

    //排序，先展示枪械 ,优先高等级的
    m_stElmtDataLst.clear();
    m_pCellHBox->clearBox();
    for(int i = 0; i < stRsp.elmtlst_size(); ++i) {
        m_stElmtDataLst.push_back(stRsp.elmtlst(i));
    }
    sort(m_stElmtDataLst.begin(), m_stElmtDataLst.end(), cmpElmtFunc);
    
    //宝箱特效
    CCPoint stPt = this->getSize();
    if(m_pBaoxiangAni) {
        m_pBaoxiangAni->removeFromParentAndCleanup(true);
    }
    m_pBaoxiangAni = CCArmature::create("ut00067");  //宝箱
    if(m_pBaoxiangAni) {
        m_pBaoxiangAni->setPosition(convertToNodeSpace(centerPos));
        m_pBaoxiangAni->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(WeaponLotResultPanel::HandleBaoxiangEffEvent));
        addNode(m_pBaoxiangAni);

		if(m_uLotteryType == vmsg::CS_LOTTERY_TYPE_BAT_NORMAL) {
			m_pBaoxiangAni->getAnimation()->play("fly", -1, -1, 0);
		} else if(m_uLotteryType == vmsg::CS_LOTTERY_TYPE_BAT_ADVANCE) {
			m_pBaoxiangAni->getAnimation()->play("fly_1", -1, -1, 0);
		} else {
			m_pBaoxiangAni->getAnimation()->play("fly", -1, -1, 0);
		}
		
    } else {
        CCLOG("[error]fail to load baoxiang armature");
    }


}


void WeaponLotResultPanel::HandleBaoxiangEffEvent(CCArmature * armature, MovementEventType type, const char * name) {
    if (type == COMPLETE){
        //armature->removeFromParentAndCleanup(true);
        showNextElmt();
        if(m_pBaoxiangAni) {
			m_pBg->setVisible(true);
            
			if(m_uLotteryType == vmsg::CS_LOTTERY_TYPE_BAT_NORMAL) {
				m_pBaoxiangAni->getAnimation()->play("fly_xh", -1, -1, 1);
			} else if(m_uLotteryType == vmsg::CS_LOTTERY_TYPE_BAT_ADVANCE) {
				m_pBaoxiangAni->getAnimation()->play("fly_xh_1", -1, -1, 1);
			} else {
				m_pBaoxiangAni->getAnimation()->play("fly_xh", -1, -1, 1);
			}

            m_pBaoxiangAni->getAnimation()->setMovementEventCallFunc(NULL, NULL);
        }
    }
}


void WeaponLotResultPanel::HandleCellEffEvent(CCArmature * armature, MovementEventType type, const char * name) {
    if (type == COMPLETE){
        if(m_pCurCellAni) {
            m_pCurCellAni->getAnimation()->play(getLastCellAniName(true).c_str(), -1, -1, 1);
            m_pCurCellAni->getAnimation()->setMovementEventCallFunc(NULL, NULL);
        }
        m_pCurCellAni = NULL;
        showNextElmt();
    }
}


void WeaponLotResultPanel::HandleAdvEffDoneEvent(CCArmature * armature, MovementEventType type, const char * name) {
    if (type == COMPLETE){
        armature->removeFromParentAndCleanup(true);
        doShowElmt();
    }
}

void WeaponLotResultPanel::showNextElmt() {
    if(m_pCellHBox->getItemObjLst()->count() >= MAX_CELL_NUM) {
        return;
    }
    
    const unsigned int idx = m_pCellHBox->getItemObjLst()->count();
    const CSAwardElement& elmt = m_stElmtDataLst[idx];

    if(vmsg::THING_TYPE_WEAPON == elmt.elementtype()) {
        //出枪械时播放高级全屏特效！
        P_GLOBAL->showAdvItemEff(elmt.elementtype(), elmt.elementid(), 
            this, movementEvent_selector(WeaponLotResultPanel::HandleAdvEffDoneEvent) );
    } else {
        doShowElmt();
    }

}

void WeaponLotResultPanel::doShowElmt() {
    if(m_pCellHBox->getItemObjLst()->count() >= MAX_CELL_NUM || !this->isOpen() ) {
        return;
    }
    const unsigned int idx = m_pCellHBox->getItemObjLst()->count();
    const CSAwardElement& elmt = m_stElmtDataLst[idx];

    CCLOG("[trace]WeaponLotResultPanel::showNextElmt,type=%d,cnt=%d,id=%d", elmt.elementtype(), elmt.elementcnt(), elmt.elementid());
    WeaponLotResultCell *pstCell = WeaponLotResultCell::create();
    pstCell->setInfo(elmt.elementtype(), elmt.elementcnt(), elmt.elementid());
    m_pCellHBox->addItem(pstCell);

    m_pCurCell = pstCell;

    //只显示高品级的特效框
    if(getLastCellAniName(false).size() > 0) {
        m_pCurCellAni = CCArmature::create("ut00018"); 
        if(m_pCurCellAni) {
            CCPoint stPt = pstCell->getCellBg()->getPosition(); 

            m_pCurCellAni->setPosition(stPt);

            pstCell->addNode(m_pCurCellAni);
            m_pCurCellAni->setVisible(false); //飞下去后再显示
        } else {
            CCLOG("[error]fail to load cell armature");
        }
    } else {
        m_pCurCellAni = NULL;
    }
    

    m_pCurCell->setVisible(false);


    //ut00068从屏幕中间飞下来,然后显示出来
    if(NULL == m_pFlyBallAni) {
        m_pFlyBallAni = CCArmature::create("ut00068"); 
        addNode(m_pFlyBallAni);
    }
    m_pFlyBallAni->setPosition(m_pBaoxiangAni->getPosition() + ccp(0, 270));
    m_pFlyBallAni->setVisible(true);
    m_pFlyBallAni->getAnimation()->play("fly", -1, -1, 1);
    CCPoint toPos = pstCell->getWorldPosition();
    toPos.x += pstCell->getSize().width/2;
    toPos.y += pstCell->getSize().height/2;

    CCDirector *pDirector = CCDirector::sharedDirector();
    CCPoint visiblePos = pDirector->getVisibleOrigin();
    toPos = toPos - visiblePos;

    //fly
    CCActionInterval *pEaseMove = CCEaseIn::create(CCMoveTo::create(0.4f, toPos), 1.0f);
    CCActionInstant *pFunc = CCCallFunc::create(this, callfunc_selector(WeaponLotResultPanel::onFlyBallArrived));
    m_pFlyBallAni->runAction(CCSequence::create(pEaseMove, pFunc, NULL));
}


void WeaponLotResultPanel::onFlyBallArrived() {
    m_pFlyBallAni->getAnimation()->stop();
    m_pFlyBallAni->setVisible(false);

    //飞完显示cell
    m_pCurCell->setVisible(true);
    if(m_pCurCellAni) {
        m_pCurCellAni->setVisible(true); 
        m_pCurCellAni->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(WeaponLotResultPanel::HandleCellEffEvent));
        m_pCurCellAni->getAnimation()->play(getLastCellAniName(false).c_str(), -1, -1, 0);
    } else {
        HandleCellEffEvent(NULL, COMPLETE, "");
    }
    
}

const string WeaponLotResultPanel::getLastCellAniName(const bool isLoopAni) {
    int idx = m_pCellHBox->getItemObjLst()->count() - 1;
    WeaponLotResultCell* pCell = (WeaponLotResultCell*)(m_pCellHBox->getItemObj(idx) );
    if(NULL == pCell) {
        return "";
    }

    string strPrefix;
    string strColorName;

    const int iElmtType = pCell->getCell()->getItemType();
    const int iElmtID = pCell->getCell()->getItemCfgid();
    if(THING_TYPE_WEAPON == iElmtType) {
        strPrefix = "d_";
        strColorName = getWeaponColorString(WEAPON_CFG_MASTER->getWeaponLv(iElmtID));
        //strName = "d_" + getColorString(WEAPON_CFG_MASTER->getWeaponLv(iElmtID));
    } else if(THING_TYPE_ITEM == iElmtType) {
        strPrefix = "x_";
        strColorName = getColorString(ITEM_CFG_MASTER->GetItemColor(iElmtID));
        //strName = "x_" + getColorString(ITEM_CFG_MASTER->GetItemColor(iElmtID));
    } else if(THING_TYPE_EQUIP == iElmtType) {
        strPrefix = "x_";
        strColorName = getColorString(ItemUtils::getItemColor(THING_TYPE_EQUIP, iElmtID));
        //strName = "x_" + getColorString(ItemUtils::getItemColor(THING_TYPE_EQUIP, iElmtID));
    } else if(THING_TYPE_DEBRIS == iElmtType) {
        //碎片根据产出类型显示
        const DebrisCfg* pCfg = DEBRIS_CFG_MASTER->GetDebrisCfgByID(iElmtID);
        if(pCfg) {
            if(THING_TYPE_WEAPON == pCfg->composetype()) {
                //strName = "d_" + getColorString(pCfg->debriscolor());
                strPrefix = "d_";
                strColorName = getColorString(pCfg->debriscolor());
            } else if(THING_TYPE_ITEM == pCfg->composetype()) {
                //strName = "x_" + getColorString(pCfg->debriscolor());
                strPrefix = "x_";
                strColorName = getColorString(pCfg->debriscolor());
            } 
        }

    } else if(THING_TYPE_DOTAITEM == iElmtType) {
		strPrefix = "x_";
		strColorName = getColorString(ItemUtils::getItemColor(THING_TYPE_DOTAITEM, iElmtID));
	}


    if(strColorName.size() == 0) {
        return "";
    }
    string strName = strPrefix + strColorName;

    if(isLoopAni) {
        strName += "_xh";
    }
    return strName;
}


const std::string WeaponLotResultPanel::getColorString(const int iColor) {

    switch(iColor) {
         //低等级不显示框
    case 1:
        return "white";
    case 2:
        return "green";
    case 3:
        return "blue";
    case 4:
        return "purple";
    case 5:
        return "gold";
    default:
        return "";
    }
}

const std::string WeaponLotResultPanel::getWeaponColorString(const int iColor){
	switch(iColor) {
         //低等级不显示框
    case 1:
        return "white";
    case 2:
        return "green";
    case 3:
	case 4:
        return "blue";
	case 5:
	case 6:
        return "purple";
    case 7:
	case 8:
	case 9:
        return "gold";
    default:
        return "";
    }
}


