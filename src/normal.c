#include <math.h>                                                        

/**
 * @brief lightning fast NormCDF function approximation,
 * Uses an approximation devised by Richards et al. (2010)
 * and bit manipulation approximations, 
 * 1 billion calculations [+-.001] 60% faster than Nvidia's approximation found their Black-Scholes model
 * 20% faster than an accurate version AccCUM
 * 
 * @param Z, Z score, abs(Z) must be less than 35 to prevent (long long) overflow
 * @returns Probability(z < Z)
 */

double CUM(double Z){                                
    Z *= (0x127054FC55A662P0 + 0xD07F3F8B2FCA.2P0*(Z*Z));// Approx by Richards (2010) paper algo[2], 
                                                         // 0xB8AA3B295C17F = e^(X/2) using a bithack, value is distributed
    union { double d; unsigned long long x; } u, v;      // Union bithack for approximating scaled log2 using long long representating
    u.x = ( 0x3fdf127e83d16f12ULL + Z );                 // 0x3fdf127e83d16f12ULL is an error term for the log2 approx
    v.x = ( 0x3fdf127e83d16f12ULL - Z );                 // e^(-x/2), calculating e^x = e^(x/2) / e^(-x/2) is gives more accuracy

    return 1.0 / ((v.d/u.d) + 1.0);                      // final step of algo[2] figure 2.7 with some algebra
}

double AccCUM(double Z){
    Z *= (1.5976 + 0.070565992*(Z*Z)); // Approx defined by Richards (2010) paper algo[2]              
    return 1. - ( 1. / (exp(Z) + 1.) ); 
}

/**
 * @brief lightning fast normal CDF appproximation algorithm
 * beats the standard approximation 71% increase on average and CUM algo 25%
 * 
 * @param Z, Z-scoreinput must be less than 20
 * 
 */ 
double FastCUM(double Z){
    Z *= (0x127054FC55A662.P1 + 0xD07F3F8B2FCA.2P1*(Z*Z));                       // Approx by Richards (2010) paper algo[2], 
                                                                                 // 0x171547652B82FE = e^X using a bithack, value is distributed
    union  {double d; unsigned long long x;} u = {.x = Z + 0x3fef127e83d16f12ULL};// union bit hack, applies e^x transformation
    return 1. - ( 1. / (u.d + 1.) );                                              // final step of algo[2] figure 2.7
}

/**
 * @brief Inverse CUM algorithm
 * uses paper defined in Richards et al. (2010) paper
 * @param P, area to the left, must be in range [0,1)
 * @return Z, Z-score such that P(z < Z) = P
 */ 
double ICUM(double P){
    double Z = (2.*P - 1.);
    Z = log(1 - Z*Z); 
    Z = sqrt(-1.59576912161 * Z);
    if (P<.5) return -Z;
    return Z;
}

/**
 * @brief fast inv CDF performs approximately on par with accurate CUM algorithm
 * uses paper defined in Richards et al. (2010) paper
 * @param P, area to the left, must be in range [0,1)
 * @return Z, Z-score such that P(z < Z) = P
 */ 
double FastICUM(double P){
    double Z = (2.*P - 1.);                        // Part of algorithm

    union { double d; long long x; } hack, u, v;   // hack for ln(X), u and v for e^-X
    hack.d = (1. - Z*Z);                           // Bit hack for fast log

    // Reuses properties of hack.x to quickly calculate decebt approximation e^(ln(x)) constant is adjusted for bitshift as we need half of hack.x
    u.x = (long long)( 0x1FF7893F41E8B789LL + (hack.x>>1) );  // 0x3fdf127e83d16f12LL is an error term for the log2 approx, 
    v.x = (long long)( 0x5FE69BBDC5BA269BLL - (hack.x>>1) );  // e^(-x/2), calculating e^x = e^(x/2) / e^(-x/2) is gives more accuracy
        
    Z = sqrt((hack.x - 0x400627C5E8FCF221) * -2.4560417421350713e-16   // Approximation of ln(X) with added newton iteration
              - 1.59576912161 * hack.d * (v.d/u.d));                   // Newton iteration multiplied by constant without the +1 term as 
                                                                       // it's included in 0x400627C5E8FCF221LL which should be 0x3FEF127E83D16F12
    if (P < .5F) return -Z;                                            // sqrt returns only positive numbers, loses out on negatives
    return Z;
}

double ReadableFastICUM(double P){ // non optimized, do not use
    double Z = (2.*P - 1.); // Part of algorithm

    union { double d; long long x; } hack, u, v;   // hack for ln(X), u and v for e^-X
    hack.d = (1. - Z*Z); // Bit hack for fast log
    float log_approx = (hack.x - 0x3FEF127E83D16F12) / 0x171547652B82FE;
    // 0x3FEF127E83D16F12 is an error constant that the long long<>double transformation needs for better conversions
    // 0x171547652B82FE is the scaling factor for the log with the long long<>double transformation 

    // Reuses properties of hack.x to quickly calculate decebt approximation e^(ln(x))
    u.x = (long long)( 0x1FF7893F41E8B789LL + (hack.x>>1) );  
    // with some algebraic manipulation we can conclude that the e^(ln(x)/2) can be written as (0x3fdf127e83d16f12LL + hack.x)/2
    // This is because the transformation in log space is already done with the same constants, but since we are taking only X/2
    // 0x1FF7893F41E8B789LL = 0x171547652B82FE/2
    v.x = (long long)( 0x5FE69BBDC5BA269BLL - (hack.x>>1) );  // e^(-x/2), calculating e^x = e^(x/2) / e^(-x/2) is gives more accuracy
    //Same concept with the algebra but we end with the constant being 3/2 * 0x1FF7893F41E8B789LL

    log_approx = log_approx - (1 - hack.d * (v.d/u.d) ); // newton iteration, calculating it we get y = y - (1 - x(e^(-y))

    Z = sqrt(log_approx);                   
    if (P < .5F) return -Z;                             //sqrt returns only positive numbers, loses out on negatives
    return Z;
}

/**
 * @brief algo used to approximate CND as used in code examples
 * by Nvidia demonstrations of CUDA. All algorithms above 
 * perform better, with some up to 2x and 2.5x faster than this.
 */ 
double CND(double d) { //Standard algo by Nvidia
    const double A1 = 0.31938153;
    const double A2 = -0.356563782;
    const double A3 = 1.781477937;
    const double A4 = -1.821255978;
    const double A5 = 1.330274429;
    const double RSQRT2PI = 0.39894228040143267793994605993438;

    double K = 1.0 / (1.0 + 0.2316419 * fabs(d));

    double cnd = RSQRT2PI * exp(-0.5 * d * d) *
                (K * (A1 + K * (A2 + K * (A3 + K * (A4 + K * A5)))));

    if (d > 0) cnd = 1.0 - cnd;

    return cnd;
}
