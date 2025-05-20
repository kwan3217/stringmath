#include <cstdio>
#include "stringmath.h"
#include <type_traits>
#include <typeinfo>

const size_t n_digits=2'000'000;
StringMath<n_digits,9> e4{1},term4{1};
//StringMath<n_digits,1> e1{1},term1{1};
char buf[n_digits+n_digits/5+n_digits/100+5]; // enough chars for

int main() {
  printf("e4 Type: %s Size: %ld M: %ld l: %d N: %ld k: %d\n",typeid(decltype(e4.cells[0])).name(),sizeof(e4.cells[0])*8,e4.MM,e4.ll,e4.N,e4.k);
//  printf("e1 Type: %s Size: %ld M: %ld l: %d N: %ld k: %d\n",typeid(decltype(e1.digits[0])).name(),sizeof(e1.digits[0])*8,e1.MM,e1.ll,e1.N,e1.k);
  size_t i=1;
  while (term4) {
    //printf("i: %ld\n",i);
    //printf("term4 ");
    term4/=i;
    if (i%10==0 && i>0) {
      printf(".");
      if (i%1000==0) printf("%7ld\n",i);
    }
//    printf("term1 ");term1/=i;
    e4+=term4;
//    e1+=term1;
//    e1.sprintf(buf);
//    printf("e1:            %s\n",buf);
    i++;
  }
  printf("%7ld\n",i);
  //term4.sprintf(buf);
  //printf("term4: %s\n",buf);
  //    term1.sprintf(buf);
  //    printf("term1:         %s\n",buf);
  e4.sprintf_2M(buf);
  FILE* ouf=fopen("e.strmath","wt");
  fprintf(ouf,"e =  \n\n%s\n",buf);
  fclose(ouf);
  //  printf("ref:           2.71828182845904523536028747135266249775724709369995\n");
}

/*
 2.718281828459045235360287471352662497757247093699959574966
*/