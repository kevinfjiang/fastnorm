#include <time.h>
#include <stdio.h>

#include "../src/normal.h"
#ifdef COUNT
    #define CT COUNT
#else
    #define CT 100000000
#endif //COUNT

#ifdef INPUT
    #define INP INPUT
#else
    #define INP .6
#endif //COUNT



void time_func(double (*foo)(double)){
    clock_t t = clock();
    t =  clock();
    double inp = (double) INP;
    double out;
    for (unsigned long i=0; i<CT; i++){
        out = (*foo)(inp);
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("%f \n", time_taken);
    printf("answer is %f \n", out);

}

int main(){
    printf("MY algo \n");
    time_func(&CUM);
    time_func(&FastCUM);
    time_func(&AccCUM);
    // time_func(&CND);
    // time_func(&FastCUM);
    
    // time_func(&AccCUM);
}