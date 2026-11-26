// Useful printing functions and methods
//
// ---------------------------------------------------------------------------
// Author:       Daniel Winney (2024)
// Affiliation:  Universitat Bonn
//               Helmholtz Institute (HISKP)
// Email:        daniel.winney@gmail.com
// ---------------------------------------------------------------------------

#ifndef PRINT_HPP
#define PRINT_HPP

#include <cmath>
#include <ios>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

namespace jpacUtils
{
    // ---------------------------------------------------------------------------
    // ERROR Messages
    
    // Throw an error message then quits code 
    inline void fatal()
    {
        std::cout << std::left << "FATAL ERROR! Quitting..." << std::endl;
        exit( EXIT_FAILURE );
    };

    // Error message with location and reason messages too
    inline void fatal(std::string location, std::string reason = "")
    {
        std::cout << std::left << "FATAL ERROR! " + location + ": " + reason << std::endl;
        std::cout << std::left << "Quitting..." << std::endl;

        exit( EXIT_FAILURE );
    };

    // Warning message does not exit code or returns simply throws a message up
    inline void warning(std::string message)
    {
        std::cout << std::left << "WARNING! " + message << std::endl;
    };

    // Warning message with additional location
    inline void warning(std::string location, std::string message)
    {
        std::cout << std::left << "WARNING! " + location + ": " + message << std::endl;
    };

    // Throw an error message without location and return a value
    template<typename T> 
    inline T error(std::string location, std::string message, T return_value )
    {
        warning(location, message);
        return return_value;
    };
    
    template<typename T> 
    inline T error(std::string message, T return_value )
    {
        warning(message);
        return return_value;
    };

    // Alternatively without a return value, this simply returns void type
    inline void error(std::string message)
    {
        warning(message);
        return;
    };
   
    // ---------------------------------------------------------------------------   
    // Default values
    const int TEXT_WIDTH       = 62;
    const int PRINT_SPACING    = 15;
    const int PRINT_PRECISION  = 9;    
    const int STRING_PRECISION = 3;

    // ---------------------------------------------------------------------------   
    // Output an empty line to the terminal
    inline void line()
    {
        std::cout << std::endl;
    };

    // Print out a horizontal line
    template<uint N=TEXT_WIDTH>
    inline void divider()
    {
        std::cout << std::string(N, '-') << std::endl;
    };

    template<uint N=PRINT_SPACING>
    inline void divider(int n)
    {
        std::string unit_div = std::string(N, '-');
        std::string div;
        for (int i = 0; i < n; i++)
        {
            div = div + unit_div;
        }
        std::cout << div << std::endl;
    };
    
    inline void dashed_divider()
    {
        std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << std::endl;
    };

    template<uint N=PRINT_SPACING, typename T>
    inline void print(T x)
    {
        std::cout << std::boolalpha << std::left << std::setprecision(9);  
        std::cout << std::setw(N) << x << std::endl;
    };

    template <uint NFIRST=PRINT_SPACING, uint NREST=NFIRST, typename First, typename... Rest>
    inline void print(First first, Rest... rest)
    {
        std::cout << std::boolalpha << std::left << std::setprecision(9);  
        std::cout << std::setw(NFIRST) << first;
        print<NREST>(rest...);
    } 

    template<uint N=PRINT_SPACING, typename T>
    inline void print(std::vector<T> v)
    {
        std::cout << std::boolalpha << std::setprecision(9);  
        for (auto vi : v)
        {
            std::cout << std::left << std::setw(N) << vi << std::endl;
        };
    };

    // ---------------------------------------------------------------------------
    // String operations

    // Produce a string with the format "name = value units"

    inline std::string to_string(double d, uint precision = 8)
    {
        std::stringstream ss;
        ss << std::setprecision(precision) << d;
        return ss.str();
    };

    template <typename T>
    inline std::string var_def(std::string name, T value, std::string units = "")
    {
        std::stringstream ss;
        ss << std::setprecision(STRING_PRECISION) << name + " = " << value << " " + units;
        return ss.str();
    };

    // Print a string centered on the terminal 
    template<uint N=PRINT_SPACING>
    inline void centered(int n, std::string words)
    {
        int x = words.length();
        int gap_width = (n * N - x)/2;
        std::cout << std::left << std::setw(gap_width) << "" << std::setw(x) << words << std::setw(gap_width) << "" << std::endl;
    };

    // ---------------------------------------------------------------------------
    // Print N columns of data to file
    // We assume theyre all the same size, if not this breaks and its not our fault

    template<int N>
    inline void print_to_file(std::string outname, std::array<std::vector<double>,N> data, int precision = 12)
    {
        std::ofstream out;
        out.open(outname);
        int spacing = precision + 10;
        for (uint j = 0; j < data[0].size(); j++)
        {
            out << std::left << std::setprecision(precision);
            for (uint i = 0; i < N; i++)
            {
                out << std::setw(spacing) << data[i][j];
            }
            out << std::endl;
        };

        out.close();
        return;
    };

    template<int N>
    inline void print_to_file(std::string outname, std::array<std::string,N> headers, std::array<std::vector<double>,N> data, int precision = 12)
    {
        std::ofstream out;
        out.open(outname);
        int spacing = precision + 10;
        out << std::left << std::setprecision(precision) << std::setw(spacing) << "#" + headers[0];
        for (int i = 1; i < N; i++)
        {
            out << std::setw(spacing) << headers[i]; 
        };
        out << std::endl;

        for (uint j = 0; j < data[0].size(); j++)
        {
            out << std::left;
            for (uint i = 0; i < N; i++)
            {
                out << std::setw(spacing) << data[i][j];
            }
            out << std::endl;
        };

        out.close();
        return;
    };
};
// ---------------------------------------------------------------------------

#endif