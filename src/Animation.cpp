#include "Animation.h"

#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>

#include <boost/filesystem/fstream.hpp>

#include <SDL2/SDL_image.h>

#include <libjson/libjson.h>

#include "Game.h"

std::string toString(std::istream& in) {
	std::string str( (std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>() );
	return str;
}

Frame parseJsonFrame(JSONNode node) {
	Frame f;
	f.x = node["x"].as_int();
	f.y = node["y"].as_int();
	f.duration = node["duration"].as_int();
	return f;
}

AnimationManager::AnimationManager(boost::filesystem::path file) : _counter(0),_currentAnimation(""),
		_currentFrame(0),w(0),h(0),image(),animations() {

	boost::filesystem::ifstream fin(file);


	parseJson( toString(fin) );
	reset();
}

void AnimationManager::parseJson(std::string jsonStr) {
	JSONNode root = libjson::parse(jsonStr);

	w = root["width"].as_int();
	h = root["height"].as_int();

	std::string imagePath = root["image"].as_string();

	SurfacePtr tmp = wrap( IMG_Load(imagePath.c_str()) );

	image = createFromSurface(Game::renderer(), tmp);

	for(JSONNode animation : root["animations"]) {
		// std::cout << "Got animation: " << animation.name() << "; " << animation.write() << std::endl;
		for(JSONNode frame : animation) {
			animations[animation.name()].push_back(parseJsonFrame(frame));
		}
	}
}

void AnimationManager::update(unsigned long delta) {
	// std::cout << "Updating with delta " << delta << std::endl;

	while(_counter < delta) {
		// std::cout << "inf loop" << std::endl;
		unsigned long oldCounter = _counter;
		progressFrame();
		delta -= oldCounter;

		if(_counter == 0) break;
	}
	_counter -= delta;
}

void AnimationManager::progressFrame() {
	const std::vector<Frame>& frames = animations[_currentAnimation];

	_currentFrame++;
	if(_currentFrame >= frames.size()) _currentFrame = 0;

	_counter = frames[_currentFrame].duration;
}

void AnimationManager::setAnimation(std::string animation) {
	if(animation != _currentAnimation) {
		_currentAnimation = animation;
		reset();
	}
}

void AnimationManager::reset() {
	_currentFrame = 0;
	_counter = animations[_currentAnimation].size() > 0 ? animations[_currentAnimation][0].duration : 0;
}