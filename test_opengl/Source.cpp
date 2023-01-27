#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

using namespace std;

#define x_max 300  //���������� ��������
#define x_min 100
#define y_max 300
#define y_min 100

#define INSIDE 0 //��� ��������
#define LEFT 1   //��� ������� ����� �� ��������
#define RIGHT 2  //��� ������� ������ �� ��������
#define BOTTOM 4 //��� ������� ���� �� ��������
#define TOP 8    //��� ������� ���� �� ��������

#define k 4

float  vertices[4][2] = {{x_min,y_min},{x_max,y_min},{x_max,y_max},{x_min,y_max}};

void SectorShow() //��������� ������ �������
{

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void SegmentShow(float **xy_v,int i) //��������� ��������� ��������
{   
    glColor3f(0,0,0);
    glBegin(GL_LINES);
    glVertex2f(xy_v[i][0], xy_v[i][1]);
    glVertex2f(xy_v[i][2], xy_v[i][3]);
    glEnd();
}

int ComputeCode(float x, float y) //���������� ��� �����, ������� ������� �� ������������ ����� ������������ ��������
{
    // ���������� ����� ����� ��� 0
    int code = INSIDE;

    if (x < x_min)       // ���� ���������� � ��������� ����� ��������, �� � ���� ����������� 1
        code |= LEFT;
    else if (x > x_max) // ���� ���������� � ��������� ������ ��������, �� � ���� ����������� 2
        code |= RIGHT;
    if (y < y_min)      // ���� ���������� y ��������� ������ ��������, �� � ���� ����������� 4
        code |= BOTTOM;
    else if (y > y_max) // ���� ���������� y ��������� ������ ��������, �� � ���� ����������� 8
        code |= TOP;

    return code;
}


void SortSegment(float **xy_v, int i) // ������� �������, ������� ��������� ������ �������� ��� ���-�� ���������� �� �������
{
    // ���������� ��� ������������ ������ � ������ ����� ������� ������������ ��������
    int code1 = ComputeCode(xy_v[i][0], xy_v[i][1]);
    int code2 = ComputeCode(xy_v[i][2], xy_v[i][3]);

    bool accept = false;

    while (true) {
        if ((code1 == 0) && (code2 == 0)) //��� ����� ������� ��������� ������ ��������
        {
            accept = true;
            break;
        }
        else if (code1 & code2) //��� ����� �������� ��� �������� �� ���� ������� �� ����
        {
            break;
        }
        else
        {
            int code_out;
            float x, y;

            // ���� ����� ����� �� ��������� ��������, ���������� �����
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;

            if (code_out == code1) {

                code1 = ComputeCode(x, y);

            }
            else {

                code2 = ComputeCode(x, y);
                
            }
        }
    }
    if (accept) {
        SegmentShow(xy_v,i);
    }

}

// ������� ������� ������������ ������ � ������������ �������� ��������
// � ��������� ��� �� ����� "input.txt" � ������� x1 y1 x2 y2(���������� ���� ����� �������)
void StreamSegment() {

    ifstream in("input.txt");

    if (in.is_open())
    {

        int count = 0;
        int temp;

        while (!in.eof())
        {
            in >> temp;
            count++;
        }

        in.seekg(0, ios::beg);
        in.clear();

        int count_space = 0;
        char symbol;
        while (!in.eof())
        {
            in.get(symbol);//������� ������� ������
            if (symbol == ' ') count_space++;
            if (symbol == '\n') break;
        }

        //��������� � ������ � ������ �����
        in.seekg(0, ios::beg);
        in.clear();

        int n = count / (count_space + 1);//����� �����
        int m = count_space + 1;//����� �������� �� ������� ������ ����� ��������
        float** x;
        x = new float* [n];
        for (int i = 0; i < n; i++) x[i] = new float[m];

        //������� ������� �� �����
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                in >> x[i][j];

        //�������� ������� ���������� ���������� ��� �������
        for (int i = 0; i < n; i++)
        {
            SortSegment(x, i);
        }

        for (int i = 0; i < n; i++) delete[] x[i];
        delete[] x;

        in.close();
    }
    else
    {
        cout << "���� �� ������.";
    }
}

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 640, "glProject", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    while (!glfwWindowShouldClose(window))
    {

        glfwSwapBuffers(window);
        glPushMatrix();
        glClearColor(1, 1, 1, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 0, 0);
        glLoadIdentity();
        glScalef(0.004f, 0.004f, 0.0f);
        glTranslatef(-200, -200, 0);
        SectorShow();
        StreamSegment();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}