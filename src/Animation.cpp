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

AnimationManager::AnimationManager(boost::filesystem::path file):w(0),h(0) {
	boost::filesystem::ifstream fin(file);

	parseJson( toString(fin) );
}

void AnimationManager::parseJson(std::string jsonStr) {
	JSONNode root = libjson::parse(jsonStr);

	w = root["width"].as_int();
	h = root["height"].as_int();

	std::string imagePath = root["image"].as_string();
	SurfacePtr tmp = wrap( IMG_Load(imagePath.c_str()) );
	image = createFromSurface(Game::renderer(), tmp);

	for(JSONNode animation : root["animations"]) {
		std::cout << "Got animation: " << animation.name() << "; " << animation.write() << std::endl;
	}
}