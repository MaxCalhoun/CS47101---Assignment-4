#include <GL/glut.h>
#include <FreeImage/freeimage.h>
#include <corecrt_math_defines.h>
#include <vector>
#include <math.h>
#include "ObjectsToDraw.h"

static int windowWidth = 640;
static int windowHeight = 480;
static float aspectRatio;

GLuint texID[4]; // Texture ID's for the three textures.

char* textureFileNames[4] =
{
	(char*)"textures/dirt.jpg",
	(char*)"textures/metal003.gif",
	(char*)"textures/bark.jpg",
	(char*)"textures/leaves.jpg"
};

GLint leftMouseButton, rightMouseButton;    //status of the mouse buttons
int mouseX = 0, mouseY = 0;                 //last known X and Y of the mouse
bool sphereOn = false;                      //show the camera radius sphere

enum cameraList { CAMERA_INNER = 0, CAMERA_OUTER = 1 };
enum cameraList currentCamera = CAMERA_OUTER;

#define USING_INNER (currentCamera == CAMERA_INNER)

Vector3 outerCamTPR;
Vector3 outerCamXYZ;

Vector3 innerCamXYZ;
Vector3 innerCamTPR;
Vector3 innerCamDir;

Axis axis;
Robot robot;
SceneObject groundPlane;
SceneObject trunk1;
SceneObject trunk2;
SceneObject trunk3;
SceneObject leaves1;
SceneObject leaves2;
SceneObject leaves3;

bool axisEnabled = true;
bool animate = false;
bool solidFigures = true;

const float walkVelocity = 0.005f;
float zVelocity = walkVelocity;

void animateRobot()
{
	robot.animateWalking();

	if (robot.transformation.position.z >= 4.99f && robot.transformation.position.z < 5.01f)
	{
		robot.transformation.rotation += Vector3(0, 180, 0);
		zVelocity = -walkVelocity;
	}

	if (robot.transformation.position.z <= -4.99f && robot.transformation.position.z > -5.01f)
	{
		robot.transformation.rotation += Vector3(0, 180, 0);
		zVelocity = walkVelocity;
	}

	robot.transformation.position += Vector3(0, 0, zVelocity);
}

void toggleWireframe()
{
	if (solidFigures == true)
	{
		robot.makeSolid();
		trunk1.mesh = standardTextureCube;
		trunk2.mesh = standardTextureCube;
		trunk3.mesh = standardTextureCube;
		leaves1.mesh = standardTextureCube;
		leaves2.mesh = standardTextureCube;
		leaves3.mesh = standardTextureCube;
	}
	else
	{
		robot.makeWireframe();
		trunk1.mesh = standardWireCube;
		trunk2.mesh = standardWireCube;
		trunk3.mesh = standardWireCube;
		leaves1.mesh = standardWireCube;
		leaves2.mesh = standardWireCube;
		leaves3.mesh = standardWireCube;
	}
}

void recomputeOrientation(Vector3& xyz, Vector3& tpr)
{
	xyz.x = tpr.z * sinf(tpr.x) * sinf(tpr.y);
	xyz.z = tpr.z * -cosf(tpr.x) * sinf(tpr.y);
	xyz.y = tpr.z * -cosf(tpr.y);
	glutPostRedisplay();
}

void resizeWindow(int w, int h)
{
	aspectRatio = w / (float)h;

	windowWidth = w;
	windowHeight = h;

	//update the viewport to fill the window
	glViewport(0, 0, w, h);

	//update the projection matrix with the new window properties
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspectRatio, 0.1, 100000);

	glMatrixMode(GL_TEXTURE); // Matrix mode for manipulating the texture transform matrix.
	glLoadIdentity();

	glutPostRedisplay();
}

// Based on https://www3.nd.edu/~pbui/teaching/cse.40166.fa10/examples/ex_11.html
void mouseInput(int button, int state, int thisX, int thisY)
{
	//update the left and right mouse button states, if applicable
	if (button == GLUT_LEFT_BUTTON)
		leftMouseButton = state;
	else if (button == GLUT_RIGHT_BUTTON)
		rightMouseButton = state;

	//and update the last seen X and Y coordinates of the mouse
	mouseX = thisX;
	mouseY = thisY;
}

// Based on https://www3.nd.edu/~pbui/teaching/cse.40166.fa10/examples/ex_11.html
void mouseMotion(int x, int y)
{
	if (leftMouseButton == GLUT_DOWN)
	{
		Vector3* curTPR = (USING_INNER ? &innerCamTPR : &outerCamTPR);      //just for conciseness below
		curTPR->x += (x - mouseX) * 0.005;
		curTPR->y += (USING_INNER ? -1 : 1) * (y - mouseY) * 0.005;

		// make sure that phi stays within the range (0, M_PI)
		if (curTPR->y <= 0)
			curTPR->y = 0 + 0.001;
		if (curTPR->y >= M_PI)
			curTPR->y = M_PI - 0.001;

		//update camera (x,y,z) based on (radius,theta,phi)
		if (USING_INNER)
		{
			recomputeOrientation(innerCamDir, innerCamTPR);
			innerCamDir.normalize();
		}
		else
			recomputeOrientation(outerCamXYZ, outerCamTPR);
	}
	else if (rightMouseButton == GLUT_DOWN && !USING_INNER) {
		double totalChangeSq = (x - mouseX) + (y - mouseY);

		Vector3* curTPR = &outerCamTPR;      //just for conciseness below
		curTPR->z += totalChangeSq * 0.01;

		//limit the camera radius to some reasonable values so the user can't get lost
		if (curTPR->z < 2.0)
			curTPR->z = 2.0;
		if (curTPR->z > 10.0 * (currentCamera + 1))
			curTPR->z = 10.0 * (currentCamera + 1);

		//update camera (x,y,z) based on (radius,theta,phi)
		recomputeOrientation(outerCamXYZ, outerCamTPR);
	}

	mouseX = x;
	mouseY = y;
}

void initialize()
{
	glEnable(GL_DEPTH_TEST);

	float lightCol[4] = { 1, 1, 1, 1 };
	float ambientCol[4] = { 0.3, 0.3, 0.3, 1.0 };
	float lPosition[4] = { 10, 10, 10, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, lPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientCol);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_POINT_SMOOTH);

	glShadeModel(GL_SMOOTH);

	glutPostRedisplay();
}

void drawSceneElements(void)
{
	glDisable(GL_LIGHTING);

	if (axisEnabled)
		axis.draw();

	if (solidFigures == false)
	{
		glColor3f(1, 1, 1);
		for (int dir = 0; dir < 2; dir++)
		{
			for (int i = -20; i < 19; i++)
			{
				glBegin(GL_LINE_STRIP);
				for (int j = -20; j < 19; j++)
					glVertex3f(dir < 1 ? i : j, -0.01f, dir < 1 ? j : i);
				glEnd();
			}
		}
	}

	glEnable(GL_LIGHTING);

	if (solidFigures)
		glEnable(GL_TEXTURE_2D);

	// Ground Plane
	glBindTexture(GL_TEXTURE_2D, texID[0]);
	if (solidFigures)
		groundPlane.glDraw();

	// Robot
	glBindTexture(GL_TEXTURE_2D, texID[1]);
	robot.draw();
	
	if (animate)
		animateRobot();

	// Trees
	glBindTexture(GL_TEXTURE_2D, texID[2]);
	trunk1.glDraw();
	trunk2.glDraw();
	trunk3.glDraw();
	
	glBindTexture(GL_TEXTURE_2D, texID[3]);
	leaves1.glDraw();
	leaves2.glDraw();
	leaves3.glDraw();

	glDisable(GL_TEXTURE_2D);
}

// Based on https://www3.nd.edu/~pbui/teaching/cse.40166.fa10/examples/ex_11.html
void drawInnerCamera()
{
	glPushAttrib(GL_LIGHTING_BIT);
	{
		glDisable(GL_LIGHTING);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		{
			glTranslatef(innerCamXYZ.x, innerCamXYZ.y, innerCamXYZ.z);
			glRotatef(-innerCamTPR.x * 180.0 / M_PI, 0, 1, 0);
			glRotatef(innerCamTPR.y * 180.0 / M_PI, 1, 0, 0);
			glScalef(1, -2, 0.75);

			glColor3f(0, 1, 0);
			glutWireCube(1.0f);

			//draw the reels on top of the camera...
			for (int currentReel = 0; currentReel < 2; currentReel++)
			{
				float radius = 0.25f;
				int resolution = 32;
				Vector3 reelCenter = Vector3(0, -0.25 + (currentReel == 0 ? 0 : 0.5), -0.5);

				glBegin(GL_LINES);
				{
					Vector3 s = reelCenter - Vector3(0, 0.25, 0);
					glVertex3f(s.x, s.y, s.z);

					for (int i = 0; i < resolution; i++)
					{
						float ex = -cosf(i / (float)resolution * M_PI);
						float why = sinf(i / (float)resolution * M_PI);
						Vector3 p = Vector3(0, ex * radius, -why * radius * 3) + reelCenter;
						p.glVertex();  //end of this line...
						p.glVertex();  //and start of the next
					}

					Vector3 f = reelCenter + Vector3(0, 0.25, 0);
					f.glVertex();
				}
				glEnd();
			}

			//and just draw the lens shield manually because
			//i don't want to think about shear matrices.
			//clockwise looking from behind the camera:
			float lensOff = 0.3f;
			float lensOut = 0.2f;
			Vector3 v0(0.5, 0.5, -0.5);
			Vector3 v1(-0.5, 0.5, -0.5);
			Vector3 v2(-0.5, 0.5, 0.5);
			Vector3 v3(0.5, 0.5, 0.5);

			Vector3 l0 = v0 + Vector3(lensOut, 0, 0) + Vector3(0, lensOut, 0) + Vector3(0, 0, -lensOff);
			Vector3 l1 = v1 + Vector3(-lensOut, 0, 0) + Vector3(0, lensOut, 0) + Vector3(0, 0, -lensOff);
			Vector3 l2 = v2 + Vector3(-lensOut, 0, 0) + Vector3(0, lensOut, 0) + Vector3(0, 0, lensOff);
			Vector3 l3 = v3 + Vector3(lensOut, 0, 0) + Vector3(0, lensOut, 0) + Vector3(0, 0, lensOff);

			glBegin(GL_LINE_STRIP);
			{
				l0.glVertex();
				l1.glVertex();
				l2.glVertex();
				l3.glVertex();
				l0.glVertex();
			}
			glEnd();

			//and connect the two
			glBegin(GL_LINES);
			{
				v0.glVertex();  l0.glVertex();
				v1.glVertex();  l1.glVertex();
				v2.glVertex();  l2.glVertex();
				v3.glVertex();  l3.glVertex();
			}
			glEnd();

			glPopMatrix();
		}
	}
	glPopAttrib();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float borderWidth = 3;
	//start with the code from the outer camera, which covers the whole screen!
	glViewport(0, 0, windowWidth, windowHeight);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	{
		glLoadIdentity();
		gluOrtho2D(0, 1, 0, 1);
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		if (currentCamera == CAMERA_OUTER)
			glColor3f(1, 0, 0);
		else
			glColor3f(1, 1, 1);

		glBegin(GL_QUADS);
		{
			glVertex2f(0, 0); glVertex2f(0, 1); glVertex2f(1, 1); glVertex2f(1, 0);
		}
		glEnd();

		glViewport(borderWidth, borderWidth, windowWidth - borderWidth * 2, windowHeight - borderWidth * 2);
		glColor3f(0, 0, 0);

		glBegin(GL_QUADS);
		{
			glVertex2f(0, 0); glVertex2f(0, 1); glVertex2f(1, 1); glVertex2f(1, 0);
		}
		glEnd();

		glMatrixMode(GL_PROJECTION);
	}
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	//update the modelview matrix based on the camera's position
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(outerCamXYZ.x, outerCamXYZ.y, outerCamXYZ.z, 0, 0, 0, 0, 1, 0);

	drawSceneElements();
	drawInnerCamera();

	/// draw border and background for preview box in upper corner
	// next, do the code for the inner camera, which only sets in the top-right
	// corner!
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	//step 1: set the projection matrix using gluOrtho2D -- but save it first!
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	{
		glLoadIdentity();
		gluOrtho2D(0, 1, 0, 1);

		//step 2: clear the modelview matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//step 3: set the viewport matrix a little larger than needed...
		glViewport(2 * windowWidth / 3.0 - borderWidth, 2 * windowHeight / 3.0 - borderWidth, windowWidth / 3.0 + borderWidth, windowHeight / 3.0 + borderWidth);

		//step 3a: and fill it with a white rectangle!
		if (currentCamera == CAMERA_OUTER)
			glColor3f(1, 1, 1);
		else
			glColor3f(1, 0, 0);

		glBegin(GL_QUADS);
		{
			glVertex2f(0, 0);
			glVertex2f(0, 1);
			glVertex2f(1, 1);
			glVertex2f(1, 0);
		}
		glEnd();

		//step 4: trim the viewport window to the size we want it...
		glViewport(2 * windowWidth / 3.0, 2 * windowHeight / 3.0, windowWidth / 3.0, windowHeight / 3.0);

		//step 4a: and color it black! the padding we gave it before is now a border.
		glColor3f(0, 0, 0);

		glBegin(GL_QUADS);
		{
			glVertex2f(0, 0);
			glVertex2f(0, 1);
			glVertex2f(1, 1);
			glVertex2f(1, 0);
		}
		glEnd();

		//before rendering the scene in the corner, pop the old projection matrix back
		//and re-enable lighting!
		glMatrixMode(GL_PROJECTION);
	}
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	/// begin drawing scene in upper corner
	glViewport(2 * windowWidth / 3.0, 2 * windowHeight / 3.0, windowWidth / 3.0, windowHeight / 3.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(innerCamXYZ.x, innerCamXYZ.y, innerCamXYZ.z,      //camera is located at (x,y,z)
		innerCamXYZ.x + innerCamDir.x,                  //looking at a point that is
		innerCamXYZ.y + innerCamDir.y,                  //one unit along its direction
		innerCamXYZ.z + innerCamDir.z,
		0.0f, 1.0f, 0.0f);                                //up vector is (0,1,0) (positive Y)

	glClear(GL_DEPTH_BUFFER_BIT);                   //ensure that the overlay is always on top!

	drawSceneElements();

	glutSwapBuffers();
}

void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
		case '1': // Toggle Wireframe
			solidFigures = !solidFigures;
			toggleWireframe();
			break;
		case '2': // Toggle Axis
			axisEnabled = !axisEnabled;
			break;
		case 'a': // Toggle Animation
			animate = !animate;
			break;
		case 'i': // Select Inner Camera
			currentCamera = CAMERA_INNER;
			break;
		case 'o': // Select Outer Camera
			currentCamera = CAMERA_OUTER;
			break;
		case 27:  // Quit Program
			exit(0);
	}
}

// Based on https://www3.nd.edu/~pbui/teaching/cse.40166.fa10/examples/ex_11.html
void specialKeyInput(int key, int x, int y)
{
	const float movementConstant = 0.3;

	if (key == GLUT_KEY_UP)
	{
		if (USING_INNER)
		{
			innerCamXYZ += innerCamDir * movementConstant;
		}
		else {
			outerCamTPR.z -= 1.0f * movementConstant;

			//limit the camera radius to some reasonable values so the user can't get lost
			if (outerCamTPR.z < 2.0)
				outerCamTPR.z = 2.0;
			if (outerCamTPR.z > 10.0 * (currentCamera + 1))
				outerCamTPR.z = 10.0 * (currentCamera + 1);
			recomputeOrientation(outerCamXYZ, outerCamTPR);
		}
	}
	else if (key == GLUT_KEY_DOWN)
	{
		if (USING_INNER)
		{
			innerCamXYZ -= innerCamDir * movementConstant;
		}
		else {
			outerCamTPR.z += 1.0f * movementConstant;

			//limit the camera radius to some reasonable values so the user can't get lost
			if (outerCamTPR.z < 2.0)
				outerCamTPR.z = 2.0;
			if (outerCamTPR.z > 10.0 * (currentCamera + 1))
				outerCamTPR.z = 10.0 * (currentCamera + 1);
			recomputeOrientation(outerCamXYZ, outerCamTPR);
		}
	}
	else if (key == GLUT_KEY_LEFT)
	{
		if (USING_INNER)
		{
			innerCamXYZ -= innerCamDir.cross(Vector3(0,1,0)) * movementConstant;
		}
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		if (USING_INNER)
		{
			innerCamXYZ += innerCamDir.cross(Vector3(0, 1, 0)) * movementConstant;
		}
	}
}

void loadTextures()
{
	glGenTextures(4, texID); // Get the texture object IDs.
	for (int i = 0; i < 4; i++)
	{
		void* imgData; // Pointer to image color data read from the file.
		int imgWidth; // The width of the image that was read.
		int imgHeight; // The height.

		FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(textureFileNames[i]);

		if (format == FIF_UNKNOWN)
		{
			printf("Unknown file type for texture image file %s\n", textureFileNames[i]);
			continue;
		}

		FIBITMAP* bitmap = FreeImage_Load(format, textureFileNames[i], 0); // Read image from file.

		if (!bitmap)
		{
			printf("Failed to load image %s\n", textureFileNames[i]);
			continue;
		}

		FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap); // Convert to RGB or BGR format

		FreeImage_Unload(bitmap);
		imgData = FreeImage_GetBits(bitmap2); // Grab the data we need from the bitmap.
		imgWidth = FreeImage_GetWidth(bitmap2);
		imgHeight = FreeImage_GetHeight(bitmap2);

		if (imgData)
		{
			printf("Texture image loaded from file %s, size %dx%d\n", textureFileNames[i], imgWidth, imgHeight);
			glBindTexture(GL_TEXTURE_2D, texID[i]); // Will load image data into texture object #i
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Required since there are no mipmaps.
		}
		else
			printf("Failed to get texture data from %s\n", textureFileNames[i]);
	}
}

void printInstructions()
{
	std::cout << "APPLICATION CONTROLS" << std::endl;
	std::cout << "\t1 - Toggle Wireframe" << std::endl
			  << "\t2 - Toggle Axis" << std::endl
			  << "\ta - Toggle Animation" << std::endl
			  << "\ti - Select Inner Camera" << std::endl
			  << "\to - Select Outer Camera" << std::endl
			  << "\tEsc - Quit Program" << std::endl;

	std::cout << std::endl << "INNER CAMERA CONTROLS" << std::endl;
	std::cout << "\tLeft Arrow Key - Move Inner Camera Left" << std::endl
			  << "\tRight Arrow Key - Move Inner Camera Right" << std::endl
			  << "\tUp Arrow Key - Move Inner Camera Foward" << std::endl
			  << "\tDown Arrow key - Move Inner Camera Backward" << std::endl;

	std::cout << std::endl << "Click and Drag to Rotate Cameras" << std::endl;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Maxwell Patrick Calhoun - 811007606");

	//give the camera a 'pretty' starting point!
	innerCamXYZ = Vector3(5, 5, 5);
	innerCamTPR = Vector3(-M_PI / 4.0, M_PI / 4.0, 1);
	recomputeOrientation(innerCamDir, innerCamTPR);
	innerCamDir.normalize();

	outerCamTPR = Vector3(1.50, 2.0, 14.0);
	outerCamXYZ = Vector3(0, 0, 0);
	recomputeOrientation(outerCamXYZ, outerCamTPR);

	//register callback functions...
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(resizeWindow);
	glutMouseFunc(mouseInput);
	glutMotionFunc(mouseMotion);

	initialize();
	loadTextures();
	printInstructions();

	// Needs to be re-constructed here, not really sure why
	// If this is not here, the Mesh in SceneObject does not work and aborts program
	robot = Robot();
	robot.transformation.position = Vector3(0, 1.2f, 0);
	robot.transformation.scale = Vector3(0.2f, 0.2f, 0.2f);

	groundPlane = SceneObject(standardTexturePlane);
	groundPlane.transformation.position = Vector3(0, -0.01f, 0);
	groundPlane.transformation.scale = Vector3(40, 1, 40);

	trunk1 = SceneObject();
	trunk1.transformation.position = Vector3(-13.5, 2, 4);
	trunk1.transformation.scale = Vector3(1, 4, 1);

	trunk2 = SceneObject();
	trunk2.transformation.position = Vector3(-6, 2, 10.5);
	trunk2.transformation.scale = Vector3(1, 4, 1);

	trunk3 = SceneObject();
	trunk3.transformation.position = Vector3(7.5, 2, -4.5);
	trunk3.transformation.scale = Vector3(1, 4, 1);

	leaves1 = SceneObject();
	leaves1.transformation.position = Vector3(-13.5, 6, 4);
	leaves1.transformation.scale = Vector3(4, 4, 4);

	leaves2 = SceneObject();
	leaves2.transformation.position = Vector3(-6, 6, 10.5);
	leaves2.transformation.scale = Vector3(4, 4, 4);

	leaves3 = SceneObject();
	leaves3.transformation.position = Vector3(7.5, 6, -4.5);
	leaves3.transformation.scale = Vector3(4, 4, 4);

	glutMainLoop();

	return 1;
}


