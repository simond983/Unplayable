#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "Component.h"
#include <SDL_image.h>
#include "../cute_c2.h"
#include <iostream>



class SpriteComponent : public Component
{
public:
	//Constructor
	//@param x: x point on the SOURCE asset
	//@param y: y point on the SOURCE asset
	//@param w: width of the SOURCE asset
	//@param h: height of the SOURCE asset
	SpriteComponent();
	SpriteComponent(int x, int y, int w, int h);
	~SpriteComponent();

	std::string m_tag = "SPRITE";

	std::string getType() { return m_tag; }
	
	//Optimized texture loading function
	//@param path: path to the asset
	//@param gRenderer: reference to the global renderer
	bool loadFromFile(std::string path, SDL_Renderer *gRenderer);

	//Render method for the sprite
	//@param gRenderer: pointer to the global renderer
	void render(SDL_Renderer* gRenderer);

	//Resource freeing method for textures
	void freeTexture();

	//Color setting function
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	
	//Scale setting function
	//@param scalar: cute_v2
	void setScale(c2v scalar);

	//Position setting function
	//@param position: cute_v2 new position to be set
	void setPosition(c2v position);

	//Texture blending function
	//@param blending: SDL_BlendMode of the blend type you want to apply
	void setBlendMode(SDL_BlendMode blending);

	//Function to set the alpha of the sprite
	//@param alpha: Uint8 alpha value between 0 and 255;
	void setAlpha(Uint8 alpha);

	//Persistent move method
	//@param offset: cute_v2 of the amount in each axis to move by on method call
	void move(c2v offset);

	//Rotation set method
	//@param angle: angle in degrees
	void setRotation(int angle);

	//Persistent rotation method
	//@param rotationAmount: amount to add to the rotation angle each time the function is called.
	void rotate(int rotationAmount);

	//Function to set a texture that is already loaded rather than loading one with the loadFromFile() function
	//\param texture: pointer to SDL_Texture
	//\param width: width of the texture
	//\param height: height of the texture
	void setTexture(SDL_Texture* texture, int width, int height);

	//Function to set the source rectangle for a texture
	//\param rect: Pointer to an SDL_Rect object
	void setSRect(SDL_Rect* rect);

	//Function to return the source rectangle for a texture
	SDL_Rect* getSRect();
	

	//Functions for getting properties of a sprite after transformations have been applied
	c2v getPosition();
	c2v getScale();
	int getRotation();
	int getWidth();
	int getHeight();
	void setCentreX(double x);
	void setCentreY(double y);

	SDL_RendererFlip m_flipValue;


	SDL_Point m_centre;
	SDL_Texture* getTexture();

private:
	SDL_Texture* m_texture;
	SDL_Rect* m_sRect;
	SDL_Rect* m_dRect;

	c2v m_scale;
	c2v m_position;

	std::string m_path;
	
	int m_rotationAngle;
	int m_width;
	int m_height;

	int m_scaledWidth;
	int m_scaledHeight;


};
#endif // !SPRITECOMPONENT_H
