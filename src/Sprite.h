/*
 * Sprite.h
 *
 *  Created on: 2010-08-16
 *      Author: Michael Yagudaev
 *      Copyright: yagudaev.com
 *      version: $0.1.0$
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 3 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef SPRITE_H_
#define SPRITE_H_

#include <GL/glut.h>
#include <string>

using namespace std;

class ImageLoader;

class Sprite
{
public:
	/**
	 * Enable 2D drawing mode to draw our sprites. This function MUST be called before
	 * any sprite is drawn on screen using the draw method.
	 */
	static void enable2D();

	/**
	 * Disables the 2D drawing. This can be called before you are done drawing all 2D
	 * sprites and want to draw 3D now.
	 */
	static void disable2D();

	Sprite(string filename);
	virtual ~Sprite();

	virtual void draw();
	virtual void rotate(GLint degrees);

	// getter and setter methods
	GLint getAngle() const;
	void setAngle(GLint degrees);
	void setX(GLdouble x);
	void setY(GLdouble y);
	GLint getHeight() const;
	GLint getWidth() const;

	/**
	 * Sets the pivot point in relation to the sprite itself, that is using the object
	 * coordiantes system. In this coordiantes system the bottom left point of the object
	 * is at (0, 0) and the top right is at (1, 1).
	 *
	 * E.g. to set the pivot to be in the middle of the sprite use (0.5, 0.5)
	 * Default values are (1, 1).
	 * @param pivotX Can be any value, but when x is in the range [0, 1] the pivot is inside the
	 *        sprite where 0 is the left edge of the sprite and 1 is the right edge of the sprite.
	 * @param pivotY Can be any value, but when y is in the range [0, 1] the pivot is inside the
	 *        sprite where 0 is the bottom edge of the sprite and 1 is the top edge of the sprite.
	 */
	void setPivot(GLfloat pivotX, GLfloat pivotY);
	GLfloat getPivotX() const;
	GLfloat getPivotY() const;

	GLdouble getX() const;
	GLdouble getY() const;

	/**
	 * Sets the pivot to be at the point where object's pivot is set.
	 * @param obj The reference object to whose pivot we will set this pivot to be.
	 * Note: if the obj pivot changes or the obj moves after the setPivot call has
	 * been issued, the pivot of this object will not reflect this changes. You must
	 * call setPivot again with that object to update the pivot information.
	 */
	void setPivot(const Sprite &obj);

	/**
	 * Sets the scale of the object. A scale of (1.0, 1.0) means the sprite
	 * maintains its original size. Values larger than 1 scale the sprite up
	 * while values less than 1 shrink it down.
	 */
	void setScale(GLfloat x, GLfloat y);
private:
	ImageLoader *image;
	GLuint textureID;
	GLint angle;
	GLdouble x;
	GLdouble y;
	GLfloat pivotX;
	GLfloat pivotY;
	GLfloat scaleX;
	GLfloat scaleY;

	//-----------------------------------------------------------------------------
	// Initializes extensions, textures, render states, etc. before rendering
	//-----------------------------------------------------------------------------
	void initScene();

	/**
	 * A helper function taken from http://www.opengl.org/resources/features/OGLextensions/
	 * to help determine if an OpenGL extension is supported on the target machine at run-time
	 * @param extension The extension name as a string.
	 * @return True if extension is supported and false if it is not.
	 */
	bool isExtensionSupported(const char *extension) const;
};

#endif /* SPRITE_H_ */
