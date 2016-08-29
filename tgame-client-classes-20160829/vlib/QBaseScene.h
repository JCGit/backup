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
    void loadRes(); //加载场景所需的资源文件
    void onEnter(){}; //进入场景
    void onExit(){};  //切换出场景
    void onRelease(){};//释放场景
protected:
    //
private:
    //

};



#endif