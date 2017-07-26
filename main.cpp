#include "classes.h"

Snake snake;
Block block(HEIGHT);

int mouse_x, mouse_y;

bool scorer(double x, double y)
{
    //cout<<snake.length<<endl;
    if((snake.position-WIDTH/2-x)*(snake.position-WIDTH/2-x) + y*y <= 1.5*snake.radius*snake.radius) {
            snake.length++;
            return true;
    }
    return false;
}
bool blocker(double x1, double x2)
{
    if(snake.position>x1 && snake.position<x2) return true;
    return false;
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH/2, WIDTH/2, -HEIGHT/2, HEIGHT/2, 0.0f, 1.0f);
    double ww = WIDTH/5;
    double hh = HEIGHT/5;

    //Score
    glPushMatrix();
        glTranslated(WIDTH/7, HEIGHT/2.5, 0);
        glLineWidth(2);
        glColor3f(1,1,1);
        glScalef(.2,.2,1);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'S');
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'C');
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'O');
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'R');
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'E');
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, ':');
        const char *tmp;
        string str;
        str = SSTR(snake.score);
        tmp = str.c_str();
        for(int i=0; i<strlen(tmp); i++)
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, tmp[i]);
    glPopMatrix();

    //Game Over
    if(snake.length<=0) {
        glPushMatrix();
            glTranslated(-WIDTH/3, 0, 0);
            glLineWidth(3);
            glColor3f(1,0,0);
            glScalef(.4,.4,1);
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'G');
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'A');
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'M');
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'E');
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, ' ');
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'O');
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'V');
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'E');
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 'R');
        glPopMatrix();

        glFlush();
        glutSwapBuffers();
        return;
    }

    //blocks
    if(block.position< -(HEIGHT/2+1.75*hh)) {
        block = Block(HEIGHT);

        snake.max_food = rand()%3+1;
        snake.resetFood();
        for(int i=0; i<snake.max_food; i++) snake.food_upper1.set(rand()%10);
        for(int i=0; i<snake.max_food; i++) snake.food_lower.set(rand()%5);
        //cout<<snake.food_lower<<endl;
    }
    glPushMatrix();
        block.block_frames--;
        if(block.block_frames==0) {
            block.block_frames = block.block_frames_set;
            block.position-=block.speed;
            //block.speed+=.002;
        }
        glTranslated(0, block.position, 0);

        //food
        double food_x = -5.5*WIDTH/10;
        for(int i=0; i<10; i++) {
            if(snake.food_upper1[i]) {
                glPushMatrix();
                    glTranslated(food_x,hh*1.75,0);
                    glColor3f(snake.color_r, snake.color_g, snake.color_b);
                    glBegin(GL_POLYGON);
                        for(double i = 0; i < 2 * PI; i += PI / 360)
                            glVertex2f(cos(i) * snake.radius, sin(i) * snake.radius);
                    glEnd();
                glPopMatrix();
                if(scorer(food_x, block.position+hh*1.75)) snake.food_upper1[i] = 0;
            }
            if((snake.food_upper1[i] && i%2==0)) {
                glPushMatrix();
                    glTranslated(food_x,-hh*.75,0);
                    glColor3f(snake.color_r, snake.color_g, snake.color_b);
                    glBegin(GL_POLYGON);
                        for(double i = 0; i < 2 * PI; i += PI / 360)
                            glVertex2f(cos(i) * snake.radius, sin(i) * snake.radius);
                    glEnd();
                glPopMatrix();
                if(scorer(food_x, block.position-hh*.75)) snake.food_upper1[i] = 0;
            }
            food_x+=WIDTH/10;
        }

        food_x = -3*WIDTH/5;
        for(int i=0; i<5; i++) {
            if(snake.food_lower[i]) {
                glPushMatrix();
                    glTranslated(food_x+=WIDTH/5,-hh*1.5,0);
                    glColor3f(snake.color_r, snake.color_g, snake.color_b);
                    glBegin(GL_POLYGON);
                        for(double i = 0; i < 2 * PI; i += PI / 360)
                            glVertex2f(cos(i) * snake.radius, sin(i) * snake.radius);
                    glEnd();
                glPopMatrix();
                if(scorer(food_x, block.position-hh*1.5)) snake.food_lower[i] = 0;
            }
            else food_x+=WIDTH/5;
        }
        //food end

        if(blocker(4*WIDTH/5, WIDTH) && floor(block.position)<1 && block.view_to_set) {
            block.view_to_set = block.view[0]=0;
            snake.score+=block.weight[0];
            snake.length-=block.weight[0];
        }
        if(block.view[0]) {
            glColor3f(block.color_r[0], block.color_g[0] , block.color_b[0]);
            glBegin(GL_QUADS);
                glVertex2d(ww*1.5+block.gap, 0);
                glVertex2d(ww*1.5+block.gap, hh);
                glVertex2d(ww*2.5-block.gap, hh);
                glVertex2d(ww*2.5-block.gap, 0);
            glEnd();
            glPushMatrix();
                glTranslated(ww*1.8, hh/4, 0);
                glLineWidth(3);
                glColor3f(1-block.color_r[0], 1-block.color_g[0] , 1-block.color_b[0]);
                glScalef(.5,.5,1);
                glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, '0'+block.weight[0]);
            glPopMatrix();
        }

        if(blocker(3*WIDTH/5, 4*WIDTH/5) && floor(block.position)<1 && block.view_to_set) {
            block.view_to_set = block.view[1]=0;
            snake.score+=block.weight[1];
            snake.length-=block.weight[1];
        }
        if(block.view[1]) {
            glColor3f(block.color_r[1], block.color_g[1] , block.color_b[1]);
            glBegin(GL_QUADS);
                glVertex2d(ww/2+block.gap, 0);
                glVertex2d(ww/2+block.gap, hh);
                glVertex2d(ww*1.5-block.gap, hh);
                glVertex2d(ww*1.5-block.gap, 0);
            glEnd();
            glPushMatrix();
                glTranslated(ww*0.8, hh/4, 0);
                glLineWidth(3);
                glColor3f(1-block.color_r[1], 1-block.color_g[1] , 1-block.color_b[1]);
                glScalef(.5,.5,1);
                glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, '0'+block.weight[1]);
            glPopMatrix();
        }

        if(blocker(2*WIDTH/5, 3*WIDTH/5) && floor(block.position)<1 && block.view_to_set) {
            block.view_to_set = block.view[2]=0;
            snake.score+=block.weight[2];
            snake.length-=block.weight[2];
        }
        if(block.view[2]) {
            glColor3f(block.color_r[2], block.color_g[2] , block.color_b[2]);
            glBegin(GL_QUADS);
                glVertex2d(-ww/2+block.gap, 0);
                glVertex2d(-ww/2+block.gap, hh);
                glVertex2d(ww/2-block.gap, hh);
                glVertex2d(ww/2-block.gap, 0);
            glEnd();
            glPushMatrix();
                glTranslated(-ww*0.18, hh/4, 0);
                glLineWidth(3);
                glColor3f(1-block.color_r[2], 1-block.color_g[2] , 1-block.color_b[2]);
                glScalef(.5,.5,1);
                glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, '0'+block.weight[2]);
            glPopMatrix();
        }

        if(blocker(WIDTH/5, 2*WIDTH/5) && floor(block.position)<1 && block.view_to_set) {
            block.view_to_set = block.view[3]=0;
            snake.score+=block.weight[3];
            snake.length-=block.weight[3];
        }
        if(block.view[3]) {
            glColor3f(block.color_r[3], block.color_g[3] , block.color_b[3]);
            glBegin(GL_QUADS);
                glVertex2d(-ww*1.5+block.gap, 0);
                glVertex2d(-ww*1.5+block.gap, hh);
                glVertex2d(-ww/2-block.gap, hh);
                glVertex2d(-ww/2-block.gap, 0);
            glEnd();
            glPushMatrix();
                glTranslated(-ww*1.18, hh/4, 0);
                glLineWidth(3);
                glColor3f(1-block.color_r[3], 1-block.color_g[3] , 1-block.color_b[3]);
                glScalef(.5,.5,1);
                glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, '0'+block.weight[3]);
            glPopMatrix();
        }

        if(blocker(0, WIDTH/5) && floor(block.position)<1 && block.view_to_set) {
            block.view_to_set = block.view[4]=0;
            snake.score+=block.weight[4];
            snake.length-=block.weight[4];
        }
        if(block.view[4]) {
            glColor3f(block.color_r[4], block.color_g[4] , block.color_b[4]);
            glBegin(GL_QUADS);
                glVertex2d(-ww*2.5+block.gap, 0);
                glVertex2d(-ww*2.5+block.gap, hh);
                glVertex2d(-ww*1.5-block.gap, hh);
                glVertex2d(-ww*1.5-block.gap, 0);
            glEnd();
            glPushMatrix();
                glTranslated(-ww*2.18, hh/4, 0);
                glLineWidth(3);
                glColor3f(1-block.color_r[4], 1-block.color_g[4] , 1-block.color_b[4]);
                glScalef(.5,.5,1);
                glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, '0'+block.weight[4]);
            glPopMatrix();
        }
    glPopMatrix();
    //blocks end

    //Snake
	glColor3f(snake.color_r, snake.color_g, snake.color_b);
	glPushMatrix();
        double del = (abs((snake.position-WIDTH/2)*snake.del_coeff-snake.last_position[0])>snake.smoothness*snake.radius ? (snake.position-WIDTH/2)*snake.del_coeff:snake.last_position[0]);
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
    //snake ends

    glFlush();
	glutSwapBuffers();
}

void processMouse(int x, int y) {
    snake.position = mouse_x = x;
    mouse_y = y;
}
void reshape(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	HEIGHT = h;
	WIDTH = w;
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);

	// Get Back to the Modelview
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
	glutReshapeFunc(reshape);
	glutIdleFunc(render);
	glutPassiveMotionFunc(processMouse);

	//// enter GLUT event processing cycle
	glutWarpPointer(WIDTH/2, HEIGHT/2);
	glutMainLoop();

    return 0;
}
