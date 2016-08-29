/*68888886866668688888886866686
68666666866668686666666866866
68666666866668686666666868666
68888666866668686666666886666
68666666866668686666666868666
68666666866668686666666866866
68666666888888688888886866686
66666666666666666666666666666
68666668688888686668666666666
66866686686668686668666666666
66686866686668686668666666666
66668666686668686668666666666
66668666686668686668666666666
66668666688888688888666666666
/**
* @file     BuffManager.h
* @brief    buff������
* @details  
* @author   Alvin
* @date     2015-8-26
*/
#ifndef __BUFFMANAGER_H__
#define __BUFFMANAGER_H__
#include "data/Tables.h"
#include "data/TableManager.h"
#include "Message.h" 
enum eTargetType
{
	E_TARGET_CAMP = 0, //ȫ��Ӫ����
	E_TARGET_BUILD = 1 //�ض���������
};

class BuffNode
{
public:
	BuffNode();
	void resetValue();
	float attackSpeed;
	float moveSpeed;
	float attack;
	float defence;
	float productSpeed;
	bool  peopleFull;

	float attackSpeedRate;
	float moveSpeedRate;
	float attackRate;
	float defenceRate;
	float productSpeedRate;
};
 
typedef std::map<int, int> BuffIDMap;
class BuffManager: public MessageHandler
{
public:
	BuffManager();  
	void clearBuff();
	void update(float deleta);
	void onReceiveMassage(GameMessage * message);  

	
	const BuffIDMap* getBuffListByCamp(eCampType camp);

	float getBuffAttackSpeed(eCampType camp, float baseValue);
	float getBuffMoveSpeed(eCampType camp, float baseValue);
	float getBuffAttack(eCampType camp, float baseValue);
	float getBuffDefence(eCampType camp, float baseValue);
	float getBuffProduceSpeed(eCampType camp, float baseValue);
	void  setCampPeopleToFull(eCampType camp, int buffID); 
	bool  getExistBuff(eCampType camp, int buffID); 
private:
	//����buff
	void addBuff(eCampType camp, int buffID);

	//��¼��ǰbuff
	void recordBuff(eCampType camp, int buffID);

	//�Ƴ�Buff
	void removeBuff(eCampType camp, int buffID); 

	//������Ӫbuff��ֵ
	void addCampBuff(eCampType camp, int buffID, int addType);  

	void initCampBuffValue(BuffNode* buffNode);
	void updateCampBuffValue(eCampType camp, BuffNode* buffNode, int buffID,  int addType);
	void resetCampBuff(eCampType camp);
	
	float getAddBuffValue(float srcValue, float addValue, int addType);
	float getResetBuffValue(float setValue);

	//����buff��ʱ����������ڸü�ʱ��������ʱ��
	void createBuffTimer(eCampType camp, int buffID, float time);
	//����buff��ʱ��
	void createBuffTimer(eCampType camp, int keyID, int buffID, float time);
	//�Ƴ�buff��ʱ��
	void removeBuffTimer(eCampType camp, int keyID, int buffID);

	//��ѯBuff�Ƿ���Ч 
	bool checkBuffActive(eCampType camp, int keyID, int buffID);

	//��ʾBuffЧ��
	void showBuffEffect(eCampType camp, int buffID, int buffSrc);

	void resetBuff(eCampType camp, int keyID, int buffID);

	 
	//��ѯBuff�Ƿ����
	bool checkBuffIDExist(eCampType type, int buffID);

	//�Ƴ���ǰ����BUFF
	void removeCurBuff(eCampType type);
	void removeCurBuffView(eCampType type);
	 
	typedef std::map<eCampType, BuffIDMap*> CampBuffListMap; 
	CampBuffListMap m_CampBuffListMap;//���ڱ��������Ӫӵ�е�buffID�б�

	

	typedef std::map<eCampType, BuffNode*> CampBuffValueMap; 
	CampBuffValueMap m_CampBuffValueMap;//���ڱ��������Ӫӵ�е�buffЧ��

	static int mBuffKey;
	int createBuffKey();
	 
};

 #define sgBuffManager() Singleton<BuffManager>::Get()
 
#endif