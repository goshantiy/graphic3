#include <math.h>
#include "figura.h"

Matrix::Matrix(std::initializer_list<row> matrix) : std::vector<row>(matrix)
{
    rowCount = matrix.size();
    columnCount = matrix.begin()->size();
}

Matrix::Matrix(int rows, int columns) : std::vector<row>(rows, row(columns))
{
    rowCount = rows;
    columnCount = columns;
}

const Matrix Matrix::operator*(const Matrix &obj) const
{
    Matrix res(rowCount, obj.columnCount);

    for (int j = 0; j < rowCount; j++)
    {
        for (int i = 0; i < res.columnCount; i++)
        {
            for (int k = 0; k < columnCount; k++)
            {
                res[j][i] += (*this)[j][k] * obj[k][i];
            }
        }
    }

    return res;
}

void Figura::computeBodyMatrix()
{
    for(int i = 0; i < 3; i++)
    {
        bodyCentre[i] = 0;
        for(int j = 0; j < NV; j++)
            bodyCentre[i] += Ver[j][i];
        bodyCentre[i] /= (double)NV;
    }


    int i = 0;
    for(auto &facet : gran)
    {
        double A = (Ver[facet[2]][1] - Ver[facet[0]][1])*(Ver[facet[1]][2] - Ver[facet[0]][2]) -
                (Ver[facet[1]][1] - Ver[facet[0]][1])*(Ver[facet[2]][2] - Ver[facet[0]][2]);
        double B = (Ver[facet[1]][0] - Ver[facet[0]][0])*(Ver[facet[2]][2] - Ver[facet[0]][2]) -
                (Ver[facet[2]][0] - Ver[facet[0]][0])*(Ver[facet[1]][2] - Ver[facet[0]][2]);
        double C = (Ver[facet[2]][0] - Ver[facet[0]][0])*(Ver[facet[1]][1] - Ver[facet[0]][1]) -
                (Ver[facet[1]][0] - Ver[facet[0]][0])*(Ver[facet[2]][1] - Ver[facet[0]][1]);
        double D = -(A*Ver[facet[0]][0] + B*Ver[facet[0]][1] + C*Ver[facet[0]][2]);

        if (A*bodyCentre[0] + B*bodyCentre[1] + C*bodyCentre[2] + D < 0)
        {
            A *= -1;
            B *= -1;
            C *= -1;
            D *= -1;
        }

        bodyMatrix[0][i] = A;
        bodyMatrix[1][i] = B;
        bodyMatrix[2][i] = C;
        bodyMatrix[3][i] = D;
        i++;
    }
}


void Figura::checkGran(double obsCoord[3])
{

    NGrVid = 0;
    for(int i = 0; i < NGr; i++)
    {
        if(bodyMatrix[0][i]*obsCoord[0] + bodyMatrix[1][i]*obsCoord[1] + bodyMatrix[2][i]*obsCoord[2] + bodyMatrix[3][i] < 0)
        {
            GranVid[NGrVid] = i;
            NGrVid ++;
        }
    }

}



void Figura::computeCoordinates(double obsCoord[3])
{

    Matrix T = {{1,0,0,0},
                {0,1,0,0},
                {0,0,1,0},
                {-obsCoord[0],-obsCoord[1],-obsCoord[2],1}};
    Matrix S = {{-1,0,0,0},
                {0,1,0,0},
                {0,0,1,0},
                {0,0,0,1}};
    Matrix Rx90 = {{1,0,0,0},
                   {0,0,-1,0},
                   {0,1,0,0},
                   {0,0,0,1}};

    double d = std::sqrt(std::pow(obsCoord[0],2) + std::pow(obsCoord[1],2));
    double sinU;
    double cosU;
    if (d == 0)
    {
        sinU = 0;
        cosU = 1;
    } else {
        sinU = obsCoord[0] / d;
        cosU = obsCoord[1] / d;
    }

    Matrix Ry = {{cosU,0,sinU,0},
                 {0,1,0,0},
                 {-sinU,0,cosU,0},
                 {0,0,0,1}};

    double s = std::sqrt(std::pow(obsCoord[0],2)    + std::pow(obsCoord[1],2) + std::pow(obsCoord[2],2));
    double sinW;
    double cosW;
    if(s == 0)
    {
        sinW = 0;
        cosW = 1;
    } else {
        sinW = obsCoord[2] / s;
        cosW = d / s;
    }

    Matrix Rx = {{1,0,0,0},
                 {0,cosW,-sinW,0},
                 {0,sinW,cosW,0},
                 {0,0,0,1}};


    Matrix V = T*S*Rx90*Ry*Rx;

    Matrix VerNa(NV,3);

    for(int i = 0; i < NV; i++)
    {
        Matrix buf = {{Ver[i][0],Ver[i][1],Ver[i][2],1}};
        VerNa[i] = (buf * V)[0];
    }

    Matrix VerKa(NV,2);

    for(int i = 0; i < NV; i++)
    {
        if (VerNa[i][2] != 0)
        {
            VerKa[i][0] = VerNa[i][0] / VerNa[i][2] * s;
            VerKa[i][1] = VerNa[i][1] / VerNa[i][2] * s;
        }
        else
            VerNa[i][0] = VerNa[i][1] = -1;
    }


    double Pk = 3;
//    double Xc = 500;
//    double Yc = 300;
//    double Xe = 300;
//    double Ye = 200;
    double Xc = 500;
    double Yc = 600;
    double Xe = 200;
    double Ye = 200;

    for(int i = 0 ; i < NV; i++)
    {
        VerEk[i][0] = (VerKa[i][0] * Xe / Pk) + Xc;
        VerEk[i][1] = (VerKa[i][1] * Ye / Pk + Yc) * (-1) + 2*Yc;
    }

}

void Figura::draw(QPainter *painter)
{

    for(int i = 0; i < NGrVid; i++)
    {
        int curGr = GranVid[i];
        int curVer1 = gran[curGr][0];
        int curVer2 = gran[curGr][1];
        int k = 2;
        while (curVer2 >= 0)
        {
            painter->drawLine(VerEk[curVer1][0],VerEk[curVer1][1],VerEk[curVer2][0],VerEk[curVer2][1]);\
            curVer1 = curVer2;
            curVer2 = gran[curGr][k];
            k++;
        }

    }
}

