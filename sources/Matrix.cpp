#include <string>
#include <iostream>
#include <vector>
#include "Matrix.hpp"
using namespace std;
#include <numeric>
#include <sstream>

namespace zich
{
    /**
     * @brief Construct a new Matrix:: Matrix object
     * 
     * @param input_v 
     * @param rows 
     * @param cols 
     */
    Matrix::Matrix(const std::vector<double>& input_v, int rows, int cols){
        //test parameters
        if(rows < 1 || cols < 1){
            throw invalid_argument("bad arguments");
        }
        //test order.
        if((rows*cols) != input_v.size()){
            throw invalid_argument(to_string(rows) +" "+to_string(cols) + "incompatible vector with matrix sizes. " + to_string(rows*cols) + " "+ to_string(input_v.size()));
        }
        this->_vector = input_v;
        this->_rows = rows;
        this->_cols = cols;
    };
    /**
     * @brief Check if two matrices are of the same order.
     * 
     * @param m1 
     * @param m2 
     * @return true 
     * @return false 
     */
    bool check_order(const Matrix& m1, const Matrix& m2){
        return (m1._rows==m2._rows && m1._cols==m2._cols);
    }

    //+
    Matrix Matrix::operator +(const Matrix& other){
        if(check_order(*this, other))
        {
            std::vector<double> sum_vec(other._vector.size());
            std::vector<double> this_vec = _vector;
            std::vector<double> other_vec = other._vector;

            std::transform (this_vec.begin(), this_vec.end(), other_vec.begin(), sum_vec.begin(), std::plus<int>());
            Matrix sum_mat{sum_vec,other._rows,other._cols};
            return sum_mat;
        }
        throw invalid_argument("Matrices are not of same size");
        };

    Matrix Matrix::operator +()
    {
        return Matrix{this->_vector, this->_rows, this->_cols};
    };
    //-
    Matrix Matrix::operator -(const Matrix& other){
        if(check_order(*this, other))
        {
            std::vector<double> sub_vec(other._vector.size());
            std::vector<double> this_vec = _vector;
            std::vector<double> other_vec = other._vector;

            std::transform (this_vec.begin(), this_vec.end(), other_vec.begin(), sub_vec.begin(), std::minus<int>());
            Matrix sub_mat{sub_vec,other._rows,other._cols};
            return sub_mat;
        }
        throw invalid_argument("Matrices are not of same size");
        };
    Matrix Matrix::operator -(){
        const int scale = -1;
        std::vector<double> this_vec = _vector;
        std::vector<double> neg_vec(this_vec.size());
        std::transform(this_vec.begin(), this_vec.end(), neg_vec.begin(),
                   [&scale](double element) { return element *= scale; });
        return Matrix{neg_vec, this->_rows, this->_cols};
    };
    //++
    Matrix& Matrix::operator ++()
    {
        const int scale = 1;
        std::transform(_vector.begin(), _vector.end(), _vector.begin(),
                   [&scale](double element) { return element += scale; });
        return *this;
    }; //pre increment ++x
    Matrix Matrix::operator ++(int){
        const int scale = 1;
        vector<double> old_vec(_vector);
        std::transform(_vector.begin(), _vector.end(), _vector.begin(),
                   [&scale](double element) { return element += scale; });
        return Matrix{old_vec, this->_rows, this->_cols};
    }; //post increment x++
    //--
    Matrix& Matrix::operator --(){
        const int scale = 1;
        std::transform(_vector.begin(), _vector.end(), _vector.begin(),
                   [&scale](double element) { return element -= scale; });
        return *this;
    }; //pre increment --x
    Matrix Matrix::operator --(int){
        const int scale = 1;
        vector<double> old_vec(_vector);
        std::transform(_vector.begin(), _vector.end(), _vector.begin(),
                   [&scale](double element) { return element -= scale; });
        return Matrix{old_vec, this->_rows, this->_cols};
    }; //post increment x--
    //x=
    Matrix& Matrix::operator +=(const Matrix& other){
        if(check_order(*this, other)){
            std::transform (this->_vector.begin(), this->_vector.end(), other._vector.begin(), this->_vector.begin(), std::plus<int>());
            return *this;
        }
        throw invalid_argument("Incompatible sizes");
    };
    Matrix& Matrix::operator -=(const Matrix& other){
        if(check_order(*this, other)){
            std::transform (this->_vector.begin(), this->_vector.end(), other._vector.begin(), this->_vector.begin(), std::minus<int>());
            return *this;
        }
        throw invalid_argument("Incompatible sizes");
    };
    Matrix& Matrix::operator*=(const double scalar){
        std::transform(_vector.begin(), _vector.end(), _vector.begin(),
                   [&scalar](double element) { return element *= scalar; });
        return *this;
    };
    Matrix& Matrix::operator*=(const Matrix& other){
        if(this->_cols == other._rows){
            vector<double> result((size_t)(this->_rows*other._cols));
            for(size_t i=0;i < this->_rows;i++){
                for(size_t j=0;j< other._cols ;j++){
                    for(size_t k=0; k< other._rows;k++){
                        result[(i*(size_t)other._cols)+j] += this->_vector[(i*(size_t)this->_cols)+k] * other._vector[(k*(size_t)other._cols) +j];
                        //cout<< to_string(i+j)<< " " <<to_string(result[i+j]) << " += " << to_string(m1._vector[(i*(size_t)m1._cols)+k]) << " * " << to_string(m2._vector[(k*(size_t)m2._cols) +j]) << "\n";
                    }
                }
        
            }
            Matrix b{result,this->_rows,other._cols};
            *this = b;
            return *this;
        }
        throw invalid_argument("incompatible matrices");
    };
    //*
    /* Here we use friend because of the symmetry of the * operator,
    a*b is the same is b*a.
    */
    Matrix operator*(const double scalar, const Matrix& other){
        vector<double> product(other._vector.size());
        std::transform(other._vector.begin(), other._vector.end(), product.begin(),
                   [&scalar](double element) { return element *= scalar; });
        return Matrix{product, other._rows, other._cols};
    };
    Matrix operator*(const Matrix& other, const double scalar){
        vector<double> product(other._vector.size());
        std::transform(other._vector.begin(), other._vector.end(), product.begin(),
                   [&scalar](double element) { return element *= scalar; });
        return Matrix{product, other._rows, other._cols};
    };
    /**
     * @brief Matrix multiplication, as we know from liniear algebra.
     * works both on square or rectangle matrices, here I used a new vector
     * and treated the old arrays as if they were matrices. I used x + y*offset.
     * 
     * 
     * @param m1 
     * @param m2 
     * @return Matrix 
     */
    Matrix operator *(const Matrix& m1, const Matrix& m2){
        if(m1._cols == m2._rows){
        vector<double> result((size_t)(m1._rows*m2._cols));
        for(size_t i=0;i < m1._rows;i++){
            for(size_t j=0;j< m2._cols ;j++){
                for(size_t k=0; k< m2._rows;k++){
                    result[(i*(size_t)m2._cols)+j] += m1._vector[(i*(size_t)m1._cols)+k] * m2._vector[(k*(size_t)m2._cols) +j];
                    //cout<< to_string(i+j)<< " " <<to_string(result[i+j]) << " += " << to_string(m1._vector[(i*(size_t)m1._cols)+k]) << " * " << to_string(m2._vector[(k*(size_t)m2._cols) +j]) << "\n";
                }
            }
    
        }
            Matrix b{result,m1._rows,m2._cols};
            return b;
        }
        throw invalid_argument("incompatible matrices");
    
    }
            
    // bool operators will be defined as friends 
    bool operator>(const Matrix& m1, const Matrix& m2){
        if(check_order(m1, m2)){
            double sum_of_m1 = std::accumulate(m1._vector.begin(), m1._vector.end(), 0.0);
            double sum_of_m2 = std::accumulate(m2._vector.begin(), m2._vector.end(), 0.0);
            return sum_of_m1 > sum_of_m2;
        }
        throw invalid_argument("matrices are different sizes");
        
    };
    bool operator<(const Matrix& m1, const Matrix& m2){
        if(check_order(m1, m2)){
            double sum_of_m1 = std::accumulate(m1._vector.begin(), m1._vector.end(), 0.0);
            double sum_of_m2 = std::accumulate(m2._vector.begin(), m2._vector.end(), 0.0);
            return sum_of_m1 < sum_of_m2;
        }
        throw invalid_argument("matrices are different sizes");
    };
    bool operator==(const Matrix& m1, const Matrix& m2){
        if(check_order(m1, m2)){
            return m1._vector==m2._vector;
        }
        throw invalid_argument("matrices are different sizes");
        };
    bool operator>=(const Matrix& m1, const Matrix& m2){
        if(check_order(m1, m2)){
            double sum_of_m1 = std::accumulate(m1._vector.begin(), m1._vector.end(), 0.0);
            double sum_of_m2 = std::accumulate(m2._vector.begin(), m2._vector.end(), 0.0);
            return sum_of_m1 >= sum_of_m2;
        }
        throw invalid_argument("matrices are different sizes");
    };
    bool operator<=(const Matrix& m1, const Matrix& m2){
        if(check_order(m1, m2)){
            double sum_of_m1 = std::accumulate(m1._vector.begin(), m1._vector.end(), 0.0);
            double sum_of_m2 = std::accumulate(m2._vector.begin(), m2._vector.end(), 0.0);
            return sum_of_m1 <= sum_of_m2;
            }
        throw invalid_argument("matrices are different sizes");
        
    };
    bool operator!=(const Matrix& m1, const Matrix& m2){
        if(check_order(m1, m2)){
            return m1._vector!=m2._vector;
        }
        throw invalid_argument("matrices are different sizes");
    };

    // input, output
    std::istream& operator>>(istream& in, Matrix& matrix){
        string buff;
        getline(in, buff, '\n'); // get all the input.
        std::vector<double> vect;
        int rows = 0;
        int cols = 0;
        size_t start_index = 1;
        for(size_t i =0 ; i< buff.size(); i++){
            if(buff[i] == ','){
                rows++;
            }
            if(buff[i] == ',' && buff[i+1] != ' '){
                throw invalid_argument("bad input");
            }
            if((buff[i] == ' ' || buff[i] == ']') && buff[i-1] != ','){
                cols++;
                vect.push_back(stod(buff.substr(start_index, i-1)));
                start_index = i-1;
            }
        }

        rows++;
        cols = cols/rows;
        matrix = Matrix{vect, rows, cols};
        cout << matrix;
        return in;
    };
    std::ostream& operator<<(ostream& out, const Matrix& matrix){
        string ans;
        size_t idx=0;
        for(int i=0;i<matrix._rows;i++){
            ans+= "[";
            for(int j=0;j<matrix._cols && idx<matrix._vector.size();j++){
                ans+=to_string((int)matrix._vector[idx++]);
                if(j != matrix._cols-1){
                    ans+=" ";}
            }
            ans+= "]";
            if( i != matrix._rows-1){
            ans+="\n";
            }
        }
        out << ans ;
        return out;
    };

};       


