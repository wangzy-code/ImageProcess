
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
// ��������: initGlcm
// ����˵��: vectorGlcm,Ҫ���г�ʼ���Ĺ�������,Ϊ��ά����
//			size, ��ά����Ĵ�С,������ͼ�񻮷ֵĻҶȵȼ����
// ��������: ��ʼ����ά����
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
// ��������: cooccurrenceEdgeMatrix
// ����˵��: src,����ͼ��
//			size, ��Ե��ӡ�������Сƫ����
// ��������: ��Ե������������ӡ��С
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
    //namedWindow("Canny��Եͼ��", 0);
    //imshow("Canny��Եͼ��", dst);
    //imwrite("C:\\Users\\jiance04\\Desktop\\13.bmp", edge);
    //waitKey(0);

    //----------------------------------------------------------------------------------
    //��1��ȷ����ӡ��Χ����S ,Tֵ�Ĵ�С����ֵӰ�������
    //��2����ӡ�Ƿ�ֻ�����������ĻҶȹ������󣬼�������������������
    //��3��ʹ��vector���ɶ�ά���飬�����ʼ�����ڴ��ͷţ����ô�С
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

