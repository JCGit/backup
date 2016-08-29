#ifndef MovieView_h__
#define MovieView_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include <vector>
using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

//����������Ϣ
struct MovieInfo {
    int iNextIdx; //�ò����ĸ�������
    vector<string> stAniLst; //�����б�
};


typedef void (CCObject::*SEL_MovieDoneCallFunc)();
#define onMovieDone_selector(_SELECTOR) (SEL_MovieDoneCallFunc)(&_SELECTOR)

class MovieView: public UILayout
{
public:
	enum {
        MOVIE_ENTER_GAME = 1, //1.�����ʺŽ�����Ϸ���֡�
    };
public:
	MovieView();
	virtual ~MovieView();
	
	static MovieView* create();

	// override start

	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool hitTest(const CCPoint &pt);

	// override end

	void initUI();
    

    void playMovie(const int iMovieType, CCObject* pTargetObj = NULL, SEL_MovieDoneCallFunc pCallbackFunc = NULL );
private:
    void onOneFrameDone(CCArmature * armature, MovementEventType type, const char * name);
    void onFrameDelayDone(float dt);
    void playNextFrame();
    void showEndTip();
    void endPlay();
    void initArmature();
private:
	CCLayerColor *m_pDarkLayer;

    UIImageView* m_pEndTip;
    CCArmature* m_pArmature; //����
    MovieInfo m_stNowMovieInfo;

    CCObject* m_pTargetObj;
    SEL_MovieDoneCallFunc m_pCallbackFunc;

    string m_stSceneBgSound;
};


#endif // MovieView_h__