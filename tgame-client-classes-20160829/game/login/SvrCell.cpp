#include "SvrCell.h"
#include "TextHelper.h"

SvrCell::SvrCell()
:m_pBgImg(NULL)
,m_pSvrNameTxt(NULL)
{
}

SvrCell::~SvrCell()
{
	m_pBgImg = NULL;
	m_pSvrNameTxt = NULL;
}

SvrCell* SvrCell::create() {
	SvrCell *pSvrCell = new SvrCell();

	if(pSvrCell != NULL && pSvrCell->init()) {
		pSvrCell->autorelease();
	} else {
		CC_SAFE_DELETE(pSvrCell);
	}

	return pSvrCell;
}

bool SvrCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

        initLayoutFromJsonFile("ui_v2/SvrCellUI.ExportJson");
		addChild(m_pRootLayout);

		m_pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("cell_naomal_img"));

		m_pSvrNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("svr_name_txt"));

		bRet = true;
	} while(0);

	return bRet;
}

std::string SvrCell::getDescription() const {
	return "SvrCell";
}

const SvrInfo& SvrCell::getInfo() {
	return m_stSvrInfo;
}

void SvrCell::setInfo(const SvrInfo &stSvrInfo) {
	m_stSvrInfo = stSvrInfo;
	m_pSvrNameTxt->setText(m_stSvrInfo.name);
	return;
	//TODO: jacey.
	eSvrStatus eStatus = (eSvrStatus)stSvrInfo.iFlag;

	std::string szStatues = "";
	switch (eStatus)
	{
	case ESS_RECOMMEND:
		szStatues = TextStr(TEXT_SVR_RECOMMEND);
		break;
	case ESS_CROWED:
		szStatues = TextStr(TEXT_SVR_CROWED);
		break;
	case ESS_FULL:
		szStatues = TextStr(TEXT_SVR_FULL);
		break;
	default:
		break;
	}
	m_pSvrNameTxt->setText(m_stSvrInfo.name + szStatues);
}
