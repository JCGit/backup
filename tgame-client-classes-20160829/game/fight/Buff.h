#ifndef __BUFF_H__
#define __BUFF_H__

#include "Macros.pb.h"
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CBuff: public CCNode{
public:
	unsigned int m_uOwnerID;   //造成buff的对象的uid
	unsigned int m_uiBuffID;
	unsigned int m_uiBuffDeadline;
	unsigned int m_uiLastCheckTime;
	unsigned int m_uiEffType[BUFF_MAX_EFFECTS_IN_ONE];
	unsigned int m_uiVal[BUFF_MAX_EFFECTS_IN_ONE];
	unsigned int m_uiMagicType[BUFF_MAX_EFFECTS_IN_ONE];
	unsigned int m_uiAddOrDec;
	unsigned int m_uiBuffEffDisplayPos; //1 表示脚下。2表示身上，3表示头上
private:
	CCArmature* m_pstArmature;  //特效动画
public:
	//CREATE_FUNC(CBuff);

	static CBuff* create(unsigned int uID);  //造成buff的对象的m_uID

	bool initWithID(unsigned int uID);

	CBuff(){
		m_uiBuffID = 0;
		m_uiLastCheckTime = 0;
		m_uiBuffDeadline = 0;
		m_pstArmature = NULL;
		for (int i = 0; i <BUFF_MAX_EFFECTS_IN_ONE; i++){
			m_uiEffType[i] = 0;
			m_uiVal[i] = 0;
			m_uiMagicType[i] = 0;
		}
		
	}

	unsigned int getOwnerID(){
		return m_uOwnerID;
	}

	CCArmature* GetArmature(){
		return m_pstArmature;
	}


	void SetArmature(CCArmature* pstArmature){
		m_pstArmature = pstArmature;
		addChild(m_pstArmature);
	}

	void Play();

	void StopPlay(){
		if (m_pstArmature){
			m_pstArmature->getAnimation()->stop();
		}
	}

	bool IsInvincible() const{
		if (m_uiBuffID == 0){
			return false;
		}
		for (int i = 0; i <BUFF_MAX_EFFECTS_IN_ONE; i++){
			if (m_uiEffType[i] == BUFF_INVINCIBLE){
				return true;
			}else if (m_uiEffType[i] == 0){
				return false;
			}
		}

		return false;
	}

	bool IsPositiveBuff() const{
		return (m_uiAddOrDec == BUFF_ADD_EFF && m_uiBuffID != 0);
	}

	virtual void onExit(){
		this->removeAllChildrenWithCleanup(true);
		m_pstArmature = NULL;
	}

	void emergeMovementCallBack(CCArmature * armature, MovementEventType type, const char * name);
};


#endif