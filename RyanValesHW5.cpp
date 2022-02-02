/*
Ryan Vales
11/15/21
S00339723
CSCI 4550 : Computer Graphics
Homework 5
*/

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "getBMP.h"

using namespace std;

#define MSEC_PER_FRAME 16.6 // 60 fps => 1000 / 60

bool cubeShouldRotate = true;
bool light0_ON = GL_TRUE; // Turn light ON and OFF
float cubeRotateAngle = 0.0;
unsigned int texture[4]; // Array of texture ids.

// Load external textures.
void loadTextures() {
	// Local storage for image data.
	imageFile* image[4];

	// Load the image.
	image[0] = getBMP("./fred.bmp");
	image[1] = getBMP("./halo4.bmp");
	image[2] = getBMP("./infinite.bmp");
	image[3] = getBMP("./jorge.bmp");

	// Bind fred image to texture[0]
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->width, image[0]->height, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);

	// Set texture parameters for wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE

	// Set texture parameters for filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Bind halo4 image to texture[1]
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->width, image[1]->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);

	// Set texture parameters for wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE

	// Set texture parameters for filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Bind infinite image to texture[2]
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[2]->width, image[2]->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[2]->data);

	// Set texture parameters for wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE

	// Set texture parameters for filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Bind jorge image to texture[3]
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[3]->width, image[3]->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[3]->data);

	// Set texture parameters for wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE

	// Set texture parameters for filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void drawAxis(float size) {
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(-size, 0.0, 0.0);
	glVertex3f(size, 0.0, 0.0);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, -size, 0.0);
	glVertex3f(0.0, size, 0.0);
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, -size);
	glVertex3f(0.0, 0.0, size);
	glEnd();
}

void drawMeshedCube() {
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Activate texture object for face 1.
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Face 1 material property vectors.
	float matAmb1[] = { 0.2, 0.2, 0.2, 1.0 };
	float matDif1[] = { 1.0, 1.0, 1.0, 1.0 };
	float matSpec1[] = { 1.0, 1.0, 1.0, 1.0 };

	// Material property vectors for all faces.
	float matShininess[] = { 20 };
	float matEmission[] = { 0.0, 0.0, 0.0, 1.0 };

	glNormal3f(-1.0, 0.0, 0.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);

	// Face 1(Left)
	float s = 0.0;
	float t = 0.0;
	for (float y = -5.0; y < 5.0; y += 2.5) {
		glBegin(GL_TRIANGLE_STRIP);
		for (float z = -5.0; z < 7.5; z += 2.5) {
			glTexCoord2f(s, t); glVertex3f(-5.0, y, z);
			t += 1.0 / 4.0;
			glTexCoord2f(s, t); glVertex3f(-5.0, y + 2.5, z);
			t -= 1.0 / 4.0;
			s += 1.0 / 4.0;
		}
		t += 1.0 / 4.0;
		s = 0.0;
		glEnd();
	}

	// Activate texture object for face 2.
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	// Face 2 material property vectors.
	float matAmb2[] = { 0.2, 0.2, 0.2, 1.0 };
	float matDif2[] = { 1.0, 1.0, 1.0, 1.0 };
	float matSpec2[] = { 1.0, 1.0, 1.0, 1.0 };

	glNormal3f(0.0, 0.0, 1.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
	
	// Face 2(Front)
	s = 0.0;
	t = 0.0;
	for (float y = -5.0; y < 5.0; y += 2.5) {
		glBegin(GL_TRIANGLE_STRIP);
		for (float x = -5.0; x < 7.5; x += 2.5) {
			glTexCoord2f(s, t); glVertex3f(x, y, 5.0);
			t += 1.0 / 4.0;
			glTexCoord2f(s, t); glVertex3f(x, y + 2.5, 5.0);
			t -= 1.0 / 4.0;
			s += 1.0 / 4.0;
		}
		t += 1.0 / 4.0;
		s = 0.0;
		glEnd();
	}

	// Activate texture object for face 3.
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	// Face 3 material property vectors.
	float matAmb3[] = { 0.2, 0.2, 0.2, 1.0 };
	float matDif3[] = { 1.0, 1.0, 1.0, 1.0 };
	float matSpec3[] = { 0.0, 0.0, 0.0, 1.0 };

	glNormal3f(1.0, 0.0, 0.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);

	// Face 3(Right)
	s = 0.0;
	t = 0.0;
	for (float y = -5.0; y < 5.0; y += 2.5) {
		glBegin(GL_TRIANGLE_STRIP);
		for (float z = -5.0; z < 7.5; z += 2.5) {
			glTexCoord2f(s, t); glVertex3f(5.0, y, z);
			t += 1.0 / 4.0;
			glTexCoord2f(s, t); glVertex3f(5.0, y + 2.5, z);
			t -= 1.0 / 4.0;
			s += 1.0 / 4.0;
		}
		t += 1.0 / 4.0;
		s = 0.0;
		glEnd();
	}

	// Activate texture object for face 4.
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	float matAmb4[] = { 0.2, 0.2, 0.2, 1.0 };
	float matDif4[] = { 1.0, 1.0, 1.0, 1.0 };
	float matSpec4[] = { 0.0, 0.0, 0.0, 1.0 };

	glNormal3f(0.0, 0.0, -1.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb4);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif4);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec4);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);

	// Face 4(Back)
	s = 0.0;
	t = 0.0;
	for (float y = -5.0; y < 5.0; y += 2.5) {
		glBegin(GL_TRIANGLE_STRIP);
		for (float x = -5.0; x < 7.5; x += 2.5) {
			glTexCoord2f(s, t); glVertex3f(x, y, -5.0);
			t += 1.0 / 4.0;
			glTexCoord2f(s, t); glVertex3f(x, y + 2.5, -5.0);
			t -= 1.0 / 4.0;
			s += 1.0 / 4.0;
		}
		t += 1.0 / 4.0;
		s = 0.0;
		glEnd();
	}

	// Face 5 material property vectors.
	float matAmb5[] = { 0.2, 0.2, 0.2, 1.0 };
	float matDif5[] = { 0.8, 0.0, 0.0, 1.0 };
	float matSpec5[] = { 1.0, 1.0, 1.0, 1.0 };

	glNormal3f(0.0, 1.0, 0.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb5);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif5);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec5);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);

	// Face 5(Top)
	for (float z = -5.0; z < 5.0; z += 2.5) {
		glBegin(GL_TRIANGLE_STRIP);
		for (float x = -5.0; x < 7.5; x += 2.5) {
			glVertex3f(x, 5.0, z);
			glVertex3f(x, 5.0, z + 2.5);
		}
		glEnd();
	}

	// Face 6 material property vectors.
	float matAmb6[] = { 0.2, 0.2, 0.2, 1.0 };
	float matDif6[] = { 0.0, 0.0, 0.0, 1.0 };
	float matSpec6[] = { 0.0, 0.0, 0.0, 1.0 };

	glNormal3f(0.0, -1.0, 0.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb6);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif6);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec6);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);

	// Face 6(Bottom)
	for (float z = -5.0; z < 5.0; z += 2.5) {
		glBegin(GL_TRIANGLE_STRIP);
		for (float x = -5.0; x < 7.5; x += 2.5) {
			glVertex3f(x, -5.0, z);
			glVertex3f(x,-5.0, z + 2.5);
		}
		glEnd();
	}
}

void drawCube() {
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Activate texture object for face 1.
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Face 1 material property vectors.
	float matAmb1[] = { 0.2, 0.2, 0.2, 1.0 };
	float matDif1[] = { 1.0, 1.0, 1.0, 1.0 };
	float matSpec1[] = { 1.0, 1.0, 1.0, 1.0 };
	
	// Material property vectors for all faces.
	float matShininess[] = { 20 };	
	float matEmission[] = { 0.0, 0.0, 0.0, 1.0 }; 

	glNormal3f(-1.0, 0.0, 0.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);

	// Face 1(Left)
	glBegin(GL_POLYGON);
	
	glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 5.0, -5.0); // Top left vertex
	glTexCoord2f(1.0, 1.0); glVertex3f(-5.0, 5.0, 5.0); // Top right vertex
	glTexCoord2f(1.0, 0.0); glVertex3f(-5.0, -5.0, 5.0); // Bottom right vertex
	glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, -5.0, -5.0); // Bottom left vertex
	
	glEnd();

	// Activate texture object for face 2.
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	// Face 2 material property vectors.
	float matAmb2[] = { 0.2, 0.2, 0.2, 1.0 };
	float matDif2[] = { 1.0, 1.0, 1.0, 1.0 };
	float matSpec2[] = { 1.0, 1.0, 1.0, 1.0 };

	glNormal3f(0.0, 0.0, 1.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);

	// Face 2(Front)
	glBegin(GL_POLYGON);

	glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 5.0, 5.0); // Top left vertex
	glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 5.0, 5.0); // Top right vertex
	glTexCoord2f(1.0, 0.0); glVertex3f(5.0, -5.0, 5.0); // Bottom right vertex
	glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, -5.0, 5.0); // Bottom left vertex

	glEnd();

	// Activate texture object for face 3.
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	// Face 3 material property vectors.
	float matAmb3[] = { 0.2, 0.2, 0.2, 1.0 };
	float matDif3[] = { 1.0, 1.0, 1.0, 1.0 };
	float matSpec3[] = { 0.0, 0.0, 0.0, 1.0 };

	glNormal3f(1.0, 0.0, 0.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);

	// Face 3(Right)
	glBegin(GL_POLYGON);

	glTexCoord2f(0.0, 1.0); glVertex3f(5.0, 5.0, 5.0); // Top left vertex
	glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 5.0, -5.0); // Top right vertex
	glTexCoord2f(1.0, 0.0); glVertex3f(5.0, -5.0, -5.0); // Bottom right vertex
	glTexCoord2f(0.0, 0.0); glVertex3f(5.0, -5.0, 5.0); // Bottom left vertex

	glEnd();

	// Activate texture object for face 4.
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	// Face 4 material property vectors.
	float matAmb4[] = { 0.2, 0.2, 0.2, 1.0 };
	float matDif4[] = { 1.0, 1.0, 1.0, 1.0 };
	float matSpec4[] = { 0.0, 0.0, 0.0, 1.0 };

	glNormal3f(0.0, 0.0, -1.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb4);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif4);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec4);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);

	// Face 4(Back)
	glBegin(GL_POLYGON);

	glTexCoord2f(0.0, 1.0); glVertex3f(5.0, 5.0, -5.0); // Top left vertex
	glTexCoord2f(1.0, 1.0); glVertex3f(-5.0, 5.0, -5.0); // Top right vertex
	glTexCoord2f(1.0, 0.0); glVertex3f(-5.0, -5.0, -5.0); // Bottom right vertex
	glTexCoord2f(0.0, 0.0); glVertex3f(5.0, -5.0, -5.0); // Bottom left vertex

	glEnd();

	// Face 5 material property vectors.
	float matAmb5[] = { 0.2, 0.2, 0.2, 1.0 };
	float matDif5[] = { 0.8, 0.0, 0.0, 1.0 };
	float matSpec5[] = { 1.0, 1.0, 1.0, 1.0 };

	glNormal3f(0.0, 1.0, 0.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb5);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif5);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec5);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);

	// Face 5(Top)
	for (float z = -5.0; z < 5.0; z += 2.5) {
		glBegin(GL_TRIANGLE_STRIP);
		for (float x = -5.0; x < 7.5; x += 2.5) {
			glVertex3f(x, 5.0, z);
			glVertex3f(x, 5.0, z + 2.5);
		}
		glEnd();
	}

	// Face 6 material property vectors.
	float matAmb6[] = { 0.2, 0.2, 0.2, 1.0 };
	float matDif6[] = { 0.0, 0.0, 0.0, 1.0 };
	float matSpec6[] = { 0.0, 0.0, 0.0, 1.0 };

	glNormal3f(0.0, -1.0, 0.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb6);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif6);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec6);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);

	// Face 6(Bottom)
	for (float z = -5.0; z < 5.0; z += 2.5) {
		glBegin(GL_TRIANGLE_STRIP);
		for (float x = -5.0; x < 7.5; x += 2.5) {
			glVertex3f(x, -5.0, z);
			glVertex3f(x, -5.0, z + 2.5);
		}
		glEnd();
	}
}

void setLightProperties() {
	// Light property vectors.
	float lightAmb[] = { 1.0, 1.0, 1.0, 1.0 };
	float lightDif[] = { 1.0, 1.0, 1.0, 1.0 };
	float lightSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float globAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
	float lightPos[] = { -20.0, 20.0, 20.0, 0.0 };

	// Light properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmbient); // Global ambient light, applied to all lights
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	if (light0_ON) {
		glEnable(GL_LIGHT0);
	} else {
		glDisable(GL_LIGHT0);
	}

	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // Enable two-sided lighting.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

	// Enable separate specular light calculation.
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
}

// Drawing routine.
void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glEnable(GL_DEPTH_TEST);

	glLoadIdentity();
	gluLookAt(20, 20, 20, 0, 0, 0, 0, 1, 0);

	glFrontFace(GL_CW); //default GL_CCW

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK); //default GL_BACK

	drawAxis(40);

	glEnable(GL_LIGHTING);
	setLightProperties();

	
	glRotatef(cubeRotateAngle, 0.0, 1.0, 0.0);
	drawMeshedCube();
	//drawCube();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glutSwapBuffers();
}

// Initialization routine.
void setup(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Create texture ids. I want to create four texture objects.
	glGenTextures(4, texture);

	// Load external textures.
	loadTextures();

	// Specify how texture values combine with current surface color values.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Turn on OpenGL texturing.
	glEnable(GL_TEXTURE_2D);
}

// OpenGL window reshape routine.
void resize(int w, int h) {
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (float)w / (float)h, 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	case 32:
		if (cubeShouldRotate) {
			cubeShouldRotate = false;
		} else {
			cubeShouldRotate = true;
		}
		break;
	case 65:
		light0_ON = !light0_ON;
		break;
	case 97:
		light0_ON = !light0_ON;
		break;
	default:
		break;
	}
}

void timerFunc(int value) {
	if (cubeShouldRotate) {
		if (cubeRotateAngle < 360.0) {
			cubeRotateAngle += 0.5;
		} else if (cubeRotateAngle == 360.0) {
			cubeRotateAngle = 0.0;
		}
	}
	
	glutPostRedisplay();
	glutTimerFunc(MSEC_PER_FRAME, timerFunc, value);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("HW5.cpp");

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glutTimerFunc(MSEC_PER_FRAME, timerFunc, 1);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}