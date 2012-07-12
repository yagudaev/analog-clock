/*
 * main.c
 *
 * Example of using the sprite class to create an Analog Clock.
 *
 *  Created on: 2010-12-26
 *      Author: Michael Yagudaev
 *      Copyright: yagudaev.com
 *      Version: $0.1.0$
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
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <time.h>
#include "Sprite.h"

#define ESCAPE_KEY 27

using namespace std;

static const time_t TIME_INTERVAL = 1; // in seconds

static int windowWidth = 524;
static int windowHeight = 524;

static Sprite *clockFace = NULL;
static Sprite *hoursHand = NULL;
static Sprite *minutesHand = NULL;
static Sprite *secondsHand = NULL;

void display (void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);

	// draw the clock
	clockFace->setPivot(0.5, 0.5);
	clockFace->setX(0);
	clockFace->setY(0);
	clockFace->draw();

	hoursHand->setX(0);
	hoursHand->setY(0);
	hoursHand->setPivot(0.5, 0.075);
	hoursHand->draw();

	minutesHand->setX(0);
	minutesHand->setY(0);
	minutesHand->setPivot(0.5, 0.0566);
	minutesHand->draw();

	secondsHand->setX(0);
	secondsHand->setY(0);
	secondsHand->setPivot(0.5, 0.0545);
	secondsHand->draw();

	glFlush();
	glutSwapBuffers();
	glDisable(GL_TEXTURE_2D);
}

void reshape(int w, int h)
{
	windowWidth = w;
	windowHeight = h;

	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w/2, w/2, -h/2, h/2, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init (void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_FLAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	Sprite::enable2D();

	clockFace = new Sprite("graphics/clockface.bmp");
	hoursHand = new Sprite("graphics/hours_hand.bmp");
	minutesHand = new Sprite("graphics/minutes_hand.bmp");
	secondsHand = new Sprite("graphics/seconds_hand.bmp");

	// clear buffer and display image
	reshape(windowWidth, windowHeight);
	display();
}

void clockAnimation()
{
	static time_t lastRendered = 0;

	time_t unixTime = time(NULL);
	struct tm *currentTime = localtime(&unixTime);

	// only re-render the screen after the time interval has passed. Remember we
	// do not want to render the screen to often because otherwise it will become
	// too expensive.
	if(lastRendered == 0 || unixTime - lastRendered >= TIME_INTERVAL)
	{
		// note we use negative angles because in math angles are always measured counter-clockwise
		// so by using a negative angle we will get a clockwise angle needed for our clock.
		hoursHand->setAngle(-1 * (30 * currentTime->tm_hour + ((int)(6 * currentTime->tm_min / 90.0)) * 7.5));
		minutesHand->setAngle(-1 * 6 * currentTime->tm_min);
		secondsHand->setAngle(-1 * 6 * currentTime->tm_sec);

		lastRendered = unixTime;
		glutPostRedisplay();
	}
}

/**
 * Clean up before exiting the program
 */
void cleanup()
{
	delete clockFace;
	delete hoursHand;
	delete minutesHand;
	delete secondsHand;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ESCAPE_KEY:
		cleanup();
		exit(0);
		break;
	default:
		break;
	}
}

int main (int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Analog Clock");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(clockAnimation);
	glutMainLoop();

	return 0;
}
