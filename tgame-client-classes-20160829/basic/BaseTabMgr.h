#ifndef __BASE_TAB_MGR_H__
#define __BASE_TAB_MGR_H__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
#include "BaseTabLayer.h"
#include "vlib/QEventDispatcher.h"
using namespace CVLib;
using namespace cocos2d::gui;
using namespace std;

#define TAB_CHANGE_EVENT "tab_change_event"
/**
tab页管理类
**/
class BaseTabMgr : public CCObject , public CQEventDispatcher
{
public:
    BaseTabMgr() {
        m_pTabBtnLst = CCArray::create();
        m_pTabBtnLst->retain();
        m_pTabLayerLst = CCArray::create();
        m_pTabLayerLst->retain();
        m_bSelIdx = -1;
    }
    
    virtual ~BaseTabMgr(){
        m_pTabBtnLst->release();
        m_pTabLayerLst->release();
    }

    /************************************************************************/
    /* 添加一个tab页
    * rootLayer 按钮的父容器
    * name 按钮在父容器中的名字
    * strBtnName 按钮上显示的文本
    * pLayer tab页对象
    */
    /************************************************************************/
    void addTab(UILayout *rootLayer, const char *name, const std::string& strBtnName, const BaseTabLayer* pLayer) {
        UIButton *pTabBtn = dynamic_cast<UIButton*>(rootLayer->getChildByName(name));
        if (NULL == pTabBtn || NULL == pLayer)
        {
            CCLOG("[error]%s, add tab fail,para is null", __FUNCTION__);
            return;
        }
        if(strBtnName.size() > 0) {
            pTabBtn->setTitleText(strBtnName);
        }
        m_pTabBtnLst->addObject((CCObject*)pTabBtn);
        m_pTabLayerLst->addObject((CCObject*)pLayer);
        pTabBtn->addTouchEventListener(this, toucheventselector(BaseTabMgr::touchTabBtnEventHandler));
    }

    //显示指定的tab页
    void selectByIdx(const int iSelTabIdx) {
        if (m_pTabBtnLst->count() <= 0 || m_pTabBtnLst->count() != m_pTabLayerLst->count())
        {
            CCLOG("[error]%s, fail to select tab at %d,btn array size %d, layer array size %d", 
                __FUNCTION__, iSelTabIdx, m_pTabBtnLst->count(), m_pTabLayerLst->count());
            return;
        }
        //m_bSelIdx = iSelTabIdx; //set
        bool isSel = false;
        for (unsigned int i = 0; i <m_pTabBtnLst->count(); ++i)
        {
            isSel = (i == iSelTabIdx);
            setStat(i, isSel);
        }
    }

    //获取当前选择显示的tab页
    BaseTabLayer* getSelTab() {
        return (BaseTabLayer*)m_pTabLayerLst->objectAtIndex(m_bSelIdx);
    }

    int getSelTabIdx() {
        return m_bSelIdx;
    }

    void updateSelTab() {
        BaseTabLayer* pLayer = getSelTab();
        if (pLayer) {
            pLayer->updateUI();
        }
    }
private:
    void touchTabBtnEventHandler(CCObject *pSender, TouchEventType type) {
        UIButton *pTouchBtn = dynamic_cast<UIButton*>(pSender);
        switch(type) {
        case TOUCH_EVENT_ENDED:
            UIButton* pBtn = NULL;
            for (unsigned int i = 0; i <m_pTabBtnLst->count(); ++i) {
                pBtn = (UIButton*)m_pTabBtnLst->objectAtIndex(i);
                setStat(i, (pTouchBtn == pBtn) );
            }
            break;
        }
    }

    void setStat(const int iIdx, const bool isSel) {
        UIButton* pBtn = (UIButton*)m_pTabBtnLst->objectAtIndex(iIdx);
        BaseTabLayer* pLayer = (BaseTabLayer*)m_pTabLayerLst->objectAtIndex(iIdx);

        pBtn->setBright(!isSel);
        pLayer->setVisible(isSel);

        if (isSel) {
			if(m_bSelIdx != iIdx){
				pLayer->updateUI();
				m_bSelIdx = iIdx;

				CCString* strValue = CCString::createWithFormat("%d", m_bSelIdx);
				DispatchEvent(TAB_CHANGE_EVENT, strValue);
			}
        }
    }

private:
    CCArray* m_pTabBtnLst;
    CCArray* m_pTabLayerLst;
    int m_bSelIdx;
};



#endif //__BASE_TAB_MGR_H__