#ifndef __QUIBUTTON_H__
#define __QUIBUTTON_H__
#include "cocos2d.h"
USING_NS_CC;
class QBaseScene : public CCScene
{
public:
    QBaseScene(){};
    virtual ~QBaseScene(){}; 

    QBaseScene* create()
    {
        QBaseScene *pRet = new QBaseScene();
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            CC_SAFE_DELETE(pRet);
            return NULL;
        }
    };

    virtual bool init(){return CCScene::init();};
    void loadRes(); //���س����������Դ�ļ�
    void onEnter(){}; //���볡��
    void onExit(){};  //�л�������
    void onRelease(){};//�ͷų���
protected:
    //
private:
    //

};



#endif