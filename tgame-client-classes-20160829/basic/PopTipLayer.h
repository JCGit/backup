#ifndef PopTipLayer_h__
#define PopTipLayer_h__

#include "cocos2d.h"
USING_NS_CC;
//#include <string>

/*
* 
* tips弹出层
* 
* 
*/
class CPopTipLayer : public CCNode
{
public:
    enum {
        POP_TIP_STR     = 10, //奖励提示等
        POP_SCROLL_STR  = 20, //通知文本
        POP_ERROR_STR   = 30, //错误提示

        POP_ERROR_MAX_NUM = 6, //最多同时显示6个错误提示吧
    };

	CPopTipLayer();
	~CPopTipLayer();

	void initUI();

    void ShowNotifyStr(const char* pStrTip, const float iSeconds = 5.0f); //通知文字，仅文本
    void ShowTipStr(const char* pStrTip, const int iType = 0, const int iCfgID = 0); //提示文本，可以附加显示一个Icon
    void ClearAllTips();

    void ShowAwardTip(const int iType, const int iCfgID, const int iCnt); //奖励提示

    void ShowErrorStr(const char* pStrError); //错误提示
private:
    void ShowTipStrCallBack(CCNode* pSender, void* pData);
    void ShowNotifyStrCallBack(CCNode* pSender, void* pData);
    void ShowErrorStrCallBack(CCNode* pSender, void* pData);

    CCArray* m_stTipLst; //奖励提示
    std::vector<std::string> m_stNotifyStrBuff; //notify str buffer
    CCArray* m_pErrorLabelLst; //最多加6个吧 POP_ERROR_MAX_NUM

	CCNode* m_pErrorNode;
};


#define POP_TIP_LAYER_MASTER (CVLib::CVSingleton<CPopTipLayer>::Instance())


#endif // PopTipLayer_h__
