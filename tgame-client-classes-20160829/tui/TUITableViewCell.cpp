#include "TUITableViewCell.h"

void TUITableViewCell::reset()
{
	m_uIdx = CC_INVALID_INDEX;
}

void TUITableViewCell::setObjectID(unsigned int uIdx)
{
	m_uIdx = uIdx;
}

unsigned int TUITableViewCell::getObjectID()
{
	return m_uIdx;
}

unsigned int TUITableViewCell::getIdx()
{
	return m_uIdx;
}

void TUITableViewCell::setIdx(unsigned int uIdx)
{
	m_uIdx = uIdx;
}