// Header file with useful functions
//
// ---------------------------------------------------------------------------
// Author:       Daniel Winney (2019)
// Affiliation:  Joint Physics Analysis Center (JPAC)
// Email:        dwinney@iu.edu
// ---------------------------------------------------------------------------

#include "data_set.hpp"

namespace jpacPlotter
{
    std::vector<double> real(std::vector<std::complex<double>> vx)
    {
        std::vector<double> out;
        for (auto x : vx) out.push_back(real(x));
        return out;
    };
    
    std::vector<double> imag(std::vector<std::complex<double>> vx)
    {
        std::vector<double> out;
        for (auto x : vx) out.push_back(imag(x));
        return out;
    };
};