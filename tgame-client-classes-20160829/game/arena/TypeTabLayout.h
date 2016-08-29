#ifndef TypeTabLayout_h__
#define TypeTabLayout_h__

#include "BaseLayout.h"

class CTypeTabLayout : public BaseLayout
{
public:
	CTypeTabLayout();
	virtual ~CTypeTabLayout();

	CREATE_FUNC(CTypeTabLayout);

	static CTypeTabLayout* create(UILayout *pRefLayout);

	bool init();

	bool initWithLayout(UILayout *pRefLayout);

	void initUI();

	void setType(unsigned int uType);
	unsigned int getType(){
		return m_uType;
	}

	Button* getTabBtn(){
		return m_pBtn;
	}
protected:
private:
	Button* m_pBtn;

	unsigned int m_uType;
	bool m_bIsSelect;
};

#endif // TypeTabLayout_h__
