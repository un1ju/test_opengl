#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

using namespace std;

#define INSIDE 0 //код квадрата
#define LEFT 1   //код сектора слева от квадрата
#define RIGHT 2  //код сектора справа от квадрата
#define BOTTOM 4 //код сектора ниже от квадрата
#define TOP 8    //код сектора выше от квадрата

const float x_max = 300;  //координаты квадрата
const float x_min = 100;
const float y_max = 300;
const float y_min = 100;

float  vertices[4][2] = {{x_min,y_min},{x_max,y_min},{x_max,y_max},{x_min,y_max}}; 
GLFWwindow* window;

void ShowSector() //процедура рисует квадрат
{

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void ShowSegment(float **xyCoordinate,int i) //процедура рисования отрезков
{   
    glColor3f(0,0,0);
    glBegin(GL_LINES);
    glVertex2f(xyCoordinate[i][0], xyCoordinate[i][1]);
    glVertex2f(xyCoordinate[i][2], xyCoordinate[i][3]);
    glEnd();
}

int ComputeCode(float x, float y) //определяем код точки, который зависит от расположения точки относительно квадрата
{
    // изначально точка имеет код 0
    int code = INSIDE;

    if (x < x_min)       // если координата х находится левее квадрата, то к коду добавляется 1
        code |= LEFT;
    else if (x > x_max) // если координата х находится правее квадрата, то к коду добавляется 2
        code |= RIGHT;
    if (y < y_min)      // если координата y находится правее квадрата, то к коду добавляется 4
        code |= BOTTOM;
    else if (y > y_max) // если координата y находится правее квадрата, то к коду добавляется 8
        code |= TOP;

    return code;
}


void SortSegment(float **xyCoordinate, int i) // находит отрезки, которые находятся внутри квадрата или как-то пересекают ее границы
{
    // определяем код расположения первой и второй точки отрезка относительно квадрата
    int code1 = ComputeCode(xyCoordinate[i][0], xyCoordinate[i][1]);
    int code2 = ComputeCode(xyCoordinate[i][2], xyCoordinate[i][3]);

    bool accept = false;

    while (true) {
        if ((code1 == 0) && (code2 == 0)) //обе точки отрезка находятся внутри квадрата
        {
            accept = true;
            break;
        }
        else if (code1 & code2) //обе точки находтся вне квадрата по одну сторону от него
        {
            break;
        }
        else
        {
            int code_out;
            float x, y;

            // Одна точка лежит за пределами квадрата, определяем какая
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;

            if (code_out & TOP) {
                // точка находится выше квадрата
                x = xyCoordinate[i][0] + (xyCoordinate[i][2] - xyCoordinate[i][0]) * (y_max - xyCoordinate[i][1]) / (xyCoordinate[i][3] - xyCoordinate[i][1]);
                y = y_max;
            }
            else if (code_out & BOTTOM) {
                // точка находится ниже квадрата
                x = xyCoordinate[i][0] + (xyCoordinate[i][2] - xyCoordinate[i][0]) * (y_min - xyCoordinate[i][1]) / (xyCoordinate[i][3] - xyCoordinate[i][1]);
                y = y_min;
            }
            else if (code_out & RIGHT) {
                // точка находится справа от квадрата
                y = xyCoordinate[i][1] + (xyCoordinate[i][3] - xyCoordinate[i][1]) * (x_max - xyCoordinate[i][0]) / (xyCoordinate[i][2] - xyCoordinate[i][0]);
                x = x_max;
            }
            else if (code_out & LEFT) {
                // точка находится слева от квадрата
                y = xyCoordinate[i][1] + (xyCoordinate[i][3] - xyCoordinate[i][1]) * (x_min - xyCoordinate[i][0]) / (xyCoordinate[i][2] - xyCoordinate[i][0]);
                x = x_min;
            }

            if (code_out == code1) {

                code1 = ComputeCode(x, y);

            }
            else {

                code2 = ComputeCode(x, y);
                
            }
        }
    }
    if (accept) {
        ShowSegment(xyCoordinate,i);
    }

}
struct CountCoordinate //создаем структуру, которую будет возвращать функция StreamSegment
{
    int countach; // количество отрезков
    float** AnswArray; // массив с координатами отрезков
};

CountCoordinate SegmentAnswer;

// Функция создает динамический массив с координатами рисуемых отрезков
// и заполняет его из файла "input.txt" в формате x1 y1 x2 y2(координаты двух точек отрезка)
CountCoordinate StreamSegment() {

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
            in.get(symbol);//считали текущий символ
            if (symbol == ' ') count_space++;
            if (symbol == '\n') break;
        }

        //переходим в потоке в начало файла
        in.seekg(0, ios::beg);
        in.clear();
        SegmentAnswer.countach = count / (count_space + 1);
        int m = count_space + 1;//число столбцов на единицу больше числа пробелов
        SegmentAnswer.AnswArray = new float* [SegmentAnswer.countach];
        for (int i = 0; i < SegmentAnswer.countach; i++) SegmentAnswer.AnswArray[i] = new float[m];
        //Считаем матрицу из файла
        for (int i = 0; i < SegmentAnswer.countach; i++)
            for (int j = 0; j < m; j++)
            {
                in >> SegmentAnswer.AnswArray[i][j];
            }
        //Вызываем функцию отбирающую подходящие нам отрезки
        return SegmentAnswer;
        for (int i = 0; i < SegmentAnswer.countach; i++) delete[]  SegmentAnswer.AnswArray[i];
        delete[]  SegmentAnswer.AnswArray;

        in.close();
    }
    else
    {
        cout << "Файл не найден.";
    }
}

int main(void)
{
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 640, "glProject", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    StreamSegment();

    while (!glfwWindowShouldClose(window))
    {

        glfwSwapBuffers(window);
        glPushMatrix();
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 0.0f, 0.0f);
        glLoadIdentity();
        glScalef(0.004f, 0.004f, 0.0f);
        glTranslatef(-200.0f, -200.0f, 0.0f);
        ShowSector();
        for (int i = 0; i < SegmentAnswer.countach; i++) 
        {
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                ShowSegment(SegmentAnswer.AnswArray, i);
            else
                SortSegment(SegmentAnswer.AnswArray, i);
        }
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
