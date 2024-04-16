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

int getMaxAreaContourId(vector <vector<cv::Point>> contours) {  //������� ������������ ������ ����� ���� ��������
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

void bubbleSort(int list[], int listLength){   //���������� �� �����������
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
    Mat3b mask = imread("C:/Users/����/Desktop/OpenCV/Flowchart/Flowchart/shem.png");

    //�������� ������ ����������� ���� �������� ������� �������
    Size sz = mask.size();
    int imageWidth = sz.width;
    int imageHeight = sz.height;
    cout << "������ �����������: " << sz.width << " " << sz.height << endl;
    if (sz.width > 1000 || sz.height > 1000)
        sz = mask.size() /2;
    resize(mask, mask, sz);

    Mat img_gray;
    cvtColor(mask, img_gray, COLOR_BGR2GRAY);   //������� �� �������� ����� �� ����

    //������� �� ��������� ����������� ��������
    Mat thresh;
    threshold(img_gray, thresh, 150, 255, THRESH_BINARY);  //������� �� ��������� ����� �������� ���� 

    //������� ������� �.�. �� ���� ����� ������� �� ������ ���� 
    Mat img_neg;
    img_neg = Scalar(255) - thresh;
    imshow("None approximation", img_neg);
    waitKey(0);
    destroyAllWindows();

    // ��������� ������� �� �������� ����������� � ������� CHAIN_APPROX_NONE
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(img_neg, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);   //������ ������� �������

    // �������� ������� �� �������� �����������
    Mat image_copy = mask.clone();
    drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);
    imshow("None approximation", image_copy);
    waitKey(0);
    destroyAllWindows();

    int Kol = 0; //���������� ������ ����� 

    int** mas = new int* [10];   //������ ��� ����������� ������� ������� ��������
    for (int i = 0; i < 10; ++i)
        mas[i] = new int[4];

    Point point1, point2, point3, point4;  //����� �������
    int width, height;    //������ � ������ ������
    int maxX, maxY, minX, minY;

    cout << "������ ����� ��� ��������� �����:\n";
    vector<vector<Point>> contours_poly(contours.size());
    //int maxArea = getMaxAreaContourId(contours);  //���� ������� ������ �������� �������
    //cout << getMaxAreaContourId(contours) << endl;
    int maxArea = 100;
    for (int i = 0; i < contours.size(); i++) {
        double a = arcLength(Mat(contours[i]), true);   //������� ����� 
        approxPolyDP(Mat(contours[i]), contours_poly[i], 0.02 * a, true);   //������� �� ������� �������� ������� ������ ����. �����
        if (contours_poly[i].size() == 8)  //���� ������ � 4 ���������
            drawContours(image_copy, contours_poly, i, Scalar(0, 255, 255), 2, 8);  //������ ����� �������
        if (contours_poly[i].size() == 4) {
            double a = arcLength(Mat(contours[i]), true);   //������� ����� 
            if (contourArea(contours[i]) > maxArea / 2) {     //��������� ��� ��� ������ ������ �������
                drawContours(image_copy, contours_poly, i, Scalar(0, 0, 255), 2, 8);  //������ ����� �������

                //������� ������� ����� � ������ ������ ����� ������
                point1 = contours_poly[i][0];
                point2 = contours_poly[i][1];
                point3 = contours_poly[i][2];
                point4 = contours_poly[i][3];
                //������� ������ � ������ �������� 
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
                    //��������� ������ ������� ������� � ������
                    mas[Kol][0] = 0; //�������������
                    mas[Kol][1] = minX;
                    mas[Kol][2] = minY;
                    mas[Kol][3] = maxX - minX;
                    mas[Kol][4] = maxY - minY;
                    Kol++;
                }
                else if ((point1.x > point3.x - 10 && point1.x < point3.x + 10 && point2.y > point4.y - 10 && point2.y < point4.y + 10) 
                    or (point1.y > point3.y - 10 && point1.y < point3.y + 10 && point2.x > point4.x - 10 && point2.x < point4.x + 10)) {
                    mas[Kol][0] = 1; //����
                    mas[Kol][1] = minX;
                    mas[Kol][2] = minY;
                    mas[Kol][3] = maxX - minX;
                    mas[Kol][4] = maxY - minY;
                    Kol++;
                }
                else {
                    mas[Kol][0] = 2; //������������
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

    //����������� �� ����������� ������ ������� ����� Y ������� ������� 
    int* sort = new int[Kol];
    for (int i = 0; i < Kol; i++) {
        sort[i] = mas[i][2];
    }
    //������� ������ � ���������� ��������� ����� � �� ����� (0 - �������������, 1 - ����, 2 - ������������)
    cout << "���������� ��������� �����: " << Kol << endl;
    cout << "��� �����:\n";
    for (int i = 0; i < Kol; i++) {
        cout << mas[i][0] << " ";
    }
    //����������
    bubbleSort(sort, Kol);
   
    cout << "\n������� ���������:";
    ofstream fout("output.txt");
    fout << "int main(){\n";
    cout << "\n\nint main(){\n";

    //�������� �������� �� ����������� ���������� Y (���� ���������� Y �����, ������������� ������� ������� ������ ������)
    int k = Kol;
    int j = 0; //����� ��������
    int q = 0; //����� ����� �������� (���������������)
    int* Elif = new int[Kol];
    int El = 0;
    string str;
    Mat croppedImage;
    Mat gray;
    while (k != 0) {
        if (sort[q] == mas[j][2]) {
            //cout << "fig" << q << ": " << mas[j][0] << " " << mas[j][1] << " " << mas[j][2] << " " << mas[j][3] << " " << mas[j][4] << endl;
            //������� ��� ��������� ��������
            Mat ROI(image_copy, Rect(mas[j][1], mas[j][2], mas[j][3], mas[j][4]));
            ROI.copyTo(croppedImage);  //�������
            str = "FindImg" + to_string(q);
            //imshow(str, croppedImage);
            Elif[q] = mas[j][0];
            //�������� ����� ����� ��� ������ else ����� if
            if (El == 2) {
                cout << "else\n";
                fout << "else\n";
                El = 0;
            }
            if (El == 1) El++;
            if (Elif[q] == 1) El++;
            //�� ���� ������ ��������� �������
            if (mas[j][0] == 1) {
                cout << "if ( ";
                fout << "if ( ";
            }
            if (mas[j][0] == 2) {
                cout << "std::cout << ";
                fout << "std::cout << ";
            }

            //����������� � ���������� �������� ������ �.�. � ��������� � ��� �����
            cvtColor(croppedImage, gray, cv::COLOR_BGR2GRAY);
            tesseract::TessBaseAPI api;
            api.Init("C:/Program Files/Tesseract-OCR/tessdata", "eng");
            api.SetImage(gray.data, gray.cols, gray.rows, 1, gray.cols);

            //������� ��������� �� ������������ �����
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