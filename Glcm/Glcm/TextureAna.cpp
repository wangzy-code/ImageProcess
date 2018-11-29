
#include "TextureAna.h"
using namespace cv;
CGlcm::CGlcm() :
    m_grayLevel(10)
{
};

CGlcm::~CGlcm()
{

}

//==============================================================================
// 函数名称: initGlcm
// 参数说明: vectorGlcm,要进行初始化的共生矩阵,为二维方阵
//			size, 二维矩阵的大小,必须与图像划分的灰度等级相等
// 函数功能: 初始化二维矩阵
//==============================================================================
void CGlcm::initGlcm(vGLCM &vectorGlcm, int size)
{
    assert(size == m_grayLevel);
    vectorGlcm.resize(size);
    for (int i = 0; i < size; i++)
    {
        vectorGlcm[i].resize(size);
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            vectorGlcm[i][j] = 0;
        }
    }
}

//==============================================================================
// 函数名称: cooccurrenceEdgeMatrix
// 参数说明: src,输入图像
//			size, 边缘套印的最大最小偏移量
// 函数功能: 边缘共生矩阵检测套印大小
//==============================================================================
void CGlcm::cooccurrenceEdgeMatrix(Mat & src, int size, int &shiftDistVer, int &shiftDistHori)
{
    Mat srcCopy = src.clone();
    Mat dst, edge, blurImg;
    dst.create(srcCopy.size(), srcCopy.type());
    medianBlur(srcCopy, blurImg, 7);
    Canny(blurImg, edge, 26, 45, 3);
    dst = Scalar::all(0);
    srcCopy.copyTo(dst, edge);
    //namedWindow("Canny边缘图像", 0);
    //imshow("Canny边缘图像", dst);
    //imwrite("C:\\Users\\jiance04\\Desktop\\13.bmp", edge);
    //waitKey(0);

    //----------------------------------------------------------------------------------
    //【1】确定套印范围决定S ,T值的大小，该值影响计算量
    //【2】套印是否只检测两个方向的灰度共生矩阵，减少数据量及遍历次数
    //【3】使用vector生成二维数组，方便初始化，内存释放，重置大小
    //---------------------------------------------------------------------------------
    vGLCM vectorGlcm;
    CGlcm cGlcm;
    cGlcm.initGlcm(vectorGlcm, size);

    for (int s = 0; s < size; s++)
    {
        for (int i = 0; i < edge.rows; i++)
        {
            for (int j = 0; j < edge.cols; j++)
            {
                if ((i + s) < edge.rows && (j) < edge.cols)
                {
                    uchar value1 = edge.at<uchar>(i, j);
                    uchar value2 = edge.at<uchar>(i + s, j);
                    if ((value1 == 255) && (value2 == 255))
                    {
                        vectorGlcm[s][0]++;
                    }

                }
            }
        }
    }

    for (int t = 0; t < size; t++)
    {
        for (int i = 0; i < edge.rows; i++)
        {
            for (int j = 0; j < edge.cols; j++)
            {
                if ((i) < edge.rows && (j + t) < edge.cols)
                {
                    uchar value1 = edge.at<uchar>(i, j);
                    uchar value2 = edge.at<uchar>(i, j + t);
                    if ((value1 == 255) && (value2 == 255))
                    {
                        vectorGlcm[0][t]++;
                    }

                }
            }
        }
    }

    shiftDistVer = 0;
    shiftDistHori = 0;

    for (int s = 1; s < size - 2; s++)
    {
        if (vectorGlcm[s][0] > vectorGlcm[s - 1][0])
        {
            if (vectorGlcm[s][0] > vectorGlcm[s + 1][0])
            {
                shiftDistVer = s;
            }
        }
    }
    for (int t = 1; t < size - 2; t++)
    {
        if (vectorGlcm[0][t] > vectorGlcm[0][t - 1])
        {
            if (vectorGlcm[0][t] > vectorGlcm[0][t + 1])
            {
                shiftDistHori = t;
            }
        }
    }
    for (int s = 0; s < size; s++)
    {
        for (int t = 0; t < size; t++)
        {
            std::cout << vectorGlcm[s][t] << "    ";
        }
        std::cout << std::endl;
    }

    /*std::cout << shiftDistVer << endl;
    std::cout << shiftDistHori << endl;*/
    //waitKey(0);
}

