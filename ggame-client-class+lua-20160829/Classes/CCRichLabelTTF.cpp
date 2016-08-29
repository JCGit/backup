#include "CCRichLabelTTF.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//CCRichLabelTTF
CCRichLabelTTF::CCRichLabelTTF()
	:m_splitInfos(NULL), m_splitSize(0), m_messageHandler(NULL)
{

}
CCRichLabelTTF::~CCRichLabelTTF()
{
	for (auto it = m_itemInfo.begin(); it != m_itemInfo.end(); ++it)
	{
		it->first->release();
	}
	m_itemInfo.clear();
	for (auto it = m_LabelsInfo.begin(); it != m_LabelsInfo.end(); ++it)
	{
		it->second->release();
	}
	m_LabelsInfo.clear();

	delete[] m_splitInfos;
}

CCRichLabelTTF * CCRichLabelTTF::create(void)
{
	CCRichLabelTTF * pRet = new CCRichLabelTTF();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

void CCRichLabelTTF::splitString(const std::string& src_string, split_info splitInfos[], unsigned int size)
{
	if (m_splitInfos || !splitInfos)
		return;
	
	float offset_x_pos = 0.0f;

	m_splitSize = size;
	m_splitInfos = new split_info[size];
	for (unsigned int i = 0; i < size; ++i)
	{
		m_splitInfos[i] = splitInfos[i];
		createLabelTTF(src_string, i, offset_x_pos);
	}
}
void CCRichLabelTTF::createLabelTTF(const std::string& src_string, unsigned int index, float& offset_x_pos)
{
	const split_info& splitInfo = m_splitInfos[index];

	std::string real_show = src_string.substr(splitInfo.begin_pos, splitInfo.end_pos - splitInfo.begin_pos);

	CCLabelTTF *label = CCLabelTTF::create(real_show.c_str(), splitInfo.font_name.c_str(), splitInfo.font_size, CCSizeZero ,
							kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop, splitInfo.is_bold, false/*splitInfo.is_under_line*/);

	label->setAnchorPoint(ccp(0,0));
	label->setPosition(ccp(offset_x_pos, 0));
	label->setColor(splitInfo.color);
	
	if (splitInfo.is_under_line)
	{
		CCDrawNode *draw = CCDrawNode::create();
		label->addChild(draw);
		draw->drawSegment(CCPoint(0, 0), CCPoint(label->getContentSize().width, 0), 1.0f, ccc4FFromccc3B(splitInfo.color));
	}

	this->addChild(label);
	
	label->retain();
	m_LabelsInfo.insert(std::pair<unsigned int, CCLabelTTF*>(index, label));

	//if (splitInfo.goto_page != NULL)
	{
		CCMenu* menu = CCMenu::create();
		menu->setAnchorPoint(ccp(0,0));
		menu->setPosition(CCPointZero);
		label->addChild(menu);
		CCMenuItemFont* fon = CCMenuItemFont::create(" ", this, menu_selector(CCRichLabelTTF::menuCallback));
		fon->setAnchorPoint(ccp(0,0));
		fon->setContentSize(label->getContentSize());
		fon->setPosition(ccp(0, 0));
		menu->addChild(fon);

		fon->retain();
		m_itemInfo.insert(std::pair<CCObject*, unsigned int>(fon, index));
	}

	offset_x_pos += label->getContentSize().width;
}
void CCRichLabelTTF::menuCallback(CCObject* pSender)
{
	auto find_it = m_itemInfo.find(pSender);
	if (find_it != m_itemInfo.end())
	{
		unsigned int index = find_it->second;
		if (index >=0 && index < m_splitSize)
		{
			onMenuItemClicked(index);
		}
	}
}
void CCRichLabelTTF::setColor(const ccColor3B& color)
{
	for (auto it = m_LabelsInfo.begin(); it != m_LabelsInfo.end(); ++it)
	{
		it->second->setColor(color);
	}
}
void CCRichLabelTTF::setColor(int index, const ccColor3B& color)
{
	auto find_it = m_LabelsInfo.find(index);
	if (find_it != m_LabelsInfo.end())
	{
		find_it->second->setColor(color);
	}
}

void CCRichLabelTTF::onMenuItemClicked(int index)
{
	const split_info& info = m_splitInfos[index];
	if (m_messageHandler)
	{
		m_messageHandler->onReceiveMassage(index, info);
	}
}
float CCRichLabelTTF::getContentHeight()
{
	float height = 0.0f;
	for (auto it = m_LabelsInfo.begin(); it != m_LabelsInfo.end(); ++it)
	{
		float tmp_height = it->second->getContentSize().height;
		if (tmp_height > height)
		{
			height = tmp_height;
		}
	}
	return height;
}
float CCRichLabelTTF::getContentWidth()
{
	float width = 0.0f;
	for (auto it = m_LabelsInfo.begin(); it != m_LabelsInfo.end(); ++it)
	{
		width += it->second->getContentSize().width;
	}
	return width;
}