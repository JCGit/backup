#include "CityObj.h"

CityObj::CityObj()
:m_uArriveNPCID(0)
{
}

CityObj::~CityObj()
{
}

const unsigned int CityObj::getArriveNPCID() {
	return m_uArriveNPCID;
}

void CityObj::setArriveNPCID(const unsigned int uArriveNPCID) {
	m_uArriveNPCID = uArriveNPCID;
}
