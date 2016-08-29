#ifndef __BASE_TAB_LAYER_H__
#define __BASE_TAB_LAYER_H__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

using namespace cocos2d::gui;
/**
tab页的基类
该类不是显示对象，仅配合tab btn操作显示对象m_pTabLayer是否可见
**/
class BaseTabLayer : public CCObject
{
public:
    BaseTabLayer():m_pTabLayer(NULL),m_bVisible(false){};
    virtual ~BaseTabLayer(){};

    /************************************************************************/
    /* 初始化tab页显示对象
    * rootLayer tab页显示对象m_pTabLayer的父容器
    * name tab页显示对象在父容器中的名字
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
    virtual void updateUI() = 0; //必须实现的接口
    
protected:
    UILayout *m_pTabLayer; //tab页显示对象
    bool m_bVisible; //是否在显示
};



#endif //__BASE_TAB_LAYER_H__