#ifndef _GLYPH_H
#define _GLYPH_H

#include "cocos2d.h"
#include "Constant.h"


class GlyphBaseData;

class Glyph: public cocos2d::Ref {
public: 
	enum eGlyphType
	{
		E_GLYPH_MIN,

		//��������ID
		E_GLYPH_ATK_BEG = 100,

		E_GLYPH_ATK_POWER = 101,				//���� ���ӹ���������%d
		E_GLYPH_ATK_INSTITUTE = 102,			//���� ÿ���о������������%d
		E_GLYPH_ATK_TIME = 103,					//���� ÿ%d�빥��������%d%
		E_GLYPH_ATK_TURRET = 104,				//���� ������������%d%%
		E_GLYPH_ATK_KILL = 105,					//---- 
		
		E_GLYPH_ATK_END,

		//��������ID
		E_GLYPH_DEF_BEG = 200,

		E_GLYPH_DEF_DEFENSE = 201,				//���� ���ӷ���������%d
		E_GLYPH_DEF_INSTITUTE = 202,			//���� ÿ���о������������%d%%
		E_GLYPH_DEF_TIME = 203,					//���� ÿ%d���������%d
		E_GLYPH_DEF_DEAD = 204,					//---- 

		E_GLYPH_DEF_END,

		//�ٶȵ���ID
		E_GLYPH_SPD_BEG = 300,

		E_GLYPH_SPD_MOVE = 301,					//���� �����ƶ��ٶ�����%d
		E_GLYPH_SPD_UP = 302,					//���� ���������ٶ�����%d%%
		E_GLYPH_SPD_PRODUCE = 303,				//ļ�� ��Ӫ�����ٶ�����%d%%
		E_GLYPH_SPD_CREEPS = 304,				//���� ��Ұ���ƶ��ٶ�����%d%%
		E_GLYPH_SPD_ASSAULT = 305,				//��Ϯ ��з���������%d�Ľ����ƶ�ʱ���ƶ��ٶ�����%d%%
		E_GLYPH_SPD_ASSIST = 306,				//��Ԯ ���ѷ���������%d�Ľ����ƶ�ʱ���ƶ��ٶ�����%d%%

		E_GLYPH_SPD_END,

		//��������ID
		E_GLYPH_PPL_BEG = 400,

		E_GLYPH_PPL_INIT = 401,					//֧Ԯ ��ʼ���%d��������������%d��
		E_GLYPH_PPL_MAX = 402,					//ǿ�� ����������������%d
		E_GLYPH_PPL_HOLDLIMIT = 403,			//��Ӫ ��ǵ���������N
		E_GLYPH_PPL_UP = 404,					//�ܽ� �����ظ�%d��С��
		E_GLYPH_PPL_PRODUCE = 405,				//���� ÿ%d������%d��С��(ÿ�β�����%d%%����%d��С��)
		E_GLYPH_PPL_OCCUPY = 406,				//���� ռ�콨���󣬸ý����ظ�%d����
		
		E_GLYPH_PPL_END,

		//ʿ������ID
		E_GLYPH_MRL_BEG = 500,

		E_GLYPH_MRL_ADD = 501,					//���� ÿ�λ�ȡʿ������%d%%
		E_GLYPH_MRL_DEC = 502,					//���� ʿ��˥������%d%%
		E_GLYPH_MRL_TIME = 503,					//�Ⱦ� ÿ%d��ʿ������%d��

		E_GLYPH_MRL_END,

		E_GLYPH_MAX,
	};

public:
	static Glyph* create(eGlyphType type, GlyphBaseData* data);

	eGlyphType getType();
    GlyphBaseData* getData();

	bool isSpiritAffected() const;
	float getDataMainAttr() const;
	float getDataViceAttr() const;
	bool isAffectedByBuildLevel() const;
	float getDataBuildAffectAttr(int buildLevel) const;
	std::string getDataBattleTip() const;

	float getValue() const;
	float getValueAsAddition() const{ return mValue; }
	float getValueAsPercent() const { return mValue / 100.0f; }

	void onSpiritChange(float spirit);
	void onIntervalTimeout(eCampType campType);

protected:
	Glyph();
	virtual ~Glyph();

	bool init(eGlyphType type, GlyphBaseData* data);

protected: 
	eGlyphType mType;
    GlyphBaseData* mData;

	mutable float mValue;
};

#endif //_GLYPH_H