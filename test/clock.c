#include <time.h>
#include <stdio.h>

#include "normal.c"
#ifdef COUNT
    #define CT COUNT
#else
    #define CT 100000000
#endif //COUNT

#ifdef INPUT
    #define INP INPUT
#else
    #define INP .3
#endif //COUNT

void time_func(float (*foo)(float)){
    clock_t t = clock();
    t =  clock();
    float inp = (float) INP;
    float out;
    for (unsigned long i=0; i<CT; i++){
        out = (*foo)(inp);
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("%f \n", time_taken);
    printf("answer is %f \n", out);

}

int main(){
    time_func(&CUM);
    time_func(&FastCUM);
    time_func(&CND);
    time_func(&ICUM);
    time_func(&FastICUM);
}