#include "BaseLayout.h"

using namespace std;
BaseLayout::BaseLayout()
:m_pRootLayout(NULL)
,m_bUIInited(false)
,m_bUIDelayed(false)
{
}

BaseLayout::~BaseLayout()
{
	CC_SAFE_RELEASE_NULL(m_pRootLayout); // retain at BaseLayout::setRootLayout(UILayout *pRootLayout)
}

bool BaseLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!UILayout::init());

		setTouchEnabled(true); // 默认接收触摸事件

		bRet = true;
	} while(0);

	return bRet;
}

std::string BaseLayout::getDescription() const {
	return "BaseLayout";
}

UILayout* BaseLayout::getRootLayout() {
	return m_pRootLayout;
}

void BaseLayout::setRootLayout(UILayout *pRootLayout) {
	CCAssert(pRootLayout != NULL, "Not null");

	CC_SAFE_RETAIN(pRootLayout); // release at BaseLayout::~BaseLayout()
	CC_SAFE_RELEASE(m_pRootLayout);
	m_pRootLayout = pRootLayout;
	m_pRootLayout->setVisible(true); // 可见
	m_pRootLayout->setPosition(CCPointZero); // 匹配坐标
	setSize(m_pRootLayout->getSize()); // 大小和UI资源匹配
}

bool BaseLayout::isUIInited() {
	return m_bUIInited;
}

void BaseLayout::initLayoutFromJsonFile(const char *pszFileName) {
    string strFinalPath(pszFileName);
    string strPath(pszFileName);
    if(string::npos != strPath.find(".ExportJson")) {
        strFinalPath = strPath;
    } else if(string::npos != strPath.find(".json")) {
        strFinalPath = strPath;
        int iBeginIdx = strPath.find("ui/");
        int iEndIdx = strPath.find(".json");
        if(iBeginIdx >= 0 && iEndIdx >= 0) {
            string tmpStr = strFinalPath.substr(iBeginIdx + 3, iEndIdx - iBeginIdx - 3);
            strFinalPath = "ui_v2/" + tmpStr + ".ExportJson";
            CCLOG("[trace]BaseLayout::initLayoutFromJsonFile,change path %s to %s", pszFileName, strFinalPath.c_str());
        }
    } 
    //CCLOG("[trace]BaseLayout::initLayoutFromJsonFile,path %s", strFinalPath.c_str());

    //CCTexture2DPixelFormat currentFormat = CCTexture2D::defaultAlphaPixelFormat();
    //CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);//强制使用该格式

	UILayout *pRootLayout = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile(strFinalPath.c_str()));
    //CCLog("%s", pRootLayout->getName());
    pRootLayout->setName(strFinalPath.c_str());

    //CCTexture2D::setDefaultAlphaPixelFormat(currentFormat); //用完后恢复
	setRootLayout(pRootLayout);
}

void BaseLayout::initLayoutWithUI(UILayout *pTargetLayout) {
	UILayout *pParentLayout = dynamic_cast<UILayout*>(pTargetLayout->getParent());
	CCPoint pos = pTargetLayout->getPosition(); // 保存位置
	CCPoint anPos = pTargetLayout->getAnchorPoint();
	int nZOrder = pTargetLayout->getZOrder(); // 保存Z序

	pTargetLayout->removeFromParent();
	setRootLayout(pTargetLayout);
	addChild(m_pRootLayout);

	setPosition(pos);
	setZOrder(nZOrder);
	setAnchorPoint(anPos);
	pParentLayout->addChild(this);
}

void BaseLayout::initLayoutWithUICopy(UILayout *pRefLayout) {
	UILayout *pRootLayout = dynamic_cast<UILayout*>(pRefLayout->clone());
	setRootLayout(pRootLayout);
}
