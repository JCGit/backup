#pragma once

#include "cocos2d.h"

USING_NS_CC;

class CCTagedRichTTF
{
public:
	CCTagedRichTTF(const std::string& richText, CCNode* parent);

	static bool isTagedRichText(const std::string& richText);

private:
	void buildRichText();
	void parseAlignStrategy(int* strategy);
	int parseVerticalSpace();

private:
	CCNode* m_parent;
	std::string m_richText;
};
class CCTagedRichTTFContainer;
class CCTagedRichTTFContainer : public CCNode
{
private:
	friend class CCTagedRichTTF;
	CREATE_FUNC(CCTagedRichTTFContainer);

	void buildRichText(const std::string& richText, int verSpace);

private:

};

struct RichLabelInfo
{
	std::string font;
	float fontSize;
	ccColor3B color;
	bool underLine;
	bool bold;
	bool newLine;
	bool superLink;
	struct SuperLinkData
	{
		int type;
		std::string pageName;
		SuperLinkData():type(0){}
	}linkData;
	RichLabelInfo():font("ºÚÌå"),fontSize(20),color(ccBLACK), underLine(false), bold(false), newLine(false), superLink(false) {}
};

class CCTagedRichTTFContent : public CCNode
{
private:
	friend class CCTagedRichTTFContainer;
	CREATE_FUNC(CCTagedRichTTFContent);

	void buildRichText(const std::string& richText);

	bool isNewLine();

private:
	void parseKeyValues(const std::string& text);
	void parseOneValue(const std::string& text, std::string& value);
	void parseRichLabelInfo();
	void buildLabels();
	void menuCallback(CCObject* pSender);
	void parseSuperLinkInfo(const std::string& text);

private:
	RichLabelInfo m_labelInfo;
	std::string m_showString;
	std::map<std::string, std::string> m_keyValues;
};