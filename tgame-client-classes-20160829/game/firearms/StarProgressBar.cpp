#include "StarProgressBar.h"

const static int STAR_SIZE = 30;

CStarProgressBar::CStarProgressBar()
:m_nStarNum(0)
,m_nStarTotalNum(7)
,m_nStarMargin(3)
,m_strBgImgURL("")
,m_strProgressImgURL("")
{
    m_pBatchNode = NULL;
}

CStarProgressBar::~CStarProgressBar(){
	m_stProgressImgList.clear();
	/*for(int i = 0; i < (int)m_stProgressImgList.size(); i++) {
		CC_SAFE_DELETE(m_stProgressImgList.at(i));
	}*/
}

CStarProgressBar* CStarProgressBar::create(const char *pstBgFileName, const char *pstProgressFileName, 
                                           const char* pszSpSheetName, const int iMaxStarNum){
	CStarProgressBar* pStarProgressBar = new CStarProgressBar();

	if(pStarProgressBar != NULL && pStarProgressBar->initWithTextures(pstBgFileName, pstProgressFileName,pszSpSheetName, iMaxStarNum)) {
		pStarProgressBar->autorelease();
	} else {
		CC_SAFE_DELETE(pStarProgressBar);
	}

	return pStarProgressBar;
}

bool CStarProgressBar::initWithTextures(const char *pstBgFileName, const char *pstProgressFileName, 
                                        const char* pszSpSheetName, const int iMaxStarNum){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		setTouchEnabled(false);

		m_strBgImgURL = pstBgFileName;
		m_strProgressImgURL = pstProgressFileName;

        m_pBatchNode = CCSpriteBatchNode::create(pszSpSheetName);
        addNode(m_pBatchNode);

        m_nStarTotalNum = iMaxStarNum;

		m_stProgressImgList.clear();
		int width = STAR_SIZE * m_nStarTotalNum + m_nStarMargin * (m_nStarTotalNum - 1);
		int height = STAR_SIZE;

		setSize(CCSizeMake(width, height));

		setStarNum(0, iMaxStarNum);

		bRet = true;
	} while(0);

	return bRet;
}

void CStarProgressBar::onSizeChanged(){
	BaseLayout::onSizeChanged();

	if(m_stProgressImgList.size() > 0){
		m_nStarMargin = (getSize().width - STAR_SIZE * m_nStarTotalNum) / (m_nStarTotalNum - 1);
	}

	resizeStars();
}

int CStarProgressBar::getStarNum(){
	return m_nStarNum;
}

void CStarProgressBar::setStarNum(int num) {
    setStarNum(num, m_nStarTotalNum);
}

void CStarProgressBar::setStarNum(int num, const int iTotalNum){
	if(num > iTotalNum){
		CCLOG("the number is over total stars");
		return;
	}
    m_nStarTotalNum = iTotalNum;
    m_nStarNum = num;

    m_pBatchNode->removeAllChildrenWithCleanup(true);
    m_stProgressImgList.clear();
    for(int i = 0; i < m_nStarTotalNum; ++i) {
        CCSprite* sp = CCSprite::create();
        if(i < num) {
            sp->initWithSpriteFrameName(m_strProgressImgURL.c_str());
        } else {
            sp->initWithSpriteFrameName(m_strBgImgURL.c_str());
        }
        m_pBatchNode->addChild(sp);
        m_stProgressImgList.push_back(sp);
    }
    resizeStars();
}

int CStarProgressBar::getStarMargin() const{
	return m_nStarMargin;
}

void CStarProgressBar::setStarMargin(int nMargin){
	if(m_nStarMargin != nMargin){
		m_nStarMargin = nMargin;

		int width = STAR_SIZE * m_nStarTotalNum + m_nStarMargin * (m_nStarTotalNum - 1);
		int height = STAR_SIZE;

		setSize(CCSizeMake(width, height));
	}
}

void CStarProgressBar::resizeStars(){
	for(unsigned int i = 0; i < (int)m_stProgressImgList.size(); i++) {
		CCSprite *pImg = m_stProgressImgList.at(i);
		float posX = STAR_SIZE * pImg->getAnchorPoint().x + i * (STAR_SIZE + m_nStarMargin);
		float posY = STAR_SIZE * pImg->getAnchorPoint().y;
		pImg->setPosition(ccp(posX, posY));
	}
}