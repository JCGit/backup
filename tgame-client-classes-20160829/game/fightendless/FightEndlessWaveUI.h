#ifndef FightEndlessWaveUI_h__
#define FightEndlessWaveUI_h__

#include "BaseLayout.h"
#include "ImageNumber.h"

class CFightEndlessWaveUI : public BaseLayout
{
public:
	CFightEndlessWaveUI();
	virtual ~CFightEndlessWaveUI();

	CREATE_FUNC(CFightEndlessWaveUI);

	bool init();

	void setWave(unsigned int wave);

	void startTip();				//��ʾ�²���������

	bool isTiping(){				//�Ƿ����ڲ�����ʾ����
		return m_bIsTiping;
	}				
protected:
	void binkComplete();
private:
	ImageView* m_pLeftLabel;
	ImageView* m_pRightLabel;
	ImageView* m_pTipImg;
	ImageNumber* m_pWaveTxt;
	ImageNumber* m_pTimeTxt;

	bool m_bIsTiping;
};

#endif // FightEndlessWaveUI_h__
