#include "GlcmAlgo.h"
#include "TextureAna.h"
#include <fstream>
#include <iostream>
#include <string>
#include "dirent.h"
#include<memory>
using namespace cv;
using namespace std;

//µßµ¹¹Þ¼ì²âµÄËã·¨
double MeanHashDetect(Mat src, bool tringFlage,vector<Mat> *standImageData,int zoomSize, string convertColor, Mat standHashMat)
{
    Mat imageSub, zoomImage, zoomImage32;
    if (!src.data || src.channels() != 3 )
    {
        return -1;
    }
    resize(src, zoomImage, Size(zoomSize, zoomSize), INTER_NEAREST);
    convertColor == "CV_BGR2HSV" ? cvtColor(zoomImage, zoomImage, CV_BGR2HSV) : zoomImage;
    zoomImage.convertTo(zoomImage32, CV_32F);
    double  valueDist = 255;
    for (int index = 0; index < (*standImageData).size(); index++)
    {
        absdiff((*standImageData)[index], zoomImage32, imageSub);
        double sumValue = 0;
        for (int i = 0; i < imageSub.rows; i++)
        {
            for (int j = 0; j < imageSub.cols; j++)
            {
                sumValue = sumValue + imageSub.at<Vec3f>(i, j)[0] + imageSub.at<Vec3f>(i, j)[1] + imageSub.at<Vec3f>(i, j)[2];
            }
        }
        sumValue = sumValue / (zoomSize*zoomSize * 3);
        if (valueDist > sumValue)
        {
            valueDist = sumValue;
            standHashMat = (*standImageData)[index];
        }
    }

    if (valueDist > 20 && tringFlage==1)
    {
        (*standImageData).push_back(zoomImage32);
        //standImageDataOrigin.push_back(img);
    }

   
   
    return  valueDist;
}
void RogueCanDetect()
{
    DIR *dir;
    struct dirent *entry;
    vector<Mat>standImageData;
    vector<Mat>standImageDataOrigin;
    if ((dir = opendir("C:\\Users\\jiance04\\Desktop\\1")) == NULL)
        printf("Error opening \n ");
    else
    {
        Mat scr1;
        while ((entry = readdir(dir)) != NULL)
        {
            string imagePath = "C:\\Users\\jiance04\\Desktop\\1\\" + string(entry->d_name);

            Mat img = imread(imagePath, IMREAD_COLOR);
            Mat standHashMat;
            double distValue = MeanHashDetect(img, 1, &standImageData, 80, "", standHashMat);
        }
    }
    closedir(dir);
    //
    DIR *dir2;
    struct dirent *entry2;
    dir2 = opendir("C:\\Users\\jiance04\\Desktop\\2");
    while ((entry2 = readdir(dir2)) != NULL)
    {
        string imagePath = "C:\\Users\\jiance04\\Desktop\\2\\" + string(entry2->d_name);
        Mat img = imread(imagePath, IMREAD_COLOR);
        Mat standHashMat;
        double t = (double)cvGetTickCount();
        double distValue = MeanHashDetect(img, 0, &standImageData, 80, "", standHashMat);

        t = (double)cvGetTickCount() - t;
        cout << "sumValue=" << distValue << "    " << "run time =" << t / (cvGetTickFrequency() * 1000) << endl;
    }

    closedir(dir2);

}
//

void main()
{
    RogueCanDetect();
   
    return;
}
