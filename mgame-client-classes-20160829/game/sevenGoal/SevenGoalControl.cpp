
#include "SevenGoalControl.h"
#include "game/ui/PageFunction.h"

SevenGoalControl::SevenGoalControl():
  mNowDay(0)
, mIsRetrived(false)
, mNowNum(0)
, mNextNum(0)
{
	mProxy = new SevenGoalProxy();
}

SevenGoalControl::~SevenGoalControl()
{

}

void SevenGoalControl::sendGetSevenGoalListRqst()
{
	mProxy->sendGetSevenGoalListRqst();
}

void SevenGoalControl::sendRetriveSevenGoalRqst(int nowDay)
{
	mProxy->sendRetriveSevenGoalRqst(nowDay);
}

void SevenGoalControl::doGetSevenGoalListRsp(const fgame::CSGetSevenGoalListResp& stMsg)
{	
	setNowDay(stMsg.nowday());				//��ǰ�ڼ���
	setIsRetrived(stMsg.isretrived());		//�����Ƿ���ȡ
	setNowNum(stMsg.num());					//��������˵ڼ���
	setNextNum(stMsg.nextnum());			//��������˼���

	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("eSevenGoalUpdate", NULL);
}

void SevenGoalControl::setSevenGoalNodeState()
{
	//�����죬���������
	if (mNowDay == 7 && mIsRetrived == true)		
	{
		return;
	}

	//�ж��ڲ���7�췶Χ�� ��������ʾͼ�꣬������ʾ
	if ((mNowDay > 0 && mNowDay <= 7))
	{
		//sgPageFunction()->gotoFunction(MSG_FUC_LUA_WIN, "SevenGoalNode.csb", 1);
	}
}

void SevenGoalControl::doRetriveSevenGoalResp(const fgame::CSRetriveSevenGoalResp& stMsg)
{
	//��������Ϣ�ͱ�����ȡ�ɹ���
}
