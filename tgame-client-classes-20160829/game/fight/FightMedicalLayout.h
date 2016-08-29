#ifndef FightMedicalLayout_h__
#define FightMedicalLayout_h__

#include "BaseLayout.h"

class CFightMedicalLayout : public BaseLayout
{
public:
	CFightMedicalLayout();
	virtual ~CFightMedicalLayout();

	CREATE_FUNC(CFightMedicalLayout);

	bool init();

	void updateUI();

	Button* getMedicalBtn(){
		return m_pMedicalBtn;
	}

protected:
private:
	void initUI();

	Button* m_pMedicalBtn;

	Label* m_pNumTxt;
};

#endif // FightMedicalLayout_h__
