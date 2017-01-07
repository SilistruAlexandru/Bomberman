#pragma once
#include "SFML/Graphics.hpp"
#include <cstdio>
#include <list>

using namespace std;
using namespace sf;

class Animator
{
public:
	struct Animation
	{
		string Name;
		string TextureName;
		vector<IntRect> Frames;
		Time Duration;
		bool Looped;

		Animation(string const& name, string const& textureName, Time const& duration, bool looped):
			Name(name), TextureName(textureName), Duration(duration), Looped(looped)
		{}

	//Adds frames horizontally
		void addFrames(Vector2i const& startFrom, Vector2i const& frameSize, unsigned int frames)
		{
			Vector2i current = startFrom;
			for (unsigned int i = 0; i < frames; i++)
			{
				//Add current frame from position and frame size
				Frames.push_back(IntRect(current.x, current.y, frameSize.x, frameSize.y));
				//Advance current frame horizontally
				current.x += frameSize.x;
			}
		}
	};

	Animator(Sprite& sprite);

	Animator::Animation& createAnimation(string const& name, string const& textureName, Time const& duration, bool looped = false);

	void update(Time const& dt);

	//Returns if the switch was successful
	bool switchAnimation(string const& name);

	string getCurrentAnimationName() const;

private:
	//Returns the animation with the passed name
	//Returns nullptr if no such animation is found
	Animator::Animation* findAnimation(string const& name);

	void switchAnimation(Animator::Animation* animation);

	//Reference to the sprite
	Sprite& Sprite;
	Time CurrentTime;
	list<Animator::Animation> Animations;
	Animator::Animation* CurrentAnimation;

};