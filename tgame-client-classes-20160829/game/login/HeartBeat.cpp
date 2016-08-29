#include "HeartBeat.h"

#include "LoginControl.h"
#include "LoginProxy.h"

HeartBeat::HeartBeat()
{
}

HeartBeat::~HeartBeat()
{
}

void HeartBeat::startHeart(){
	P_LOGIN_CTRL->getProxy()->sendHeartBeatRqst();
}
