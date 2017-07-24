#include <windows.h>
#include <bits/stdc++.h>
#include <GL/glut.h>
using namespace std;

#define PI acos(-1)
#define WIDTH 600
#define HEIGHT 600

class Snake {
public:
    int position;
    double color_r,color_g,color_b;
    vector<double> last_position;
    int length;
    double radius;
    double del_coeff;
    double tail_lag;

    Snake()
    {
        position = 0;
        color_r = 1;
        color_g = 1;
        color_b = 0;
        length = 5;
        radius = 0.03;
        del_coeff = .003;
        tail_lag = .0005;
        for(int i=0; i<length; i++) last_position.push_back(0);
    }

}snake;

class Block {
public:
    double color_r, color_g, color_b;

    Block()
    {
        color_r = rand();
        color_g = rand();
        color_b = rand();
    }
};

class Bar {
public:
    double length,width;
    int position;

    Bar()
    {

    }
    bool appear()
    {
        return rand()<.5;
    }
};

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(snake.color_r, snake.color_g, snake.color_b);

	glPushMatrix();
        double del = (abs((snake.position-WIDTH/2)*snake.del_coeff-snake.last_position[0])>2*snake.radius ? (snake.position-WIDTH/2)*snake.del_coeff:snake.last_position[0]);
        snake.last_position[0] = del;
        for(int i=1; i<snake.length; i++)
        {
            glPushMatrix();
                snake.last_position[i] += (del-snake.last_position[i]>0 ? 1:-1)*snake.tail_lag;
                if(snake.last_position[i]-snake.last_position[i-1]<-snake.radius)
                    snake.last_position[i]=snake.last_position[i-1]-snake.radius;
                else if(snake.last_position[i]-snake.last_position[i-1]>snake.radius)
                    snake.last_position[i]=snake.last_position[i-1]+snake.radius;
                glTranslated(snake.last_position[i],-i*2*snake.radius,0);
                glBegin(GL_POLYGON);
                    for(double i = 0; i < 2 * PI; i += PI / 360)
                        glVertex2f(cos(i) * snake.radius, sin(i) * snake.radius);
                glEnd();
            glPopMatrix();
        }
        glTranslated(snake.last_position[0],0,0);
        glBegin(GL_POLYGON);
            for(double i = 0; i < 2 * PI; i += PI / 360)
                glVertex2f(cos(i) * snake.radius, sin(i) * snake.radius);
        glEnd();
    glPopMatrix();

    glFlush();
	glutSwapBuffers();
}

void processMouse(int x, int y) {
    snake.position = x;
}
void reshape(int w, int h)
{
	if(h == 0) h = 1;
	float ratio = 1.0* w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45,ratio,1,1000);
	gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("SnakeVsBlocks");
    glutInitWindowPosition(100,100);
    //glutFullScreen();

    snake = Snake();

    //// register callbacks
	glutDisplayFunc(render);
	//glutReshapeFunc(reshape);
	glutIdleFunc(render);
	glutPassiveMotionFunc(processMouse);

	//// enter GLUT event processing cycle
	glutWarpPointer(WIDTH/2, HEIGHT/2);
	glutMainLoop();

    return 0;
}
