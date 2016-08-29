#include "StarAwardPanel.h"
#include "CfgMaster/PackageCfgMaster.h"
#include "IconCell.h"
#include "basic/ToolUtils.h"
#include "game/tip/TipControl.h"
#include "ClientSvrMsg.pb.h"

using namespace vmsg;
#define  ICONITEM_NAME "iconitem_name"
StarAwardPanel::StarAwardPanel(void)
	:m_yesBtn(NULL),
	m_closeBtn(NULL),
	m_desTxt(NULL)
{
}


StarAwardPanel::~StarAwardPanel(void)
{
}


bool StarAwardPanel::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(false);

		bRet = true;
	} while (0);

	return bRet;
}

void StarAwardPanel::open()
{
	BasePanel::open();

	if (!m_bUIInited)
	{
		initUI();
	}
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN, false);
}

void StarAwardPanel::initUI()
{

	initLayoutFromJsonFile("ui_v2/StarAwardPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_yesBtn = dynamic_cast<UIButton*> (m_pRootLayout->getChildByName("yes_btn"));

	m_closeBtn = dynamic_cast<UIButton*> (m_pRootLayout->getChildByName("close_btn"));

	m_desTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("desc_txt"));

	for (int i = 1; i<=3; i++)
	{
		UIImageView* p_image = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName(CCString::createWithFormat("item_%d",i)->getCString()));
		m_itemVec.push_back(p_image);
		m_posXVec.push_back(p_image->getPositionX());
		p_image->setEnabled(false);
	}

	m_bUIInited = true;
}


void	StarAwardPanel::show(const string& pdes,  unsigned int packageid)
{
	open	();

	m_desTxt->setText(pdes);
	const PackageCfg* p_PackageCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(packageid);
	int idx = 0;
	while (idx < 3)
	{
		if (idx < p_PackageCfg->elmtlst_size())
		{
			m_itemVec[idx]->setEnabled(true);
			m_itemVec[idx]->setPositionX(m_posXVec[idx]);
			IconCell* p_icon = IconCell::create();
			p_icon->setName(ICONITEM_NAME);
			p_icon->showNumTxt();
			m_itemVec[idx]->removeAllChildren();
			m_itemVec[idx]->addChild(p_icon);
			p_icon->setInfo(p_PackageCfg->elmtlst(idx).elementtype(),
				p_PackageCfg->elmtlst(idx).elementcntmax(),
				p_PackageCfg->elmtlst(idx).elementid());

			CToolUtils::adjustIconBgSize(p_PackageCfg->elmtlst(idx).elementtype(), p_PackageCfg->elmtlst(idx).elementid(), m_itemVec[idx]);
			CToolUtils::adjustFragIcon(p_PackageCfg->elmtlst(idx).elementtype(), m_itemVec[idx]);
			/*
			if (idx == 1&&p_PackageCfg->elmtlst(idx).elementtype() == THING_TYPE_WEAPON)
			{
				if (p_PackageCfg->elmtlst(idx).elementtype() == THING_TYPE_WEAPON)
				{
					m_itemVec[idx]->setPositionX(m_desTxt->getPositionX()+50);
				}
				
			}
			*/
		}else
		{
			m_itemVec[idx]->setEnabled(false);
		}
		idx++;
	}

	if (p_PackageCfg->elmtlst_size() == 1)
	{
		m_itemVec[0]->setPositionX(m_desTxt->getPositionX());
	}
}

bool StarAwardPanel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{

	return true;
}
void StarAwardPanel::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	this->onTouch(pTouch->getLocation());
}

void StarAwardPanel::onTouch(const CCPoint& stGlobalPt)
{
	if (m_closeBtn->hitTest(stGlobalPt)|| m_yesBtn->hitTest(stGlobalPt))
	{
		close();
	}else
	{
		for (auto p_image : m_itemVec)
		{
			if (p_image->hitTest(stGlobalPt) && p_image->isEnabled())
			{
				IconCell* p_cell = dynamic_cast<IconCell*>(p_image->getChildByName(ICONITEM_NAME));
				P_TIP_CTRL->showItemTipPanel(p_cell->getItemType(),p_cell->getItemCfgid(),p_cell->getItemNum());
			}
		}
	}
	
}

void StarAwardPanel::close()
{
	BasePanel::close();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}