#ifndef FightWeaponArrLayout_h__
#define FightWeaponArrLayout_h__

#include "BaseLayout.h"
#include "BasicTypes.h"
#include "CSWeaponMsg.pb.h"
#include "vlib/CVArray.h"
#include "vlib/QEventDispatcher.h"
#include "../role/MainHero.h"

using namespace vmsg;
using namespace CVLib;

class CFightWeaponArrCell : public BaseLayout
{
public:
	CFightWeaponArrCell();
	~CFightWeaponArrCell() {}

	static CFightWeaponArrCell* create(ImageView *pRefLayout);
	bool initWithImg(ImageView* pImg);

	void setInfo(const CSWeapon& stInfo, const int iIDx, const int iState);
	const CSWeapon& getInfo(){
		return m_stInfo;
	}


	void show();
	void hide();

	ImageView* getRefImg(){
		return m_pRefImg;
	}

	void setSelected(bool var);
	bool getSelected(){
		return m_bSelected;
	}

	bool getIsShow(){
		return m_bIsShow;
	}

	void updateCurBulletShow(float fCurBulletVal);
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	ImageView* m_pRefImg;
	ImageView* m_pIconImg;
	ImageView* m_pBulletBg;

	Label* m_pBulletTxt;

	CSWeapon m_stInfo;
	bool m_bSelected;											//�Ƿ�ѡ��
	bool m_bIsShow;												//�Ƿ���ʾ

	CC_SYNTHESIZE(float, m_fMaxBulletVal, MaxBulletVal);		//���ҩ,��λ��
	CC_SYNTHESIZE(float, m_fCurBulletVal, CurBulletVal);		//��ǰ��ҩ,��λ��
	CC_SYNTHESIZE(int, m_iIDx, IDX);							//λ���±꣬��ʱǹеΪ-1
	CC_SYNTHESIZE(int, m_iState, State);						//��λ�Ƿ���
};

class CFightWeaponArrLayout : public BaseLayout, public CQEventDispatcher
{
public:
	static const string HERO_CHANGE_WEAPON_EVENT;				//��ǹ�¼�
public:
	enum {
		MAX_WEAPON_NUM = 5,
	};
public:
	CFightWeaponArrLayout();
	virtual ~CFightWeaponArrLayout();

	static CFightWeaponArrLayout* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);

	void updateUI();

	void setSelectByIdx(int iIDx);
	int getSelectIdx(){
		return m_iIdx;
	}

	void setSelectByGUID(UINT64_t uWeaponGUID);

	void showTempWeapon(const CSWeapon& stInfo);

	void updateCurCell(float fTime);
	float getCurCellBulletVal();

	void recordAllBulletVal(CVArray<float, 6>& stArr);
	void updateCellsByVec(CVArray<float, 6> stArr);

	CFightWeaponArrCell* getCellByInfo(UINT64_t uWeaponGUID);

	bool IsAllWeaponOutBullet();						//�ж��Ƿ����е�ǹ��û��ҩ
	void fillAllWeaponBullet();							//������ҩ

	void autoChangeNextWeapon();						//�Զ��л�����һ�е�ҩ��ǹе
	void RestorePreIdx(){
		setSelectByIdx(m_iPreIdx);

	};

	void initOwnerHero(CMainHero* pstHero){
		m_pstOnwerHero = pstHero;
	}
protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void doTouchByIdx(int iIDx);

	CFightWeaponArrCell* m_pTempCell;

	CCArray* m_stCellLst;

	int m_iIdx;
	int m_iMaxIdx;
	int m_iPreIdx;
	CMainHero* m_pstOnwerHero; ///����֮�٣���������ʾ��˭�ڻ�ǹ������˿����ǲ��ܻ�ǹ��
};

#endif // FightWeaponArrLayout_h__
