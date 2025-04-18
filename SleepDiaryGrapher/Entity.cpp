#pragma once
#include "Entity.h"

const vec2 voidVal = vec2(0.27834f, 0.72395f);

entity::entity() {
	pos = { 0, 0 };
	posLerpOrig = pos;
	animateTo = voidVal;
	lerping = false;
	scale = 1.0f;
	speed = 1.0f;
	animatingTime = 0.0f;
	timeToGoal = 2.0f;
}

entity::entity(const entity &clone)
{
	pos = clone.pos;
	posLerpOrig = clone.posLerpOrig;
	animateTo = clone.animateTo;
	lerping = clone.lerping;
	scale = clone.scale;
	speed = clone.speed;
	animatingTime = 0.0f;
	timeToGoal = 2.0f;
}

entity::entity(float X, float Y, float Scale) {
	pos = { X, Y };
	posLerpOrig = pos;
	animateTo = voidVal;
	lerping = false;
	scale = Scale;
	speed = 1.0f;
	animatingTime = 0.0f;
	timeToGoal = 2.0f;
}

void entity::Update(float deltaTime)
{
	if (animateTo != voidVal)
	{
		animatingTime += deltaTime;
		if (lerping)
		{
			if (animatingTime >= timeToGoal * 1000.0f) //if pos reaches or passes animateTo on this frame
			{
				pos = animateTo;
				animateTo = voidVal;
				animatingTime = 0.0f;
			}
			else
			{
				pos = LERPvec2(posLerpOrig, animateTo, animatingTime / (timeToGoal* 1000.0f));
			}
		}
		else
		{
			if ((animateTo - pos).Length() <= speed * deltaTime) //if pos reaches or passes animateTo on this frame
			{
				pos = animateTo;
				animateTo = voidVal;
				animatingTime = 0.0f;
			}
			else
			{
				pos += (animateTo - pos).Normalized() * speed * deltaTime;
			}
		}
	}
}

void entity::Move(float x, float y, bool animate, bool doLerp)
{
	if (animate)
	{
		lerping = doLerp;
		animatingTime = 0.0f;
		posLerpOrig = pos;
		if (animateTo == voidVal)
		{
			animateTo = pos + vec2(x, y);
		}
		else
		{
			animateTo += vec2(x, y);
		}
	}
	else
	{
		if (animateTo == voidVal)
		{
			pos += {x, y};
		}
		else
		{
			pos = animateTo + vec2(x, y);
			animateTo = voidVal;
			animatingTime = 0.0f;
			posLerpOrig = pos;
		}
	}
}

void entity::Move(vec2 xy, bool animate, bool doLerp)
{
	Move(xy.x, xy.y, animate, doLerp);
}

void entity::MoveTo(float x, float y, bool animate)
{
	if (animate)
	{
		animatingTime = 0.0f;
		posLerpOrig = pos;
		animateTo = vec2(x, y);
	}
	else
	{
		animateTo = voidVal;
		pos = { x, y };
	}
}

void entity::MoveTo(vec2 xy, bool animate)
{
	MoveTo(xy.x, xy.y, animate);
}

void entity::StopAnim()
{
	if (animateTo != voidVal)
	{
		animateTo = voidVal;
		animatingTime = 0.0f;
		posLerpOrig = pos;
	}
}

vec2 entity::IntendedPosition()
{
	if (animateTo == voidVal)
	{
		return pos;
	}
	else
	{
		return animateTo;
	}
}

void entity::show()
{
	drawRectangle(0, pos.x - (scale / 2.0f), pos.y - (scale / 2.0f), scale, scale, { 1,0,0 });
}

camera::camera()
	:entity()
{
	mode = 0;
	animateScale = voidVal.x;
	scaleLerpOrig = voidVal.x;
}
camera::camera(float X, float Y, float zoom, float lerpSpeed)
	: entity(X, Y, zoom)
{
	mode = 0;
	animateScale = voidVal.x;
	scaleLerpOrig = voidVal.x;
	timeToGoal = lerpSpeed;
}

void camera::Update(float deltaTime)
{
	//entity::Update(deltaTime); //To use the Lerping movement from entity - doing a code copypaste for now though, cus there's a few differences.
	if (animateTo != voidVal)
	{
		animatingTime += deltaTime;
		if (lerping)
		{
			if (animatingTime >= timeToGoal * 1000.0f) //if pos reaches or passes animateTo on this frame
			{
				pos = animateTo;
				scale = animateScale;
				animateTo = voidVal;
				animateScale = voidVal.x;
				animatingTime = 0.0f;
			}
			else
			{
				pos = LERPvec2(posLerpOrig, animateTo, animatingTime / (timeToGoal* 1000.0f));
				scale = LERP(scaleLerpOrig, animateScale, animatingTime / (timeToGoal * 1000.0f));
			}
		}
		else
		{
			if ((vec3(animateTo.x, animateTo.y, animateScale) - vec3(pos.x, pos.y, scale)).Length() <= speed * deltaTime) //if pos reaches or passes animateTo on this frame
			{
				pos = animateTo;
				scale = animateScale;
				animateTo = voidVal;
				animateScale = voidVal.x;
			}
			else
			{
				vec3 resultMov = (vec3(animateTo.x, animateTo.y, animateScale) - vec3(pos.x, pos.y, scale)).Normalized();
				pos += vec2(resultMov.x, resultMov.y) * speed * deltaTime;
				scale += resultMov.z * speed * deltaTime;
			}
			/*
			if ((animateTo - pos).Length() <= speed * deltaTime) //if pos reaches or passes animateTo on this frame
			{
				pos = animateTo;
				animateTo = voidVal;
			}
			else
			{
				pos += (animateTo - pos).Normalized() * speed * deltaTime;
			}*/

		}
	}
}

void camera::lookAt(float X, float Y) {
	pos.x = X;
	pos.y = Y;
}

void camera::Move(float x, float y, float deltaTime)
{
	pos += vec2(x, y).Normalized() * speed * deltaTime;

	if (animateTo != voidVal)
	{
		animateTo = voidVal;
		animateScale = voidVal.x;
		animatingTime = 0.0f;
		posLerpOrig = pos;
		scaleLerpOrig = scale;
	}
}

void camera::MoveTo(float x, float y, float z, bool animate, bool doLerp)
{
	entity::MoveTo(x, y, animate);
	if (animate)
	{
		lerping = doLerp;
		if (z != -1.0f)
		{
			animateScale = z;
		}
		scaleLerpOrig = scale;
	}
	else
	{
		if (z != -1.0f)
		{
			scale = z;
		}
	}
}

void camera::MoveTo(vec2 xy, float z, bool animate, bool doLerp)
{
	MoveTo(xy.x, xy.y, z, animate, doLerp);
}

void camera::MoveTo(vec2 xy, bool animate, bool doLerp)
{
	MoveTo(xy.x, xy.y, -1.0f, animate, doLerp);
}

void camera::MoveTo(float x, float y, bool animate, bool doLerp)
{
	MoveTo(x, y, -1.0f, animate, doLerp);
}

void camera::ZoomIn(float deltaTime)
{
	scale -= speed / 400.0f * deltaTime;
	if (scale < 0.3f)
	{
		scale = 0.3f;
	}
	if (animateTo != voidVal)
	{
		animateTo = voidVal;
		animateScale = voidVal.x;
	}
}
void camera::ZoomOut(float deltaTime)
{
	scale += speed / 400.0f * deltaTime;
	if (animateTo != voidVal)
	{
		animateTo = voidVal;
		animateScale = voidVal.x;
	}
}

float camera::GetOrtho(int side)
{
	if (side == 0)
	{
		return pos.x - scale;
	}
	else if (side == 1)
	{
		return pos.x + scale;
	}
	else if (side == 2)
	{
		return pos.y - scale;
	}
	else if (side == 3)
	{
		return pos.y + scale;
	}
}

vec2 camera::GetViewSize(sf::RenderWindow &window)
{
	return vec2(window.getSize()) * scale;
}

vec2 camera::ScreenToWorldCoords(vec2 point, sf::RenderWindow &window)
{
	return (point - vec2(window.getSize()) / 2.0f) * scale + pos;
}

vec2 camera::WorldToScreenCoords(vec2 point, sf::RenderWindow &window)
{
	return (point - pos) / scale + vec2(window.getSize()) / 2.0f;
}

sprite::sprite()
	:entity()
{
	img = sf::Texture();
}
sprite::sprite(const sprite &clone)
	: entity(clone)
{
	img = clone.img;
}

sprite::sprite(std::string texFilepath)
	: entity()
{
	loadTex(texFilepath);
}
sprite::sprite(float X, float Y, float Scale, std::string texFilepath)
	: entity(X, Y, Scale)
{
	if (texFilepath == "Null")
	{
		img = sf::Texture();
	}
	else
	{
		loadTex(texFilepath);
	}
}

void sprite::Update(float deltaTime)
{
	entity::Update(deltaTime);
}
void sprite::loadTex(std::string filepath)
{
	if (filepath != "Null")
	{
		img.loadFromFile(filepath);
	}
}
void sprite::drawPic()
{
	if (!(img.getSize().x + img.getSize().y == 0))
	{
		glEnable(GL_TEXTURE_2D);
		sf::Texture::bind(&img);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 1); glVertex3f(pos.x - (scale / 2.0f), pos.y - (scale / 2.0f), -0.5);
		glTexCoord2f(1, 1); glVertex3f(pos.x + (scale / 2.0f), pos.y - (scale / 2.0f), -0.5);
		glTexCoord2f(1, 0); glVertex3f(pos.x + (scale / 2.0f), pos.y + (scale / 2.0f), -0.5);
		glTexCoord2f(0, 0); glVertex3f(pos.x - (scale / 2.0f), pos.y + (scale / 2.0f), -0.5);
		glEnd();
		sf::Texture::bind(NULL);
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		drawRectangle(1, pos.x - (scale / 2.0f), pos.y - (scale / 2.0f), scale, scale, torgbColor(1.0f, 0.0f, 1.0f));
	}
}
vec3 sprite::GetImageMainColor()
{
	if (!(img.getSize().x + img.getSize().y == 0))
	{
		vec3 avgColor = vec3(0, 0, 0);
		sf::Image pixelInfo = img.copyToImage();
		for (int py = 0; py < img.getSize().y; py++)
		{
			for (int px = 0; px < img.getSize().x; px++)
			{
				sf::Color pxSfClr = pixelInfo.getPixel(px, py);
				vec3 pxClr = vec3();
				pxClr.x = pxSfClr.r;
				pxClr.y = pxSfClr.g;
				pxClr.z = pxSfClr.b;
				if (pxSfClr.a != 0)
				{
					avgColor += pxClr;
				}
			}
		}
		avgColor /= 255.0f;
		avgColor /= img.getSize().x * img.getSize().y;
		//Vibrant-ize colour
		//Method1:
		//avgColor.Normalize();
		//Method2:
		//avgColor = (avgColor + avgColor.getNormalize()) / 2.0f;
		//Method2:
		avgColor = (avgColor * 0.7f + avgColor.Normalized()* 0.3f);
		return avgColor;
	}
	else
	{
		return vec3(1.0f, 0.0f, 1.0f);
	}
}

uiElement::uiElement()
{
	name = "";
	screenSpace = true;
	color = { 0.5f, 0.5f, 0.5f };
	position = { 0,0 };
	size = { 300,50 };
}
uiElement::uiElement(std::string Name, vec2 Position, vec2 Size, vec3 Color)
{
	name = Name;
	screenSpace = true;
	color = Color;
	position = Position;
	size = Size;
}
void uiElement::Draw(camera &cam, sf::Text text, sf::RenderWindow &window)
{
	vec3 drawColor = color;
	vec2 adjustedPos = position;
	if (screenSpace)
	{
		adjustedPos.x *= window.getSize().x;
		adjustedPos.y *= window.getSize().y;
	}
	else //Make negative numbers "loop around" - or really, interpret negatives as, they want it anchored away from the other side of the screen
	{
		if (adjustedPos.x < 0)
		{
			adjustedPos.x = window.getSize().x + adjustedPos.x;
		}
		if (adjustedPos.y < 0)
		{
			adjustedPos.y = window.getSize().y + adjustedPos.y;
		}
	}
	vec2 scaledSize = size * cam.scale;
	vec2 worldPos = cam.ScreenToWorldCoords(adjustedPos, window);

	drawRectangle(1, worldPos.x - scaledSize.x / 2.0f, worldPos.y - scaledSize.y / 2.0f, scaledSize.x, scaledSize.y, torgbColor(drawColor));
	drawRectangle(0, worldPos.x - scaledSize.x / 2.0f, worldPos.y - scaledSize.y / 2.0f, scaledSize.x, scaledSize.y, { 0,0,0 });
	float textSize = size.y * 0.8f;
	vec2 a = getTextBounds(name, text, textSize, 0);
	if (a.x > scaledSize.x)
	{
		textSize = scaledSize.x / (a.x / a.y);
	}
	drawText(name, text, adjustedPos.x, adjustedPos.y - ((size.y <= 12 ? 12 : size.y - 12) * 0.8f) / 2.0f, textSize, -254, window, 1);
}
bool uiElement::Overlaps(vec2 point, sf::RenderWindow &window)
{
	vec2 adjustedPos = position;
	if (screenSpace)
	{
		adjustedPos.x *= window.getSize().x;
		adjustedPos.y *= window.getSize().y;
	}
	else //Make negative numbers "loop around" - or really, interpret negatives as, they want it anchored away from the other side of the screen
	{
		if (adjustedPos.x < 0)
		{
			adjustedPos.x = window.getSize().x + adjustedPos.x;
		}
		if (adjustedPos.y < 0)
		{
			adjustedPos.y = window.getSize().y + adjustedPos.y;
		}
	}
	if (point.x >= adjustedPos.x - (size.x / 2.0f) && point.x <= adjustedPos.x + (size.x / 2.0f) && point.y >= adjustedPos.y - (size.y / 2.0f) && point.y <= adjustedPos.y + (size.y / 2.0f))
	{
		return true;
	}
	else
	{
		return false;
	}
}
/*bool uiElement::Overlaps(vec2 point)
{
	if (screenSpace)
	{
		std::cout << "Error: This button, \"" + name + "\", is set to Screen Space, so overlap detection will not work. Please use the other overload of the Overlaps function and pass it a reference to the RenderWindow.\n";
		return false;
	}
	if (point.x >= position.x - (size.x / 2.0f) && point.x <= position.x + (size.x / 2.0f) && point.y >= position.y - (size.y / 2.0f) && point.y <= position.y + (size.y / 2.0f))
	{
		return true;
	}
	else
	{
		return false;
	}
}*/
void uiElement::Click()
{
}

button::button()
{
	name = "";
	function = nullptr;
	screenSpace = true;
	color = { 0.5f, 0.5f, 0.5f };
	position = { 0,0 };
	size = { 300,50 };
	highLighted = false;
}
button::button(std::string Name, void(*Function)(), vec2 Position, vec2 Size, vec3 Color)
{
	name = Name;
	function = Function;
	screenSpace = true;
	color = Color;
	position = Position;
	size = Size;
	highLighted = false;
}
void button::Draw(camera &cam, sf::Text text, sf::RenderWindow &window, bool showBackground)
{
	vec3 drawColor = color;
	if (highLighted)
	{
		drawColor.x += 0.1f;
		drawColor.y += 0.1f;
		drawColor.z += 0.1f;
		if (drawColor.x > 1.0f)
		{
			drawColor.x = 1.0f;
		}
		if (drawColor.y > 1.0f)
		{
			drawColor.y = 1.0f;
		}
		if (drawColor.z > 1.0f)
		{
			drawColor.z = 1.0f;
		}
	}
	vec2 adjustedPos = position;
	if (screenSpace)
	{
		adjustedPos.x *= window.getSize().x;
		adjustedPos.y *= window.getSize().y;
	}
	else //Make negative numbers "loop around" - or really, interpret negatives as, they want it anchored away from the other side of the screen
	{
		if (adjustedPos.x < 0)
		{
			adjustedPos.x = window.getSize().x + adjustedPos.x;
		}
		if (adjustedPos.y < 0)
		{
			adjustedPos.y = window.getSize().y + adjustedPos.y;
		}
	}
	vec2 scaledSize = size * cam.scale;
	vec2 worldPos = cam.ScreenToWorldCoords(adjustedPos, window);

	if (showBackground)
	{
		drawRectangle(1, worldPos.x - scaledSize.x / 2.0f, worldPos.y - scaledSize.y / 2.0f, scaledSize.x, scaledSize.y, torgbColor(drawColor));
	}
	drawRectangle(0, worldPos.x - scaledSize.x / 2.0f, worldPos.y - scaledSize.y / 2.0f, scaledSize.x, scaledSize.y, { 0,0,0 });
	float textSize = size.y * 0.8f;
	vec2 a = getTextBounds(name, text, textSize, 0);
	if (a.x > scaledSize.x)
	{
		textSize = scaledSize.x / (a.x / a.y);
	}
	drawText(name, text, adjustedPos.x, adjustedPos.y - ((size.y <= 12 ? 12 : size.y - 12) * 0.8f) / 2.0f, textSize, -254, window, 1);
}
void button::Click()
{
	function();
}

checkbox::checkbox()
{
	name = "";
	checked = false;
	screenSpace = true;
	color = { 0.9f, 0.9f, 0.9f };
	position = { 0,0 };
	size = { 300,50 };
}
checkbox::checkbox(std::string Name, bool isChecked, vec2 Position, vec2 Size, vec3 Color)
{
	name = Name;
	checked = isChecked;
	screenSpace = true;
	color = Color;
	position = Position;
	size = Size;
}
void checkbox::Draw(camera &cam, sf::Text text, sf::RenderWindow &window)
{
	vec3 drawColor = color;
	if (checked)
	{
		drawColor.x += 0.1f;
		drawColor.y += 0.1f;
		drawColor.z += 0.1f;
		if (drawColor.x > 1.0f)
		{
			drawColor.x = 1.0f;
		}
		if (drawColor.y > 1.0f)
		{
			drawColor.y = 1.0f;
		}
		if (drawColor.z > 1.0f)
		{
			drawColor.z = 1.0f;
		}
	}
	vec2 adjustedPos = position;
	if (screenSpace)
	{
		adjustedPos.x *= window.getSize().x;
		adjustedPos.y *= window.getSize().y;
	}
	else //Make negative numbers "loop around" - or really, interpret negatives as, they want it anchored away from the other side of the screen
	{
		if (adjustedPos.x < 0)
		{
			adjustedPos.x = window.getSize().x + adjustedPos.x;
		}
		if (adjustedPos.y < 0)
		{
			adjustedPos.y = window.getSize().y + adjustedPos.y;
		}
	}
	vec2 scaledSize = size * cam.scale;
	vec2 worldPos = cam.ScreenToWorldCoords(adjustedPos, window);

	drawRectangle(1, worldPos.x - scaledSize.y / 2.0f, worldPos.y - scaledSize.y / 2.0f, scaledSize.y, scaledSize.y, torgbColor(drawColor));
	drawRectangle(0, worldPos.x - scaledSize.y / 2.0f, worldPos.y - scaledSize.y / 2.0f, scaledSize.y, scaledSize.y, { 0,0,0 });
	float textSize = size.y * 0.8f;
	vec2 a = getTextBounds(name, text, textSize, 0);
	if (a.x > scaledSize.x)
	{
		textSize = scaledSize.x / (a.x / a.y);
	}
	if (checked)
	{
		drawRectangle(1, worldPos.x - scaledSize.y / 2.8f, worldPos.y - scaledSize.y / 3.0f, scaledSize.y * 0.7f, scaledSize.y * 0.7f, torgbColor(vec3()));
	}
	drawText(name, text, adjustedPos.x - size.y, adjustedPos.y - ((size.y <= 12 ? 12 : size.y - 12) * 0.8f) / 2.0f, textSize, -254, window, 2);
}
bool checkbox::Overlaps(vec2 point, sf::RenderWindow &window)
{
	vec2 adjustedPos = position;
	if (screenSpace)
	{
		adjustedPos.x *= window.getSize().x;
		adjustedPos.y *= window.getSize().y;
	}
	else //Make negative numbers "loop around" - or really, interpret negatives as, they want it anchored away from the other side of the screen
	{
		if (adjustedPos.x < 0)
		{
			adjustedPos.x = window.getSize().x + adjustedPos.x;
		}
		if (adjustedPos.y < 0)
		{
			adjustedPos.y = window.getSize().y + adjustedPos.y;
		}
	}
	if (point.x >= adjustedPos.x - (size.y / 2.0f) && point.x <= adjustedPos.x + (size.y / 2.0f) && point.y >= adjustedPos.y - (size.y / 2.0f) && point.y <= adjustedPos.y + (size.y / 2.0f))
	{
		return true;
	}
	else
	{
		return false;
	}
}
/*bool checkbox::Overlaps(vec2 point)
{
	if (screenSpace)
	{
		std::cout << "Error: This checkbox, \"" + name + "\", is set to Screen Space, so overlap detection will not work. Please use the other overload of the Overlaps function and pass it a reference to the RenderWindow.\n";
		return false;
	}
	if (point.x >= position.x - (size.y / 2.0f) && point.x <= position.x + (size.y / 2.0f) && point.y >= position.y - (size.y / 2.0f) && point.y <= position.y + (size.y / 2.0f))
	{
		return true;
	}
	else
	{
		return false;
	}
}*/
void checkbox::Click()
{
	checked = !checked;
}


textField::textField()
{
	name = "";
	standbyText = "value";
	inputText = "";
	numericalOnly = false;
	screenSpace = true;
	color = { 0.85f, 0.85f, 0.85f };
	position = { 0,0 };
	size = { 300,50 };
	isActive = false;
	characterCap = 20;
}
textField::textField(std::string Name, vec2 Position, vec2 Size, int letterCap, bool onlyNumbers, vec3 Color)
{
	name = Name;
	standbyText = "value";
	inputText = "";
	numericalOnly = onlyNumbers;
	screenSpace = true;
	color = Color;
	position = Position;
	size = Size;
	isActive = false;
	characterCap = letterCap;
}
void textField::Draw(camera &cam, sf::Text text, sf::RenderWindow &window)
{
	vec3 drawColor = color;
	if (isActive)
	{
		drawColor.x += 0.3f;
		drawColor.y += 0.3f;
		drawColor.z += 0.3f;
		if (drawColor.x > 1.0f)
		{
			drawColor.x = 1.0f;
		}
		if (drawColor.y > 1.0f)
		{
			drawColor.y = 1.0f;
		}
		if (drawColor.z > 1.0f)
		{
			drawColor.z = 1.0f;
		}
	}
	vec2 adjustedPos = position;
	if (screenSpace)
	{
		adjustedPos.x *= window.getSize().x;
		adjustedPos.y *= window.getSize().y;
	}
	else //Make negative numbers "loop around" - or really, interpret negatives as, they want it anchored away from the other side of the screen
	{
		if (adjustedPos.x < 0)
		{
			adjustedPos.x = window.getSize().x + adjustedPos.x;
		}
		if (adjustedPos.y < 0)
		{
			adjustedPos.y = window.getSize().y + adjustedPos.y;
		}
	}
	vec2 scaledSize = size * cam.scale;
	vec2 worldPos = cam.ScreenToWorldCoords(adjustedPos, window);

	drawRectangle(1, worldPos.x - scaledSize.x / 2.0f, worldPos.y - scaledSize.y / 2.0f, scaledSize.x, scaledSize.y, torgbColor(drawColor));
	drawRectangle(0, worldPos.x - scaledSize.x / 2.0f, worldPos.y - scaledSize.y / 2.0f, scaledSize.x, scaledSize.y, { 0,0,0 });
	float textSize = size.y * 0.8f;
	std::string stringToDraw = standbyText;
	int textColor = -200;
	if (inputText.length() > 0)
	{
		stringToDraw = inputText;
		textColor = -254;
	}
	vec2 a = getTextBounds(stringToDraw, text, textSize, 0);
	if (a.x > scaledSize.x)
	{
		textSize = scaledSize.x / (a.x / a.y);
	}
	drawText(stringToDraw, text, adjustedPos.x, adjustedPos.y - ((size.y <= 12 ? 12 : size.y - 12) * 0.8f) / 2.0f, textSize, textColor, window, 1);
}
void textField::TypedIn(sf::Uint32 character)
{
	if (!numericalOnly || (character >= 48 && character <= 57 || character == 8 || character == 46 || character == 45))
	{
		if (character == 8)
		{
			inputText = inputText.substr(0, inputText.length() - 1);
		}
		else
		{
			if (!(character == 45 && numericalOnly && inputText.length() != 0) && !(character == 46 && numericalOnly && inputText.find_first_of('.') != std::string::npos) && inputText.length() <= characterCap)
			{
				inputText += (char)character;
			}
		}
	}
}
std::string textField::GetTypedText()
{
	return (inputText.length() == 0 ? standbyText : inputText);
}
float textField::GetTypedNumber(bool allowNegatives)
{
	if (numericalOnly && inputText.length() > 0)
	{
		return stoi(inputText);
	}
	else
	{
		try
		{
			stoi(inputText);
		}
		catch (std::invalid_argument)
		{
			return -1;
		}
		return stoi(inputText);
	}
}
void textField::Click()
{
	isActive = true;
	//make active text field / give blinky line
}

customButtonSetA::customButtonSetA()
{
	name = "Null";
	position = { 0,0 };
	size = { 300,50 };
	mySprite = sprite();
	myButton = button();
	myButton.size = size;
	myField = textField();
	myField.size = size;
}
customButtonSetA::customButtonSetA(std::string Name, vec2 Position)
{
	name = Name;
	position = Position;
	size = { 300,50 };
	mySprite = sprite(Position.x, Position.y);
	myButton = button(Name, nullptr, position, size);
	myButton.size = size;
	myButton.screenSpace = false;
	myField = textField(Name, Position, size);
	myField.size = size;
	myField.screenSpace = false;
}
void customButtonSetA::SetSize(float x, float y)
{
	mySprite.scale = (y < 0 ? mySprite.scale : y);
	size = { std::max(mySprite.scale + 10.0f + 5.0f + 5.0f, x), mySprite.scale };
}
void customButtonSetA::UpdatePositions()
{
	mySprite.pos = { position.x + mySprite.scale / 2, position.y };
	myButton.size = { size.y, size.y };
	myButton.position = { position.x + myButton.size.x / 2, position.y };
	float fieldPosX = myButton.size.x + 10.0f;
	myField.size = { std::min(size.x -5.0f - fieldPosX, 50.0f), size.y };
	myField.position = { position.x + fieldPosX + myField.size.x / 2, position.y };
}
void customButtonSetA::Draw(camera &cam, sf::Text text, sf::RenderWindow &window, bool perSecond)
{
	UpdatePositions();
	mySprite.drawPic();
	myButton.Draw(cam, text, window, false);
	//myField.Draw(cam,window); //Being drawn elsewhere, in main loop as part of proper button list
	//Draw extra text
	vec2 adjustedPos = myField.position + vec2(myField.size.x / 2, 0);
	vec2 scaledSize = vec2(size.x - (adjustedPos.x - position.x), size.y) * cam.scale;
	float textSize = size.y * 0.8f;
	std::string stringToDraw = (perSecond ? " / sec" : " machine(s)");
	vec2 a = getTextBounds(stringToDraw, text, textSize, 0);
	if (a.x > scaledSize.x)
	{
		textSize = scaledSize.x / (a.x / a.y);
	}
	drawText(stringToDraw, text, adjustedPos.x, adjustedPos.y - ((size.y <= 12 ? 12 : size.y - 12) * 0.8f) / 2.0f, textSize, -254, window);
}