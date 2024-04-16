#include <opencv2/opencv.hpp>
#include <iostream>
#include <windows.h>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <string>

#include <baseapi.h>
#include <allheaders.h>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <fstream>

using namespace std;
using namespace cv;

int getMaxAreaContourId(vector <vector<cv::Point>> contours) {  //находим максимвльный контур среди всех контуров
    double maxArea = 0;
    int maxAreaContourId = -1;
    for (int j = 0; j < contours.size(); j++) {
        double newArea = cv::contourArea(contours.at(j));
        if (newArea > maxArea) {
            maxArea = newArea;
            maxAreaContourId = j;
        }
    }
    return maxAreaContourId;
}

void bubbleSort(int list[], int listLength){   //сортировка по возрастанию
    while (listLength--){
        bool swapped = false;
        for (int i = 0; i < listLength; i++){
            if (list[i] > list[i + 1]){
                swap(list[i], list[i + 1]);
                swapped = true;
            }
        }
        if (swapped == false)
            break;
    }
}

int maxElem(int x1, int x2, int x3, int x4) {
    int max = x1;
    int* mas = new int[4];
    mas[0] = x1;
    mas[1] = x2;
    mas[2] = x3;
    mas[3] = x4;
    for (int i = 0; i < 4; i++)
        if (mas[i] > max) max = mas[i];
    return max;
}
int minElem(int x1, int x2, int x3, int x4) {
    int min = x1;
    int* mas = new int[4];
    mas[0] = x1;
    mas[1] = x2;
    mas[2] = x3;
    mas[3] = x4;
    for (int i = 0; i < 4; i++)
        if (mas[i] < min) min = mas[i];
    return min;
}

int main() {
    setlocale(LC_ALL, "RUS");
    Mat3b mask = imread("C:/Users/Артём/Desktop/OpenCV/Flowchart/Flowchart/shem.png");

    //уменьшим размер изображение если исходное слишком большое
    Size sz = mask.size();
    int imageWidth = sz.width;
    int imageHeight = sz.height;
    cout << "Размер изображения: " << sz.width << " " << sz.height << endl;
    if (sz.width > 1000 || sz.height > 1000)
        sz = mask.size() /2;
    resize(mask, mask, sz);

    Mat img_gray;
    cvtColor(mask, img_gray, COLOR_BGR2GRAY);   //сделаем из цветного файла чб файл

    //сделаем из исходного изображения бинарное
    Mat thresh;
    threshold(img_gray, thresh, 150, 255, THRESH_BINARY);  //сделаем из исходного файла двоичный файл 

    //возьмем негатив т.к. мы ищем белые границы на черном фоне 
    Mat img_neg;
    img_neg = Scalar(255) - thresh;
    imshow("None approximation", img_neg);
    waitKey(0);
    destroyAllWindows();

    // определим контуры на двоичном изображении с помощью CHAIN_APPROX_NONE
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(img_neg, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);   //найдем внешние контуры

    // нарисуем контуры на исходном изображении
    Mat image_copy = mask.clone();
    drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);
    imshow("None approximation", image_copy);
    waitKey(0);
    destroyAllWindows();

    int Kol = 0; //количество нужных фигур 

    int** mas = new int* [10];   //массив для определения области обрезки картинки
    for (int i = 0; i < 10; ++i)
        mas[i] = new int[4];

    Point point1, point2, point3, point4;  //точки контура
    int width, height;    //ширина и высота фигуры
    int maxX, maxY, minX, minY;

    cout << "Найдем точки для рисования фигур:\n";
    vector<vector<Point>> contours_poly(contours.size());
    //int maxArea = getMaxAreaContourId(contours);  //ищем площадь самого большого контура
    //cout << getMaxAreaContourId(contours) << endl;
    int maxArea = 100;
    for (int i = 0; i < contours.size(); i++) {
        double a = arcLength(Mat(contours[i]), true);   //находим длину 
        approxPolyDP(Mat(contours[i]), contours_poly[i], 0.02 * a, true);   //создаем из сложных контуров контуры легкий геом. фигур
        if (contours_poly[i].size() == 8)  //ищем печати с 4 сторонами
            drawContours(image_copy, contours_poly, i, Scalar(0, 255, 255), 2, 8);  //рисуем новые контуры
        if (contours_poly[i].size() == 4) {
            double a = arcLength(Mat(contours[i]), true);   //находим длину 
            if (contourArea(contours[i]) > maxArea / 2) {     //оттметаем все что меньше нужной площади
                drawContours(image_copy, contours_poly, i, Scalar(0, 0, 255), 2, 8);  //рисуем новые контуры

                //находим верхнюю левую и нижнюю правую точки фигуры
                point1 = contours_poly[i][0];
                point2 = contours_poly[i][1];
                point3 = contours_poly[i][2];
                point4 = contours_poly[i][3];
                //находим ширину и высоту картинки 
                width = point3.x - point1.x;
                height = point3.y - point1.y;
                maxX = maxElem(point1.x, point2.x, point3.x, point4.x);
                maxY = maxElem(point1.y, point2.y, point3.y, point4.y);
                minX = minElem(point1.x, point2.x, point3.x, point4.x);
                minY = minElem(point1.y, point2.y, point3.y, point4.y);
                cout << "\n\npoint1: " << point1.x << " " << point1.y << "\npoint2: " << point2.x << " " << point2.y << "\npoint3: " << point3.x << " " << point3.y << "\npoint4: " << point4.x << " " << point4.y << endl;
                cout << "max: " << maxX << " " << maxY << " min: " << minX << " " << minY << endl;
                if ((point1.x > point2.x-10 && point1.x < point2.x+10 && point2.y > point3.y-10 && point2.y < point3.y + 10)
                    or (point1.y > point2.y - 10 && point1.y < point2.y + 10 && point2.x > point3.x - 10 && point2.x < point3.x + 10)) {
                    //закидывем данные каждого контура в массив
                    mas[Kol][0] = 0; //прямоугольник
                    mas[Kol][1] = minX;
                    mas[Kol][2] = minY;
                    mas[Kol][3] = maxX - minX;
                    mas[Kol][4] = maxY - minY;
                    Kol++;
                }
                else if ((point1.x > point3.x - 10 && point1.x < point3.x + 10 && point2.y > point4.y - 10 && point2.y < point4.y + 10) 
                    or (point1.y > point3.y - 10 && point1.y < point3.y + 10 && point2.x > point4.x - 10 && point2.x < point4.x + 10)) {
                    mas[Kol][0] = 1; //ромб
                    mas[Kol][1] = minX;
                    mas[Kol][2] = minY;
                    mas[Kol][3] = maxX - minX;
                    mas[Kol][4] = maxY - minY;
                    Kol++;
                }
                else {
                    mas[Kol][0] = 2; //параллепипед
                    mas[Kol][1] = minX;
                    mas[Kol][2] = minY;
                    mas[Kol][3] = maxX - minX;
                    mas[Kol][4] = maxY - minY;
                    Kol++;
                }
            }
        }
    }
    imshow("None approximation", image_copy);
    waitKey(0);

    //отсортируем по возрастанию массив верхних точек Y каждого контура 
    int* sort = new int[Kol];
    for (int i = 0; i < Kol; i++) {
        sort[i] = mas[i][2];
    }
    //выведем данные о количестве найденных фигур и их типах (0 - прямоугольник, 1 - ромб, 2 - параллепипед)
    cout << "Количество найденных фигур: " << Kol << endl;
    cout << "тип фигур:\n";
    for (int i = 0; i < Kol; i++) {
        cout << mas[i][0] << " ";
    }
    //сортировка
    bubbleSort(sort, Kol);
   
    cout << "\nНапишем программу:";
    ofstream fout("output.txt");
    fout << "int main(){\n";
    cout << "\n\nint main(){\n";

    //нарисуем картинки по возрастанию координаты Y (если координаты Y равны, автоматически берется сначала правый контур)
    int k = Kol;
    int j = 0; //номер картинки
    int q = 0; //новый номер картинки (отсортированный)
    int* Elif = new int[Kol];
    int El = 0;
    string str;
    Mat croppedImage;
    Mat gray;
    while (k != 0) {
        if (sort[q] == mas[j][2]) {
            //cout << "fig" << q << ": " << mas[j][0] << " " << mas[j][1] << " " << mas[j][2] << " " << mas[j][3] << " " << mas[j][4] << endl;
            //обрежем все найденные картинки
            Mat ROI(image_copy, Rect(mas[j][1], mas[j][2], mas[j][3], mas[j][4]));
            ROI.copyTo(croppedImage);  //обрезка
            str = "FindImg" + to_string(q);
            //imshow(str, croppedImage);
            Elif[q] = mas[j][0];
            //алгоритм чтобы найти где писать else после if
            if (El == 2) {
                cout << "else\n";
                fout << "else\n";
                El = 0;
            }
            if (El == 1) El++;
            if (Elif[q] == 1) El++;
            //по типу фигуры определим команду
            if (mas[j][0] == 1) {
                cout << "if ( ";
                fout << "if ( ";
            }
            if (mas[j][0] == 2) {
                cout << "std::cout << ";
                fout << "std::cout << ";
            }

            //изображения с найденными фигураби делаем ч.б. и считываем с них текст
            cvtColor(croppedImage, gray, cv::COLOR_BGR2GRAY);
            tesseract::TessBaseAPI api;
            api.Init("C:/Program Files/Tesseract-OCR/tessdata", "eng");
            api.SetImage(gray.data, gray.cols, gray.rows, 1, gray.cols);

            //выведем найденный на изображениях текст
            char* outText = api.GetUTF8Text();
            cout << outText << endl;
            fout << outText << endl;

            if (mas[j][0] == 1) {
                cout << ")\n";
                fout << ")\n";
            }
            if (mas[j][0] == 2) {
                cout << " << endl;\n";
                fout << " << endl;\n";
            }

            delete[] outText;
            api.End();
            k--;
            q++;
        }
        if (j == Kol - 1)
            j = -1;
        j++;
    }

    cout << "\nreturn 0;\n}\n";
    fout << "\nreturn 0;\n}\n";
    fout.close();
    waitKey(0);
    destroyAllWindows();
}