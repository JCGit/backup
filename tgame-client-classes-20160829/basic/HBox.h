#ifndef __H_BOX_H__
#define __H_BOX_H__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

using namespace cocos2d::gui;
/**
HBox,�����������size��item
����m_iOneRowNum����ˮƽ���У���ֱ����
**/
class HBox : public BaseLayout
{
public:
    /*
    enum HBOXSTAT
    {
        HBOX_HORIZONTAL, //ˮƽ����
        HBOX_VERTICAL, //��ֱ����
        HBOX_BOTH, //��ˮƽ���У���ֱ����
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
    /* ��ʼ��HBox
    * stMaxSize, HBox�����size
    * iOneRowNum,һ���м���Ԫ��
    * iIntervalX��item��X���
    * iIntervalY��item��Y���
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
        //��ȡ���һ��item��xy����
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
    unsigned int m_iOneRowNum; // һ���м���Ԫ��
    unsigned int m_iIntervalX; // Ԫ�ص�X���
    unsigned int m_iIntervalY; // Ԫ�ص�Y���

    CCArray *m_pObjLst; //��ʾ����

};



#endif //__H_BOX_H__