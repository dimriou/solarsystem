#include <stdio.h>		// - Just for some ASCII messages
#include <stdlib.h>		// rand,srand
#include <time.h>		// time
#include <iostream>		// cout
#include <fstream>		// ifstream
#include <sstream>		// istringstream
#include "gl/glut.h"	// - An interface and windows 
						//   management library
#include "visuals.h"	// Header file for our OpenGL functions
#include <math.h>

using namespace std;

// Planet model
model planet;

//Animation parameters
bool pause = false;

// Star appearance definitions
#define STARS 150
#define UPPER_LIMIT 1100		// distance maximum
#define LOWER_LIMIT 450			// distance minimum
float Radius_stars = 1.0;

// Camera properties
int CameraAngle_X = 0;
int CameraAngle_Y = 0;

// Planets' parameters
float rotation = 0.0;
float revolution[5] = { 0.0 };

//Stars array
float Stars[STARS][3];


// Sun parameters
float Opacity = 0.51;
float Radius = 1.0;
float v = 1.0;					// opacity animation speed


void onPauseText(const char *str, float size)
{
	glPushMatrix();

	// Set Material for the text
	// Color: full red
	GLfloat specref[4];
	specref[0] = 1.0; specref[1] = 0.0; specref[2] = 0.0; specref[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, specref);
	specref[0] = 1.0; specref[1] = 0.0; specref[2] = 0.0; specref[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, specref);
	specref[0] = 1.0; specref[1] = 0.0; specref[2] = 0.0; specref[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);
	// Translate to distance equal to sun's distance
	glTranslatef(0, 0, -8.0);
	// Rotate to the reverse angles of the camera so the camera's rotation is cancelled out
	glRotatef(-CameraAngle_Y, 0, 1, 0);
	glRotatef(-CameraAngle_X, 1, 0, 0);
	// Translate back to a position where it is visible (-2.0 on z-axis)
	glTranslatef(-1.0, 0.5, 6.0);
	// Scale down so that it looks nice
	glScalef(size, size, size);

	// print characters
	for (int i = 0; i<strlen(str); i++)		
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);

	glPopMatrix();

}

void set_SunMaterial(bool mode,float opacity) {
	if (mode) {
		//inner sun sphere
		//glColor4f(0.992, 0.721,0.074,1);
		// Material Properties
		GLfloat specref[4];
		specref[0] = 0.0; specref[1] = 0.0; specref[2] = 0.0; specref[3] = opacity;
		glMaterialfv(GL_FRONT, GL_AMBIENT, specref);
		specref[0] = 0.992; specref[1] = 0.722; specref[2] = 0.075; specref[3] = opacity;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, specref);
		specref[0] = 0.992; specref[1] = 0.722; specref[2] = 0.175; specref[3] = opacity;
		glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
		glMaterialf(GL_FRONT, GL_SHININESS, 0);
	}
	else {
		// outer sphere
		//glColor4f(0.98, 0.725, 0.243, opacity);
		// Material Properties
		GLfloat specref[4];
		specref[0] = 0.0; specref[1] = 0.0; specref[2] = 0.0; specref[3] = opacity;
		glMaterialfv(GL_FRONT, GL_AMBIENT, specref);
		specref[0] = 0.98; specref[1] = 0.725; specref[2] = 0.243; specref[3] = opacity;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, specref);
		specref[0] = 0.98; specref[1] = 0.725; specref[2] = 0.343; specref[3] = opacity;
		glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
		glMaterialf(GL_FRONT, GL_SHININESS, 0.2);
	}	
}

void sun(float opacity,float radius)
{
	
	//Sun (inner sphere)
	glPushMatrix();
	set_SunMaterial(true,1.0);
	glTranslatef(0, 0, -8.0);
	glutSolidSphere(1,150,150);
	glPopMatrix(); 

	//Sun (outer sphere)
	glPushMatrix();
	set_SunMaterial(false,opacity);
	glTranslatef(0, 0, -8.0);	
	glutSolidSphere(radius, 150, 150);
	// max radius value: 1.19

	glPopMatrix();


}

void set_StarsMaterial(bool mode,float opacity) {
	if (mode) {
		//inner sphere
		//glColor4f(0.757, 0.855, 0.839, 1);
		// Material Properties
		GLfloat specref[4];
		specref[0] = 0.0; specref[1] = 0.0; specref[2] = 0.0; specref[3] = opacity;
		glMaterialfv(GL_FRONT, GL_AMBIENT, specref);
		specref[0] = 0.757; specref[1] = 0.855; specref[2] = 0.839; specref[3] = opacity;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, specref);
		specref[0] = 0.757; specref[1] = 0.855; specref[2] = 0.939; specref[3] = opacity;
		glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
		glMaterialf(GL_FRONT, GL_SHININESS, 0);
	}
	else {
		// outer sphere
		//glColor4f(1,1,1, opacity);
		// Material Properties
		GLfloat specref[4];
		specref[0] = 0.0; specref[1] = 0.0; specref[2] = 0.0; specref[3] = opacity;
		glMaterialfv(GL_FRONT, GL_AMBIENT, specref);
		specref[0] = 1.0; specref[1] = 1.0; specref[2] = 1.0; specref[3] = opacity;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, specref);
		specref[0] = 1.0; specref[1] = 1.0; specref[2] = 1.0; specref[3] = opacity;
		glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
		glMaterialf(GL_FRONT, GL_SHININESS, 0.2);
	}
}

void star( float opacity, float radius,int index)
{
	
	//Sun (inner sphere)
	glPushMatrix();	
	set_StarsMaterial(true,1.0);
	glRotatef(Stars[index][1], 0, 1, 0);
	glRotatef(Stars[index][0], 1, 0, 0);
	glTranslatef(0, 0, Stars[index][2]);	
	glutSolidSphere(1, 8, 5);
	glPopMatrix();

	//Sun (outer sphere)
	glPushMatrix();
	set_StarsMaterial(false,opacity);
	glRotatef(Stars[index][1], 0, 1, 0);
	glRotatef(Stars[index][0], 1, 0, 0);
	glTranslatef(0, 0, Stars[index][2]);	
	glutSolidSphere( radius, 8, 5);
	// initial radius value: 1.19
	glPopMatrix();

}


void Render()
{    
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Camera Setup
	glTranslatef(0, 0, -8);	
	glRotatef(CameraAngle_X, 1, 0, 0);
	glRotatef(CameraAngle_Y, 0, 1, 0);
	glTranslatef(0, 0, 8);

	//create sun
	sun(Opacity,Radius);

	//Create stars
	for (int i = 0; i < STARS; i++) {
		star(Opacity, Radius_stars,i);
	}
	
	//Planets
	DisplayModel(5.0,planet,PLANET_1,rotation);
	DisplayModel(4.0, planet, PLANET_2, rotation);
	DisplayModel(3.0, planet, PLANET_3, rotation);
	DisplayModel(6.0, planet, PLANET_4, rotation);
	DisplayModel(10.0, planet, PLANET_5, rotation);

	// Show text on pause
	if (pause) {
		onPauseText("PAUSED", 0.004f);
	}

	glutSwapBuffers();
}

//-----------------------------------------------------------


void Idle()
{
	float g = 10;
	float dt = 0.015;

	if (!pause) {
		if ((Opacity > 1.0) || (Opacity < 0.5)) {
			v = -v;
		}
		Opacity = Opacity + v*dt;
		// Allagh klimakas xronou
		Radius = Radius + v*dt*(0.19 / 0.5);
		Radius_stars = Radius_stars + v*dt*(1 / 0.5);
		// Planets' rotation
		rotation += 25 * dt;
		// Planets' revolution
		revolution[0] += fmod(25 * dt, 360);
		revolution[1] += fmod(20 * dt, 360);
		revolution[2] += fmod(15 * dt, 360);
		revolution[3] += fmod(30 * dt, 360);
		revolution[4] += fmod(18 * dt, 360);
	}		

    glutPostRedisplay(); 
}

//-----------------------------------------------------------

void Resize(int w, int h)
{ 
	// define the visible area of the window ( in pixels )
	if (h==0) h=1;
	glViewport(0,0,w,h); 

	// Setup viewing volume
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();

	float aspect = (float)w/(float)h;             // aspect ratio
	gluPerspective(60.0, aspect, 1.0, 1500.0);
}


//-----------------------------------------------------------

void Setup()  
{ 
   // Read planet.obj
   ReadFile(&planet);

   //Enable Face Culling
   glEnable(GL_CULL_FACE);
   glFrontFace(GL_CCW);
 
   // Smooth model shading
   glShadeModel( GL_SMOOTH );

   // Depth test (less or equal)
   glEnable(GL_DEPTH_TEST);
   glDepthFunc( GL_LEQUAL );      
   glClearDepth(1.0); 		      
   
   //Set up light source
   GLfloat light_position[] = { 0.0, 0.0, -5.0, 1.0 };
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   GLfloat ambientLight[] = { 0.5, 0.5, 0.5, 1.0 };
   GLfloat diffuseLight[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };


   glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
   glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);


   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   // Normalize faces' normal vectors so that all have length 1 (the way OpenGL wants them)
   glEnable(GL_NORMALIZE);

   // polygon rendering mode and material properties
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   // Enable blend for the transparent objects using their 'alpha' values
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);

	//Setup stars
	float position_z;
	for (int i = 0; i < STARS; i++) {
		position_z = -LOWER_LIMIT - (rand() % (UPPER_LIMIT - LOWER_LIMIT));
		//z coordinate
		Stars[i][2] = position_z;
		// x rotation angle
		Stars[i][0] = rand() % 360;
		// y rotation angle
		Stars[i][1] = rand() % 360;
	}
}

//-----------------------------------------------------------
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q': exit(0);
		break;
	case 'a': CameraAngle_Y += 5; // move camera to the left (world rotated to the right by Y axis)
		break;
	case 'd': CameraAngle_Y -= 5; // move camera to the right (world rotated to the left by Y axis)
		break;
	case 'w': CameraAngle_X += 5; // move camera up (world rotated down by X axis)
		break;
	case 's': CameraAngle_X -= 5; // move camera down (world rotated up by X axis)
		break;
	case 'p': pause = !pause;	  // toggle pause 
		break;
	default: break;
	}

	glutPostRedisplay();

}
//-----------------------------------------------------------

void ReadFile(model *md)
{

	ifstream obj_file("planet.obj");                   

	if (obj_file.fail())
		exit(1);

	md->vertices = 0;
	md->faces = 0;
	md->normals = 0;

	string line;
	
	while (getline(obj_file,line)) {
		
		if (line.length() > 0){	// ignore empty lines	
			
			if ((line.at(0) == 'v')){	// 'v' or 'vn' - vertexes and normals
				if (line.at(1) == ' '){	// 'v'
					istringstream stm(line);
					stm.ignore(1, ' ');	// ignore 'v'
					stm >> md->obj_points[md->vertices].x;
					stm >> md->obj_points[md->vertices].y;
					stm >> md->obj_points[md->vertices].z;
					md->vertices++;		
				}
				else if (line.at(1) == 'n') {	// 'vn'
					istringstream stm(line);
					stm.ignore(2, ' ');	// ignore 'vn'
					stm >> md->obj_normals[md->normals].x;
					stm >> md->obj_normals[md->normals].y;
					stm >> md->obj_normals[md->normals].z;
					md->normals++;
				}
			}
			else if (line.at(0) == 'f') {	// 'f' - faces	
				// Split line to its components: v1//n1 v2//n2 v3//n3
				int pos;
				pos = line.find("//");
				line.replace(pos, 2, "  ");
				pos = line.find("//",pos+1);
				line.replace(pos, 2, "  ");
				pos = line.find("//",pos+1);
				line.replace(pos, 2, "  ");
				// Create a string stream
				istringstream stm(line);
				// ignore 'f'
				stm.ignore(1, ' ');
				stm >> md->obj_faces[md->faces].vtx[0];
				stm >> md->obj_faces[md->faces].norm[0];
				stm >> md->obj_faces[md->faces].vtx[1];
				stm >> md->obj_faces[md->faces].norm[1];
				stm >> md->obj_faces[md->faces].vtx[2];
				stm >> md->obj_faces[md->faces].norm[2];
				md->faces++;
			}
		}
	}
	obj_file.close();
}
//-----------------------------------------------------------
void DisplayModel(float distance,model md,planet_type type,float angle)
{
	glPushMatrix();
	// Setup material Properties
	GLfloat specref[4];
	switch (type) {

	case PLANET_1:		
		//Mars color
		//RGB: 0.98, 0.392, 0.071
		specref[0] = 0.0; specref[1] = 0.0; specref[2] = 0.0; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_AMBIENT, specref);
		specref[0] = 0.98; specref[1] = 0.392; specref[2] = 0.071; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, specref);
		specref[0] = 0.98; specref[1] = 0.392; specref[2] = 0.171; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
		glMaterialf(GL_FRONT, GL_SHININESS, 2);		
		// Translate to distance equal to sun's distance
		glTranslatef(0, 0,- 8.0);
		// Rotate an already translated planet (simulating planet's revolution)
		glRotatef(revolution[0], 0, 1, 0);
		// Translate to distance equal to (sun - planet)
		glTranslatef(0, 0, -distance);
		// Scale down model to look natural
		glScalef(0.001, 0.001, 0.001);

		break;

	case PLANET_2:
		//Earth color
		//RGB: 0.494, 0.753, 0.933
		specref[0] = 0.0; specref[1] = 0.0; specref[2] = 0.0; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_AMBIENT, specref);
		specref[0] = 0.494; specref[1] = 0.753; specref[2] = 1.0; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, specref);
		specref[0] = 0.494; specref[1] = 0.753; specref[2] = 1.0; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
		glMaterialf(GL_FRONT, GL_SHININESS, 2);
		// Translate to distance equal to sun's distance
		glTranslatef(0, 0, -8.0);
		// Rotate an already translated planet (simulating planet's revolution)
		glRotatef(revolution[1], 0, 1, 0);
		// Translate to distance equal to (sun - planet)
		glTranslatef(0, 0, -distance);
		// Scale down model to look natural
		glScalef(0.0008, 0.0008, 0.0008);

		break;

	case PLANET_3:
		//Moon color
		//RGB: 0.839, 0.835, 0.753
		specref[0] = 0.0; specref[1] = 0.0; specref[2] = 0.0; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_AMBIENT, specref);
		specref[0] = 0.839; specref[1] = 0.835; specref[2] = 0.753; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, specref);
		specref[0] = 0.839; specref[1] = 0.835; specref[2] = 0.653; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
		glMaterialf(GL_FRONT, GL_SHININESS, 2);
		// Translate to distance equal to sun's distance
		glTranslatef(0, 0, -8.0);
		// Rotate an already translated planet (simulating planet's revolution)
		glRotatef(revolution[2], 0, 1, 0);
		// Translate to distance equal to (sun - planet)
		glTranslatef(0, 0, -distance);
		// Scale down model to look natural
		glScalef(0.0005, 0.0005, 0.0005);

		break;

	case PLANET_4:
		//Red color
		//RGB: 0.667, 0.004, 0.078
		specref[0] = 0.0; specref[1] = 0.0; specref[2] = 0.0; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_AMBIENT, specref);
		specref[0] = 0.667; specref[1] = 0.004; specref[2] = 0.078; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, specref);
		specref[0] = 0.667; specref[1] = 0.004; specref[2] = 0.078; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
		glMaterialf(GL_FRONT, GL_SHININESS, 2);
		// Translate to distance equal to sun's distance
		glTranslatef(0, 0, -8.0);
		// Rotate an already translated planet (simulating planet's revolution)
		glRotatef(revolution[3], 0, 1, 0);
		// Translate to distance equal to (sun - planet)
		glTranslatef(0, 0, -distance);
		// Scale down model to look natural
		glScalef(0.0007, 0.0007, 0.0007);

		break;

	case PLANET_5:
		// Mystery color
		//RGB: 1, 0.729, 0.824
		specref[0] = 0.0; specref[1] = 0.0; specref[2] = 0.0; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_AMBIENT, specref);
		specref[0] = 1.0; specref[1] = 0.729; specref[2] = 0.824; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, specref);
		specref[0] = 0.0; specref[1] = 0.0; specref[2] = 0.0; specref[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
		glMaterialf(GL_FRONT, GL_SHININESS, 2);
		// Translate to distance equal to sun's distance
		glTranslatef(0, 0, -8.0);
		// Rotate an already translated planet (simulating planet's revolution)
		glRotatef(revolution[4], 0, 1, 0);
		// Translate to distance equal to (sun - planet)
		glTranslatef(0, 0, -distance);
		// Scale down model to look natural
		glScalef(0.0018, 0.0018, 0.0018);

		break;

	default:
		break;
	}
	
	// Rotate planet along its vertical axis (simulating planet's rotation)
	glRotatef(angle, 0, 1, 0);

	// Create model faces
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < md.faces; i++)
	{
		glNormal3f(md.obj_normals[md.obj_faces[i].norm[0] - 1].x,md.obj_normals[md.obj_faces[i].norm[0] - 1].y,md.obj_normals[md.obj_faces[i].norm[0] - 1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[0] - 1].x, md.obj_points[md.obj_faces[i].vtx[0] - 1].y, md.obj_points[md.obj_faces[i].vtx[0] - 1].z);

		glNormal3f(md.obj_normals[md.obj_faces[i].norm[1] - 1].x,md.obj_normals[md.obj_faces[i].norm[1] - 1].y,md.obj_normals[md.obj_faces[i].norm[1] - 1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[1] - 1].x, md.obj_points[md.obj_faces[i].vtx[1] - 1].y, md.obj_points[md.obj_faces[i].vtx[1] - 1].z);

		glNormal3f(md.obj_normals[md.obj_faces[i].norm[2] - 1].x,md.obj_normals[md.obj_faces[i].norm[2] - 1].y,md.obj_normals[md.obj_faces[i].norm[2] - 1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[2] - 1].x, md.obj_points[md.obj_faces[i].vtx[2] - 1].y, md.obj_points[md.obj_faces[i].vtx[2] - 1].z);
	}

	glEnd();
	glPopMatrix();
}
