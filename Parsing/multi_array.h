
#pragma once
#include <array>
#include <iostream>
#include <iomanip>

#include <ranges>

namespace my {

    //template <typename T, size_t N> using multi_array = std::array<T, N> ;
/*
    template<typename T, size_t Dim1, size_t Dim2, size_t... Dims>
    class multi_array :
        public std::array<multi_array<T, Dim2, Dims...>, Dim1>
    {};

    template<typename T, size_t Dim1, size_t Dim2> 
    using matrix = multi_array<T, Dim1, Dim2>;

    template<typename T, size_t Dim>
    using square_matrix = matrix<T, Dim, Dim>;

*/
    template <typename T, size_t R, size_t C>
    class multi_array :
        public std::array<std::array<T, C>, R>
    {
    private:
        /*
            size_t _capacity = R * C;
            size_t _rows;
            size_t _cols;
        */
    public:

        multi_array<T, R, C>() : std::array<std::array<T, R>, C>() {};

        multi_array(const multi_array& M) : std::array <std::array<T, R>, C>(M) {  };

        //    multi_array<T, R, C>& operator=(const multi_array<T, R, C>& M) { };

        constexpr size_t no_columns() const { return C; };
        constexpr size_t no_rows() const { return R; };
        constexpr size_t capacity() const { return R * C; };


    };

    constexpr size_t pin(const size_t _row, const size_t _col) { return _row * _col + _col; };

    template <typename T, size_t R, size_t C>
    multi_array<T, R, C> operator+(const multi_array<T, R, C>& M1, const multi_array<T, R, C>& M2)
    {
        multi_array<T, R, C> M;

        for (int i = 0; i < M.capacity(); ++i) { M[i] = M1[i] + M2[i]; }

        return std::move(M);
    }

    template <typename T, size_t R, size_t C, size_t X >
    multi_array<T, R, C> operator*(const multi_array<T, R, X>& M1, const multi_array <T, X, C>& M2)
    {
        multi_array<T, R, C> M;
        T temp = 0;
        //size_t pos = 0;

        for (int i = 0; i < R; ++i)
        {
            for (int j = 0; j < C; ++j)
            {
                temp = 0;
                for (int k = 0; k < X; ++k)
                {
                    temp += M1[pin(i, k)] * M2[pin(k, j)];
                }
                M[pin(i, j)] = temp;
            }

        }

        return M;
    }

    template <typename T, size_t R, size_t C>
    std::ostream& operator<<(std::ostream& os, const multi_array<T, R, C>& M)
    {

        std::cout << std::endl;

        int counter = 0;

        for (auto& elem : M)
        {
            std::cout << "|" << std::setw(3) << elem;

            ++counter% C ? std::cout << " " : std::cout << "|\n";
        }

        return os;

    }
} // end of namespace my