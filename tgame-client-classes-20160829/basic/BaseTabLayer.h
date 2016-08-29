#ifndef __BASE_TAB_LAYER_H__
#define __BASE_TAB_LAYER_H__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

using namespace cocos2d::gui;
/**
tabҳ�Ļ���
���಻����ʾ���󣬽����tab btn������ʾ����m_pTabLayer�Ƿ�ɼ�
**/
class BaseTabLayer : public CCObject
{
public:
    BaseTabLayer():m_pTabLayer(NULL),m_bVisible(false){};
    virtual ~BaseTabLayer(){};

    /************************************************************************/
    /* ��ʼ��tabҳ��ʾ����
    * rootLayer tabҳ��ʾ����m_pTabLayer�ĸ�����
    * name tabҳ��ʾ�����ڸ������е�����
    */
    /************************************************************************/
    virtual void initUI(UILayout *rootLayer, const char *name) {
        UILayout *pUI = dynamic_cast<UILayout*>(rootLayer->getChildByName(name));
        if (NULL == pUI)
        {
            CCLOG("%s", __FUNCTION__);
            return;
        }
        m_pTabLayer = pUI;
    }
    virtual void setVisible(bool isVisible) {
        m_bVisible = isVisible;
        if (m_pTabLayer) {
            m_pTabLayer->setVisible(isVisible);
            m_pTabLayer->setEnabled(isVisible);
        }
    }
    bool isVisible() { return m_bVisible; }
    virtual void updateUI() = 0; //����ʵ�ֵĽӿ�
    
protected:
    UILayout *m_pTabLayer; //tabҳ��ʾ����
    bool m_bVisible; //�Ƿ�����ʾ
};



#endif //__BASE_TAB_LAYER_H__