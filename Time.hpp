#ifndef TIME_H
#define TIME_H

#include <sfml/system.hpp>

class Time
{
	public:
		Time();
		~Time();

		static void update();
		static float now();
		static float step();

		static sf::Clock clock;
		static float currentTime;
		static float deltaTime;

};


#endif // TIME_H
