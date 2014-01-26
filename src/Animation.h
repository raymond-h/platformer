#include <string>

#include <boost/filesystem.hpp>

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "util/SDLUtils.h"

typedef struct {

	long x, y;
	unsigned long duration;

} Frame;

class AnimationManager {
public:
	AnimationManager(boost::filesystem::path file);

	void parseJson(std::string jsonStr);

	void update(unsigned long delta);

private:
	unsigned long _counter;

	unsigned long w, h;
	TexturePtr image;
};

#endif /* ANIMATION_H_ */