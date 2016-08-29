#ifndef PanelSeqMgr_h__
#define PanelSeqMgr_h__

#include "vlib/base/CVSingleton.h"

#include <vector>

using namespace std;

class BasePanel;

//��巵�ػ���
class PanelSeqMgr 
{
public:
    PanelSeqMgr();
    ~PanelSeqMgr();

    BasePanel* getLast();
    bool isLast(BasePanel* pPanel);

    void pushBack(BasePanel* pPanel);
    BasePanel* popBack();

    void delInStack(BasePanel* pPanel); //��stack ��ɾ��

    void clearStack();
private:
    vector<BasePanel*> m_stPanelStack;
};


#define PANEL_SEQ_MGR CVLib::CVSingleton<PanelSeqMgr>::Instance()

#endif // PanelSeqMgr_h__