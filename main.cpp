#include<iostream>
#include<stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265359

using namespace std;

float camera_angle = 0;
float camera_x = -3;
float camera_y = 0;
float camera_height = 6;
float camera_rotate_speed = 0.003;
float camera_distance = 40;

float eye_x = 0;
float eye_y = 0;
float eye_z = 0;

float camera_x_angle = 0;
float camera_y_angle = 0;
float mouse_old_pos_x = 0;
float mouse_old_pos_y = 0;

GLuint texture[10];


struct Image
{
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};

void renderBitmapString(float x, float y, void *font,const char *string){
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


int ImageLoad(char *filename, Image *image)
{
    FILE *file;
    unsigned long size; //size of the image in bytes
    unsigned long i; //standard counter
    unsigned short int planes; //number of planes in Image(must be 1)
    unsigned short int bpp; //number of bits per pixel(must be 24)
    char temp; //temporary color storage for bgr-rgb conversion

    //make sure the file in here
    file = fopen(filename, "rb");
    if(file==NULL)
    {
        printf("Can't open file!");
    }

    //seek through the bmp header, up to width/height
    fseek(file,18,SEEK_CUR);

    //read the width
    i = fread(&image->sizeX,4,1,file);
    //read the height
    i = fread(&image->sizeY,4,1,file);
    //calculate the size (assuming 24 bits or 3 bytes per pixel)
    size = image->sizeX * image->sizeY * 3;
    //read the plane
    fread(&planes, 2, 1, file);
    //read the bits per pixel
    fseek(file, 24, SEEK_CUR);
    //read the data
    image->data = (char*)malloc(size);

    i=fread(image->data, size, 1, file);

    for(i=0; i<size; i+=3) //reverse all ol color bgr -> rgb
    {
        temp = image->data[i];
        image->data[i] = image->data[i+1];
        image->data[i+1] = temp;

    }

    fclose(file);
    //we're done
    return 1;

}

void loadTexture(char* file_name, int tex_id)
{
    Image *image1;
    //allocate space for texture
    image1 = (Image*) malloc(sizeof(Image));
    //Image not loaded
    if(!ImageLoad(file_name,image1))
    {
        exit(1);
    }

    texture[tex_id] = tex_id;

    glBindTexture(GL_TEXTURE_2D,tex_id);

    glTexImage2D(GL_TEXTURE_2D,0,3,image1->sizeX,image1->sizeY,0,GL_RGB,GL_UNSIGNED_BYTE,image1->data);

    //scale linearly when image is bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //scale linearly when image is smaller than texture

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}

void draw_solid_cube(GLfloat size)
{
    float x1,y1,z1;
    float x2,y2,z2;
    float x3,y3,z3;
    float x4,y4,z4;

    x1 = -size/2;
    y1 = -size/2;
    z1 = -size/2;

    x2 = size/2;
    y2 = -size/2;
    z2 = -size/2;

    x3 = size/2;
    y3 = -size/2;
    z3 = size/2;

    x4 = -size/2;
    y4 = -size/2;
    z4 = size/2;

    float x5,y5,z5;
    float x6,y6,z6;
    float x7,y7,z7;
    float x8,y8,z8;

    x5 = -size/2;
    y5 = size/2;
    z5 = -size/2;

    x6 = size/2;
    y6 = size/2;
    z6 = -size/2;

    x7 = size/2;
    y7 = size/2;
    z7 = size/2;

    x8 = -size/2;
    y8 = size/2;
    z8 = size/2;

    float n1_x = 0, n1_y = -1, n1_z = 0; //face that faces towards bottom
    float n2_x = 0, n2_y = 1, n2_z = 0; //up
    float n3_x = 0, n3_y = 0, n3_z = -1; //front
    float n4_x = 0, n4_y = 0, n4_z = 1; //back
    float n5_x = 1, n5_y = 0, n5_z = 0; //right
    float n6_x = -1, n6_y = 0, n6_z = 0; //left

    glBegin(GL_QUADS);
    //Quad 1

    glNormal3f(n1_x,n1_y,n1_z);

    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(1,0);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(1,1);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(0,1);
    glVertex3f(x4,y4,z4);

    //Quad 2

    glNormal3f(n2_x,n2_y,n2_z);

    glTexCoord2f(0,0);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(1,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,1);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(0,1);
    glVertex3f(x5,y5,z5);
    //Quad 3

    glNormal3f(n3_x,n3_y,n3_z);

    glTexCoord2f(0,0);
    glVertex3f(x5,y5,z5);

    glTexCoord2f(1,0);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(1,1);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(0,1);
    glVertex3f(x1,y1,z1);
    //Quad 4


    glNormal3f(n4_x,n4_y,n4_z);

    glTexCoord2f(0,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,0);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(1,1);
    glVertex3f(x4,y4,z4);

    glTexCoord2f(0,1);
    glVertex3f(x3,y3,z3);
    //Quad 5

    glNormal3f(n5_x,n5_y,n5_z);

    glTexCoord2f(0,0);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(1,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,1);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(0,1);
    glVertex3f(x2,y2,z2);

    //Quad 6

    glNormal3f(n6_x,n6_y,n6_z);

    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(1,0);
    glVertex3f(x4,y4,z4);

    glTexCoord2f(1,1);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(0,1);
    glVertex3f(x5,y5,z5);


    glEnd();
}

void draw_wall()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glPushMatrix();
    glTranslatef(25,10,0);
    glScalef(0.2,40,50);
    draw_solid_cube(1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

int num_bricks = 5;
float bricks_z[] = {2,4,6,8,10};
float bricks_y[] = {2.5,3.5,1.5,4.5,2.5};
float bricks_speed[] = {0.1,0.2,0.5,0.3,0.2};
float bricks_top_height = 20;
float bricks_bottom_start = -5;
float screen_right = 12;
float arrow_z = -10;
float arrow_y = 5.5;
int hit = 0;
int miss = 0;

bool arrow_on_hand = true;

int rand_int_range(int min_v, int max_v)
{
    return min_v + (rand() % (max_v - min_v + 1));
}

void draw_arrow()

{
    if(miss>=5)
    {
        printf("\n");
        printf("Game over");
        exit(1);
    }

    if (arrow_on_hand == true){
        arrow_y = arrow_y + 0.7;
        if (arrow_y > bricks_top_height)
        {
            arrow_y = bricks_bottom_start;
        }
    }
    else if (arrow_on_hand == false)
    {
        arrow_z = arrow_z + 0.5;
        if (arrow_z > screen_right)
        {
            arrow_z = -10;
            arrow_on_hand = true;
        }

        ////////////////////////////////////
        for (int i = 0; i < num_bricks; i++)
        {
            float b_y = bricks_y[i];
            float b_z = bricks_z[i];

            float y1 = b_y + 1.5;
            float z1 = b_z - 0.8;

            float y2 = b_y - 1.5;
            float z2 = b_z + 0.8;

            float a_z = arrow_z+(3.5/2);
            float a_y = arrow_y;

            if (a_z >= z1 && a_z <= z2)
            {
                if (a_y >= y2 && a_y <= y1)
                {
                     bricks_y[i] = bricks_bottom_start;
                     bricks_speed[i] = ((float)rand_int_range(10,100) / 1000.0);

                      hit++;
                     system("CLS");
                     printf("Hit %d", hit);
                     printf("Miss %d", miss);
                }
            }
        }
    }

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[0]);

        glPushMatrix();

        glTranslatef(20,arrow_y,arrow_z);
        glScalef(0.2,0.2,3.5);
        glutSolidCube(1);
        glPopMatrix();

        glPushMatrix();

        glTranslatef(20,arrow_y,arrow_z+1.5);
        glScalef(0.2,0.2,3.5);
        glutSolidCone(2,0.1,30,30);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
}

void draw_Sphere()
{

    for (int j = 0; j < num_bricks; j++){

        bricks_y[j] = bricks_y[j] + bricks_speed[j];
        if (bricks_y[j] > bricks_top_height)
        {
            bricks_y[j] = bricks_bottom_start;
            bricks_speed[j] = ((float)rand_int_range(100,200) / 1000.0);
            miss++;
        }
        system("CLS");
         printf("Hit %d", hit);
         printf("\n");
        printf("Miss %d", miss);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        glPushMatrix();

        glTranslatef(20,bricks_y[j],bricks_z[j]);
        glScalef(0.8,1.5,2);

        draw_solid_cube(1);
        glPopMatrix();


        glDisable(GL_TEXTURE_2D);

    }
}

void handleKeyPressed(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27:
        exit(0);
        break;
    }
}

void handleResize(int w, int h)
{
    glViewport(0,0,w,h);

}

void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    gluPerspective(60,1,5,100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(camera_x, camera_height, camera_y, camera_x + cos(camera_x_angle), camera_height, camera_y + sin(camera_y_angle), 0, 1, 0);


    draw_arrow();
    draw_Sphere();
    draw_wall();


    glutSwapBuffers();

}

static void idle(void)
{
    glutPostRedisplay();
}

static void mouse_func(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (arrow_on_hand == true)
        {
            arrow_on_hand = false;
        }
    }
}


int main(int argc, char **argv)
{

    cout<<"Draw Arrow"<<"\n";
    cout<<"For start type 1: ";
    string s;
    cin >> s;

    if (s != "1")
        return 0;

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000,800);
    glutInitWindowPosition(100,100);

    glutCreateWindow("Draw arrow");
    glEnable(GL_DEPTH_TEST);

    glClearColor(0,0,0,1);

    glEnable(GL_NORMALIZE);
    //glEnable(GL_LIGHTING);


    glGenTextures(10,texture);
    loadTexture("F:\\Total CSE term courses soft copy\\CSE 4-2\\Lab\\Computer graphics\\Shooting_Bricks\\Draw_arrow\\back.bmp", 0);
    loadTexture("F:\\Total CSE term courses soft copy\\CSE 4-2\\Lab\\Computer graphics\\Shooting_Bricks\\Draw_arrow\\brick.bmp", 1);


    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeyPressed);
    glutReshapeFunc(handleResize);
    glutIdleFunc(idle);
    glutMouseFunc(mouse_func);
    glutMainLoop();
    return 0;
}
