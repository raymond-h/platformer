#include <string>
#include <map>
#include <vector>

#include <boost/filesystem.hpp>

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "SDLUtils.h"

typedef struct {

	long x, y;
	unsigned long duration;

} Frame;

class AnimationManager {
public:
	AnimationManager(boost::filesystem::path file);

	void parseJson(std::string jsonStr);

	void update(unsigned long delta);

	void setAnimation(std::string animation);

	void reset();

	SDL_Rect getArea() { Frame f = animations[_currentAnimation][_currentFrame]; return { f.x, f.y, w, h }; };
	TexturePtr texture() { return image; };

private:
	void progressFrame();

private:
	unsigned long _counter;
	std::string _currentAnimation;
	unsigned long _currentFrame;

	unsigned long w, h;
	TexturePtr image;

	std::map< std::string, std::vector< Frame > > animations;
};

#endif /* ANIMATION_H_ */