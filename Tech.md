# **Technical Writeup**
> boy will this be long and complicated


## Discussion
I will be explaining every step of the `float CUM(float Z)` algorithm line by line, and sometimes even more in-depth. For any recruiters, namely those at HFT firms, pls hit me up. I will only be discussing the original algorithm due to time constraints


### Prelims
floats abide by the IEEE754 standard where longs are just read without a complicated standard


### The algorithm approximation
$$
  F(x) = \frac{e^y}{e^y+1},  y =0.7988x(1 + 0.04417x^2)
$$
This algorithm may seem unintuitive but the algorithm is a very good approximation, likely because the derivative of $\frac{e^y}{e^y+1}$ is very similar to a normal curve. The algorithm was chosen to be the fastest because only one complex operation $e^x$ needed to be performed, as multiplication is trivially fast and division has gotten much faster. The main challenge is to find a function that accurately and quickly calculates  $e^x$.

### `float CUM(float Z){` step by step
```Z *= (1.5976F + (Z*Z)*0.070565992F);  ```

This step calculates $y$ as clearly shown. The rearranging ensures the least amount o work is done on the multiplicative end. This was determined to be the fastest order by trial and error

`union { long x; float f; } hack =`

This step creates an anonymous struct that acts as both `long` and `float` with bits overlapping. It is useful in a read-only context where we need to read the values as a `long` and a `float`. Using pointer casting (which if done often can be slightly slow) is undefined behavior in C and union bit hacking is defined according to some random source I read. According to the Quake3 article and trial and error, reading IEEE754 digits as a `long` performs a pseudo transformation or, the number that approximates to applying a $log(1+x)$ transformation in constant time (with minor error). This is useful in applying complex transformations.

`{ .x = Z*(0xB3BC6AL) + 0x3F7A3BEAL }; ` To explain we need to understand the constants. `0x3F7A3BEAL` is a correction term for applying the `long`<>float, found from the Quake3 algo but modified for more general cases for transformation to a `float` and `0xB3BC6AL` is a mix of the above error term and $e$, which we use to perform our operation $e^x$. To explain given $x$ based on what we know of logs, we can take the exponent out and use it as multiplication, which is why we multiply the 2nd constant by `Z`. At the same time, the base is a constant, thus that can be evaluated ahead of time and modified with the error term to get `0xB3BC6AL`. Finally, we read the bits into our struct to get it as both a `long` and a `float`. Doing the computation and having the constant ahead of time prevents the need to pointer cast from our original value of a `float` to a `long` which saves a lot of time by going directly to a `long` to the mixed struct. 

`hack.f*(1 + Z - (hack.x - 0x3F74FB9DL) * 8.262958405176314e-8f);` Recall we're currently approximating $e^x$ and we already are trying to minimize error, so naturally we should do something to minimize error if it's fast. We will use a newton iteration to speed up the process. Given the equation $y=e^x$ we can rearrange to $ln(y) - x=0$, indicating roots to the equation $ln(y) - x$ solves our problem. To get the roots we used a newton iteration  which states $y_{new} = y - \frac{f(y)}{f'(y)}$ therefore our newton iteration is $y_{new} = y - \frac{ln(y)-x}{1/y}$ which simplifies to ${y_{new} = y(1+x-ln(y))}$. We know our approximation of $y$ is `hack.f` as the floating point value of our struct is our initial approximation.However, this raises a problem, how do we calculate $ln(y)$.

`(hack.x - 0x3F74FB9DL) * 8.262958405176314e-8f)` Diving into this part here, which is based on an approximation here [`float logf_fast_ub(float a)`](https://github.com/ekmett/approximate/blob/f92bcca8e211a795aba5d0a733582c23cbba5f98/cbits/fast.c#L150). it uses a similar bithack as the $e^x$ hack by reading the `long` digits of the input and applying a transformation including an error term and multiplication. Something to note, we already have the `long` transformed input since it is the `struct hack.l`, meaning we don't even have to calculate it as we already have it since we read the `long` transformation into the hack initially (see `{ .x = Z*(0xB3BC6AL) + 0x3F7A3BEAL }; `). In other words, we calculated the long representation and we're using the long representation again, along with the float representation to calculate $y*ln(y)$. This just reuses code and doesn't make me recalculate much for the newton iteration, meaning this iteration is very fast. Do note, because reassigning values for structs is quite slow, so we cannot do a second-newton iteration as we cannot reassign the values quickly. Thus we have an improved approximation of $e^x$ without sacrificing too much time as the complex logarithm we already had, just needed some multiplication processing

` return val/(val+1;                                                           // Last step of algorithm
}` This is the final step, find the ratio of $\frac{e^y}{e^y+1}$ and return the result as according to the Richards et al paper. Nothing complicated here, we now have our approximation of the normal in the fastest time I've ever seen.