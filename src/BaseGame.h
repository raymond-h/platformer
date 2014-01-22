/*
 * Game.h
 *
 *  Created on: 11 feb 2013
 *      Author: Raymond
 */

#ifndef BASEGAME_H_
#define BASEGAME_H_

class BaseGame {
public:
	BaseGame();
	virtual ~BaseGame();
	
	int run(int argc, char** argv);
	
	bool isRunning() { return running; };

protected:
	virtual int init(int argc, char** argv);
	
	virtual int loop();
	
	virtual void exit(int exitCode);

protected:
	bool running;
};

#endif /* BASEGAME_H_ */
