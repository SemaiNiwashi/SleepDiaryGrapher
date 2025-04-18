#pragma once
#include <string> // must be first
#include <iostream>
#include <SFML\OpenGL.hpp>
#include <SFML\System\Clock.hpp>
#include <SFML\System\Time.hpp>

//sfml::Clock clock;
//Time time1 = clock.getElapsedTime();

/*
 * timer class
 * - this lets us keep track of the elapsed time
 *   in our program easily so we can use it in things
 *   like physics calculations 
 */
class Timer
{
public:
	sf::Time *myTime;
	Timer(std::string _name)
	{
		myTime = new sf::Time();
		previousTime = float(myClock.getElapsedTime().asMilliseconds());
		this->tick();
		name = _name;
	}
	
	~Timer(void);

	// update the timer clock
	float tick()
	{
		currentTime = float(myClock.getElapsedTime().asMilliseconds());
		elapsedTime = currentTime - previousTime;
		previousTime = currentTime;
		return elapsedTime;
	}

	// delta time in milliseconds 
	float getElapsedTimeMS()
	{
		return elapsedTime;
	}

	// delta time in seconds
	float getElapsedTimeSeconds()
	{
		return (elapsedTime/1000.f);
	}

	float getCurrentTime()
	{
		return currentTime;
	}
	void print()
	{
		std::cout << name << ":" << elapsedTime<<std::endl;
	}

private:
	sf::Clock myClock;
	float currentTime, previousTime, elapsedTime;
	std::string name;
};
