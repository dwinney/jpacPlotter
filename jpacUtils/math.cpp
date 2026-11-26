// Useful mathematical functions and methods
//
// ---------------------------------------------------------------------------
// Author:       Daniel Winney (2026)
// Affiliation:  Joint Physics Analysis Center (JPAC),
//               Instituto de Ciencias Nuclears (ICN),
//               Universidad Nacional Autonoma de México (UNAM)
// Email:        daniel.winney@nucleares.unam.mx
// ------------------------------------------------------------------------------

#include "math.hpp"

namespace jpacUtils
{
    std::complex<double> cgamma(std::complex<double> z,int OPT)
    // OPT = 0 for Gamma ; OPT = 1 for log(Gamma)
    {
        std::complex<double> I(0,1);
        std::complex<double> g, infini= 1e308+ 0.*I; // z0,z1
        double x0,q1,q2,x,y,th,th1,th2,g0,gr,gi,gr1,gi1;
        double na,t,x1,y1,sr,si;
        int j,k;
        x1=9e9;
        na=9e9;

        static double a[] = {
            8.333333333333333e-02,
            -2.777777777777778e-03,
            7.936507936507937e-04,
            -5.952380952380952e-04,
            8.417508417508418e-04,
            -1.917526917526918e-03,
            6.410256410256410e-03,
            -2.955065359477124e-02,
            1.796443723688307e-01,
            -1.39243221690590};

        x = real(z); x1 = x;
        y = imag(z); y1 = y;
        if (x > 171) return infini;
        if ((y == 0.0) && (x == (int)x) && (x <= 0.0))
            return infini;
        else if (x < 0.0) {
            x = -x;
            y = -y;
        }
        x0 = x;
        if (x <= 7.0) {
            na = (int)(7.0-x);
            x0 = x+na;
        }
        q1 = sqrt(x0*x0+y*y);
        th = atan(y/x0);
        gr = (x0-0.5)*log(q1)-th*y-x0+0.5*log(2.0*M_PI);
        gi = th*(x0-0.5)+y*log(q1)-y;
        for (k=0;k<10;k++){
            t = pow(q1,-1.0-2.0*k);
            gr += (a[k]*t*cos((2.0*k+1.0)*th));
            gi -= (a[k]*t*sin((2.0*k+1.0)*th));
        }
        if (x <= 7.0) {
            gr1 = 0.0;
            gi1 = 0.0;
            for (j=0;j<na;j++) {
            gr1 += (0.5*log((x+j)*(x+j)+y*y));
            gi1 += atan(y/(x+j));
            }
            gr -= gr1;
            gi -= gi1;
        }
        if (x1 <= 0.0) {
            q1 = sqrt(x*x+y*y);
            th1 = atan(y/x);
            sr = -sin(M_PI*x)*cosh(M_PI*y);
            si = -cos(M_PI*x)*sinh(M_PI*y);
            q2 = sqrt(sr*sr+si*si);
            th2 = atan(si/sr);
            if (sr < 0.0) th2 += M_PI;
            gr = log(M_PI/(q1*q2))-gr;
            gi = -th1-th2-gi;
            x = x1;
            y = y1;
        }
        if (OPT == 0) {
            g0 = exp(gr);
            gr = g0*cos(gi);
            gi = g0*sin(gi);
        }
        g = gr + I*gi;
        return g;
    };

    unsigned int factorial(unsigned int n) 
    {
        if (n == 0)
        return 1;
        return n * factorial(n - 1);
    };

    complex legendre(int l, complex z)
    {
        switch (l) 
        {
            case 0: return 1.;
            case 1: return z;
            case 2: return 0.5*(3.*z*z - 1.);
            case 3: return 0.5*z*(5.*z*z - 3.);
            case 4: return (35.*z*z*z*z - 30.*z*z + 3.)/8.;
            case 5: return z*(63.*z*z*z*z - 70.*z*z + 15.)/8.;
            default:
            {
                error("legendre - L value " + std::to_string(l) + " not available! Returning 0.", NAN);
            }
        };

        return 0.;
    };

       // --------------------------------------------------------------------------
    // Angular function

    double wigner_leading_coeff(int j, int lam1, int lam2)
    {
        int M = std::max(std::abs(lam1), std::abs(lam2));
        int N = std::min(std::abs(lam1), std::abs(lam2));

        int lambda = std::abs(lam1 - lam2) + lam1 - lam2;

        double result = (double) factorial(2*j);
        result /= sqrt( (double) factorial(j-M));
        result /= sqrt( (double) factorial(j+M));
        result /= sqrt( (double) factorial(j-N));
        result /= sqrt( (double) factorial(j+N));
        result /= pow(2.,  double(j-M));
        result *= pow(-1., double(lambda)/2.);

        return result;
    };

    // USING WIKIPEDIA SIGN CONVENTION
    // theta is in radians
    // lam1 = 2 * lambda and lam2 = 2 * lambda^prime are integers
    double wigner_d_half(int j, int lam1, int lam2, double theta)
    {
        double phase = 1.;
        if ( j % 2 == 0 || (lam1 + lam2) % 2 != 0 )
        {
            error("wigner_d_half - Invalid arguments passed! Returning 0.", NAN);
        };

        if (theta < 0)
        {
            phase *= pow(-1., double(lam1 - lam2)/2.);
            theta *= -1.;
        };

        // If first lam argument is smaller, switch them
        if (std::abs(lam1) < std::abs(lam2))
        {
            int temp = lam1;
            lam1 = lam2;
            lam2 = temp;
            phase *= pow(-1., double((lam1 - lam2)/2.));
        };

        // If first lam is negative, switch them
        if (lam1 < 0)
        {
            lam1 *= -1;
            lam2 *= -1;
            phase *= pow(-1., double((lam1 - lam2)/2.));
        }

        double result = 0.;

        int id = ((lam2 > 0) - (lam2 < 0)) * (j * 100 + lam1 * 10 + std::abs(lam2)); // negative sign refers to negative lam2
        switch (id)
        {
            // spin 1/2 
            case  111: 
            {
                result =  cos(theta / 2.); 
                break;
            };
            case -111: 
            {
                result = -sin(theta / 2.); 
                break;
            };

            // spin 3/2
            case  333:       
            {
                result = cos(theta / 2.) / 2.;
                result *= (1. + cos(theta));
                break;
            }
            case  331:
            {
                result = - sqrt(3.) / 2.;
                result *= sin(theta / 2.);
                result *= 1. + cos(theta);
                break;
            }
            case -331:
            {
                result = sqrt(3.) / 2.;
                result *= cos(theta / 2.);
                result *= 1. - cos(theta);
                break;
            }
            case -333:
            {
                result = - sin(theta / 2.) / 2.;
                result *= 1. - cos(theta);
                break;
            }
            case  311:
            {
                result = 1. / 2.;
                result *= 3. * cos(theta) - 1.;
                result *= cos(theta / 2.);
                break;
            }
            case -311:
            {
                result = -1. / 2.;
                result *= 3. * cos(theta) + 1.;
                result *= sin(theta / 2.);
                break;
            }

            // Spin- 5/2
            case  533:
            {
                result = -1. / 4.;
                result *= cos(theta / 2.);
                result *= (1. + cos(theta)) * (3. - 5. * cos(theta));
                break;
            }
            case  531:
            {
                result = sqrt(2.) / 4.;
                result *= sin(theta / 2.);
                result *= (1. + cos(theta)) * (1. - 5. * cos(theta));
                break;
            }
            case -531:
            {
                result =  sqrt(2.) / 4.;
                result *= cos(theta / 2.);
                result *= (1. - cos(theta)) * (1. + 5. * cos(theta));
                break;
            }
            case -533:
            {
                result = -1. / 4.;
                result *= sin(theta / 2.);
                result *= (1. - cos(theta)) * (3. + 5. * cos(theta));
                break;
            }
            case  511:
            {
                result = -1. / 2.;
                result *= cos(theta / 2.);
                result *= (1. + 2. * cos(theta) - 5. * cos(theta)*cos(theta));
                break;
            }
            case -511:
            {
                result = 1. / 2.;
                result *= sin(theta / 2.);
                result *= (1. - 2. * cos(theta) - 5. * cos(theta)*cos(theta));
                break;
            }

            default: return NAN;
        };

        return phase * result;
    };

    double wigner_d_int(int j, int lam1, int lam2, double theta)
    {

        double phase = 1.;

        if (theta < 0)
        {
            phase *= pow(-1., double(lam1 - lam2));
            theta *= -1.;
        };

        // If first lam argument is smaller, switch them
        if (std::abs(lam1) < std::abs(lam2))
        {
            int temp = lam1;
            lam1 = lam2;
            lam2 = temp;
            phase *= pow(-1., double(lam1 - lam2));
        };

        // If first lam is negative, smitch them
        if (lam1 < 0)
        {
            lam1 *= -1;
            lam2 *= -1;
            phase *= pow(-1., double(lam1 - lam2));
        }

        // Output
        double result = 0.;
        int id = ((lam2 >= 0) - (lam2 < 0)) * (j * 100 + lam1 * 10 + std::abs(lam2)); // negative sign refers to negative lam2
        switch (id)
        {   
            // Spin 1
            case  111:
            {
                result = (1. + cos(theta)) / 2.;
                break;
            }
            case  110:
            {
                result = - sin(theta) / sqrt(2.);
                break;
            }
            case -111:
            {
                result = (1. - cos(theta)) / 2.;
                break;
            }
            case  100:
            {
                result = cos(theta);
                break;
            }

            default: return 0.;
        }

        return phase * result;
    };

    // Wigner functions butn ow in terms of the costheta, this allows 
    // an analytic continuation to complex angular polynomials
    complex wigner_d_int_cos(int j, int lam1, int lam2, complex cosine)
    {
        // Careful because this loses the +- phase of the sintheta. 
        complex sine = csqrt(1. - cosine * cosine);

        double phase = 1.;
        // If first lam argument is smaller, switch them
        if (std::abs(lam1) < std::abs(lam2))
        {
            int temp = lam1;
            lam1 = lam2;
            lam2 = temp;
            phase *= pow(-1., double(lam1 - lam2));
        };

        // If first lam is negative, smitch them
        if (lam1 < 0)
        {
            lam1 *= -1;
            lam2 *= -1;
            phase *= pow(-1., double(lam1 - lam2));
        }

        complex result = 0.;
        int id = ((lam2 >= 0) - (lam2 < 0)) * (j * 100 + lam1 * 10 + std::abs(lam2)); // negative sign refers to negative lam2
        switch (id)
        {   
            // Spin 1
            case  111:
            {
                result = (1. + cosine) / 2.;
                break;
            }
            case  110:
            {
                result = - sine / sqrt(2.);
                break;
            }
            case -111:
            {
                result = (1. - cosine) / 2.;
                break;
            }
            case  100:
            {
                result = cosine;
                break;
            }
            default: return NAN;
        }

        return phase * result;
    };

    complex wigner_d_half_cos(int j, int lam1, int lam2, complex cosine)
    {
        // Careful because this loses the +- phase of the sintheta. 
        complex sine = csqrt(1. - cosine * cosine);

        // Also need the half-angle factors
        complex sinhalf =  csqrt((1. - cosine) / 2.);
        complex coshalf =  csqrt((1. + cosine) / 2.);

        double phase = 1.;
        if ( j % 2 == 0 || (lam1 + lam2) % 2 != 0 )
        {
            error("wigner_d_half - Invalid arguments passed!", NAN);
        };

        // If first lam argument is smaller, switch them
        if (std::abs(lam1) < std::abs(lam2))
        {
            int temp = lam1;
            lam1 = lam2;
            lam2 = temp;
            phase *= pow(-1., double((lam1 - lam2)/2.));
        };

        // If first lam is negative, switch them
        if (lam1 < 0)
        {
            lam1 *= -1;
            lam2 *= -1;
            phase *= pow(-1., double((lam1 - lam2)/2.));
        }

        
        int id = ((lam2 > 0) - (lam2 < 0)) * (j * 100 + lam1 * 10 + std::abs(lam2)); // negative sign refers to negative lam2
        complex result = 0.;
        switch (id)
        {
            // spin 1/2 
            case  111: 
            {
                result =  coshalf; 
                break;
            };
            case -111: 
            {
                result = -sinhalf; 
                break;
            };

            // spin 3/2
            case  333:       
            {
                result = coshalf / 2.;
                result *= (1. + cosine);
                break;
            }
            case  331:
            {
                result = - sqrt(3.) / 2.;
                result *= coshalf;
                result *= 1. + cosine;
                break;
            }
            case -331:
            {
                result = sqrt(3.) / 2.;
                result *= coshalf;
                result *= 1. - cosine;
                break;
            }
            case -333:
            {
                result = - sinhalf / 2.;
                result *= 1. - cosine;
                break;
            }
            case  311:
            {
                result = 1. / 2.;
                result *= coshalf;
                result *= 3. * cosine - 1.;
                break;
            }
            case -311:
            {
                result = -1. / 2.;
                result *= sinhalf;
                result *= 3. * cosine + 1.;
                break;
            }

            // Spin- 5/2
            case  533:
            {
                result = -1. / 4.;
                result *= coshalf;
                result *= (1. + cosine) * (3. - 5. * cosine);
                break;
            }
            case  531:
            {
                result = sqrt(2.) / 4.;
                result *= sinhalf;
                result *= (1. + cosine) * (1. - 5. * cosine);
                break;
            }
            case -531:
            {
                result =  sqrt(2.) / 4.;
                result *= coshalf;
                result *= (1. - cosine) * (1. + 5. * cosine);
                break;
            }
            case -533:
            {
                result = -1. / 4.;
                result *= sinhalf;
                result *= (1. - cosine) * (3. + 5. * cosine);
                break;
            }
            case  511:
            {
                result = -1. / 2.;
                result *= coshalf;
                result *= (1. + 2. * cosine - 5. * cosine*cosine);
                break;
            }
            case -511:
            {
                result = 1. / 2.;
                result *= sinhalf;
                result *= (1. - 2. * cosine - 5. * cosine*cosine);
                break;
            }
            default: return NAN;
        };
        return phase * result;
    };
};
