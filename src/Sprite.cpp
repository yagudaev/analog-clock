/*
 * Sprite.cpp
 *
 *  Created on: 2010-08-16
 *      Author: Michael Yagudaev
 *   Copyright: yagudaev.com
 *     version: $0.1.1$
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

#include <GL/glut.h>
#include <iostream>
#include <cstring>
#include "Sprite.h"
#include "ImageLoader.h"

///////////////////////////////////////////////////////////////////////////////
// implementation is based on this article:
// http://www.gamedev.net/reference/articles/article2429.asp
///////////////////////////////////////////////////////////////////////////////

Sprite::Sprite(string filename)
{
	image = new ImageLoader(filename.c_str());
	angle = 0;
	x = 0.0;
	y = 0.0;
	setPivot(0.0, 0.0);
	setScale(1.0, 1.0);
}

Sprite::~Sprite()
{
	delete image;
}

void Sprite::rotate(GLint degrees)
{
	angle += degrees;
}

void Sprite::setAngle(GLint angle)
{
	this->angle = angle;
}

GLint Sprite::getAngle() const
{
	return angle;
}

void Sprite::enable2D()
{
	GLint iViewport[4];

	// Get a copy of the viewport
	glGetIntegerv( GL_VIEWPORT, iViewport );

	// Save a copy of the projection matrix so that we can restore it
	// when it's time to do 3D rendering again.
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	// Set up the orthographic projection
	glOrtho( iViewport[0], iViewport[0] + iViewport[2],
			 iViewport[1] + iViewport[3], iViewport[1], -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	// Make sure depth testing and lighting are disabled for 2D rendering until
	// we are finished rendering in 2D
	glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
}

void Sprite::disable2D()
{
	glPopAttrib();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}

void Sprite::initScene()
{

	// Disable lighting
	glDisable( GL_LIGHTING );

	// Disable dithering
	glDisable( GL_DITHER );

	// Disable blending (for now)
	glDisable( GL_BLEND );

	// Disable depth testing
	glDisable( GL_DEPTH_TEST );

	// Is the extension supported on this driver/card?
	if( !isExtensionSupported( "GL_ARB_texture_rectangle" ) )
	{
		cout << "ERROR: Texture rectangles not supported on this video card!" << endl;
		exit(-1);
	}

	// NOTE: If your comp doesn't support GL_NV_texture_rectangle, you can try
	// using GL_EXT_texture_rectangle if you want, it should work fine.

	// Enable the texture rectangle extension
	glEnable( GL_TEXTURE_RECTANGLE_ARB );

	// Generate one texture ID
	glGenTextures( 1, &textureID );
	// Bind the texture using GL_TEXTURE_RECTANGLE_NV
	glBindTexture( GL_TEXTURE_RECTANGLE_ARB, textureID );
	// Enable bilinear filtering on this texture
	glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// Write the 32-bit RGBA texture buffer to video memory
	glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, image->getWidth(), image->getHeight(),
				  0, GL_RGBA, GL_UNSIGNED_BYTE, image->getPixelData() );
}

GLfloat Sprite::getPivotX() const
{
	return pivotX;
}

GLfloat Sprite::getPivotY() const
{
	return pivotY;
}

void Sprite::setPivot(GLfloat pivotX, GLfloat pivotY)
{
	GLfloat deltaPivotX = pivotX - getPivotX();
	GLfloat deltaPivotY = pivotY - getPivotY();

	this->pivotX = pivotX;
	this->pivotY = pivotY;

	x += deltaPivotX * image->getWidth();
	y += deltaPivotY * image->getHeight();
}

void Sprite::setPivot(const Sprite &obj)
{
	GLint worldX; // this x location if pivot was at setPivot(0, 0)
	GLint worldY; // this y location if  pivot was at setPivot(0, 0)
	GLfloat newPivotX;
	GLfloat newPivotY;

	worldX = x - getPivotX() * image->getWidth();
	worldY = y - getPivotY() * image->getHeight();

	newPivotX = (float)(obj.x - worldX) / image->getWidth();
	newPivotY = (float)(obj.y - worldY) / image->getHeight();

	setPivot(newPivotX, newPivotY);
}

bool Sprite::isExtensionSupported(const char *extension) const
{

	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	/* Extension names should not have spaces. */
	where = (GLubyte *) strchr(extension, ' ');

	if (where || *extension == '\0')
	{
		return false;
	}

	extensions = glGetString(GL_EXTENSIONS);

	/* It takes a bit of care to be fool-proof about parsing the
	 OpenGL extensions string. Don't be fooled by sub-strings,
	 etc. */
	start = extensions;

	for (;;)
	{
		where = (GLubyte *) strstr((const char *) start, extension);

		if (!where)
		{
			break;
		}

		terminator = where + strlen(extension);

		if (where == start || *(where - 1) == ' ')
		{
			if (*terminator == ' ' || *terminator == '\0')
			{
				return true;
			}
		}

		start = terminator;
	}

	return false;
}

void Sprite::draw()
{
	initScene();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	// Set the primitive color to white
	glColor3f(1.0f, 1.0f, 1.0f);
	// Bind the texture to the polygons
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, textureID);

	glPushMatrix();

	GLfloat transX = 1;
	GLfloat transY = 1;

	if(x != 0.0)
	{
		transX = x;
	}

	if(y != 0.0)
	{
		transY = y;
	}

	glLoadIdentity();
	glTranslatef(transX, transY, 0);
	glScalef(scaleX, scaleY, 1.0);
	glRotatef(angle, 0.0, 0.0, 1.0);

	// Render a quad
	// Instead of the using (s,t) coordinates, with the  GL_NV_texture_rectangle
	// extension, you need to use the actual dimensions of the texture.
	// This makes using 2D sprites for games and emulators much easier now
	// that you won't have to convert :)
	//
	// convert the coordinates so that the bottom left corner changes to
	// (0, 0) -> (1, 1) and the top right corner changes from (1, 1) -> (0, 0)
	// we will use this new coordinate system to calculate the location of the sprite
	// in the world coordinates to do the rotation and scaling. This mapping is done in
	// order to make implementation simpler in this class and let the caller keep using
	// the standard OpenGL coordinates system (bottom left corner at (0, 0))
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0);
		glVertex2i(-pivotX * image->getWidth(), -pivotY * image->getHeight());

		glTexCoord2i(0, image->getHeight());
		glVertex2i(-pivotX * image->getWidth(), (1 - pivotY) * image->getHeight());

		glTexCoord2i(image->getWidth(), image->getHeight());
		glVertex2i( (1 - pivotX) * image->getWidth(), (1 - pivotY) * image->getHeight());

		glTexCoord2i(image->getWidth(), 0);
		glVertex2i( (1 - pivotX) * image->getWidth(), -pivotY * image->getHeight());
	glEnd();

	glPopMatrix();
}

void Sprite::setX(GLdouble x)
{
	this->x = x;
}

void Sprite::setY(GLdouble y)
{
	this->y = y;
}

void Sprite::setScale(GLfloat x, GLfloat y)
{
	scaleX = x;
	scaleY = y;
}

GLint Sprite::getHeight() const
{
	return image->getHeight() * scaleY;
}

GLint Sprite::getWidth() const
{
	return image->getWidth() * scaleX;
}

GLdouble Sprite::getX() const
{
	return x;
}

GLdouble Sprite::getY() const
{
	return y;
}
