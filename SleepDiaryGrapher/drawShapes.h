#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>
#include <iostream>
#include <string>
#include "vector.h"
#include "Utilities.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct rgbColour
{
	float red;
	float green;
	float blue;
};

rgbColour torgbColor(float r, float g, float b);
rgbColour torgbColor(vec3 clr);

rgbColour colourValueSet(int clr);

rgbColour monochromeValueSet(int clr);

void setLineWidth(float width);

//void setColor(float red, float green, float blue);

void drawLine(float x1, float y1, float x2, float y2, rgbColour clr);
void drawLine(float x1, float y1, float x2, float y2, int clr);

void drawBezierLine(float x1, float y1, float x2, float y2, float hx1, float hy1, float hx2, float hy2, rgbColour clr);
void drawBezierLine(float x1, float y1, float x2, float y2, float hx1, float hy1, float hx2, float hy2, int clr);

void drawRectangle(bool filled, float cornerX, float cornerY, int width, int height, rgbColour clr);
void drawRectanglePoints(bool filled, float corner1X, float corner1Y, float corner2X, float corner2Y, rgbColour clr);

void drawCircle(bool filled, int subdivisions, float radius, float centerX, float centerY, rgbColour clr);
void drawCircle(bool filled, int subdivisions, float radius, float centerX, float centerY, int clr);

void drawTriangle(bool filled, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, rgbColour clr);
void drawTriangle(bool filled, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, int clr);

void drawQuad(bool filled, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, int clr);

void drawArrow(float x1, float y1, float x2, float y2, float arrowSize, rgbColour clr);
void drawArrow(float x1, float y1, float x2, float y2, float arrowSize, int clr);

void drawSquarrow(float x1, float y1, float x2, float y2, float arrowHeight, float arrowSize, int arrowDir, rgbColour clr);
void drawSquarrow(float x1, float y1, float x2, float y2, float arrowHeight, float arrowSize, int arrowDir, int clr);
void drawSquarrow(float x1, float y1, float x2, float y2, float arrowSize, int arrowDir, rgbColour clr);
void drawSquarrow(float x1, float y1, float x2, float y2, float arrowSize, int arrowDir, int clr);

void drawBezierArrow(float x1, float y1, float x2, float y2, float arrowHeight, float arrowSize, int arrowDir, rgbColour clr);
void drawBezierArrow(float x1, float y1, float x2, float y2, float arrowHeight, float arrowSize, int arrowDir, int clr);
void drawBezierArrow(float x1, float y1, float x2, float y2, float arrowSize, int arrowDir, int rgbColour);
void drawBezierArrow(float x1, float y1, float x2, float y2, float arrowSize, int arrowDir, int clr);

vec2 getTextBounds(std::string s, sf::Text text, int size, int style);
int getTextWidth(std::string s, sf::Text text, int style, int size);

sf::Font loadFont(std::string fontFile); // = "Fonts\\arial.ttf"
void drawText(std::string s, sf::Text text, float posX, float posY, int size, int clr, sf::RenderWindow &window, int justification = 0, int style = 0);
