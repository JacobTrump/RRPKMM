﻿#include "AnimatePacker.h"

using namespace std;
using namespace cocos2d;

class AnimateSaxDelegator : public CCSAXDelegator {
public:
    enum {
        STATE_NONE,
        STATE_PLIST,
        STATE_ANIMATION,
        STATE_NAME,
        STATE_DELAY,
        STATE_FLIP_X,
        STATE_FLIP_Y,
        STATE_SPRITE_FRAME
    } state;

    void startElement(void *ctx, const char *name, const char **atts) ;
    void endElement(void *ctx, const char *name) ;
    void textHandler(void *ctx, const char *s, int len) ;

    vector<string> plists;//All plist name
    vector<Animate> animates;//All animate data
};

void AnimateSaxDelegator::startElement( void *ctx, const char *name, const char **atts ) {
    string tag((char*)name);

    if (tag == "plist") {
        state = STATE_PLIST;
    } else if (tag == "animation") {
        state = STATE_ANIMATION;
        animates.push_back(Animate());
    } else if (tag == "name") {
        state = STATE_NAME;
    } else if (tag == "delay") {
        state = STATE_DELAY;
    } else if (tag == "spriteFrame") {
        state = STATE_SPRITE_FRAME;
    } else if (tag == "flipX") {
        state = STATE_FLIP_X;
    } else if (tag == "flipY") {
        state = STATE_FLIP_Y;
    } else {
        state = STATE_NONE;
    }
}

void AnimateSaxDelegator::endElement( void *ctx, const char *name ) {
    string tag((char*)name);

    if (tag == "plist") {
    } else if (tag == "animation") {
    } else if (tag == "name") {
    } else if (tag == "delay") {
    } else if (tag == "spriteFrame") {
    } else if (tag == "flipX") {
    } else if (tag == "flipY") {
    } else {
    }

    state = STATE_NONE;
}

void AnimateSaxDelegator::textHandler( void *ctx, const char *ch, int len ) {
    if (state == STATE_NONE) {
        return;
    }

    string text((char*)ch, 0, len);
    int index;
    float delay;

    switch (state) {
    case STATE_PLIST:
        plists.push_back(text);
        break;
    case STATE_ANIMATION:
        break;
    case STATE_NAME:
        index = animates.size() - 1;
        animates[index].name = text;
        break;
    case STATE_DELAY:
        index = animates.size() - 1;
        delay = atof(text.c_str());
        animates[index].delay = delay;
        break;
    case STATE_SPRITE_FRAME:
        index = animates.size() - 1;
        animates[index].spriteFrames.push_back(text);
        break;
    case STATE_FLIP_X:
        index = animates.size() - 1;
        animates[index].flipX = (text == "true");
        break;
    case STATE_FLIP_Y:
        index = animates.size() - 1;
        animates[index].flipY = (text == "true");
        break;
    default:
        break;
    }

}

void AnimatePacker::loadAnimations(const char *path ) {
//	const char *pszPath = CCFileUtils::fullPathFromRelativePath(path);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
   // std::string sk = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(path,"");
   std::string sk = CCFileUtils::sharedFileUtils()->fullPathForFilename(path);
#else
    std::string sk = CCFileUtils::sharedFileUtils()->fullPathForFilename(path);
#endif
	const char *pszPath = sk.c_str();
    CCSAXParser parser;
    AnimateSaxDelegator delegator;

    if (false == parser.init("UTF-8")) {
        //TODO
        return;
    }
    parser.setDelegator(&delegator);
    parser.parse(pszPath);

    //load plist
    vector<string> plists = delegator.plists;
    for (unsigned int i = 0; i < plists.size(); i++) {
//	    string plistPath = CCFileUtils::fullPathFromRelativeFile(plists[i].c_str(), pszPath);
        string plistPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(plists[i].c_str(), pszPath);
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistPath.c_str());
    }

    //load animate
    vector<Animate> animates = delegator.animates;
    CCArray *spriteFramesArray = CCArray::create();

    for (unsigned int i = 0; i < animates.size(); i++) {
        Animate animate = animates[i];
        vector<string> spriteFrames = animate.spriteFrames;

        for (unsigned int j = 0; j < spriteFrames.size(); j++) {
            CCSpriteFrame *spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteFrames[j].c_str());
            spriteFramesArray->addObject(spriteFrame);
        }

        CCAnimation *animation = CCAnimation::createWithSpriteFrames(spriteFramesArray, animate.delay);
        CCAnimationCache::sharedAnimationCache()->addAnimation(animation, animate.name.c_str());

        spriteFramesArray->removeAllObjects();
    }
}

CCAnimate* AnimatePacker::getAnimate(const char *name ) {
    CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName(name);

    if(animation) {
        return CCAnimate::create(animation);
    }
    return NULL;
}