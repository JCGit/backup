#include "MaskSprite.h"

MaskSprite::MaskSprite()
:_textureLocaltion(0)
,_maskLocation(0)
,m_pMaskTexture(NULL)
,m_bInitialized(false)
{
}

MaskSprite::~MaskSprite()
{
	CC_SAFE_RELEASE(m_pMaskTexture);
}

MaskSprite* MaskSprite::create(const char *pszTexFileName, const char *pszMaskFileName) {
	MaskSprite *pRet = new MaskSprite();

	if(pRet != NULL && pRet->initWithFile(pszTexFileName, pszMaskFileName)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

MaskSprite* MaskSprite::createWithTexture(CCTexture2D *pTexture, CCTexture2D *pMask) {
	MaskSprite *pRet = new MaskSprite();

	if(pRet != NULL && pRet->initWithTexture(pTexture, pMask)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool MaskSprite::initWithFile(const char *pszTexFileName, const char *pszMaskFileName) {
	bool bRet = false;

	do {
		CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(pszTexFileName);
		CCTexture2D *pMask = CCTextureCache::sharedTextureCache()->addImage(pszMaskFileName);

		bRet = initWithTexture(pTexture, pMask);
	} while(0);

	return bRet;
}

bool MaskSprite::initWithTexture(CCTexture2D *pTexture, CCTexture2D *pMask) {
	bool bRet = false;

	do {
		if(pTexture == NULL) {
			CC_BREAK_IF(!CCSprite::initWithTexture(pTexture, CCRectZero));
		} else {
			CC_BREAK_IF(!CCSprite::initWithTexture(pTexture));
		}
		
		if(m_pMaskTexture != pMask) {
			CC_SAFE_RETAIN(pMask);
			CC_SAFE_RELEASE(m_pMaskTexture);
			m_pMaskTexture = pMask;
		}

		CCAssert(m_pMaskTexture != NULL, "mask texture must be inited");

		CCGLProgram *pShaderProgram = new CCGLProgram();
		pShaderProgram->autorelease();
		setShaderProgram(pShaderProgram);
		m_pShaderProgram->initWithVertexShaderFilename("shaders/Mask.vsh", "shaders/Mask.fsh");

		CHECK_GL_ERROR_DEBUG();

		m_pShaderProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		m_pShaderProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		m_pShaderProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

		CHECK_GL_ERROR_DEBUG();

		m_pShaderProgram->link();

		CHECK_GL_ERROR_DEBUG();

		m_pShaderProgram->updateUniforms();

		CHECK_GL_ERROR_DEBUG();

		_textureLocaltion = glGetUniformLocation(m_pShaderProgram->getProgram(), "u_texture");
		_maskLocation = glGetUniformLocation(m_pShaderProgram->getProgram(), "u_mask");

		CHECK_GL_ERROR_DEBUG();

		m_bInitialized = true;

		bRet = true;
	} while(0);

	return true;
}

void MaskSprite::draw() {
	CC_NODE_DRAW_SETUP();

	ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );

	//ccGLBindTexture2D( m_pobTexture->getName() );
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pobTexture->getName());
	glUniform1i(_textureLocaltion, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_pMaskTexture->getName());
	glUniform1i(_maskLocation, 1);

	ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );

#define kQuadSize sizeof(m_sQuad.bl)
#ifdef EMSCRIPTEN
	long offset = 0;
	setGLBufferData(&m_sQuad, 4 * kQuadSize, 0);
#else
	long offset = (long)&m_sQuad;
#endif // EMSCRIPTEN

	// vertex
	int diff = offsetof( ccV3F_C4B_T2F, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));

	// texCoods
	diff = offsetof( ccV3F_C4B_T2F, texCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// color
	diff = offsetof( ccV3F_C4B_T2F, colors);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	CHECK_GL_ERROR_DEBUG();


#if CC_SPRITE_DEBUG_DRAW == 1
	// draw bounding box
	CCPoint vertices[4]={
		ccp(m_sQuad.tl.vertices.x,m_sQuad.tl.vertices.y),
		ccp(m_sQuad.bl.vertices.x,m_sQuad.bl.vertices.y),
		ccp(m_sQuad.br.vertices.x,m_sQuad.br.vertices.y),
		ccp(m_sQuad.tr.vertices.x,m_sQuad.tr.vertices.y),
	};
	ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
	// draw texture box
	CCSize s = this->getTextureRect().size;
	CCPoint offsetPix = this->getOffsetPosition();
	CCPoint vertices[4] = {
		ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
		ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
	};
	ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW

	CC_INCREMENT_GL_DRAWS(1);
}

void MaskSprite::loadTexture(const char *pszFileName) {
	CCAssert(m_bInitialized, "not init");

	if(pszFileName == NULL && std::strcmp(pszFileName, "") == 0) {
		return;
	}

	CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(pszFileName);
	initWithTexture(pTexture, m_pMaskTexture);
}

void MaskSprite::loadTexture(CCTexture2D *pTexture) {
	CCAssert(m_bInitialized, "not init");

	if(pTexture != NULL) {
		initWithTexture(pTexture, m_pMaskTexture);
	}
}
