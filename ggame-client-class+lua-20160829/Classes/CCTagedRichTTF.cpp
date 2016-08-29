#include "CCTagedRichTTF.h"
#include "StringConverter.h"
#include "GameMessages.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////
//CCTagedRichTTF
CCTagedRichTTF::CCTagedRichTTF(const std::string& richText, CCNode* parent)
	:m_richText(richText), m_parent(parent)
{
	buildRichText();
}

bool CCTagedRichTTF::isTagedRichText(const std::string& richText)
{
	return richText.empty() ? false : (*richText.begin() == '<');
}

void CCTagedRichTTF::buildRichText()
{
	int strategy[2] = {0, 2};
	parseAlignStrategy(strategy);
	int verSpace =  parseVerticalSpace();
	m_parent->removeAllChildrenWithCleanup(true);
	CCTagedRichTTFContainer* richContainer = CCTagedRichTTFContainer::create();
	richContainer->buildRichText(m_richText, verSpace);
	m_parent->addChild(richContainer);
	richContainer->setPositionX(-strategy[0] * richContainer->getContentSize().width/2);
	richContainer->setPositionY(richContainer->getContentSize().height - strategy[1] * richContainer->getContentSize().height / 2);
	m_parent->setContentSize(richContainer->getContentSize());
}

void CCTagedRichTTF::parseAlignStrategy(int* strategy)
{
	auto hlpos = m_richText.find("<HL>");
	auto hmpos = m_richText.find("<HM>");
	auto hrpos = m_richText.find("<HR>");
	if ((hlpos != std::string::npos) | (hmpos != std::string::npos) | (hrpos != std::string::npos))
	{
		auto tmpPos = (hlpos != std::string::npos) ? hlpos : (hmpos != std::string::npos) ? hmpos : hrpos;
		strategy[0] = (hlpos != std::string::npos) ? 0 : (hmpos != std::string::npos) ? 1 : 2;
		m_richText = m_richText.substr(tmpPos + std::strlen("<HL>"));
		auto vbpos = m_richText.find("<VB>");
		auto vmpos = m_richText.find("<VM>");
		auto vtpos = m_richText.find("<VT>");
		if ((vbpos != std::string::npos) | (vmpos != std::string::npos) | (vtpos != std::string::npos))
		{
			auto tmpPos = (vbpos != std::string::npos) ? vbpos : (vmpos != std::string::npos) ? vmpos : vtpos;
			strategy[1] = (vbpos != std::string::npos) ? 0 : (vmpos != std::string::npos) ? 1 : 2;
			m_richText = m_richText.substr(tmpPos + std::strlen("<VB>"));
		}
	}
}

int CCTagedRichTTF::parseVerticalSpace()
{
	auto pos = m_richText.find("<VS=");
	if (pos != std::string::npos)
	{
		m_richText = m_richText.substr(pos + std::strlen("<VS="));
		auto endPos = m_richText.find(">");
		std::string vsStr = m_richText.substr(0, endPos);
		m_richText = m_richText.substr(endPos+1);
		return StringConverter::parseInt(vsStr);
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//CCTagedRichTTFContainer
#define SETSAMELINE_POSITION_Y()\
for (auto inner_it = sameLineTTFs.begin(); inner_it != sameLineTTFs.end(); ++inner_it)\
{\
	auto tmpContent = *inner_it;\
	tmpContent->setPositionY(curPositionY);\
}
void CCTagedRichTTFContainer::buildRichText(const std::string& richText, int verSpace)
{
	// step1: seperate everyone richtext
	std::string tmpText = richText;
	std::vector<std::string> subRichText;
	for (auto pos = tmpText.find("</>"); pos != std::string::npos; pos = tmpText.find("</>"))
	{
		auto endPos = pos + std::strlen("</>");
		std::string substr = tmpText.substr(0, endPos);
		tmpText = tmpText.substr(endPos);
		subRichText.push_back(substr);
	}
	// step2: build each rich text
	std::vector<CCTagedRichTTFContent*> richTTFs;
	for(auto it = subRichText.begin(); it != subRichText.end(); ++it)
	{
		auto content = CCTagedRichTTFContent::create();
		content->buildRichText(*it);
		richTTFs.push_back(content);
		this->addChild(content);
	}
	// step3: set each rich position
	CCSize contentSize;
	float curPositionX = 0.0f;
	float curPositionY = 0.0f;
	float curlineMaxHeight = 0.0f;
	std::vector<CCTagedRichTTFContent*> sameLineTTFs;
	for (auto it = richTTFs.begin(); it != richTTFs.end(); ++it)
	{
		auto content = *it;
		if (content->isNewLine())
		{
			curPositionY -= curlineMaxHeight + verSpace;
			SETSAMELINE_POSITION_Y();
			contentSize.width = max(contentSize.width, curPositionX);
			sameLineTTFs.clear();
			curPositionX = 0.0f;
			curlineMaxHeight = 0.0f;
		}
		sameLineTTFs.push_back(content);
		content->setPositionX(curPositionX);
		curPositionX += content->getContentSize().width;
		curlineMaxHeight = max(curlineMaxHeight, content->getContentSize().height);
	}
	curPositionY -= curlineMaxHeight +verSpace;
	SETSAMELINE_POSITION_Y();
	contentSize.width = max(contentSize.width, curPositionX);
	contentSize.height = -curPositionY;
	this->setContentSize(contentSize);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//CCTagedRichTTFContent
void CCTagedRichTTFContent::buildRichText(const std::string& richText)
{
	parseKeyValues(richText);

	parseRichLabelInfo();

	buildLabels();
}
bool CCTagedRichTTFContent::isNewLine()
{
	return m_labelInfo.newLine;
}
void CCTagedRichTTFContent::parseKeyValues(const std::string& text)
{
	auto endPos = text.find(">");
	std::string valueStr = text.substr(1, endPos - 1);
	static const std::string keys[] = { "F=", "FS=", "B=", "C=", "U=", "N=", "L="};
	for (int i = 0; i <7; ++i)
	{
		auto pos = valueStr.find(keys[i]);
		if (pos!= std::string::npos)
		{
			std::string tmpStr = valueStr.substr(pos);
			std::string value;
			parseOneValue(tmpStr, value);
			m_keyValues.insert(make_pair(keys[i], value));
		}
	}
	std::string tmpShowStr = text.substr(endPos + 1);
	auto showStrEndpos = tmpShowStr.find("<");
	m_showString = tmpShowStr.substr(0, showStrEndpos);
}
void CCTagedRichTTFContent::parseOneValue(const std::string& valueStr, std::string& value)
{
	auto headPos = valueStr.find("'");
	std::string tmpStr2 = valueStr.substr(headPos+1);
	auto endPos = tmpStr2.find("'");
	value = tmpStr2.substr(0, endPos);
}
void CCTagedRichTTFContent::parseRichLabelInfo()
{
	if(m_keyValues.find("F=") != m_keyValues.end())
	{
		m_labelInfo.font = m_keyValues["F="];
	}
	if(m_keyValues.find("FS=") != m_keyValues.end())
	{
		m_labelInfo.fontSize = StringConverter::parseInt(m_keyValues["FS="]);
	}
	if (m_keyValues.find("B=") != m_keyValues.end())
	{
		m_labelInfo.bold = StringConverter::parseBool(m_keyValues["B="]);
	}
	if(m_keyValues.find("C=") != m_keyValues.end())
	{
		m_labelInfo.color = StringConverter::parseColor3B(m_keyValues["C="]);
	}
	if(m_keyValues.find("U=") != m_keyValues.end())
	{
		m_labelInfo.underLine = StringConverter::parseBool(m_keyValues["U="]);
	}
	if(m_keyValues.find("N=") != m_keyValues.end())
	{
		m_labelInfo.newLine = StringConverter::parseBool(m_keyValues["N="]);
	}
	if(m_keyValues.find("L=") != m_keyValues.end())
	{
		m_labelInfo.superLink = true;
		parseSuperLinkInfo(m_keyValues["L="]);
	}
}
void CCTagedRichTTFContent::buildLabels()
{
	CCLabelTTF *label = CCLabelTTF::create(m_showString.c_str(),  m_labelInfo.font.c_str(), m_labelInfo.fontSize,
		CCSizeZero , kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop, m_labelInfo.bold);
	this->setAnchorPoint(CCPointZero);
	label->setAnchorPoint(CCPointZero);
	label->setColor(m_labelInfo.color);
	this->addChild(label);
	this->setContentSize(label->getContentSize());

	if (m_labelInfo.underLine)
	{
		CCDrawNode *draw = CCDrawNode::create();
		label->addChild(draw);
		draw->drawSegment(CCPoint(0, 0), CCPoint(label->getContentSize().width, 0), 1.0f, ccc4FFromccc3B(m_labelInfo.color));
	}

	if (m_labelInfo.superLink)
	{
		CCMenu* menu = CCMenu::create();
		menu->setAnchorPoint(ccp(0,0));
		menu->setPosition(CCPointZero);
		label->addChild(menu);
		CCMenuItemFont* fon = CCMenuItemFont::create(" ", this, menu_selector(CCTagedRichTTFContent::menuCallback));
		fon->setAnchorPoint(ccp(0,0));
		fon->setContentSize(label->getContentSize());
		fon->setPosition(ccp(0, 0));
		menu->addChild(fon);
	}
}
void CCTagedRichTTFContent::menuCallback(CCObject* pSender)
{
	if (1 == m_labelInfo.linkData.type)
	{
		MsgMainFrameChangePage msg;
		msg.pageName = m_labelInfo.linkData.pageName;
		msg.needPopAllPage = true;
		MessageManager::Get()->sendMessage(&msg);
	}
	else if(2 == m_labelInfo.linkData.type)
	{
		MsgMainFramePushPage msg;
		msg.pageName = m_labelInfo.linkData.pageName;
		MessageManager::Get()->sendMessage(&msg);
	}
}
void CCTagedRichTTFContent::parseSuperLinkInfo(const std::string& text)
{
	auto pos = text.find(",");
	if (pos != std::string::npos)
	{
		auto tmpStr = text.substr(0, pos);
		m_labelInfo.linkData.type = StringConverter::parseInt(tmpStr.c_str());
		tmpStr = text.substr(pos +1);
		m_labelInfo.linkData.pageName = tmpStr;
	}
}