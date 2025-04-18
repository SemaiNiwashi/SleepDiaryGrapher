#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>
#include <vector>
#include <algorithm>
#include "Vector.h"
#include "drawShapes.h"
#include "Utilities.h"

class entity
{
public:
	vec2 pos;
	vec2 posLerpOrig;
	vec2 animateTo;
	bool lerping;
	float animatingTime;
	float scale;
	float speed;
	float timeToGoal;

	entity();
	entity(const entity &clone);
	entity(float X, float Y, float Scale = 1.0f);

	void Update(float deltaTime);
	void Move(float x, float y, bool animate, bool doLerp);
	void Move(vec2 xy, bool animate, bool doLerp);
	void MoveTo(float x, float y, bool animate);
	void MoveTo(vec2 xy, bool animate);
	void StopAnim();
	vec2 IntendedPosition();
	void show();
};

class camera :public entity
{
public:
	int mode;
	float animateScale;
	float scaleLerpOrig;

	camera();
	camera(float X, float Y, float zoom = 1.0f, float lerpSpeed = 2.0f);

	void Update(float deltaTime);
	void lookAt(float X, float Y);
	void Move(float x, float y, float deltaTime);
	void MoveTo(float x, float y, float z, bool animate = false, bool doLerp = false);
	void MoveTo(vec2 xy, float z, bool animate, bool doLerp);
	void MoveTo(vec2 xy, bool animate = false, bool doLerp = false);
	void MoveTo(float x, float y, bool animate = false, bool doLerp = false);
	void ZoomIn(float deltaTime);
	void ZoomOut(float deltaTime);
	float GetOrtho(int side);
	vec2 GetViewSize(sf::RenderWindow &window);
	vec2 ScreenToWorldCoords(vec2 point, sf::RenderWindow &window);
	vec2 WorldToScreenCoords(vec2 point, sf::RenderWindow &window);
};

class sprite :public entity
{
public:
	sf::Texture img;

	sprite();
	sprite(const sprite &clone);
	sprite(std::string texFilepath);
	sprite(float X, float Y, float Scale = 1.0f, std::string texFilepath = "Null");
	
	void Update(float deltaTime);
	void loadTex(std::string filepath);
	void drawPic();
	vec3 GetImageMainColor();
};

class uiElement
{
public:
	std::string name;
	bool screenSpace;
	vec3 color;
	vec2 position;
	vec2 size;

	uiElement();
	uiElement(std::string Name, vec2 Position = { 0,0 }, vec2 Size = { 300,50 }, vec3 Color = { 0.5f, 0.5f, 0.5f });

	virtual void Draw(camera &cam, sf::Text text, sf::RenderWindow &window);
	virtual bool Overlaps(vec2 point, sf::RenderWindow &window);
	//virtual bool Overlaps(vec2 point);
	virtual void Click();
};


class button :public uiElement
{
public:
	void (*function)();
	bool highLighted;

	button();
	button(std::string Name, void (*Function)(), vec2 Position = { 0,0 }, vec2 Size = { 300,50 }, vec3 Color = { 0.5f, 0.5f, 0.5f });

	void Draw(camera &cam, sf::Text text, sf::RenderWindow &window, bool showBackground = true);
	void Click();
};

class checkbox :public uiElement
{
public:
	bool checked;

	checkbox();
	checkbox(std::string Name, bool isChecked = false, vec2 Position = { 0,0 }, vec2 Size = { 300,50 }, vec3 Color = { 0.9f, 0.9f, 0.9f });

	void Draw(camera &cam, sf::Text text, sf::RenderWindow &window);
	bool Overlaps(vec2 point, sf::RenderWindow &window);
	//bool Overlaps(vec2 point);
	void Click();
};


class textField :public uiElement
{
public:
	std::string inputText;
	std::string standbyText;
	bool isActive;
	bool numericalOnly;
	int characterCap;

	textField();
	textField(std::string Name, vec2 Position = { 0,0 }, vec2 Size = { 300,50 }, int letterCap = 20, bool onlyNumbers = false, vec3 Color = { 0.85f, 0.85f, 0.85f });

	void Draw(camera &cam, sf::Text text, sf::RenderWindow &window);
	void TypedIn(sf::Uint32 character);

	std::string GetTypedText();
	float GetTypedNumber(bool allowNegatives);
	void Click();
};


class customButtonSetA
{
public:
	std::string name;
	vec2 position;
	vec2 size;
	sprite mySprite;
	button myButton;
	textField myField;

	customButtonSetA();
	customButtonSetA(std::string Name, vec2 Position);
	
	void SetSize(float x, float y);
	void UpdatePositions();
	
	void Draw(camera &cam, sf::Text text, sf::RenderWindow &window, bool perSecond);
};