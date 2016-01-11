#include <iostream>
#include <math.h>

float InvSqrt(float x)
{
  return 1.0f / sqrtf(x);
}

float InvSqrt_carmack(float x)
{
  float xhalf = 0.5f*x;
  int i = *(int*)&x; // get bits for floating value
  i = 0x5f375a86 - (i >> 1); // gives initial guess y0
  x = *(float*)&i; // convert bits back to float
  x = x*(1.5f - xhalf*x*x); // Newton step, repeating increases accuracy
  return x;
}


void main(){
  float a = 6345.0f;

  std::cout << "InvSqrt(" << a << ") = " << InvSqrt_carmack(a)      << std::endl;
  std::cout << "InvSqrt(" << a << ") = " << InvSqrt(a)              << std::endl;
}
