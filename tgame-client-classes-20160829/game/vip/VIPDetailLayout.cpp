#include "VIPDetailLayout.h"
#include "defines.h"
#include "ImageNumber.h"

#include "VIPCfgMaster.h"
#include "VIPLvCfgMaster.h"
#include "PackageCfgMaster.h"

#include "ToolUtils.h"
#include "NounHelper.h"
#include "TextHelper.h"
#include "CCColorUtil.h"

#include "UserObj.h"
#include "IconCell.h"
#include "firearms/StarProgressBar.h"
#include "WeaponCfgMaster.h"
#include "VIPControl.h"
#include "ItemUtils.h"
#include "firearms/FireArmsObj.h"
#include "PopTipLayer.h"
#include "bag/BagControl.h"

#define AWARD_ITEM_MAX_NUM 3 // 最多三个材料奖励
#define CELL_TAG 22 // IconCell tag

VIPDetailLayout::VIPDetailLayout()
:m_uVIPLv(0)
{
}

VIPDetailLayout::~VIPDetailLayout()
{
}

VIPDetailLayout* VIPDetailLayout::create(UILayout *pRefLayout) {
	VIPDetailLayout* pRet = new VIPDetailLayout();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool VIPDetailLayout::initWithLayout(UILayout *pRefLayout) {
    bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUI(pRefLayout);

		m_pPermissionScrollView1 = dynamic_cast<UIScrollView*>(m_pRootLayout->getChildByName("permission_scroll_1"));
		m_pPermissionScrollView2 = dynamic_cast<UIScrollView*>(m_pRootLayout->getChildByName("permission_scroll_2"));

		m_pPermissionDescTxt1 = dynamic_cast<UILabel*>(m_pPermissionScrollView1->getChildByName("permission_desc_txt"));
		m_pPermissionDescTxt2 = dynamic_cast<UILabel*>(m_pPermissionScrollView2->getChildByName("permission_desc_txt"));

		m_pNextVipGiftImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("next_vip_gift"));
		m_pNextVipPrivilegeImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("next_privilege_img"));
		m_pFullVipTipImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("vip_full_tip"));

		IconCell* pCell = NULL;

		for(int i = 0; i < AWARD_ITEM_MAX_NUM; ++i) {
			UIImageView* pOneCell = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName(string("award_cell_" + intToString(i)).c_str() ) );
			m_stCellBgLst.push_back(pOneCell);

			pCell = IconCell::create();
			pCell->setAnchorPoint(ccp(0.5f, 0.5f));
			pCell->showNumTxt(true);
			pOneCell->addChild(pCell, 2, CELL_TAG);
			pOneCell->setTouchEnabled(true);
			pOneCell->addTouchEventListener(this, toucheventselector(VIPDetailLayout::touchEventHandler));
		}

		m_pWeaponCellBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("award_weapon_cell"));
		pCell = IconCell::create();
		pCell->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pWeaponCellBg->addChild(pCell, 2, CELL_TAG);
		m_pWeaponCellBg->setTouchEnabled(true);
		m_pWeaponCellBg->addTouchEventListener(this, toucheventselector(VIPDetailLayout::touchEventHandler));

		//weapon level star
		m_pWeaponLvStar = CStarProgressBar::create("share/little_star_dark.png", "share/little_star_light.png", "ui_v2/share.png");
		m_pWeaponLvStar->setAnchorPoint(ccp(0.5f, 0.0f));
		float xx = 0.0f;
		float yy = 20.0f;
		m_pWeaponLvStar->setPosition(ccp(xx, yy));
		m_pWeaponLvStar->setStarMargin(1);
		m_pWeaponLvStar->setStarNum(7);
		m_pWeaponCellBg->addChild(m_pWeaponLvStar, 5);

		m_pVIPLvImgNum = ImageNumber::create(25);
		m_pVIPLvImgNum->setAnchorPoint(ccp(0.0f, 0.0f));
		ImageView* pImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("privilege_img"));
		CCPoint pt = pImg->getPosition();
		pt.x = pt.x - pImg->getSize().width/2 - 90;
		m_pVIPLvImgNum->setPosition(pt);
		m_pVIPLvImgNum->setScale(0.8f);
		m_pRootLayout->addNode(m_pVIPLvImgNum);
		m_pVIPLvImgNum->setNum("0", 2.0f, "vip");

		bRet = true;
	} while(0);

	return bRet;
}

void VIPDetailLayout::updateUI(unsigned int uVIPLv) {
	m_uVIPLv = uVIPLv;

	m_pVIPLvImgNum->setNum(intToString(uVIPLv), 2.0f, "vip");

	const VIPLvCfgSet &stVIPLvCfgSet = VIP_LV_CFG_MASTER->GetCfg();
	unsigned int uMaxVIPLv = stVIPLvCfgSet.viplvcfg(stVIPLvCfgSet.viplvcfg_size() - 1).viplv();
	bool isTopVIP = (m_uVIPLv == uMaxVIPLv);

	updatePrivilege(m_uVIPLv, m_pPermissionScrollView1, m_pPermissionDescTxt1);
	if(!isTopVIP){
		updatePrivilege(m_uVIPLv + 1, m_pPermissionScrollView2, m_pPermissionDescTxt2);
	}

	m_pNextVipPrivilegeImg->setVisible(!isTopVIP);
	m_pPermissionDescTxt2->setVisible(!isTopVIP);
	m_pPermissionScrollView2->setVisible(!isTopVIP);
	m_pFullVipTipImg->setVisible(isTopVIP);
}

void VIPDetailLayout::updatePrivilege(unsigned int uLv, UIScrollView* pScroll, UILabel* pLabel){
	std::string text = "";
	const VIPCfgSet &stCfgSet = VIP_CFG_MASTER->GetCfg();

	for(int i = 0; i < stCfgSet.vipcfg_size(); i++) {
		const VIPCfg &stVIPCfg = stCfgSet.vipcfg(i);

		unsigned int num = stVIPCfg.vipparam(uLv);

		if(num > 0) {
			char desc[100];
			snprintf(desc, 100, stVIPCfg.desc().c_str(), num);
			string tmp = desc;
			limitWith(tmp, 16);
			text += tmp;

			if(i < stCfgSet.vipcfg_size() - 1) {
				text += "\n\n";
			}
		}
	}

	pLabel->setText(text);

	const CCSize &textSize = pLabel->getSize();
	const CCSize &viewSize = pScroll->getSize();
	float scrollW = viewSize.width;
	float scrollH = viewSize.height > textSize.height ? viewSize.height : textSize.height;
	pScroll->setInnerContainerSize(CCSizeMake(scrollH, scrollH));
	pLabel->setPosition(ccp(pLabel->getPositionX(), scrollH));
}

void VIPDetailLayout::updateAwardCell() {
	unsigned int uCurVip = P_USER_OBJ->getUserInfo()->vipLv;
	const VIPLvCfgSet &stVIPLvCfgSet = VIP_LV_CFG_MASTER->GetCfg();
	unsigned int uMaxVIPLv = stVIPLvCfgSet.viplvcfg(stVIPLvCfgSet.viplvcfg_size() - 1).viplv();

	if(uCurVip == uMaxVIPLv){
		for(unsigned int i = 0; i < AWARD_ITEM_MAX_NUM; ++i) {
			m_stCellBgLst[i]->setVisible(false);
		}
		m_pWeaponCellBg->setVisible(false);
		m_pNextVipGiftImg->setVisible(false);
		return;
	}

    VIPLvCfg *pVIPLvCfg = VIP_LV_CFG_MASTER->getVIPLvCfg(uCurVip + 1);
    if(NULL == pVIPLvCfg) {
        return;
    }
    
    unsigned int uPackageID = pVIPLvCfg->packageid();
    const PackageCfg *pPackageCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(uPackageID);

    int iCellIdx = 0;

    if(pPackageCfg == NULL) {
        //0级时没有礼包
        m_pWeaponCellBg->setVisible(false);
        for(; iCellIdx < AWARD_ITEM_MAX_NUM; ++iCellIdx) {
            m_stCellBgLst[iCellIdx]->setVisible(false);
        }
        return;
    }

    unsigned int level = P_USER_OBJ->getUserInfo()->level;

    // 检查开放等级
    if(level < pPackageCfg->openlv()) { 
        return; 
    }
    
    bool bHaveWeapon = false;
    IconCell* pCell = NULL;
	iCellIdx = 0;

    for(int i = 0; i < pPackageCfg->elmtlst_size(); i++) {
        const PackageElmt &stPkgElmt = pPackageCfg->elmtlst(i);
        if(vmsg::THING_TYPE_WEAPON == stPkgElmt.elementtype()) {
            m_pWeaponCellBg->setVisible(true);
			m_pWeaponCellBg->setTouchEnabled(true);
            pCell = dynamic_cast<IconCell*>(m_pWeaponCellBg->getChildByTag(CELL_TAG));

            const WeaponCfg* stCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stPkgElmt.elementid());
            m_pWeaponLvStar->setStarNum(stCfg->initstar());

            bHaveWeapon = true;
        } else {
			if(iCellIdx < AWARD_ITEM_MAX_NUM) {
                m_stCellBgLst[iCellIdx]->setVisible(true);
				m_stCellBgLst[iCellIdx]->setTouchEnabled(true);
				pCell = dynamic_cast<IconCell*>(m_stCellBgLst[iCellIdx]->getChildByTag(CELL_TAG));
				iCellIdx++;
			}
        }
        pCell->setInfo(stPkgElmt.elementtype(), stPkgElmt.elementcntmin(), stPkgElmt.elementid() );
    }
    if(!bHaveWeapon) {
        m_pWeaponCellBg->setVisible(false);
		m_pWeaponCellBg->setTouchEnabled(false);
    }

	if(iCellIdx < AWARD_ITEM_MAX_NUM && bHaveWeapon){
		float fDisX = m_stCellBgLst[iCellIdx]->getPositionX();
		float fCellW = m_stCellBgLst[iCellIdx]->getSize().width/2;
		m_pWeaponCellBg->setPositionX(fDisX - fCellW + m_pWeaponCellBg->getSize().width/2);
	}

    for(; iCellIdx < AWARD_ITEM_MAX_NUM; ++iCellIdx) {
        m_stCellBgLst[iCellIdx]->setVisible(false);
		m_stCellBgLst[iCellIdx]->setTouchEnabled(false);
    }
}


unsigned int VIPDetailLayout::getVIPLv() {
	return m_uVIPLv;
}


void VIPDetailLayout::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, "award_weapon_cell") == 0) {
				IconCell* pCell = dynamic_cast<IconCell*>(m_pWeaponCellBg->getChildByTag(CELL_TAG));
				if(pCell){
					P_BAG_CTRL->getItemInfoPanel()->open();
					P_BAG_CTRL->getItemInfoPanel()->updateInfo(pCell->getItemCfgid(), pCell->getItemType(), false);
				}
			}else if(strWidgetName.find("award_cell_") != string::npos) {
				string sub = strWidgetName.substr(strWidgetName.find_first_not_of("award_cell_"));
				int iIdx = atoi(sub.c_str());

				if(iIdx >= 0 && iIdx < AWARD_ITEM_MAX_NUM){
					IconCell* pCell = dynamic_cast<IconCell*>(m_stCellBgLst[iIdx]->getChildByTag(CELL_TAG));
					if(pCell){
						P_BAG_CTRL->getItemInfoPanel()->open();
						P_BAG_CTRL->getItemInfoPanel()->updateInfo(pCell->getItemCfgid(), pCell->getItemType(), false);
					}
				}
			}
			break;
		default:;
	}
}

void VIPDetailLayout::limitWith(std::string &text, unsigned int width) {
	int cnt = 0;
	vector<string> vecText;

	CToolUtils::parseUTFStr(text, vecText);
	text = "";

	for(unsigned int i = 0; i < vecText.size(); i++) {
		cnt++;

		if(cnt > 1) {
			text += vecText[i];
		} else if(vecText[i] != " ") {
			text += vecText[i];
		}


		if(vecText[i] == "\n") {
			cnt = 0;
		}

		if(vecText[i] == " ") {
			unsigned int n = 0;

			while(n < vecText.size() - i - 1) {
				n++;

				if(vecText[i + n] == " ") {
					break;
				}

			}

			if(cnt + n >= width) {
				text += "\n";
				cnt = 0;
			}
		}

		if(cnt != 0 && cnt % width == 0 && i < vecText.size() - 1) {
			text += "\n";
			cnt = 0;
		}
	}
}
