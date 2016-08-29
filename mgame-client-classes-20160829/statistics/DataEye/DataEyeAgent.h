#pragma once

#include "cocos2d.h"
#include "Singleton.h"

class DataEyeAgent : cocos2d::Ref
{
public:

	enum AccountType
	{
		Anonymous = 0,
		Registered,
		SinaWeibo,
		QQ,
		QQWeibo,
		ND91,
		Type1,
		Type2,
		Type3,
		Type4,
		Type5,
		Type6,
		Type7,
		Type8,
		Type9,
		Type10
	};

	enum Gender
	{
		UNKNOWN = 0,
		MALE,
		FEMALE
	};

	enum TaskType
	{
		GuideLine = 1,
		MainLine,
		BranchLine,
		Daily,
		Activity,
		Other
	};

	typedef std::pair< std::string, std::string >   EventParamPair;
	typedef std::map< std::string, std::string >    EventParamMap;

	/*static const std::string Level_Id;

	static const std::string EventLevel_Id;
	static const std::string EventLevel_Key_Uid;
	static const std::string EventLevel_Key_StarCount;
	static const std::string EventLevel_Key_Result;
	static const std::string EventLevel_Key_Perfect;

	static const std::string EventBoss_Id;
	static const std::string EventBoss_Key_Unlock;
	static const std::string EventBoss_Key_Uid;
	static const std::string EventBoss_Key_Result;

	static const std::string EventChapter_Id;
	static const std::string EventChapter_Key_Unlock;
	static const std::string EventChapter_Key_Uid;

	static const std::string EventLostPoint_Id;
	static const std::string EventLostPoint_Key_Step;

	static const std::string EventRMBBuy_Id;*/

public:
	DataEyeAgent();
	~DataEyeAgent();

	bool initilized();

	/*************************************************
	* Description: �Զ���汾��
	* version    : �汾��
	*************************************************/
	void setVersion(const std::string& version);

	/*************************************************
	* Description: �Զ������ӿ�
	* title      : ������
	* error      : �������ݣ����鴫������ջ��Ϣ
	*************************************************/
	void reportError(const std::string& title, const std::string& error);

	/*************************************************
	* Description: �����ϱ����ݣ����û��ǳ�����һЩ���ݣ�ϣ��
	�������ϱ�ʱ�����Ե��øýӿ�
	*************************************************/
	void uploadNow();

	/*************************************************
	* Description: ��ȡ��ǰ�豸UID
	* return     : UID
	*************************************************/
	std::string getUID();


	//�����Ϣ�ӿ�

	/*************************************************
	* Description: ��ҵ�½����DC_AFTER_LOGINģʽ��ʹ��
	* accountId  : ����˺�ID���ò����뱣֤ȫ��Ψһ
	*************************************************/
	void accountLogin(const std::string& accountId);

	/*************************************************
	* Description: ��ҵ�½����DC_AFTER_LOGINģʽ��ʹ��
	* accountId  : ����˺�ID���ò����뱣֤ȫ��Ψһ
	* gameServer : ����˺���������
	*************************************************/
	void accountLogin(const std::string& accountId, const std::string& gameServer);

	/*************************************************
	* Description: ��ҵǳ�����DC_AFTER_LOGINģʽ��ʹ��
	*************************************************/
	void accountLogout();

	/*************************************************
	* Description: ��ȡ��ǰ�û�ID
	* return     : �û�ID
	*************************************************/
	std::string accountGetId();

	/*************************************************
	* Description: ��������˺�����
	* accountType: ����˺����ͣ�ֵΪDCAccountType�е�ö��
	*************************************************/
	void accountSetType(AccountType accountType);

	/*************************************************
	* Description: ������ҵȼ�
	* level      : ����˺ŵȼ�
	*************************************************/
	void accountSetLevel(int level);

	/*************************************************
	* Description: ��������Ա�
	* gender     : ����Ա�
	*************************************************/
	void accountSetGender(Gender gender);

	/*************************************************
	* Description: �����������
	* age        : �������
	*************************************************/
	void accountSetAge(int age);

	/*************************************************
	* Description: ���������������
	* gameServer : ����˺���������
	*************************************************/
	void accountSetGameServer(const std::string& gameServer);

	/*************************************************
	* Description: ����Ҵ��ǩ���û�SDK�ĺ������͹���
	* tag        : һ����ǩ
	* subTag     : ������ǩ
	*************************************************/
	void accountAddTag(const std::string& tag, const std::string& subTag);

	/*************************************************
	* Description: ȡ����ұ�ǩ���û�SDK�ĺ������͹���
	* tag        : һ����ǩ
	* subTag     : ������ǩ
	*************************************************/
	void accountRemoveTag(const std::string& tag, const std::string& subTag);


	//���ѽӿ�

	/*************************************************
	* Description: ���ѽӿڣ�֧��SDK���ѳɹ��ص�ʱ����
	* orderId       : ����ID
	* iapId         : ���ID
	* currencyAmount: ���ѽ��
	* currencyType  : ���ѱ���
	* paymentType   : ����;��
	*************************************************/
	void virtualCurrencyPaymentSuccess(const std::string& orderId, const std::string& iapId, double currencyAmount, const std::string& currencyType, const std::string& paymentType);

	/*************************************************
	* Description: ���ѽӿڣ����ڵ�ǰ�ϱ��ؿ��ڸ������ݣ�֧��SDK���ѳɹ��ص�ʱ����
	* orderId       : ����ID
	* iapId         : ���ID
	* currencyAmount: ���ѽ��
	* currencyType  : ���ѱ���
	* paymentType   : ����;��
	* levelId       : �ؿ�ID
	*************************************************/
	void virtualCurrencyPaymentSuccessInLevel(const std::string& orderId, const std::string& iapId, double currencyAmount, const std::string& currencyType, const std::string& paymentSuccess, const std::string& levelId);


	//���߽ӿ�

	/*************************************************
	* Description: �������
	* itemId    : ����ID
	* itemType  : ��������
	* itemCount : ����ĵ�������
	* virtualCurrency:����ĵ��ߵ������ֵ
	* currencyType:֧����ʽ
	* consumePoint:���ѵ㣬��ؿ������ѣ�����Ϊ��
	*************************************************/
	void itemBuy(const std::string& itemId, const std::string& itemType, int itemCount, long long virtualCurrency, const std::string& currencyType, const std::string& consumePoint);

	/*************************************************
	* Description: ������ߣ����ڹؿ��ڹ�������¼�
	* itemId    : ����ID
	* itemType  : ��������
	* itemCount : ����ĵ�������
	* virtualCurrency:����ĵ��ߵ������ֵ
	* currencyType:֧����ʽ
	* consumePoint:���ѵ㣬��ؿ������ѣ�����Ϊ��
	* levelId:����ʱ���ڵĹؿ�Id������Ϊ��
	*************************************************/
	void itemBuyInLevel(const std::string& itemId, const std::string& itemType, int itemCount, long long virtualCurrency, const std::string& currencyType, const std::string& consumePoint, const std::string& levelId);

	/*************************************************
	* Description: ��õ���
	* itemId    : ����ID
	* itemType  : ��������
	* itemCount : ��������
	* reason    : ��õ��ߵ�ԭ��
	*************************************************/
	void itemGet(const std::string& itemId, const std::string& itemType, int itemCount, const std::string& reason);

	/*************************************************
	* Description: ��õ��ߣ����ڹؿ��ڻ�õ����¼�
	* itemId    : ����ID
	* itemType  : ��������
	* itemCount : ��������
	* reason    : ��õ��ߵ�ԭ��
	* levelId   : ��õ���ʱ���ڵĹؿ�Id
	*************************************************/
	void itemGetInLevel(const std::string& itemId, const std::string& itemType, int itemCount, const std::string& reason, const std::string& levelId);

	/*************************************************
	* Description: ���ĵ���
	* itemId    : ����ID
	* itemType  : ��������
	* itemCount : ��������
	* reason    : ���ĵ��ߵ�ԭ��
	*************************************************/
	void itemConsume(const std::string& itemId, const std::string& itemType, int itemCount, const std::string& reason);

	/*************************************************
	* Description: ���ĵ��ߣ����ڹؿ������ĵ����¼�
	* itemId    : ����ID
	* itemType  : ��������
	* itemCount : ��������
	* reason    : ���ĵ��ߵ�ԭ��
	* levelId   : �ؿ������ĵ��ߵ�ԭ��
	*************************************************/
	void itemConsumeInLevel(const std::string& itemId, const std::string& itemType, int itemCount, const std::string& reason, const std::string& levelId);


	//����ӿ�

	/*************************************************
	* Description: ��ʼ����
	* taskId     : ����ID
	* taskType   : ��������
	*************************************************/
	void taskBegin(const std::string& taskId, TaskType type);

	/*************************************************
	* Description: �������
	* taskId     : ����ID
	*************************************************/
	void taskComplete(const std::string& taskId);

	/*************************************************
	* Description: ����ʧ��
	* taskId     : ����ID
	* reason     : ����ʧ��ԭ��
	*************************************************/
	void taskFail(const std::string& taskId, const std::string& reason);


	//�Զ����¼�

	/*************************************************
	* Description: ��½ǰ�Զ����¼����û���½֮��ýӿ���Ч
	* eventId    : �¼�ID
	* map        : �¼�����ʱ��ע������map
	* duration   : �¼�����ʱ��
	*************************************************/
	void onEventBeforeLogin(const std::string& eventId, EventParamMap* map, long long duration);

	/*************************************************
	* Description: �¼�����
	* eventId    : �¼�ID
	* count      : �¼���������
	*************************************************/
	void onEventCount(const std::string& eventId, int count);

	/*************************************************
	* Description: �¼�����
	* eventId    : �¼�ID
	*************************************************/
	void onEvent(const std::string& eventId);

	/*************************************************
	* Description: �¼�����
	* eventId    : �¼�ID
	* label      : �¼�����ʱ��ע��һ������
	*************************************************/
	void onEvent(const std::string& eventId, const std::string& label);

	/*************************************************
	* Description: �¼�����
	* eventId    : �¼�ID
	* label      : �¼�����ʱ��ע�Ķ������map
	*************************************************/
	void onEvent(const std::string& eventId, EventParamMap* map);

	/*************************************************
	* Description: ʱ���¼�����
	* eventId    : �¼�ID
	* duration   : �¼�����ʱ��
	*************************************************/
	void onEventDuration(const std::string& eventId, long long duration);

	/*************************************************
	* Description: ʱ���¼�����
	* eventId    : �¼�ID
	* label      : �¼�����ʱ��ע�ĵ�������
	* duration   : �¼�����ʱ��
	*************************************************/
	void onEventDuration(const std::string& eventId, const std::string& label, long long duration);

	/*************************************************
	* Description: ʱ���¼�����
	* eventId    : �¼�ID
	* map        : �¼�����ʱ��ע�Ķ������
	* duration   : �¼�����ʱ��
	*************************************************/
	void onEventDuration(const std::string& eventId, EventParamMap* map, long long duration);

	/*************************************************
	* Description: �������¼���ʼ����OnEventEnd(eventId)���ʹ��
	* eventId    : �¼�ID
	*************************************************/
	void onEventBegin(const std::string& eventId);

	/*************************************************
	* Description: �������¼���ʼ����OnEventEnd(eventId)���ʹ��
	* eventId    : �¼�ID
	* map        : �¼�����ʱ��ע�Ķ������
	*************************************************/
	void onEventBegin(const std::string& eventId, EventParamMap* map);

	/*************************************************
	* Description: �������¼���������OnEventBegin(eventId)��OnEventBegin(eventId, map)���ʹ��
	* eventId    : �¼�ID
	*************************************************/
	void onEventEnd(const std::string& eventId);

	/*************************************************
	* Description: �������¼���ʼ����OnEventEnd(eventId, flag)���ʹ��
	* eventId    : �¼�ID
	* map        : �¼�����ʱ��ע�Ķ������
	* flag       : eventId��һ����ʶ����eventId��ͬ����һ���¼���������������¼���
	eventId����Ϊ���������flag��Ϊ��Ҿ���ĵȼ�
	*************************************************/
	void onEventBegin(const std::string& eventId, EventParamMap* map, const std::string& flag);

	/*************************************************
	* Description: �������¼���������OnEventBegin(eventId, map, flag)���ʹ��
	* eventId    : �¼�ID
	* flag       : eventId��һ����ʶ����eventId��ͬ����һ���¼���������������¼���
	eventId����Ϊ���������flag��Ϊ��Ҿ���ĵȼ�
	*************************************************/
	void onEventEnd(const std::string& eventId, const std::string& flag);


	//����ҽӿ�

	/*************************************************
	* Description: �������������
	* coinNum    : ���������
	* coinType   : ���������
	*************************************************/
	void coinSetCoinNum(long long total, const std::string& coinType);

	/*************************************************
	* Description: ���������
	* id         : ���������ʱ��ע�����ԣ�������ԭ��
	* coinType   : ���������
	* lost       : ��������ҵ�����
	* left       : ���ʣ����������
	*************************************************/
	void coinLost(const std::string& reason, const std::string& coinType, long long lost, long long left);

	/*************************************************
	* Description: ��������ң����ڹؿ��������¼�
	* id         : ���������ʱ��ע�����ԣ�������ԭ��
	* coinType   : ���������
	* lost       : ��������ҵ�����
	* left       : ���ʣ����������
	* levelId    : ��ҵ�ǰ���ڵĹؿ�ID
	*************************************************/
	void coinLostInLevel(const std::string& reason, const std::string& coinType, long long lost, long long left, const std::string& levelId);

	/*************************************************
	* Description: ���������
	* id         : ���������ʱ��ע�����ԣ�������ԭ��
	* coinType   : ���������
	* gain       : ��������ҵ�����
	* left       : ���ʣ����������
	*************************************************/
	void coinGain(const std::string& reason, const std::string& coinType, long long gain, long long left);

	/*************************************************
	* Description: �������ң����ڹؿ��ڻ���¼�
	* id         : ��������ʱ��ע�����ԣ�������ԭ��
	* coinType   : ���������
	* gain       : �������ҵ�����
	* left       : ���ʣ����������
	* levelId    : ��ҵ�ǰ���ڵĹؿ�ID
	*************************************************/
	void coinGainInLevel(const std::string& reason, const std::string& coinType, long long gain, long long left, const std::string& levelId);

	//�ؿ��ӿ�

	/*************************************************
	* Description: ��ʼ�ؿ�
	* levelId    : �ؿ�ID
	*************************************************/
	void levelsBegin(const std::string& levelId);

	/*************************************************
	* Description: �ɹ���ɹؿ�
	* levelId    : �ؿ�ID
	*************************************************/
	void levelsComplete(const std::string& levelId);

	/*************************************************
	* Description: �ؿ�ʧ�ܣ���ҹؿ����˳���Ϸʱ��������øýӿ�
	* levelId    : �ؿ�ID
	* failPoint  : ʧ��ԭ��
	*************************************************/
	void levelsFail(const std::string& levelId, const std::string& failPoint);



	/**
	*  @brief ��ҷ������һ����Ϸ�����
	*  @brief roomId ����ID
	*  @brief lostOrGainCoin    ��û��߶�ʧ�����������������Ϊ�㣩
	*  @brief roomType ��������
	*  @param taxCoin  ���һ����Ϸʱϵͳ��Ҫ���յ������������˰�գ�
	*  @param leftCoin   ���ʣ������������
	*/
	void cardsGamePlay(const std::string& roomId, const std::string& roomType, const std::string& coinType, long long lostOrGainCoin, long long taxCoin, long long leftCoin);

	/**
	*  @brief ��ҷ����ڶ�ʧ�����ʱ���ã����һ����Ϸ����play�ӿں󲻱��ٵ��øýӿڣ�
	*  @brief roomId ����ID
	*  @brief roomType ��������
	*  @param lostCoin  ��ʧ�����������
	*  @param leftCoin   ʣ������������
	*/
	void cardsGameLost(const std::string& roomId, const std::string& roomType, const std::string& coinType, long long lostCoin, long long leftCoin);

	/**
	*  @brief ��ҷ����ڻ�������ʱ���ã����һ����Ϸ����play�ӿں󲻱��ٵ��øýӿڣ�
	*  @brief roomId ����ID
	*  @brief roomType ��������
	*  @param gainCoin   Ӯ�õ����������
	*  @param leftCoin  ʣ������������
	*/
	void cardsGameGain(const std::string& roomId, const std::string& roomType, const std::string& coinType, long long gainCoin, long long leftCoin);


private:
	bool mEnable;
};

#define sgDataEyeAgent() Singleton<DataEyeAgent>::Get()