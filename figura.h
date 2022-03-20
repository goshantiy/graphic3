#ifndef figura_h
#define figura_h
#include <QtGui>
#include <vector>
#include <initializer_list>

typedef std::vector<double> row;

class Matrix : public std::vector<row>
{
public:
    Matrix(std::initializer_list<row> matrix);

    Matrix(int rows, int columns);

    void computeBodyMatrix();

    const Matrix operator * (const Matrix &matrix) const;

private:
    int rowCount;
    int columnCount;
};



class Figura
{
protected:
 Matrix Ver = {{2,2,0},   //0
               {-2,2,0},  //1
               {-2,-2,0}, //2
               {2,-2,0},  //3
               {2,2,4},   //4
               {-2,2,4},  //5
               {-2,-2,4}, //6
               {2,-2,4},  //7
               {0,0,8}};  //8
 int Reb[16][2] = {{0,1},{0,3},{0,4},{1,2},{1,5},{2,3},{2,6},{3,7},{4,5},{4,7},{4,8},{5,6},{5,8},{6,7},{6,8},{7,8}};
 int markedNode = 8;
 int markedReb = 2;

 double VerEk[16][2];
 const int NV = 9;
 const int NR = 16;

 int NGr = 9;
 int NGrVid = 0;

 int gran[9][6] = {{0,1,2,3,0,-1},
                   {0,1,5,4,0,-1},
                   {1,2,6,5,1,-1},
                   {2,3,7,6,2,-1},
                   {3,0,4,7,3,-1},
                   {4,5,8,4,-1,-1},
                   {5,6,8,5,-1,-1},
                   {6,7,8,6,-1,-1},
                   {7,4,8,7,-1,-1}};

 Matrix bodyMatrix;
 int GranVid[9];
 double bodyCentre[3];



public:
 void computeCoordinates(double obsCoord[3]);
 virtual void draw(QPainter *Painter);
 void computeBodyMatrix();
 void checkGran(double obsCoord[3]);

 Figura(int NGr):bodyMatrix(4,NGr)
 {}
};


#endif
