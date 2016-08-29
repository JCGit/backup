#ifndef BaseItem_h__
#define BaseItem_h__

#include "BaseLayout.h"

class CBaseItem : public BaseLayout
{
public:
	CBaseItem();
	virtual ~CBaseItem();

	virtual void setSelected(bool var);
	virtual bool getSelected();

	virtual void setIsEnable(bool var);
	virtual bool getIsEnable();
protected:
	bool _selected;											//�Ƿ�ѡ��
	bool _isEnable;											//�Ƿ��ѡ
private:
};

#endif // BaseItem_h__
