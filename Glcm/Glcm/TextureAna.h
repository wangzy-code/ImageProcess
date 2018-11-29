#pragma once
#include "opencv2/opencv.hpp"
#include <vector>
#include <cassert>

using namespace std;

//灰度共生矩阵类
typedef vector<vector<int>> vGLCM;
class CGlcm
{
private:
   int  m_grayLevel;
public:
    CGlcm();
    ~CGlcm();
    
public:
    void initGlcm(vGLCM &vectorGlcm , int size=16);

    void setGrayLevel(int grayLevel) { m_grayLevel = grayLevel; }

    int getGrayLevel() const { return m_grayLevel; }

    void cooccurrenceEdgeMatrix(cv::Mat & edgeImg, int size, int &shiftDistVer, int &shiftDistHori);

};

