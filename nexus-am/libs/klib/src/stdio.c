#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define MAX_LEN 256

int is_digit(char c){
  if((c >= '0') && (c <= '9'))
    return 1;
  else
    return 0;
} 

int transferChar(char *dst, char c, int width, int is_left) {
  if(width == 0)
    width = 1;
  int i = 0;
  if(is_left) {// 要求左对齐
    dst[i] = c;
    for(i = 1; i < width; i++)
      dst[i] = ' ';
  }
  else {
    for(i = 0; i < width - 1; i++)
      dst[i] = ' ';
    dst[width - 1] = c;
  }
  return width;
}

int transferNumber(char *dst, unsigned int val, int neg, int width, int is_left, char fill_char) {
  char *p = dst;
  int i = 0;
  int rem = 0;
  if(val == 0) {
    p[i] = '0';
    i++;
  }
  while(val != 0) {
    rem = val % 10;
    p[i] = rem + '0';
    val = val / 10;
    i++;
  }
  // 若为负数
  if(neg == 1) {
    p[i] = '-';
    i++;
  }
  int real_len = i;
  if(width < real_len)
    width = real_len;
  if(is_left)
    fill_char = ' ';
  if((neg == 1) && (is_left == 0) && (fill_char == '0')) {
    // 在0填充&非左对齐&为负数的情况下
    // 需要将填入的‘-’覆盖为0,并在最后添加‘-’
    for(i = real_len - 1; i < width - 1; i++)
      dst[i] = fill_char;
    dst[width - 1] = '-';
  }
  else {
    for(i = real_len; i < width; i++) {
      dst[i] = fill_char;
    }
  }

  // 反转字符数组
  int head = 0, tail = 0;
  if(is_left == 1)
    tail = real_len - 1;
  else  
    tail = width - 1;
  
  char temp;
  while(tail > head) {
    temp = dst[head];
    dst[head] = dst[tail];
    dst[tail] = temp;
    head++;
    tail--;
  }

  return width;
}

int transferString(char *dst, char *str,int width, int is_left) {
  int len = 0;
  char * p = str;
  while(*p != '\0') {
    len++;
    p++;
  }
  if(width < len)
    width = len;
  int i = 0;
  // 左对齐在尾部填充空白字符
  if(is_left == 1) {
    for( ; i < len; i++)
      dst[i] = str[i];
    for( ; i < width; i++)
      dst[i] = ' ';
  }
  // 右对齐则在前部填充空白字符
  else {
    for( ; i < width - len; i++)
      dst[i] = ' ';
    for( ; i < width; i++)
      dst[i] = str[i - width + len];
  }
  return width;
}


int printf(const char *fmt, ...) {
  va_list vap;
  va_start(vap, fmt); // 指向可变参数表中的第一个参数 
  char outBuf[MAX_LEN] = {'\0'};
  int length = vsprintf(outBuf, fmt, vap); 
  for(size_t i = 0; outBuf[i] != '\0'; i++) {
    _putc(outBuf[i]);
  }
  va_end(vap);
  return length;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char temp_str[MAX_LEN];
  int length = 0;
  char *p = fmt;
  while(1) {
    while((*p != '\0') && (*p != '%')) { // 遍历格式字符串，直至遇到%或结束
      out[length] = *p;
      length++;
      p++;
    }
    if(*p == '\0') 
      break;
    p++; // p指向%的后一位置

    int width = 0;      // 输出宽度
    int is_left = 0;    // 是否为左对齐
    int neg = 0;        // 负数
    char fill_char = ' '; // 用于填充的字符，可能为‘ ’或‘0’,默认为‘ ’

    // 判断对齐方式及填充方式
    if(*p == '0') {
      // 用0填充
      fill_char = '0';
      p++;
    }
    else if(*p == '-') {
      // 左对齐
      is_left = 1;
      p++;
    }

    // 计算输出宽度
    while(is_digit(*p) == 1) {
      width = width * 10 + ((*p) - '0');
      p++;
    }

    // 判断输出类型
    int val;
    char *s;
    char c;
    // 在判断出输出类型后，便将对应的输出字符串写入temp_str,最后再写入目标out
    int temp_length = 0; //存储temp_str当前的有效字符串长度
    switch (*p) {
      case 'd' :
        // va_arg返回参数列表中的下一个该类型的值
        val = va_arg(ap, int);
        if(val < 0) {
          neg = 1;
          val = -val;
        }
        temp_length = transferNumber(temp_str, val, neg, width, is_left,fill_char);
        break;
      case 'u' :
        val = va_arg(ap, int);
        temp_length = transferNumber(temp_str, val, 0, width, is_left,fill_char);
        break;
      case 's' :
        s = (char *)va_arg(ap, char *);
        temp_length = transferString(temp_str, s, width, is_left);
        break;
      case 'c' :
        c = (char)va_arg(ap, int);
        temp_length = transferChar(temp_str, c, width, is_left);
        break;
      case '\0' :
        p--;
        break;
      default:
        break;
    }
    p++; 
    for(val = 0; val < temp_length; val++) {
      out[length] = temp_str[val];
      length++;
    }
  }
  out[length] = '\0';
  return length;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list vap;
  va_start(vap, fmt); // 指向可变参数列表中的第一个参数 
  int length = vsprintf(out, fmt, vap);
  va_end(vap);
  return length;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
