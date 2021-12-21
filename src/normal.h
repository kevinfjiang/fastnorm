#ifndef NORM_H
#define NORM_H

#ifdef __cplusplus
extern "C" {
#endif
// Function list
static inline float FastExp(float X); 

// Norm CDF
float CUM(float Z);
float FastCUM(float Z);
static float CND(float d);

// InverseNorm
float ICUM(float P);
float FastICUM(float P);



#ifdef __cplusplus
}
#endif


#endif //NORM_H