//
// Created by jeppesen on 5/18/25.
//

#ifndef STRINGMATH_H
#define STRINGMATH_H
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <type_traits>

template <int b=10>
inline constexpr size_t clog(size_t k) {
  if (k == 0) return 1; // 0 needs 1 digit
  size_t digits = 0;
  size_t power = 1;
  while (power < k) {
    power *= b;
    ++digits;
  }
  return digits;
}

template <int b=10>
inline constexpr size_t exp(size_t l) {
  size_t k=1;
  for (int i=0;i<l;i++) k*=b;
  return k;
}

// Type trait to select smallest unsigned type for k-1
template <size_t k>
struct SelectDigitType {
  static_assert(k > 0, "Base k must be positive");
  using type = std::conditional_t<
    k - 1 <= std::numeric_limits<uint8_t>::max(), uint8_t,
    std::conditional_t<
      k - 1 <= std::numeric_limits<uint16_t>::max(), uint16_t,
      std::conditional_t<
        k - 1 <= std::numeric_limits<uint32_t>::max(), uint32_t,
        uint64_t
      >
    >
  >;
};

// Helper alias
template <size_t k>
using DigitType = typename SelectDigitType<k>::type;

// Type trait for next bigger unsigned type
template <typename T>
struct next_biggest_type {
  static_assert(std::is_unsigned_v<T>, "T must be unsigned");
  using type = std::conditional_t<
    sizeof(T)==1, uint16_t,
    std::conditional_t<
      sizeof(T)==2, uint32_t,
      std::conditional_t<
        sizeof(T)==4, uint64_t,
#ifdef __SIZEOF_INT128__
        __uint128_t  // uint64_t -> __uint128_t if available
#else
        uint64_t     // uint64_t -> uint64_t if no __uint128_t
#endif
      >
    >
  >;
};

// Helper alias
template <typename T>
using NextBiggerUInt = typename next_biggest_type<T>::type;


template<typename T>
constexpr T ceil_div(T m, T n) {
  if (n == 0) throw std::invalid_argument("Division by zero");
  return (m + n - (n > 0 ? 1 : -1)) / n;
}


template<size_t M, unsigned int l=1>
class StringMath {
public:
  static constexpr unsigned int k=exp<10>(l);
  static constexpr size_t N=ceil_div<size_t>(M,l);
  static constexpr size_t MM=M;
  static constexpr unsigned int ll=l;
  using cell_t=DigitType<k>;
  cell_t cells[N];
  StringMath(cell_t int_part=0) {
    cells[0]=int_part;
    for (size_t i=1; i<N; i++) {cells[i]=0;}
  }
  explicit operator bool() const {
    for (size_t i=0;i<N;i++) if (cells[i]>0) return true;
    return false;
  }
  StringMath& operator+=(StringMath<M,l> b) {
    cell_t c=0;
    for (ssize_t i=N-1;i>=0;--i) {
      c=cells[i]+b.cells[i]+c;
      cells[i]=c % k;
      c=c / k;
    }
    return *this;
  }
  template<typename d_t>
  StringMath& operator/=(d_t d) {
    static_assert(std::is_unsigned_v<d_t>, "Divisor type must be unsigned");
    if (d==0) {
      volatile d_t zero=0;        // volatile is intended to force these to evaluate at runtime
      volatile d_t result=1/zero;
      (void)result;
      return *this;               // If we reach here without raising SIGFPE then return as if /=1
    }
    d_t r=0;
    uint64_t temp;
    //printf("sizeof(digits[0])=%ld, sizeof(d)=%ld, sizeof(temp)=%ld\n", sizeof(digits[0]), sizeof(d),sizeof(temp));
    for (size_t i=0;i<N;i++) {
      temp=static_cast<decltype(temp)>(r)*k+cells[i];
      r=temp%d;
      temp=temp/d;
      cells[i]=temp;
    }
    return *this;
  }
  size_t constexpr strsize(int spaces=5,int lines=100) {
    size_t ndigits=N*clog<10>(k); //number of base 10 digits
    return ndigits+2;
  }
  char* format_digit(int i,char* buf) {
    // print base-10 digits represented by base-k digit digits[i]
    constexpr uint32_t width = clog<10>(k); // Number of decimal digits
    cell_t value = cells[i]; // Access digits[i]
    // Convert to decimal, right-to-left
    for (size_t j=0;j<width;j++) {
      buf[width-1-j] = '0' + (value % 10);
      value /= 10;
    }
    return buf+width;
  }
  void sprintf(char* buf) {
    size_t i_cell=0;
    char cellbuf[clog<10>(k)];
    size_t i_char=0;
    size_t i_digit=0;
    buf[i_char++]='0'+cells[0]%10;
    buf[i_char++]='.';
    for (size_t i_cell=1;i_cell<N;i_cell++) {
      format_digit(i_cell,cellbuf);
      for (size_t i_minibuf=0;i_minibuf<sizeof(cellbuf);i_minibuf++) {
        if (i_digit%5==0&&i_digit>1) {
          buf[i_char++]=(i_digit>1 && i_digit%100==0)?'\n':' ';
          if (i_digit%100==0&&i_digit>1) {
            buf[i_char++]=' ';
            buf[i_char++]=' ';
          }
        }
        buf[i_char++]=cellbuf[i_minibuf];
        i_digit++;
      }
    }
    buf[i_char]=0;
  }
  void sprintf_2M(char* buf) {
    char cellbuf[clog<10>(k)];
    size_t i_char=0;
    buf[i_char++]=' ';
    buf[i_char++]='0'+cells[0]%10;
    buf[i_char++]='.';
    int linephase=57;
    int linelen=59;
    for (size_t i_cell=1;i_cell<N;i_cell++) {
      format_digit(i_cell,cellbuf);
      for (size_t i_minibuf=0;i_minibuf<sizeof(cellbuf);i_minibuf++) {
        if (linephase==0) {
          linephase=linelen;
          buf[i_char++]='\n';
        } else {
          linephase--;
        }
        buf[i_char++]=cellbuf[i_minibuf];
      }
    }
    buf[i_char]=0;
  }
};

#endif //STRINGMATH_H
