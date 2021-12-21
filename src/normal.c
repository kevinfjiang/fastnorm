#include <math.h>

/**
 * @brief FastExp calculator for small values, must be small due to long overflow errors
 * Considering investigating an implementation for doubles and long long, to come soon
 * @param X, exponent
 * @return e^X
 */
__attribute((always_inline)) static inline float FastExp(float X){         // Inline fast exponential approximation Accidentally realized i made a super fast calculator for e^x
    union { long x; float f; } hack = { .x = X*(0xB3BC6AL) + 0x3F7A3BEAL };// Uses union bithacks, which simulate a log2 transformation,
    return hack.f*(1 + X - (hack.x - 0x3F74FB9DL) * 8.262958405176314e-8f);// Newton iteration, using log approximation
}                                                                          // If anyone at HRT sees this, pls hire me

/**
 * @brief lightning fast NormCDF function approximation,
 * Uses an approximation devised by Richards et al. (2010)
 * and bit manipulation approximations, 
 * 1 billion calculations [+-.005] in 6.4 seconds
 * 
 * @param Z, Z score, must be less than 20 to prevent long overflow
 * @return float Probability(z < Z)
 */

float CUM(float Z){
    Z *= (1.5976F + (Z*Z)*0.070565992F);                                          // Approx defined by Richards (2010) paper algo[2]              
    union { long x; float f; } hack = { .x = Z*(0xB3BC6AL) + 0x3F7A3BEAL };       // union bit hack, applies e^x transformation
    float val = hack.f*(1 + Z - (hack.x - 0x3F74FB9DL) * 8.262958405176314e-8f);  // Newton iteration, constants computes log(hack.x)
    return val/(val+1);                                                           // Last step of algorithm
}

/**
 * @brief lightning fast normal CDF appproximation algorithm
 * beats the standard approximation 2.5x on average and CUM algo .5x
 * 
 * @param Z, Z-scoreinput must be less than 20
 * 
 */ 
float FastCUM(float Z){
    Z *= (1.5976F + 0.070565992F*(Z*Z));         // Approx defined by Richards (2010) paper algo[2] 
    long hack = ((0xB3BC6AL)*Z + (0x3F7A3BEAL)); // what the fuck? jk Quake3 Q_rsqrt inspired hack to apply e^X
    Z = *(float *) (&hack);                      // Getting back float bits as float
    return Z/(Z+1);                              // final step of algo[2] figure 2.7
}

/**
 * @brief lightning fast inv CDF performs approximately on par with accurate CUM algorithm
 * uses paper defined in Richards et al. (2010) paper
 * @param P, area to the left, must be in range [0,1)
 * @return Z, Z-score such that P(z < Z) = P
 */ 
float FastICUM(float P){
    float Z = (2*P - 1); // Part of algorithm
    union { float f; long l; } hack = { 1 - Z*Z }; // Bit hack for natural log
    float root = (float)(hack.l - 0x3F7893F5L) * (-1.47815429977e-7F); 
    Z=sqrtf(root);
    if (P<.5F) return -Z; //sqrt returns only positive numbers, loses out on negatives
    return Z;
}

/**
 * @brief Inverse CUM algorithm
 * uses paper defined in Richards et al. (2010) paper
 * @param P, area to the left, must be in range [0,1)
 * @return Z, Z-score such that P(z < Z) = P
 */ 
float ICUM(float P){
    double Z = (2*P - 1);
    Z = log(1 - Z*Z); 
    Z *= -1.59576912161F;
    Z = sqrt(Z);
    if (P<.5) return -Z;
    return Z;
}
/**
 * @brief algo used to approximate CND as used in code examples
 * by Nvidia demonstrations of CUDA. All algorithms above 
 * perform better, with some up to 2x and 2.5x faster than this.
 */ 
static float CND(float d) { //Standard algo by Nvidia
    const double A1 = 0.31938153;
    const double A2 = -0.356563782;
    const double A3 = 1.781477937;
    const double A4 = -1.821255978;
    const double A5 = 1.330274429;
    const double RSQRT2PI = 0.39894228040143267793994605993438;

    float K = 1.0 / (1.0 + 0.2316419 * fabsf(d));

    //   double cnd=1;
    float cnd = RSQRT2PI * exp(-0.5 * d * d) *
                (K * (A1 + K * (A2 + K * (A3 + K * (A4 + K * A5)))));

    if (d > 0) cnd = 1.0 - cnd;

    return cnd;
}

