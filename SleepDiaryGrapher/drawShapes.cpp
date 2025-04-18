#pragma once
#include "drawShapes.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

rgbColour torgbColor(float r, float g, float b)
{
	return { r,g,b };
}

rgbColour torgbColor(vec3 clr)
{
	return { clr.x, clr.y, clr.z };
}

rgbColour colourValueSet(int clr)
{
	int colourRange = 255;
	rgbColour a;
	if (clr < 0)//no negatives
	{
		clr *= -1;
		return monochromeValueSet(clr);
	}
	if (clr > colourRange)//no numbers over the max range
	{
		clr -= colourRange*int(float(clr) / float(colourRange));
	}
	if (clr <= colourRange / 6)
	{
		a = { 1, float(clr) / float(colourRange / 6), 0 };
	}
	else if (clr > colourRange / 6 && clr <= (colourRange / 6) * 2)
	{
		a = { float(colourRange / 6 - (clr - colourRange / 6)) / float(colourRange / 6), 1, 0 };
	}
	else if (clr > (colourRange / 6) * 2 && clr <= (colourRange / 6) * 3)
	{
		a = { 0, 1, float(clr - (colourRange / 6) * 2) / float(colourRange / 6) };
	}
	else if (clr > (colourRange / 6) * 3 && clr <= (colourRange / 6) * 4)
	{
		a = { 0, float(colourRange / 6 - (clr - (colourRange / 6) * 3)) / float(colourRange / 6), 1 };
	}
	else if (clr > (colourRange / 6) * 4 && clr <= (colourRange / 6) * 5)
	{
		a = { float(clr - (colourRange / 6) * 4) / float(colourRange / 6), 0, 1 };
	}
	else if (clr > (colourRange / 6) * 5 && clr <= colourRange)
	{
		a = { 1, 0, float(colourRange / 6 - (clr - (colourRange / 6) * 5)) / float(colourRange / 6) };
	}
	else
	{
		system("quit");//a = {0.2,0.2,0.2};
	}
	return a;
}

rgbColour monochromeValueSet(int clr)
{
	int colourRange = 255;
	rgbColour a;
	if (clr < 0)//no negatives
	{
		clr *= -1;
	}
	if (clr > colourRange)//no numbers over the max range
	{
		clr -= colourRange*int(float(clr) / float(colourRange));
	}
	if (clr <= colourRange / 2)
	{
		a = { float(clr) / float(colourRange / 2), float(clr) / float(colourRange / 2), float(clr) / float(colourRange / 2) };
	}
	else
	{
		a = { float(colourRange / 2 - (clr - colourRange / 2)) / float(colourRange / 2), float(colourRange / 2 - (clr - colourRange / 2)) / float(colourRange / 2), float(colourRange / 2 - (clr - colourRange / 2)) / float(colourRange / 2) };
	}
	return a;
}

void setLineWidth(float width)
{
	glLineWidth(width);
}

/*
void setColor(float red, float green, float blue)
{
glColor3f(red,green,blue);
}
*/

//Draw a line
void drawLine(float x1, float y1, float x2, float y2, rgbColour clr)
{
	glColor3f(clr.red, clr.green, clr.blue);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0);
	glVertex3f(x2, y2, 0);
	glEnd();
}
void drawLine(float x1, float y1, float x2, float y2, int clr)
{
	drawLine(x1, y1, x2, y2, colourValueSet(clr));
}

void drawBezierLine(float x1, float y1, float x2, float y2, float hx1, float hy1, float hx2, float hy2, rgbColour clr)
{
	vec2 p1 = { x1,y1 };
	vec2 p2 = { hx1,hy1 };
	vec2 p3 = { hx2,hy2 };
	vec2 p4 = { x2,y2 };

	float oldx;
	float oldy;
	oldx = p1.x;
	oldy = p1.y;

	float precision = 0.01f;
	for (float u = 0.0f; u < 1.0f; u += precision)
	{
		drawLine(oldx, oldy, Bezier(p1.x, p2.x, p3.x, p4.x, u), Bezier(p1.y, p2.y, p3.y, p4.y, u), clr);
		oldx = Bezier(p1.x, p2.x, p3.x, p4.x, u);
		oldy = Bezier(p1.y, p2.y, p3.y, p4.y, u);
	}
	drawLine(oldx, oldy, Bezier(p1.x, p2.x, p3.x, p4.x, 1.0f), Bezier(p1.y, p2.y, p3.y, p4.y, 1.0f), clr);
}
void drawBezierLine(float x1, float y1, float x2, float y2, float hx1, float hy1, float hx2, float hy2, int clr)
{
	drawBezierLine(x1, y1, x2, y2, hx1, hy1, hx2, hy2, colourValueSet(clr));
}

//Draw a rectangle
void drawRectangle(bool filled, float cornerX, float cornerY, int width, int height, rgbColour clr)
{
	drawRectanglePoints(filled, cornerX, cornerY, cornerX + width, cornerY + height, clr);
}

//Draw a rectangle by corners
void drawRectanglePoints(bool filled, float corner1X, float corner1Y, float corner2X, float corner2Y, rgbColour clr)
{
	if (filled) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//rgbColour a = colourValueSet(clr);
	glColor3f(clr.red, clr.green, clr.blue);
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(corner1X, corner1Y, 0);
	glVertex3f(corner2X, corner1Y, 0);
	glVertex3f(corner2X, corner2Y, 0);
	glVertex3f(corner1X, corner2Y, 0);
	glEnd();
	glPopMatrix();
}

//Draw a circle: subdivisions will change how smooth of a circle it will be, radius is the radius in screen pixels, centerX/centerY/centerZ is the center of the circle
void drawCircle(bool filled, int subdivisions, float radius, float centerX, float centerY, rgbColour clr)
{
	glColor3f(clr.red, clr.green, clr.blue);
	float x, y;
	float step = 2.f*M_PI / (float)subdivisions;
	glPushMatrix();
	glTranslatef(centerX, centerY, 0);
	if (!filled) {
		glBegin(GL_LINE_LOOP);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, 0, 0);
	}
	for (float theta = 0; theta <= 2.f*M_PI; theta += step)
	{
		x = radius*cos(theta);
		y = radius*sin(theta);
		glVertex3f(x, y, 0);
	}
	if (filled) glVertex3f(radius, 0, 0);
	glEnd();
	glPopMatrix();
}
void drawCircle(bool filled, int subdivisions, float radius, float centerX, float centerY, int clr)
{
	drawCircle(filled, subdivisions, radius, centerX, centerY, colourValueSet(clr));
}

//Draw a triangle between three specified points
void drawTriangle(bool filled, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, rgbColour clr)
{
	if (filled) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(clr.red, clr.green, clr.blue);

	glBegin(GL_TRIANGLES);
	glVertex3f(p0x, p0y, 0);
	glVertex3f(p1x, p1y, 0);
	glVertex3f(p2x, p2y, 0);
	glEnd();
}
void drawTriangle(bool filled, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, int clr)
{
	drawTriangle(filled, p0x, p0y, p1x, p1y, p2x, p2y, colourValueSet(clr));
}

//Draw a quad between the specified points
void drawQuad(bool filled, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, int clr)
{
	if (filled) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	rgbColour a = colourValueSet(clr);
	glColor3f(a.red, a.green, a.blue);

	glBegin(GL_QUADS);
	glVertex3f(p0x, p0y, 0);
	glVertex3f(p1x, p1y, 0);
	glVertex3f(p2x, p2y, 0);
	glVertex3f(p3x, p3y, 0);
	glEnd();
}

//Draw an arrow
void drawArrow(float x1, float y1, float x2, float y2, float arrowSize, rgbColour clr)
{
	glColor3f(clr.red, clr.green, clr.blue);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0);
	glVertex3f(x2, y2, 0);
	glEnd();
	vec2 line = vec2(x2 - x1, y2 - y1);
	line.Normalize();
	vec2 perpLine = vec2(line.y, -line.x);
	vec2 p3 = vec2(x2, y2);
	vec2 p1 = p3 - line * arrowSize;
	vec2 p2 = p1;
	p1 += perpLine * (arrowSize / 2.0f);
	p2 -= perpLine * (arrowSize / 2.0f);
	drawTriangle(1, p1.x, p1.y, p2.x, p2.y, x2, y2, clr);
}
void drawArrow(float x1, float y1, float x2, float y2, float arrowSize, int clr)
{
	drawArrow(x1, y1, x2, y2, arrowSize, colourValueSet(clr));
}

//Draw a squareular arrow
void drawSquarrow(float x1, float y1, float x2, float y2, float arrowHeight, float arrowSize, int arrowDir, rgbColour clr)
{
	glColor3f(clr.red, clr.green, clr.blue);
	vec2 p1 = vec2(x1, y1);
	vec2 p4 = vec2(x2, y2);
	vec2 p2;
	vec2 p3;
	if (arrowDir == 0 || arrowDir == 2)
	{
		if (y1 - y2 < 0)
		{
			arrowHeight *= -1;
		}
		p2 = vec2(p1.x, p4.y + arrowHeight);
		p3 = vec2(p4.x, p4.y + arrowHeight);
	}
	else
	{
		if (x1 - x2 < 0)
		{
			arrowHeight *= -1;
		}
		p2 = vec2(p4.x + arrowHeight, p1.y);
		p3 = vec2(p4.x + arrowHeight, p4.y);
	}
	//Draw lines
	glBegin(GL_LINES);
	glVertex3f(p1.x, p1.y, 0);
	glVertex3f(p2.x, p2.y, 0);
	glVertex3f(p2.x, p2.y, 0);
	glVertex3f(p3.x, p3.y, 0);
	glVertex3f(p3.x, p3.y, 0);
	glVertex3f(p4.x, p4.y, 0);
	glEnd();
	//Draw arrowhead
	if (arrowDir == 0)//Up
	{
		drawTriangle(1, x2 - arrowSize / 2.0f, y2 - arrowSize, x2 + arrowSize / 2.0f, y2 - arrowSize, x2, y2, clr);
	}
	else if (arrowDir == 1)//Right
	{
		drawTriangle(1, x2 - arrowSize, y2 - arrowSize / 2.0f, x2 - arrowSize, y2 + arrowSize / 2.0f, x2, y2, clr);
	}
	else if (arrowDir == 2)//Down
	{
		drawTriangle(1, x2 - arrowSize / 2.0f, y2 + arrowSize, x2 + arrowSize / 2.0f, y2 + arrowSize, x2, y2, clr);
	}
	else if (arrowDir == 3)//Left
	{
		drawTriangle(1, x2 + arrowSize, y2 - arrowSize / 2.0f, x2 + arrowSize, y2 + arrowSize / 2.0f, x2, y2, clr);
	}
}
void drawSquarrow(float x1, float y1, float x2, float y2, float arrowHeight, float arrowSize, int arrowDir, int clr)
{
	drawSquarrow(x1, y1, x2, y2, arrowHeight, arrowSize, arrowDir, colourValueSet(clr));
}

//Draw a squareular arrow
void drawSquarrow(float x1, float y1, float x2, float y2, float arrowSize, int arrowDir, rgbColour clr)
{
	float arrowHeight = 0;
	if (arrowDir == 0 || arrowDir == 2)
	{
		arrowHeight = abs(y1 - y2) / 2.0f;
	}
	if (arrowDir == 1 || arrowDir == 3)
	{
		arrowHeight = abs(x1 - x2) / 2.0f;
	}
	drawSquarrow(x1, y1, x2, y2, arrowHeight, arrowSize, arrowDir, clr);
}
void drawSquarrow(float x1, float y1, float x2, float y2, float arrowSize, int arrowDir, int clr)
{
	drawSquarrow(x1, y1, x2, y2, arrowSize, arrowDir, colourValueSet(clr));
}

//Draw a bezier curve-ing arrow
void drawBezierArrow(float x1, float y1, float x2, float y2, float arrowHeight, float arrowSize, int arrowDir, rgbColour clr)
{
	glColor3f(clr.red, clr.green, clr.blue);
	float diff;
	float otherdiff;
	vec2 p1 = vec2(x1, y1);
	vec2 p2 = vec2(x2, y2);
	vec2 h1;
	vec2 h2;
	float sharpness = 0.8f; //0-1
	float mag1;
	float mag2;
	bool handlesAlwaysInwards = false; //Currently never is changed, but maybe this functionality will be desired in the future
	bool diffSignPos = true;
	float capStrength = 0;
	//Set handle positions
	if (arrowDir == 0 || arrowDir == 2)
	{
		diff = y1 - y2;
		otherdiff = abs(x1 - x2);
	}
	else
	{
		diff = x1 - x2;
		otherdiff = abs(y1 - y2);
	}
	if (handlesAlwaysInwards && diff < 0)
	{
		diffSignPos = false;
	}
	diff = abs(diff);

	if (arrowHeight > diff)
	{
		arrowHeight = diff;
	}
	if (arrowHeight < 0)
	{
		arrowHeight = 0;
	}
	float midOffsetScale = (abs(diff / 2.0f - arrowHeight) / (diff / 2.0f));

	//Change mincap strength by nearness on other axis of the two points of the arrow. Makes for nicer curves.
	capStrength = 3.5f * (1.0f + midOffsetScale) * pow(otherdiff, 0.5f);
	if (capStrength > 70)
	{
		capStrength = 70;
	}

	mag1 = diff - arrowHeight;
	mag2 = arrowHeight;

	float scaledSharpness = sharpness - (sharpness / 4.0f) * midOffsetScale;

	if (arrowDir == 2 || arrowDir == 3)//Flip diff so line preferred direction matches arrowhead
	{
		mag1 *= -1;
		mag2 *= -1;
	}
	//Mincap mag1 and 2 so that it never is a straight horizontal line - doesn't line up with arrowhead when this happens
	if (mag1 > 0 && mag1 < capStrength)
	{
		mag1 = capStrength;
	}
	else if (mag1 < 0 && mag1 > -capStrength)
	{
		mag1 = -capStrength;
	}
	else if (mag1 == 0)
	{
		mag1 = (arrowDir == 2 || arrowDir == 3 ? -capStrength : capStrength);
	}
	if (mag2 > 0 && mag2 < capStrength)
	{
		mag2 = capStrength;
	}
	else if (mag2 < 0 && mag2 > -capStrength)
	{
		mag2 = -capStrength;
	}
	else if (mag2 == 0)
	{
		mag2 = (arrowDir == 2 || arrowDir == 3 ? -capStrength : capStrength);
	}
	//Flip results if desired for handlesAlwaysInwards functionality
	if (!diffSignPos)
	{
		mag1 *= -1;
		mag2 *= -1;
	}

	if (arrowDir == 0 || arrowDir == 2)
	{
		h1 = vec2(x1, y1 + mag1 * 2.0f * scaledSharpness);
		h2 = vec2(x2, y2 - mag2 * 2.0f * scaledSharpness);
	}
	else
	{
		h1 = vec2(x1 + mag1 * 2.0f * scaledSharpness, y1);
		h2 = vec2(x2 - mag2 * 2.0f * scaledSharpness, y2);
	}
	//Draw handles (for development)
	//drawCircle(1, 10, 5, h1.x, h1.y, clr);
	//drawCircle(1, 10, 5, h2.x, h2.y, clr);

	//Draw arrowhead
	drawBezierLine(x1, y1, x2, y2, h1.x, h1.y, h2.x, h2.y, clr);
	if (arrowDir == 0)//Up
	{
		drawTriangle(1, x2 - arrowSize / 2.0f, y2 - arrowSize, x2 + arrowSize / 2.0f, y2 - arrowSize, x2, y2, clr);
	}
	else if (arrowDir == 1)//Right
	{
		drawTriangle(1, x2 - arrowSize, y2 - arrowSize / 2.0f, x2 - arrowSize, y2 + arrowSize / 2.0f, x2, y2, clr);
	}
	else if (arrowDir == 2)//Down
	{
		drawTriangle(1, x2 - arrowSize / 2.0f, y2 + arrowSize, x2 + arrowSize / 2.0f, y2 + arrowSize, x2, y2, clr);
	}
	else if (arrowDir == 3)//Left
	{
		drawTriangle(1, x2 + arrowSize, y2 - arrowSize / 2.0f, x2 + arrowSize, y2 + arrowSize / 2.0f, x2, y2, clr);
	}
}
void drawBezierArrow(float x1, float y1, float x2, float y2, float arrowHeight, float arrowSize, int arrowDir, int clr)
{
	drawBezierArrow(x1, y1, x2, y2, arrowHeight, arrowSize, arrowDir, colourValueSet(clr));
}

//Draw a bezier curve-ing arrow
void drawBezierArrow(float x1, float y1, float x2, float y2, float arrowSize, int arrowDir, rgbColour clr)
{
	float arrowHeight = 0;
	if (arrowDir == 0 || arrowDir == 2)
	{
		arrowHeight = abs(y1 - y2) / 2.0f;
	}
	if (arrowDir == 1 || arrowDir == 3)
	{
		arrowHeight = abs(x1 - x2) / 2.0f;
	}
	drawBezierArrow(x1, y1, x2, y2, arrowHeight, arrowSize, arrowDir, clr);
}
void drawBezierArrow(float x1, float y1, float x2, float y2, float arrowSize, int arrowDir, int clr)
{
	drawBezierArrow(x1, y1, x2, y2, arrowSize, arrowDir, colourValueSet(clr));
}

int getTextWidth(std::string s, sf::Text text, int style, int size)
{
	// Create a text
	text.setString(s);
	text.setCharacterSize(size);
	text.setStyle((style == 0 ? sf::Text::Regular : (style == 1 ? sf::Text::Bold : sf::Text::Italic)));
	text.setOutlineThickness(1);
	//return text.findCharacterPos(s.length()).x - 0;
	return text.getLocalBounds().width;
}

vec2 getTextBounds(std::string s, sf::Text text, int size, int style)
{
	// Create a text
	text.setString(s);
	text.setCharacterSize(size);
	text.setStyle((style == 0 ? sf::Text::Regular : (style == 1 ? sf::Text::Bold : sf::Text::Italic)));
	text.setOutlineThickness(1);
	//float textWidth = text.findCharacterPos(s.length()).x - posX;
	sf::FloatRect a = text.getLocalBounds();
	return vec2( a.width, a.height );
}

sf::Font loadFont(std::string fontFile)
{
	// Declare and load a font
	sf::Font font;
	if (!font.loadFromFile("Fonts\\arial.ttf"))
	{
		std::cout << "Font load error";
	}
	return font;
}

void drawText(std::string s, sf::Text text, float posX, float posY, int size, int clr, sf::RenderWindow &window, int justification, int style)
{
	rgbColour a = colourValueSet(clr);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	// Create a text
	text.setString(s);
	text.setPosition(posX, window.getSize().y - (posY + size));
	text.setCharacterSize(size);
	text.setStyle((style == 0 ? sf::Text::Regular : (style == 1 ? sf::Text::Bold : sf::Text::Italic)));
	text.setFillColor(sf::Color(a.red * 255.0f, a.green * 255.0f, a.blue * 255.0f));
	text.setOutlineColor(sf::Color(a.red * 255.0f, a.green * 255.0f, a.blue * 255.0f));
	text.setOutlineThickness(1);
	text.setOutlineColor(sf::Color::White);
	if (justification != 0)
	{
		//float textWidth = s.length() * (size * 0.56f);
		//float textWidth = text.findCharacterPos(s.length()).x - posX;// s.length() * (size * 0.56f);
		float textWidth = text.getLocalBounds().width;
		if (justification == 1)
		{
			text.setPosition(posX - textWidth / 2.0f, window.getSize().y - (posY + size));
		}
		else if (justification == 2)
		{
			text.setPosition(posX - textWidth, window.getSize().y - (posY + size));
		}
	}

	// Draw it
	window.pushGLStates();
	window.draw(text);
	window.popGLStates();
}