#pragma once
class matrix
{
};

#pragma once

#include <iostream>
#include <array>
#include <algorithm>

#include <stdlib.h>
#include <time.h>

//using namespace std;

/*
template<class T>
class ArrayMatrix
{
private:

	std::array<T,0> _M;

public:
//	Matrix<T>(size_t r, size_t c) : _M ( new T[r * c]) { rows = r; cols = c; };

	const int rows{ R };
	const int cols{ C };

	ArrayMatrix<typename T>(const int R, const int C) { _M = std::array<T, R* C>; }


};
*/



template <typename T>
class Matrix
{
private:
	int _dim[2]{ 0 };
	int& _rows{ _dim[0] };
	int& _cols{ _dim[1] };
	T* _M{ 0 };

protected:
	constexpr int _pos(int row, int col) const { return row > _rows || col > _cols ? -1 : (row - 1) * _cols + col - 1; };

	bool _is_fit(const Matrix& M1, const Matrix& M2) const { return M1._rows == M2._rows && M1._cols == M2._cols; };

	constexpr bool _is_square() { return _rows == _cols; };


public:

	Matrix(const int rows, const int cols); // constructor
	Matrix(const int dim[]); //constructor

	Matrix(const Matrix&); // copy constructor
	//	Matrix(Matrix&&); // move constructor

	~Matrix() { delete[] _M; }; // destructor

	Matrix& operator=(const Matrix&);  //assignment operator
	//	Matrix& operator=(Matrix&&); //move operator

	Matrix& operator=(T); // fill operator

	//	Matrix& operator+(const Matrix&);
		//	Matrix operator+(const Matrix&, const Matrix&);

	Matrix& operator-(const Matrix&);
	Matrix& operator*(const Matrix&);
	Matrix& operator/(const Matrix&);

	Matrix& operator+=(const Matrix&);
	Matrix& operator-=(const Matrix&);
	Matrix& operator*=(const Matrix&);
	Matrix& operator/=(const Matrix&);

	Matrix& operator*=(T); // muiltiply by scalar value

	Matrix& operator/=(double);

	bool operator==(const Matrix& M) const { };
	bool operator!=(const Matrix& M) const;

	bool operator<=>(const Matrix& M) const { return _rows == M._rows && _cols == M._cols; }; // same dimensions?


	T operator()(int row, int column) const { return _M[_pos(row, column)]; };
	T& operator()(int row, int column) { return _M[_pos(row, column)]; };

	constexpr bool is_null() const { return nullptr == _M; }; // is a null matrix
	constexpr bool is_same_size(const Matrix& M) const { return _dim[0] == M._dim[0] && _dim[1] == M._dim[1]; };

	//	friend std::ostream& operator<<(std::ostream& os, const Matrix& m);

	constexpr int n_of_rows() { return _rows; };
	constexpr int n_of_cols() { return _cols; };
	auto dim() const { return _dim; };

};

//int _cord( int row, int col) 

template <typename T>
Matrix<T>::Matrix(const int rows, const int cols)
{
	const long long _size = rows * cols;

	_M = _size ? new T[_size] /* {0} */ : nullptr;
	_rows = _dim[0] = rows; //!!!
	_cols = _dim[1] = cols; //!!!

}

template <typename T>
Matrix<T>::Matrix(const int dim[])
{
	_dim[0] = dim[0];
	_dim[1] = dim[1];

	const long long _size = dim[0] * dim[1];
	_M = _size ? new T[_size]{ 0 } : nullptr;

}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& M2)
{
	_rows = M2._rows;
	_cols = M2._cols;
	const long long _size = _rows * _cols;

	_M = _size ? new T[_size] : nullptr;
	if (_size) std::copy(&M2._M[0], &M2._M[_size], _M);
}




/*
template <typename T>
Matrix<T>::~Matrix()
{
	if ( _M != nullptr ) delete[] _M;
}
*/

///////////////////////

template <typename T>
static const Matrix<T> nullmatrix(0, 0);

/////////////////////////

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& M)
{
	if (this != &M)  // avoid x = x
	{
		delete[] _M;
		//_capacity = M._capacity;
		_dim = M._dim;
		_M = new double[_rows * _cols];
		const long long _size = _rows * _cols;

		for (int i = 0; i < _size; ++i) // !!!
			_M[i] = M._M[i];      // !!!
	}
	return *this;


	if (_rows * _cols)
	{
		_rows = M._rows; _cols = M._cols;
		for (int i = 0; i < _rows * _cols; ++i) { _M[i] = M._M[i]; }
		return *this;
	}
	else return nullmatrix;
}


template <typename T>
Matrix<T>& Matrix<T>::operator=(T value)
{
	for (int i = 0; i < _rows * _cols; ++i) { _M[i] = value; }
	return *this;
}


template <typename T>
Matrix<T> operator+(const Matrix<T>& M1, const Matrix<T>& M2)
{
	if (M1 <=> M2)
	{
		const long long _size = M1.dim()[0] * M1.dim()[1];
		Matrix<T> M3(M1.dim()[0], M1.dim()[1]);

		for (int i = 0; i < M1.dim()[0]; ++i) {
			for (int j = 0; j < M1.dim()[1]; ++j)
			{
				M3(i, j) = M1(i, j) + M2(i, j);
			}
		}

		return M3;
	}
	else return nullmatrix<T>;

}

template <typename T>
Matrix<T> operator-(const Matrix<T>& M1, const Matrix<T>& M2)
{
	if (M1 <=> M2)
	{
		const long long _size = M1.dim()[0] * M1.dim()[1];
		Matrix<T> M3(M1.dim()[0], M1.dim()[1]);

		for (int i = 0; i < M1.dim()[0]; ++i) {
			for (int j = 0; j < M1.dim()[1]; ++j)
			{
				M3(i, j) = M1(i, j) - M2(i, j);
			}
		}

		return M3;
	}
	else return nullmatrix<T>;

}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(T scalar)
{
	for (int i = 0; i < _rows * _cols; ++i) { _M[i] *= scalar; }

	return *this;
}

// inline auto operator<=>(const Matrix& M1, const Matrix& M2)  { return M1._rows == M2._rows && M1._cols == M2._cols; };

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& M)
{

	if (M.is_null())
	{
		std::cout << "null matrix\n\n";
	}
	else
	{
		for (int i = 1; i <= M.dim()[0]; ++i)
		{
			for (int j = 1; j <= M.dim()[1]; ++j)
			{
				//				Matrix<T> local = nullmatrix;
				std::cout << M(i, j) << "   ";
			}
			std::cout << std::endl;
		}
	}

	return os;

}


template <typename T>
T det(Matrix<T>& M) { return 0.0; };