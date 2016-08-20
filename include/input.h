#ifndef __INPUT_H
#define __INPUT_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>


class Command
{
public: 
	virtual ~Command();
	virtual  void Execute() = 0;
};

class CommandJump : public Command
{
	virtual void Execute()
	{

	}

};



#endif