#ifndef PopTipLayer_h__
#define PopTipLayer_h__

#include "cocos2d.h"
USING_NS_CC;
//#include <string>

/*
* 
* tips������
* 
* 
*/
class CPopTipLayer : public CCNode
{
public:
    enum {
        POP_TIP_STR     = 10, //������ʾ��
        POP_SCROLL_STR  = 20, //֪ͨ�ı�
        POP_ERROR_STR   = 30, //������ʾ

        POP_ERROR_MAX_NUM = 6, //���ͬʱ��ʾ6��������ʾ��
    };

	CPopTipLayer();
	~CPopTipLayer();

	void initUI();

    void ShowNotifyStr(const char* pStrTip, const float iSeconds = 5.0f); //֪ͨ���֣����ı�
    void ShowTipStr(const char* pStrTip, const int iType = 0, const int iCfgID = 0); //��ʾ�ı������Ը�����ʾһ��Icon
    void ClearAllTips();

    void ShowAwardTip(const int iType, const int iCfgID, const int iCnt); //������ʾ

    void ShowErrorStr(const char* pStrError); //������ʾ
private:
    void ShowTipStrCallBack(CCNode* pSender, void* pData);
    void ShowNotifyStrCallBack(CCNode* pSender, void* pData);
    void ShowErrorStrCallBack(CCNode* pSender, void* pData);

    CCArray* m_stTipLst; //������ʾ
    std::vector<std::string> m_stNotifyStrBuff; //notify str buffer
    CCArray* m_pErrorLabelLst; //����6���� POP_ERROR_MAX_NUM

	CCNode* m_pErrorNode;
};


#define POP_TIP_LAYER_MASTER (CVLib::CVSingleton<CPopTipLayer>::Instance())


#endif // PopTipLayer_h__
