#include <iostream>
#include <windows.h>
#include <gl/freeglut.h>
#include <cmath>


int v[9] = { 0 }, vt[9] = { 0 }, nr  = 1, player = 0;
bool win = false, AI = false;
float bl = 0;

int spotXAnim = 0, spotYAnim = 0, animator = 0;
float travelAnim = 0,loading = 0;
void animation(void);

bool winner()
{
	for (int i = 0; i < 3; i++)
		if (v[i] != 0 && v[i] == v[i + 3] && v[i + 3] == v[i + 6])
			return true;
	for (int i = 0; i < 3; i++)
		if (v[i*3] != 0 && v[i*3] == v[i*3 + 1] && v[i*3 + 1] == v[i*3 + 2])
			return true;
	if (v[0] != 0 && v[0] == v[4] && v[4] == v[8])
		return true;
	if (v[2] != 0 && v[2] == v[4] && v[4] == v[6])
		return true;
	return false;
}

int AI_Mind()
{
	// Win AI
	for (int i = 0; i < 3; i++)
		if (v[i] == 2 && v[i] == v[i + 3] && v[i+6] == 0)
			return i + 6;
	for (int i = 0; i < 3; i++)
		if (v[i] == 2 && v[i] == v[i + 6] && v[i + 3] == 0)
			return i + 3;
	for (int i = 0; i < 3; i++)
		if (v[i + 3] == 2 && v[i + 6] == v[i + 3] && v[i] == 0)
			return i;

	for (int i = 0; i < 3; i++)
		if (v[3 * i] == 2 && v[3 * i] == v[3 * i + 1] && v[3 * i + 2] == 0)
			return 3 * i + 2;
	for (int i = 0; i < 3; i++)
		if (v[3 * i] == 2 && v[3 * i] == v[3 * i + 2] && v[3 * i + 1] == 0)
			return 3 * i + 1;
	for (int i = 0; i < 3; i++)
		if (v[3 * i + 2] == 2 && v[3 * i + 2] == v[3 * i + 1] && v[3 * i] == 0)
			return 3 * i;

	if (v[0] == 2 && v[0] == v[4] && v[8] == 0)
		return 8;
	if (v[0] == 2 && v[0] == v[8] && v[4] == 0)
		return 4;
	if (v[4] == 2 && v[4] == v[8] && v[0] == 0)
		return 0;
	if (v[2] == 2 && v[2] == v[6] && v[4] == 0)
		return 4;
	if (v[2] == 2 && v[2] == v[4] && v[6] == 0)
		return 6;
	if (v[4] == 2 && v[4] == v[6] && v[2] == 0)
		return 2;

	// Lose AI
	for (int i = 0; i < 3; i++)
		if (v[i] == 1 && v[i] == v[i + 3] && v[i + 6] == 0)
			return i + 6;
	for (int i = 0; i < 3; i++)
		if (v[i] == 1 && v[i] == v[i + 6] && v[i + 3] == 0)
			return i + 3;
	for (int i = 0; i < 3; i++)
		if (v[i + 3] == 1 && v[i + 6] == v[i + 3] && v[i] == 0)
			return i;

	for (int i = 0; i < 3; i++)
		if (v[3 * i] == 1 && v[3 * i] == v[3 * i + 1] && v[3 * i + 2] == 0)
			return 3 * i + 2;
	for (int i = 0; i < 3; i++)
		if (v[3 * i] == 1 && v[3 * i] == v[3 * i + 2] && v[3 * i + 1] == 0)
			return 3 * i + 1;
	for (int i = 0; i < 3; i++)
		if (v[3 * i + 2] == 1 && v[3 * i + 2] == v[3 * i + 1] && v[3 * i] == 0)
			return 3 * i;

	if (v[0] == 1 && v[0] == v[4] && v[8] == 0)
		return 8;
	if (v[0] == 1 && v[0] == v[8] && v[4] == 0)
		return 4;
	if (v[4] == 1 && v[4] == v[8] && v[0] == 0)
		return 0;
	if (v[2] == 1 && v[2] == v[6] && v[4] == 0)
		return 4;
	if (v[2] == 1 && v[2] == v[4] && v[6] == 0)
		return 6;
	if (v[4] == 1 && v[4] == v[6] && v[2] == 0)
		return 2;

	// Focus center
	if (v[4] == 0)
		return 4;

	// Focus edges
	if (v[0] == 0)
		return 0;
	if (v[2] == 0)
		return 2;
	if (v[6] == 0)
		return 6;
	if (v[8] == 0)
		return 8;

	// Focus edges
	if (v[1] == 0)
		return 1;
	if (v[3] == 0)
		return 3;
	if (v[5] == 0)
		return 5;
	if (v[7] == 0)
		return 7;

	std::cout << "EROARE AI THINKING" << std::endl;
	return 0;
}

void AI_GO()
{
	int place = AI_Mind();
	v[place] = player + 1;
	vt[place] = nr++;
	player = (player + 1) % 2;
	animator = 1;
	spotXAnim = place / 3 + 1;
	spotYAnim = place % 3 + 1;
	win = winner();
	if (win == true)
		player = (player + 1) % 2;
}

void HUMAN(double a, double b)
{
	int ai = (int)a, bi = (int)b;

	
	if (ai >= 1 && ai < 7 && bi >= 1 && bi < 7 && win == false)
	{
		ai--;
		bi--;
		ai /= 2;
		bi /= 2;
		if (v[3*ai + bi] == 0)
		{
			v[3*ai + bi] = player + 1;
			vt[3 * ai + bi] = nr++;
			spotXAnim = ai + 1;
			spotYAnim = bi + 1;
			animator = 1;
			player = (player + 1) % 2;
		}
		win = winner();
		if (win == true)
		{
			player = (player + 1) % 2;
			return;
		}
	}
}

void clearer()
{
	for (int i = 0; i < 9; i++)
		v[i] = vt[i] = 0;
	player = 0;
	win = false;
	nr = 1;
	loading = 0;
}

void undo()
{
	int ma = 0;
	for (int i = 1; i < 9; i++)
		if (vt[i] > vt[ma])
			ma = i;
	if (vt[ma] != 0)
	{
		if(win == false)
		player = (player + 1) % 2;
		vt[ma] = 0;
		v[ma] = 0;
		loading = 0;
		win = false;
			nr--;
	}
}

void init(void)  
{
	glClearColor(1.0, 1.0, 1.0, 0.0); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION); 
	gluOrtho2D(0.0, 10.0, 0.0, 10.0); 
	glClear(GL_COLOR_BUFFER_BIT);
}

void draw_X(int a,int b)
{
	a *= 2;
	b *= 2;
	a++;
	b++;

	glLineWidth(4.0);
	glColor3d(1.0, 0.0, 0.0);
	glBegin(GL_LINES);

	glVertex2d(a + 0.5, b + 0.5);
	glVertex2d(a + 1.5, b + 1.5);

	glVertex2d(a + 0.5, b + 1.5);
	glVertex2d(a + 1.5, b + 0.5);

	glEnd();
}

void draw_O(int a, int b)
{
	a *= 2;
	b *= 2;
	a++;
	b++;

	glLineWidth(4.0);
	glColor3d(0.0, 0.0, 1.0); 
	glBegin(GL_LINE_STRIP);

	glVertex2d(a + 0.5, b + 0.5);
	glVertex2d(a + 1.5, b + 0.5);
	glVertex2d(a + 1.5, b + 1.5);
	glVertex2d(a + 0.5, b + 1.5);
	glVertex2d(a + 0.5, b + 0.5);

	glEnd();
}

void(*arr[])(int, int) = { &draw_X, &draw_O };

void animation(void)
{
	if (animator == 1)
	{
		int who = (player+1)%2, spotX = spotXAnim, spotY = spotYAnim;
		travelAnim += 0.1;
		if (travelAnim <= spotX)
		{
			glColor3f(1 - bl, 1 - bl , 1 - bl);
			glBegin(GL_QUADS);
			glVertex2f(spotX*2 - 0.8, spotY*2 - 0.8);
			glVertex2f(spotX*2 + 0.8, spotY*2 - 0.8);
			glVertex2f(spotX*2 + 0.8, spotY*2 + 0.8);
			glVertex2f(spotX*2 - 0.8, spotY*2 + 0.8);
			glEnd();
			if (win == true)
				who = (who + 1) % 2;
			glPushMatrix();
			glTranslated(travelAnim, (travelAnim / spotX)*spotY, 0);
			glScaled(travelAnim / spotX, travelAnim / spotX, 0);
			glRotated((travelAnim / spotX) * 360, 1, 3, 0);
			glTranslated(-spotX, -spotY, 0);
			arr[who](spotX - 1, spotY - 1);
			glPopMatrix();
			glLoadIdentity();
			Sleep(50);
		}
		if (travelAnim > spotX)
			{
				animator = 0;
				travelAnim = 0;
				loading = 0;
				if (AI == true && player == 1 && win == false && nr < 10)
					AI_GO();
			}
			glutPostRedisplay();
	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 10.0, 0.0, 10.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawing(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0 - bl, 1.0 - bl, 1.0 - bl);
	glBegin(GL_QUADS);
	glVertex2d(0.0, 0.0);
	glVertex2d(10.0, 0.0);
	glVertex2d(10.0, 10.0);
	glVertex2d(0.0, 10.0);
	glEnd();

	glLineWidth(6.0);
	glColor3f(abs(0.2 - bl), abs(0.2 - bl), abs(0.2 - bl));
	glBegin(GL_LINES);
	glVertex2d(1, 0.5);
	glVertex2d(7, 0.5);
	glEnd();

	glLineWidth(6.0);
	glColor3f(1.0,0.7, 0.0);
	glBegin(GL_LINES);
	glVertex2d(1, 0.5);
	glVertex2d(1 + loading, 0.5);
	glEnd();

	glColor3f(bl, bl, bl);
	glRasterPos2f(1.0, 9.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'P');
	glRasterPos2f(1.3, 9.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'L');
	glRasterPos2f(1.6, 9.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'A');
	glRasterPos2f(1.9, 9.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Y');
	glRasterPos2f(2.2, 9.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'E');
	glRasterPos2f(2.5, 9.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'R');
	if (nr < 10 || win == true)
	{
		glRasterPos2f(3.0, 9.0);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '1' + player);
	}
	else
	{
		glRasterPos2f(2.8, 9.0);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'S');
	}
	if (win == true)
	{
		glRasterPos2f(3.5, 9.0);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'W');
		glRasterPos2f(3.8, 9.0);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'O');
		glRasterPos2f(4.1, 9.0);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'N');
	}

	if (nr == 10 && win == false)
	{
		glRasterPos2f(3.5, 9.0);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'D');
		glRasterPos2f(3.8, 9.0);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'R');
		glRasterPos2f(4.1, 9.0);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'A');
		glRasterPos2f(4.4, 9.0);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'W');
	}

	glColor3f(bl, bl, bl);
	glBegin(GL_QUADS);
	glVertex2d(9.0, 9.0);
	glVertex2d(10.0, 9.0);
	glVertex2d(10.0, 10.0);
	glVertex2d(9.0, 10.0);
	glEnd();

	glColor3f(1.0 - bl, 1.0 - bl, 1.0 - bl);
	glRasterPos2f(9.0, 9.3);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'R');
	glRasterPos2f(9.3, 9.3);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'E');
	glRasterPos2f(9.6, 9.3);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'S');

	glColor3f(abs(0.2 - bl), abs(0.2 - bl), abs(0.2 - bl));
	glBegin(GL_QUADS);
	glVertex2d(9.0, 8.0);
	glVertex2d(10.0, 8.0);
	glVertex2d(10.0, 9.0);
	glVertex2d(9.0, 9.0);
	glEnd();

	glLineWidth(2.0);
	glColor3d(1.0 - bl, 1.0 - bl, 1.0 - bl);
	glBegin(GL_LINE_STRIP);

	glVertex2d(9.2, 8.2);
	glVertex2d(9.8, 8.2);
	glVertex2d(9.8, 8.7);
	glVertex2d(9.2, 8.7);

	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2d(9.2, 8.7);
	glVertex2d(9.4, 8.5);
	glVertex2d(9.4, 8.9);
	glEnd();

	glColor3f(bl, bl, bl);
	glBegin(GL_QUADS);
	glVertex2d(9.0, 7.0);
	glVertex2d(10.0, 7.0);
	glVertex2d(10.0, 8.0);
	glVertex2d(9.0, 8.0);
	glEnd();

	glColor3f(1.0 - bl, 1.0 - bl, 1.0 - bl);
	glRasterPos2f(9.2, 7.3);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'A');
	glRasterPos2f(9.6, 7.3);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'I');

	if (AI == true)
	{
		glColor3f(bl, bl, bl);
		glRasterPos2f(9.0, 6.5);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'O');
		glRasterPos2f(9.3, 6.5);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'N');
	}
	else
	{
		glColor3f(bl, bl, bl);
		glRasterPos2f(9.0, 6.5);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'O');
		glRasterPos2f(9.3, 6.5);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'F');
		glRasterPos2f(9.5, 6.5);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'F');
	}

	glColor3f(bl, bl, bl);
	glBegin(GL_QUADS);
	glVertex2d(9.0, 5.0);
	glVertex2d(10.0, 5.0);
	glVertex2d(10.0, 6.0);
	glVertex2d(9.0, 6.0);
	glEnd();

	glColor3f(1.0 - bl, 1.0 - bl, 1.0 - bl);
	glRasterPos2f(9.2, 5.3);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'T');
	glRasterPos2f(9.6, 5.3);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'H');

	glLineWidth(3.0);
	glColor3f(bl, bl, bl);
	glBegin(GL_LINES);
	glVertex2d(3.0, 1.0);
	glVertex2d(3.0, 7.0);

	glVertex2d(5.0, 1.0);
	glVertex2d(5.0, 7.0);
	
	glVertex2d(1.0, 1.0);
	glVertex2d(1.0, 7.0);
	
	glVertex2d(7.0, 1.0);
	glVertex2d(7.0, 7.0);
	
	
	
	glVertex2d(1.0, 1.0);
	glVertex2d(7.0, 1.0);
	
	glVertex2d(1.0, 3.0);
	glVertex2d(7.0, 3.0);
	
	glVertex2d(1.0, 5.0);
	glVertex2d(7.0, 5.0);
	
	glVertex2d(1.0, 7.0);
	glVertex2d(7.0, 7.0);
	glEnd();
	for (int i = 0; i < 9; i++)
		if (v[i] != 0)
			arr[v[i] - 1](i / 3, i % 3);

	animation();

	if (animator == 0 && nr < 10 && AI == false && win == false)
	{
		loading += 0.05;
		if (loading >= 6)
			AI_GO();
		Sleep(20);
		glutPostRedisplay();
	}
	glutSwapBuffers();
	glFlush();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			double a, b;
			a = (1.0*x / glutGet(GLUT_WINDOW_WIDTH)) * 10;
			b = (1.0*y / glutGet(GLUT_WINDOW_HEIGHT)) * 10;
			if (a > 9.0 && b < 1.0)
				clearer();
			else
				if (a > 9.0 && b < 2.0)
				{
					if(AI == false || nr == 10 || (win == true && player == 0))
						undo();
					else
					{
							undo(); undo();
					}
				}
				else
					if (a > 9.0 && b < 3.0)
					{
						if (AI == true)
							AI = false;
						else
							AI = true;
						clearer();
					}
					else
						if (a > 9.0 && b < 5.0 && b > 4.0)
							bl = 1 - bl;
						else
							if(animator == 0)
								HUMAN(a, 10 - b);

		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)

			break;

	}
}

void main(int argc, char** argv)
{


	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
	glutInitWindowPosition(400, 10); 
	glutInitWindowSize(500, 500); 
	glutCreateWindow("Tic Tac Toe");

	init(); 
	glutMouseFunc(mouse);
	glutDisplayFunc(drawing);
	glutReshapeFunc(reshape);
	glutMainLoop(); 
}