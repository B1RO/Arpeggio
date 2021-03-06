#include <GLFW/glfw3.h>


#include <math.h>


/* PI */
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif


void drawSphere(double r, int lats, int longs) {
	int i, j;
	for(i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
		double z0  = r * sin(lat0);
		double zr0 = r *  cos(lat0);

		double lat1 = M_PI * (-0.5 + (double) i / lats);
		double z1  = r * sin(lat1);
		double zr1 = r * cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double) (j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}
}


void drawCone(GLdouble base, GLdouble height, GLint slices, GLint stacks)
{

	// draw the upper part of the cone
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, height);
	for (int angle = 0; angle < 360; angle++) {
		glVertex3f(sin((double)angle) * base, cos((double)angle) * base, 0.f);
	}
	glEnd();

	// draw the base of the cone
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);
	for (int angle = 0; angle < 360; angle++) {
		// normal is just pointing down
		glNormal3f(0, -1, 0);
		glVertex3f(sin((double)angle) * base, cos((double)angle) * base, 0.f);
	}
	glEnd();
}

void drawCuboid(float width,float height)
{
	float halfWidth = width / 2;
	float halfHeight = height / 2;
	cout << "width/2: " << halfWidth << " height/2: " << halfHeight<< endl;

	glBegin(GL_QUADS);

	//Front
	glNormal3f(0.0f, 0.0f, 1.0f);
	//glNormal3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-halfWidth, -halfHeight, halfWidth);
	//glNormal3f(1.0f, 0.0f, 1.0f);
	glVertex3f(halfWidth, -halfHeight, halfWidth);
	//glNormal3f(1.0f, 0.0f, 1.0f);
	glVertex3f(halfWidth, halfHeight, halfWidth);
	//glNormal3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-halfWidth, halfHeight, halfWidth);

	//Right
	glNormal3f(1.0f, 0.0f, 0.0f);
	//glNormal3f(1.0f, 0.0f, -1.0f);
	glVertex3f(halfWidth, -halfHeight, -halfWidth);
	//glNormal3f(1.0f, 0.0f, -1.0f);
	glVertex3f(halfWidth, halfHeight, -halfWidth);
	//glNormal3f(1.0f, 0.0f, 1.0f);
	glVertex3f(halfWidth, halfHeight, halfWidth);
	//glNormal3f(1.0f, 0.0f, 1.0f);
	glVertex3f(halfWidth, -halfHeight, halfWidth);

	//Back
	glNormal3f(0.0f, 0.0f, -1.0f);
	//glNormal3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-halfWidth, -halfHeight, -halfWidth);
	//glNormal3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-halfWidth, halfHeight, -halfWidth);
	//glNormal3f(1.0f, 0.0f, -1.0f);
	glVertex3f(halfWidth, halfHeight, -halfWidth);
	//glNormal3f(1.0f, 0.0f, -1.0f);
	glVertex3f(halfWidth, -halfHeight, -halfWidth);

	//Left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	//glNormal3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-halfWidth, -halfHeight, -halfWidth);
	//glNormal3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-halfWidth, -halfHeight, halfWidth);
	//glNormal3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-halfWidth, halfHeight, halfWidth);
	//glNormal3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-halfWidth, halfHeight, -halfWidth);

	glEnd();
}

void drawSquare()
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, 1, 1);
	glEnd();
}