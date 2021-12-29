#ifndef NORM_H
#define NORM_H

#ifdef __cplusplus
extern "C" {
#endif
// Function list

// Norm CDF
double CUM(double Z);
double AccCUM(double Z);
double FastCUM(double Z);
static double CND(double d);

// InverseNorm
double ICUM(double P);
double FastICUM(double P);



#ifdef __cplusplus
}
#endif


#endif //NORM_H