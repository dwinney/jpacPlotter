// Header file with useful functions
//
// ---------------------------------------------------------------------------
// Author:       Daniel Winney (2019)
// Affiliation:  Joint Physics Analysis Center (JPAC)
// Email:        dwinney@iu.edu
// ---------------------------------------------------------------------------

#ifndef DATA_HPP
#define DATA_HPP

#include <ios>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <functional>
#include <fstream>
#include <map>
#include <complex>

#include "print.hpp"
#include "math.hpp"

namespace jpacUtils
{
    struct data_set
    {     
        // Number of data points
        int _N = 0;

        std::string _id = "data_set";

        // Each data set should specify what kind of data it is
        // This should match whatever is expected for a particular fitter
        int _type;
        
        // Save up to three data members for each "point"
        // These can include s, t, dsig/dt for example
        std::vector<double> _x, _y, _z;

        // Other possible vectors to store things like bin sizes, etc
        std::array<std::vector<double>, 2> _dx, _dy, _dz;

        // In additon, save any number of extra parameters that may be needed to 
        // identify the data set
        std::map<std::string,double> _extras; 

        // If we want a data entry in the legend when plotting
        bool _add_to_legend = false;
    };

        // ---------------------------------------------------------------------------
    // Read in data into vectors

    // Import a set of data with N columns with relative path 
    // and full path jpacPhoto_dir/ + rel_path
    template<int N> 
    inline std::array<std::vector<double>,N> import_data(std::string abs_path)
    {
        // Add the top level dir path to get full file path
        std::array<std::vector<double>, N> result;
        std::string file_path = abs_path;
        std::ifstream infile(file_path);

        if (!infile.is_open())
        {
            std::cout << "import_data - Cannot open file " << file_path << "!" << std::endl;
            return result;
        };

        // Import data!
        std::string line;
        while (std::getline(infile, line))
        {   
            if (line.empty()) continue;        // skips empty lines
            if (line.front() == '#') continue; // Skip comment lines 
            std::istringstream is(line);   

            for (int i = 0; i < N; i++)
            {
                double x;
                is >> x;
                result[i].push_back(x);
            };
        };
            
        return result;
    };

    // Similar to above except that the data is transposed, i.e. rows are the "categories"
    // and the columns are data points. We specify the number of rows in this case
    template<int N>
    inline std::array<std::vector<double>, N> import_transposed(std::string abs_path)
    {
        // Add the top level dir path to get full file path
        std::array<std::vector<double>, N> result;
        std::string file_path = abs_path;
        std::ifstream infile(file_path);

        if (!infile.is_open())
        {
            std::cout << "import_data - Cannot open file " << file_path << "!" << std::endl;
            return result;
        };

        // Import data!
        for (int i = 0; i < N; i++)
        {   
            std::string line;
            std::getline(infile, line);
            if (line.empty()) continue;        // skips empty lines
            if (line.front() == '#') continue; // Skip comment lines 
            std::istringstream is(line);   

            double x;
            while(is >> x)
            {
                result[i].push_back(x);
            };
        };
            
        return result;
    };

    // If data file has more columns than are actually needed,
    // import with import_data and use this to throw out all but the desired columns
    template<int Nin, int Nout> 
    inline std::array<std::vector<double>,Nout> reshape_data(std::array<std::vector<double>,Nin> data, std::array<int,Nout> to_keep)
    {
        std::array<std::vector<double>, Nout> result;

        for (int i = 0; i < Nout; i++)
        {
            result[i] = data[ to_keep[i] ];
        };
        return result;
    };

    // Make sure all the vectors are the correct size
    template<int S>
    inline int check(std::array<std::vector<double>,S> data, std::string id)
    {
        // Grab the size of the first entry
        int N = data[0].size();
        
        // And compare to the rest
        for (auto column : data)
        {
            if (column.size() != N)
            {
                std::cout << "data_set - Input vectors of " << id << " have mismatching sizes!" << std::endl;
                return 0;
            };
        };

        return N;
    };


    // ---------------------------------------------------------------------------
    // Element-wise operations on data vectors

    // Given two vector<double>s of the same size, calculate the average element wise
    template<typename T>
    inline std::vector<T> operator*( std::vector<T> lhs, double c)
    {
        std::vector<T> result;
        for (int i = 0; i < lhs.size(); i++)
        {
            result.push_back( lhs[i]*c );
        };
        return result;
    };

    template<typename T>
    inline std::vector<T> operator*(double c, std::vector<T> rhs)
    {
        std::vector<T> result;
        for (int i = 0; i < rhs.size(); i++)
        {
            result.push_back( c*rhs[i] );
        };
        return result;
    };

    template<typename T>
    inline std::vector<T> operator/( std::vector<T> lhs, double c)
    {
        std::vector<T> result;
        for (int i = 0; i < lhs.size(); i++)
        {
            result.push_back( lhs[i]/c );
        };
        return result;
    };

    template<typename T>
    inline std::vector<T> operator-(const std::vector<T> & x)
    {
        return -1. * x;
    };

    template<typename T>
    inline std::vector<T> operator/=(const std::vector<T> & x, double c)
    {
        return x/c;
    };

    template<typename T>
    inline std::vector<T> operator*=(const std::vector<T> & x, double c)
    {
        return x*c;
    };
    
    template<typename T>
    inline std::vector<T> operator+(std::vector<T> lhs, std::vector<T> rhs)
    {
        if (lhs.size() != rhs.size()) return error("Attempted to add two vectors of different sizes!", std::vector<double>());

        std::vector<T> result;
        for (uint i = 0; i < lhs.size(); i++) result.push_back( lhs[i] + rhs[i] );
        return result;
    };

    template<typename T>
    inline std::vector<T> operator-(std::vector<T> lhs, std::vector<T> rhs)
    {
        if (lhs.size() != rhs.size()) return error("Attempted to add two vectors of different sizes!", std::vector<double>());

        std::vector<double> result;
        for (uint i = 0; i < lhs.size(); i++) result.push_back( lhs[i] - rhs[i] );
        return result;
    };

    // Add a constant to all elements of a vector
    template<typename T>
    inline std::vector<T> operator+(double lhs, std::vector<T> rhs)
    {
        std::vector<T> result;
        for (uint i = 0; i < rhs.size(); i++) result.push_back( lhs + rhs[i] );
        return result;
    };   
    
    template<typename T>
    inline std::vector<T> operator-(double lhs, std::vector<T> rhs) 
    {
        return lhs + (-rhs);
    };

    template<typename T>
    inline std::vector<T> operator+(std::vector<T> lhs, double rhs)
    {
        std::vector<T> result;
        for (uint i = 0; i < lhs.size(); i++) result.push_back( rhs + lhs[i] );
        return result;
    };

    template<typename T>
    inline std::vector<T> operator-(std::vector<T> lhs, double rhs) 
    {
        return lhs + (-rhs);
    };

    inline std::vector<double> multiply_elementwise(std::vector<double> in1, std::vector<double> in2)
    {
        if (in1.size() != in2.size()) warning("multiply_elementwise()", "Input vectors not the same size!");

        std::vector<double> out;
        for (uint i = 0; i < in1.size(); i++)
        {
            out.push_back(in1[i]*in2[i]);
        }
        return out;
    };
    inline std::vector<double> square_elementwise(std::vector<double> in){ return multiply_elementwise(in, in); };

    inline std::vector<double> real(std::vector<complex> vx)
    {
        std::vector<double> out;
        for (auto x : vx) out.push_back(real(x));
        return out;
    };
    inline std::vector<double> imag(std::vector<complex> vx)
    {
        std::vector<double> out;
        for (auto x : vx) out.push_back(imag(x));
        return out;
    };
};

#endif