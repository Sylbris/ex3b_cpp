#include <string>
#include <iostream>
#include <vector>
using namespace std;

namespace zich
{
    class Matrix {
        
        private:
            int _rows;
            int _cols;
            vector<double> _vector;

        public:
            Matrix(const vector<double>& input_v, int rows, int cols);
            friend bool check_order(const Matrix& m1, const Matrix& m2);
            std::vector<double> getRow(string row);
            
            //+
            Matrix operator +(const Matrix& other);
            Matrix operator +(); //Unary
            //-
            Matrix operator -(const Matrix& other);
            Matrix operator -(); //Unary
            //++
            Matrix& operator ++(); //pre increment ++x
            Matrix operator ++(int); //post increment x++
            //--
            Matrix& operator --(); //pre increment --x
            Matrix operator --(int); //post increment x--
            //x=
            Matrix& operator +=(const Matrix& other);
            Matrix& operator -=(const Matrix& other);
            Matrix& operator *=(const double scalar);
            Matrix& operator *=(const Matrix& other);
            //*
            /* Here we use friend because of the symmetry of the * operator,
            a*b is the same is b*a.
            */
            friend Matrix operator *(const double scalar, const Matrix& other);
            friend Matrix operator *(const Matrix& other, const double scalar);
            friend Matrix operator *(const Matrix& m1, const Matrix& m2);

            
            // bool operators will be defined as friends 
            friend bool operator>(const Matrix& m1, const Matrix& m2);
            friend bool operator<(const Matrix& m1, const Matrix& m2);
            friend bool operator==(const Matrix& m1, const Matrix& m2);
            friend bool operator>=(const Matrix& m1, const Matrix& m2);
            friend bool operator<=(const Matrix& m1, const Matrix& m2);
            friend bool operator!=(const Matrix& m1, const Matrix& m2);

            // input, output
            friend std::istream& operator>>(istream& in, Matrix& matrix);
            friend std::ostream& operator<<(ostream& out, const Matrix& matrix);

        
    };    
} 

