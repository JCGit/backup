#ifndef NihilityItemLayout_h__
#define NihilityItemLayout_h__

#include "BaseLayout.h"

class IconCell;

class CNihilityItemLayout : public BaseLayout
{
public:
	CNihilityItemLayout();
	virtual ~CNihilityItemLayout();

	CREATE_FUNC(CNihilityItemLayout);

	static CNihilityItemLayout* createWithLayout(UILayout *pRefLayout);

	bool init();

	bool initWithLayout(UILayout *pRefLayout);

	void setCfgID(unsigned int uCfgID);

	unsigned int getCfgID(){
		return m_uCfgID;
	};

	void setSelected(bool bIsSelected);

	bool getSelected(){
		return m_bIsSelect;
	}

	void setIsEnable(bool var);

	bool getIsEnable(){
		return m_bIsEnable;
	}
protected:
private:
	void initUI();

	Label* m_pDescTxt;
	CheckBox* m_pSelectBtn; //Ñ¡ÖÐ¿ò
	IconCell* m_pIconCell;

	unsigned int m_uCfgID;
	bool m_bIsSelect;
	bool m_bIsEnable;
};

#endif // NihilityItemLayout_h__
