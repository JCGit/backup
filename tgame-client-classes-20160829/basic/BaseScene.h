#ifndef BaseScene_h__
#define BaseScene_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Resources.h"
#include "SimpleAudioEngine.h"
#include "ClientSvrMsg.pb.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

using namespace std;

typedef enum {
	LAYER_ID_GAME, // ��Ϸ�����
	LAYER_ID_WIDGET, // ����С�����㣬�絥���İ�ť��ͷ���
	LAYER_ID_HUD, // ҡ�˿��Ʋ�
	LAYER_ID_FIGHT,	//ս�������еİ�ť��
	LAYER_ID_UI, // UI�㣬������
	LAYER_ID_POP, // �����㣬tips�ͶԻ���
	LAYER_ID_TOP, // ���ϲ㣬��ռ�����Ʋ���
} ELayerID;

class BaseScene: public CCScene
{
public:
	BaseScene();
	virtual ~BaseScene();

	// override start

    virtual bool init(); // ��ʼ��
    virtual void onExit();

	// override end

	void addLayer(ELayerID eLayerID);		// ���layer�������У������ظ����

	void removeLayer(ELayerID eLayerID);	// �Ƴ������е�layer�����ڲŻ��Ƴ�

	CCLayer* getLayer(ELayerID eLayerID); // ��ȡ�����е�layer��û�з���null

	int getLayerZOrder(ELayerID eLayerID); // ��ȡlayer�ڳ����е�ZOrder

	void loadCfg(unsigned int uSceneID);    // ���س�������

	unsigned int getSceneID() const;

	unsigned int getSceneType() const;

    virtual void loadRes(); // ������Դ
    virtual bool loadNextRes() {return true;}

	virtual void build(); // ��������

	const set<string>& getResList(){         //��ȡ��������Դ�б�
		return m_stRes;
	}

	const set<string>& getEffSoundList(){
		return m_stEffSoudnRes;
	}

	virtual void cleanRes(); // ������Դ

	virtual void addResToList(string str);	//��ӵ���Դ(�Ѿ��ڳ����е���Դ���أ���Ԥ����)

	virtual void addEffSound(string str);	//������Ч��Դ

	void cleanUI();

    void disableHudLayer();
private:
	CCLayer* createLayer(ELayerID eLayerID); // ����ָ����layer

protected:
	unsigned int m_uSceneID; // ����ID������Ψһ

	unsigned int m_uSceneType; // ��������

	string m_strSceneName; // ��������

    set<string> m_stRes;	//��Դ�б�

	set<string> m_stEffSoudnRes;		//��Ч�б�
};

#endif // BaseScene_h__
