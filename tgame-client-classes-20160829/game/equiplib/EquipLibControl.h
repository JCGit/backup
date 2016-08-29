#ifndef __EQUIP_LIB_CONTROL_h__
#define __EQUIP_LIB_CONTROL_h__

#include "EquipLibProxy.h"
#include "EquipLibObj.h"
#include "EquipLibPanel.h"
#include "EquipSelPanel.h"
#include "EquipSelMatPanel.h"
#include "EquipLibListPanel.h"

#define MAX_EQUIP_STAR  6   //装备最大星级

class EquipLibControl
{
public:
    EquipLibControl();
    ~EquipLibControl();

    EquipLibProxy* getProxy();
    EquipLibObj* getObj() {
        return &m_stObj;
    }
    
    EquipLibPanel* getEquipLibPanel();
    EquipSelPanel* getEquipSelPanel();
    EquipSelMatPanel* getEquipSelMatPanel();
    EquipLibListPanel* getEquipLibListPanel();

	void notifyEquipLvUp();

	void notifyEquipAdvance();

    void finalizePanel();
private:
    EquipLibProxy   m_stEquipLibProxy;
    EquipLibObj     m_stObj;    

    EquipLibPanel*   m_pEquipLibPanel;
    EquipSelPanel*   m_pEquipSelPanel;
    EquipSelMatPanel* m_pEquipSelMatPanel;

    EquipLibListPanel* m_pEquipLibListPanel;

	
};

#define P_EQUIP_LIB_CTRL CVLib::CVSingleton<EquipLibControl>::Instance()

#endif // __EQUIP_LIB_CONTROL_h__
