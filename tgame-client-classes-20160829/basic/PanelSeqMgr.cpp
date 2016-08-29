#include "PanelSeqMgr.h"
#include "BasePanel.h"

#include "cocos2d.h"
USING_NS_CC;

PanelSeqMgr::PanelSeqMgr() {

}

PanelSeqMgr::~PanelSeqMgr() {

}


BasePanel* PanelSeqMgr::getLast() {
    if(m_stPanelStack.size() > 0) {
        return m_stPanelStack[m_stPanelStack.size() - 1];
    } else {
        return NULL;
    }
}

bool PanelSeqMgr::isLast(BasePanel* pPanel) {
    return (getLast() == pPanel);
}

void PanelSeqMgr::pushBack(BasePanel* pPanel) {
    if(NULL == pPanel) {
        return;
    }
    //如果在里面，就直接返回
    for(auto iter = m_stPanelStack.begin(); iter != m_stPanelStack.end(); ++iter ) {
        if(pPanel == (*iter) ) {
            return;
        }
    }
    m_stPanelStack.push_back(pPanel);
    CCLOG("[trace]PanelSeqMgr::pushBack, [%s], size %d", typeid(*pPanel).name(), m_stPanelStack.size() );
}

BasePanel* PanelSeqMgr::popBack() {
    BasePanel* pLast = getLast();
    m_stPanelStack.pop_back();
    if(pLast) {
        CCLOG("[trace]PanelSeqMgr::popBack, [%s], size %d", typeid(*pLast).name(), m_stPanelStack.size() );
    } else {
        CCLOG("[trace]PanelSeqMgr::popBack, size %d", m_stPanelStack.size() );
    }
    return pLast;
}


void PanelSeqMgr::delInStack(BasePanel* pPanel) {
    for(auto iter = m_stPanelStack.begin(); iter != m_stPanelStack.end(); ++iter ) {
        if(pPanel == (*iter) ) {
            m_stPanelStack.erase(iter);
            return;
        }
    }
}


void PanelSeqMgr::clearStack() {
    m_stPanelStack.clear();
}
