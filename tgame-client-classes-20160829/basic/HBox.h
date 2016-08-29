#ifndef __H_BOX_H__
#define __H_BOX_H__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

using namespace cocos2d::gui;
/**
HBox,用于排列相等size的item
根据m_iOneRowNum，先水平排列，后垂直排列
**/
class HBox : public BaseLayout
{
public:
    /*
    enum HBOXSTAT
    {
        HBOX_HORIZONTAL, //水平排列
        HBOX_VERTICAL, //垂直排列
        HBOX_BOTH, //先水平排列，后垂直排列
    };
    */
public:
    HBox(){
        m_pObjLst = NULL;
    }
    virtual ~HBox(){
        if (m_pObjLst) {
            m_pObjLst->release();
        }
    }

    virtual bool init() {
        bool bRet = false;
        do {
            CC_BREAK_IF(!BaseLayout::init());
            if(m_pObjLst) {
                m_pObjLst->release();
            }
            m_pObjLst = CCArray::create();
            m_pObjLst->retain();

            bRet = true;
        } while(false);

        return bRet;
    }

    static HBox* create()
    {
        HBox* widget = new HBox();
        if (widget && widget->init())
        {
            widget->autorelease();
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return NULL;
    }

    /************************************************************************/
    /* 初始化HBox
    * stMaxSize, HBox的最大size
    * iOneRowNum,一行有几个元素
    * iIntervalX，item的X间距
    * iIntervalY，item的Y间距
    */
    /************************************************************************/
    virtual void initUI(const CCSize& stMaxSize, const unsigned int iOneRowNum = 1, 
        const unsigned int iIntervalX = 10, const unsigned int iIntervalY = 10) {
       
        setSize(stMaxSize);
        if (iOneRowNum < 1) {
            CCLOG("[error]HBox::initUI,invalid OneRowNum %d", iOneRowNum);
            return;
        }
        
        m_iOneRowNum = iOneRowNum;
        m_iIntervalX = iIntervalX;
        m_iIntervalY = iIntervalY;

    }

    void addItem(Widget *m_pItemLayer) {
        m_pObjLst->addObject((CCObject*)m_pItemLayer);
        CCPoint stPoint = getLastItemPoint(m_pItemLayer->getSize());
        m_pItemLayer->setPosition(stPoint);
        addChild(m_pItemLayer);
    }

    void clearBox() {
        m_pObjLst->removeAllObjects();
        removeAllChildren();
    }

    const CCArray* getItemObjLst() const {
        return m_pObjLst;
    }
    CCObject* getItemObj(const unsigned int idx) {
        if(idx >= 0 && idx < m_pObjLst->count()) {
            return m_pObjLst->objectAtIndex(idx);
        }
        return NULL;
    }
private:
    CCPoint getLastItemPoint(const CCSize& stItemSize) {
        //获取最后一个item的xy坐标
        CCPoint stPoint;

        const int iItemNum = m_pObjLst->count();
        CCPoint stItemPoint;
        //x
        int iTmp = int((iItemNum - 1) % m_iOneRowNum);
        stItemPoint.x = iTmp * (stItemSize.width + m_iIntervalX);

        //y
        iTmp = int( (iItemNum - 1) / m_iOneRowNum);
        stItemPoint.y = iTmp * (stItemSize.height + m_iIntervalY);
        stItemPoint.y = getSize().height - stItemPoint.y;

        return stItemPoint;
    }
protected:
    unsigned int m_iOneRowNum; // 一行有几个元素
    unsigned int m_iIntervalX; // 元素的X间距
    unsigned int m_iIntervalY; // 元素的Y间距

    CCArray *m_pObjLst; //显示对象

};



#endif //__H_BOX_H__