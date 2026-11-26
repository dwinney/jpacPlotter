// Useful mathematical functions and methods
//
// ---------------------------------------------------------------------------
// Author:       Daniel Winney (2024)
// Affiliation:  Universitat Bonn
//               Helmholtz Institute (HISKP)
// Email:        daniel.winney@gmail.com
// ---------------------------------------------------------------------------

#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <ios>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <functional>
#include <complex>
#include "print.hpp"

namespace jpacUtils
{
    // ---------------------------------------------------------------------------
    // Mathematical constants 
    
    using complex = std::complex<double>;
    #ifndef PI
        const double PI = M_PI;
    #endif
    #ifndef I
        const complex I(0., 1.);
    #endif
    #ifndef NAN
        const double NAN  = std::numeric_limits<double>::quiet_nan(); 
    #endif
    
    const double DEG2RAD  = (M_PI / 180.);
    const double EPS      = 1.E-8;
    const complex IEPS    = I*EPS;
    const double ALPHA    = 1. / 137.;
    const double HBARC    = 389379; // nb/GeV^2
    const double E        = sqrt(4. * PI * ALPHA);
    
    // ---------------------------------------------------------------------------
    // Out of the box std::complex<double> doesnt play well with ints. Here we explicitly
    // give it the functionality we want

    // Additionally the complex type is a liitle dim in c++ so we need to define int & bool multiplication
    inline complex operator*(const int& c, const complex& rhs)
    {
        return complex(c*rhs.real(), c*rhs.imag());
    };

    inline complex operator*(const complex& lhs, const int& c)
    {
        return complex(c*lhs.real(), c*lhs.imag());
    };

    inline complex operator*(const bool& c, const complex& rhs)
    {
        return (c) ? rhs : 0.;
    };

    inline complex operator*(const complex& lhs, const bool& c)
    {
        return (c) ? lhs : 0.;
    };

    inline complex operator/(const complex&c, const int& i)
    {
        return (1./i)*c;
    };

    inline complex operator/(const int& i, const complex&c)
    {
        return (1./c)*i;
    };

    inline complex operator+(const complex&c, const int& i)
    {
        return c + complex(1,0)*i;
    };

    inline complex operator+(const int& i, const complex & c)
    {
        return complex(1,0)*i + c;
    };

    inline complex operator-(const complex&c, const int& i)
    {
        return c - complex(1,0)*i;
    };

    inline complex operator-(const int& i, const complex & c)
    {
        return complex(1,0)*i - c;
    };

    inline bool operator == (const complex &z,const int n)
    {
        return (z == static_cast<double> (n));
    }
    inline bool operator == (const int n, const complex &z)
    {
        return (z == static_cast<double> (n));
    }
    inline bool operator != (const complex &z,const int n)
    {
        return (z != static_cast<double> (n));
    }
    inline bool operator != (const int n, const complex &z)
    {
        return (z != static_cast<double> (n));
    }
    
    // ---------------------------------------------------------------------------
    // Angles
    
    inline double degrees(double radians){ return radians*DEG2RAD; };
    inline double radians(double degrees){ return degrees/DEG2RAD; };

    // ---------------------------------------------------------------------------
    // Useful math functions

    // This makes it so we always default to complex regardless of whether the input is an int or double
    template<typename T>
    complex csqrt(T x){ return sqrt(x * complex(1,0)); };

    // Gamma function of complex argument 'z'
    // optional parameter OPT = 1 can be used to return the log of Gamma(z)
    std::complex<double> cgamma(std::complex<double> z, int OPT = 0);

    unsigned int factorial(unsigned int n);

        // Legendre function in terms of cosine theta
    complex legendre(int l, complex z);
    
    // Kallen Triangle function
    // Only way to get a double or int Kallen is if all inputs are double/int
    template<typename T>
    inline T kallen(T x, T y, T z)
    {
        return x*x + y*y + z*z - 2. * (x*y + x*z + y*z);
    };

    // If any of them are complex, return complex
    inline complex kallen(complex z, double a, double b) { return kallen<complex>(z, complex(1,0)*a, complex(1,0)*b); };
    inline complex kallen(double a, complex z, double b) { return kallen<complex>(complex(1,0)*a, z, complex(1,0)*b); };
    inline complex kallen(double a, double b, complex z) { return kallen<complex>(complex(1,0)*a, complex(1,0)*b, z); };

    inline int sign(double x){ return (x >= 0) ? +1 : -1; }

    // ---------------------------------------------------------------------------
    // Wigner d-func coefficient of leading power
    double wigner_leading_coeff(int j, int lam1, int lam2);

    // Wigner d-function for half-integer spin
    double wigner_d_half(int j, int lam1, int lam2, double theta);

    // Wigner d-function for integer spin
    double wigner_d_int(int j, int lam1, int lam2, double theta);

    // Wigner d-function for integer spin in terms of the cosine of theta not theta
    complex wigner_d_int_cos( int j, int lam1, int lam2, complex cos);
    complex wigner_d_half_cos(int j, int lam1, int lam2, complex cos);

    // ---------------------------------------------------------------------------
    // Function for easier comparison of doubles using the EPS value defined above
    // be careful when using this in general purposes since its a fixed-tolerance comparision and not always appropriate

    inline bool are_equal(double a, double b)
    {
        return ( std::abs(a - b) < EPS );
    }

    inline bool are_equal(double a, double b, double tol)
    {
        return ( std::abs(a - b) < tol );
    }

    // Same thing for comparing complex doubles
    inline bool are_equal(complex a, complex b)
    {
        return (are_equal(real(a), real(b)) && are_equal(imag(a), imag(b)));
    };

    // Same thing for comparing complex doubles
    inline bool are_equal(complex a, complex b, double tol)
    {
        return (are_equal(real(a), real(b), tol) && are_equal(imag(a), imag(b), tol));
    };

    // Aliases for special cases of the above
    template<typename T>
    inline bool is_zero(T a)
    {
        return ( std::abs(a) < EPS);
    };

    // Aliases for special cases of the above
    template<typename T>
    inline bool is_zero(T a, double tol)
    {
        return ( std::abs(a) < tol);
    };

    inline bool is_odd(int a)
    {
        return (a%2 == 1);
    };

    inline bool is_real(complex a, double tol = EPS)
    {
        return is_zero(imag(a), tol);
    };

    inline bool is_imaginary(complex a, double tol = EPS)
    {
        return is_zero(real(a), tol);
    };

    // ---------------------------------------------------------------------------
    // Simple functions to calculate numerical derivatives up to error ~ h^4
    // Coefficients taken from https://en.wikipedia.org/wiki/Finite_difference_coefficient
    
    // Central difference 
    template<typename T> 
    inline T central_difference_derivative(uint n, std::function<T(double)> F, double x, double h = 1E-3)
    {
        T f3m, f2m, fm, f, fp, f2p, f3p;
        f   = F(x);
        fp  = F(x+h),    fm = F(x-h);
        f2p = F(x+2*h), f2m = F(x-2*h);
        if (n >= 3) { f3p = F(x+3*h); f3m = F(x-3*h); };
        
        T num;
        switch (n)
        {
            case 0  : return F(x);
            case 1  : num =          +f2m/12  -2*fm/3          +2*fp/3   -f2p/12;          break;
            case 2  : num =          -f2m/12  +4*fm/3  -5*f/2  +4*fp/3   -f2p/12;          break;
            case 3  : num = +f3m/8   -f2m    +13*fm/8         -13*fp/8   +f2p     -f3p/8;  break;
            case 4  : num = -f3m/6 +2*f2m    -13*fm/2 +28*f/3 -13*fp/2 +2*f2p     -f3p/6;  break;
            default : 
            {
                warning("central_difference_derivative", "Order n = "+to_string(n)+" derivatives not implemented!"); 
                return NAN;
            };
        };
        return num / pow(h, n);
    };

    // Forward difference
    template<typename T> 
    inline T forward_difference_derivative(uint n, std::function<T(double)> F, double x, double h = 1E-3)
    {
        if (n == 0) return F(x);

        std::vector<double> c;
        switch (n)
        {
            // O(h^4)
            case 1  : c = {-25./12, 4., -3., 4./3, -1./4}; break;
            case 2  : c = {15./4, -77./6, 107./6, -13., 61./12, -5./6}; break;
            case 3  : c = {-49./8, 29., -461./8, 62., -307./8, 13., -15./8}; break;
            case 4  : c = {28./3, -111./2, 142., -1219/6., 176., -185./2, 82./3 ,-7./2}; break;
            default : 
            {
                warning("forward_difference_derivative", "Order n = "+to_string(n)+" derivatives not implemented!"); 
                return NAN;
            };
        };
        T num = 0;
        for (int i = 0; i < c.size(); i++) num += c[i] * F(x+i*h);

        return num / pow(h, n);
    };
    
    // and finally backward finite difference
    template<typename T> 
    inline T backward_difference_derivative(uint n, std::function<T(double)> F, double x, double h = 1E-3)
    {
        if (n == 0) return F(x);

        std::vector<double> c;
        switch (n)
        {
            // These are O(h^4)
            case 1  : c = {-25./12, 4., -3., 4./3, -1./4}; break;
            case 2  : c = {15./4, -77./6, 107./6, -13., 61./12, -5./6}; break;
            case 3  : c = {-49./8, 29., -461./8, 62., -307./8, 13., -15./8}; break;
            case 4  : c = {28./3, -111./2, 142., -1219/6., 176., -185./2, 82./3 ,-7./2}; break;
            default : 
            {
                warning("forward_difference_derivative", "Order n = "+to_string(n)+" derivatives not implemented!"); 
                return NAN;
            };
        };
        T num = 0;
        for (int i = 0; i < c.size(); i++) num += c[i] * F(x-i*h);

        return pow(-1, n) * num / pow(h, n);
    };

    // Mixed central derivative of a function of 2 variables d2F(x,y)/dxdy
    template<typename T>
    inline T mixed_central_derivatives(std::function<T(double,double)> F, std::array<double,2> xs, double e)
    {
        double x = xs[0], y = xs[1];
        T f2p = central_difference_derivative<T>(1, [&](double s){ return F(s, y+2*e); },  x, e);
        T fp  = central_difference_derivative<T>(1, [&](double s){ return F(s, y+e);   },  x, e);
        T fm  = central_difference_derivative<T>(1, [&](double s){ return F(s, y-e);   },  x, e);
        T f2m = central_difference_derivative<T>(1, [&](double s){ return F(s, y-2*e); },  x, e);

        return (+f2m/12.-2.*fm/3.+2.*fp/3.-f2p/12.)/e;
    };

    // Mixed forward derivative of a function of 2 variables d2F(x,y)/dxdy
    template<typename T>
    inline T mixed_forward_derivatives(std::function<T(double,double)> F, std::array<double,2> xs, double e)
    {
        double x = xs[0], y = xs[1];
        std::array<double,5> c = {-25./12, 4., -3., 4./3, -1./4};
        T sum = 0.;
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                sum += c[i]*c[j]*F(x+i*e, y+j*e);
            };
        };
        return sum/e/e;
    };

    template<typename T>
    inline T mixed_backward_derivatives(std::function<T(double,double)> F, std::array<double,2> xs, double e)
    {
        double x = xs[0], y = xs[1];
        std::array<double,5> c = {-25./12, 4., -3., 4./3, -1./4};
        T sum = 0.;
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                sum += c[i]*c[j]*F(x-i*e, y-j*e);
            };
        };
        return sum/e/e;
    };
};
// ---------------------------------------------------------------------------

#endif