# **fastnorm**
>**Lightning fast approximations for functions.**

## **Premise**
The standard CDF is very important to many statistical problems, namely in any field that utilizes statistical work. Fast polynomial approximations are of interest and many approximation algorithms have been devised (Richards et al.). As a direct result implementing an efficient and accurate approximation is of interest. Additionally, I have implemented a fast inverse algorithm and an exponential algorithm. The technical write up will be in  `Tech.md`

## **"Results"**
I have implemented an algorithm that beats the current standard CDF approximation, based on what I saw implemented by the Black-Scholes example at Nvidia, by 2x. The algorithm is defined behavior in C, will get into the technicals a bit later. The results are all within [+-.001] accuracy (further testing necessary). My current benchmark is 1 billion iterations in 3.4 seconds. I have also implemented an algorithm that is 20% faster but slightly less accurate, within +-.01 accuracy. The inverse algorithm is slightly slower and less accurate, with further testing necessary.


## **Motivation**
For a less serious tone, I was bored and I watched the famous Quake 3 inverse square root video and thought "I can do something like that". Boy that was a mistake. We were deriving the Black-Scholes in Probability Theory and I got the feeling that based on the CUDA implementation at Nvidia, that there had to be a better approx of NormCDF. So I set off working and here we are
## TODO:
1. Look into other implementations and other basic functions to speed up.
2. Add makefile to `test/` directory

## **Citations**
1. https://github.com/tpn/cuda-samples/blob/master/v9.0/4_Finance/BlackScholes/BlackScholes_gold.cpp   `Gotta go fast`
2. Richards, Winston & Robin, s & Ashok, Sahai & Acharya, Milu. (2010). An Efficient Polynomial Approximation to the Normal Distribution Function and Its Inverse Function. Journal of Mathematics Research. 2. 10.5539/jmr.v2n4p47. `Algorithm [2] (2.7) and Inverse Algorithm [4] (2.17)`
3. https://en.wikipedia.org/wiki/Fast_inverse_square_root 
 `bithack inspiration and newton iteration for FastICUM algorithim`
4. https://github.com/ekmett/approximate/blob/master/cbits/fast.c `double better_exp_fast(double a), grabbed constants from here for fast exp approximation`