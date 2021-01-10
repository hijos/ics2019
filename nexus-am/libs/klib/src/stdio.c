#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

struct param {
	char uc:1;
	char sign;			/* '-' 				 */
	unsigned int base;	/* number base, e.g 8, 10, 16*/
	char *bf;			/* buffer for output */
};
typedef void (*putcf) (void *, char);

struct _vsprintf_putcf_data {
	char *dest;
	size_t num_chars;
};

static void _vsprintf_putcf(void *p, char c) {
	struct _vsprintf_putcf_data *data = (struct _vsprintf_putcf_data *)p;
	data->dest[data->num_chars ++] = c;
}

static void _printf_putc(void *p, char c) {  
	_putc(c);
}

static void putchw(void *putp, putcf putf, struct param *p) {
	char ch;
	char *bf = p->bf;
	while((ch = *bf++)) 
		putf(putp, ch);
}

static void ui2a(unsigned int num, struct param *p) {
	int n = 0;
	unsigned int d = 1;
	char *bf = p->bf;
	while (num / d >= p->base)
		d = d * p->base;

	while (d != 0) {
		int dgt = num /d;
		num = num % d;
		d = d / p->base;
		if (n || dgt > 0 || d == 0) {
			*bf++ = dgt + (dgt < 10 ? '0' : (p->uc ? 'A' : 'a') - 10);
			++n;
		}
	}
	*bf = 0;
}
static void i2a (int num, struct param *p) {
	//printf("num = %d\n", num);
	if (num < 0) {
		num = -num;
		p->sign = '-';
	}
	ui2a(num, p);
}


void my_format(void *putp, putcf putf, const char *fmt, va_list va) {
	struct param p;
	char bf[12];	/* int = 32b */
	char ch;
	p.bf = bf;

	while ((ch = *(fmt++))) {
		if (ch != '%') {
			putf(putp, ch);
		} else {
			ch = *(fmt++);
			switch (ch) {
				case 'd':
					p.base = 10;
					p.uc = 0;
					i2a(va_arg(va, int), &p);
					putchw(putp, putf, &p);
					break;
				case 'x' :
					p.base = 16;
					p.uc = 0;
					i2a(va_arg(va, int), &p);
					putchw(putp, putf, &p);
					break;
				case 's':
					p.bf = va_arg(va, char *);
					putchw(putp, putf, &p);
				    p.bf = bf; //???
					break;
				default : break;
			}
		}

	}
}

// int printf(const char *fmt, ...) {
//   return 0;
// }

// int vsprintf(char *out, const char *fmt, va_list ap) {
//   return 0;
// }

// int sprintf(char *out, const char *fmt, ...) {
//   return 0;
// }

// int snprintf(char *out, size_t n, const char *fmt, ...) {
//   return 0;
// }


int vsprintf(char *out, const char *fmt, va_list ap) {
	struct _vsprintf_putcf_data data;
	data.dest = out;
	data.num_chars = 0;
	my_format(&data, _vsprintf_putcf, fmt, ap);
	data.dest[data.num_chars] = '\0';

	return data.num_chars;
}
static inline void _putstr(char *s) {
}

int sprintf(char *out, const char *fmt, ...) {
	va_list ap;
	int ret;
	
	va_start(ap, fmt);
	ret = vsprintf(out, fmt, ap);
	va_end(ap);

	return ret;
}

int printf(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	my_format(NULL, _printf_putc, fmt, ap);
	va_end(ap);

	/*
		0x3F8 
		printf -> _putc(ch) -> putchar(0x3F8, ch) -> outb(addr, ch) -> *addr = ch
	 */
	return 0;
}
/* snprintf()用于将格式化的数据写入字符串 
 * out: 为要写入的字符串
 * n  : 要写入的字符的最大数据，超过n会被截断
 * fmt: 格式化字符串
 */
int snprintf(char *out, size_t n, const char *fmt, ...) {

  return 0;
}

#endif

// #include "klib.h"
// #include <stdarg.h>

// #if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

// #define MAX_LEN 256

// int is_digit(char c){
//   if((c >= '0') && (c <= '9'))
//     return 1;
//   else
//     return 0;
// } 

// /* 根据输出格式，将对应变量的输出形式存储至数组dst中，并返回最终输出字符串的长度 */

// int transferChar(char *dst, char c, int width, int is_left) {
//   if(width == 0)
//     width = 1;
//   int i = 0;
//   /* 要求左对齐 */
//   if(is_left) {
//     dst[i] = c;
//     for(i = 1; i < width; i++)
//       dst[i] = ' ';
//   }
//   else {
//     for(i = 0; i < width - 1; i++)
//       dst[i] = ' ';
//     dst[width - 1] = c;
//   }
//   return width;
// }

// int transferNumber(char *dst, unsigned int val, int neg, int width, int is_left, char fill_char) {
//   //使用循环除法取余数，得到数值num的每一位数字字符，并将其写入dst
//   //Notice： 此时写入dst的字符串与原数值字符串呈倒序关系
//   char *p = dst;
//   int i = 0;
//   int rem = 0;
//   if(val == 0) {
//     p[i] = '0';
//     i++;
//   }
//   while(val != 0) {
//     rem = val % 10;
//     p[i] = rem + '0';
//     val = val / 10;
//     i++;
//   }
//   //若为负数
//   if(neg == 1) {
//     p[i] = '-';
//     i++;
//   }
//   // _putc(p[i-1]);
//   int real_len = i;
//   // //此时i为该字符串的实际长度，与宽度进行对比，确定最终的输出宽度
//   if(width < real_len)
//     width = real_len;
//   // //%-04d依旧是表示左对齐，因此优先判断是否为左对齐,若为左对齐则填充空白字符
//   if(is_left)
//     fill_char = ' ';
//   if((neg == 1) && (is_left == 0) && (fill_char == '0')) {
//     /* 
//       在0填充&非左对齐&为负数的情况下
//       需要将填入的‘-’覆盖为0,并在最后添加‘-’
//      */
//     for(i = real_len - 1; i < width - 1; i++)
//       dst[i] = fill_char;
//     dst[width - 1] = '-';
//   }
//   else {
//     for(i = real_len; i < width; i++) {
//       dst[i] = fill_char;
//     }
//   }

//   /* 反转字符数组 */
//   int head = 0, tail = 0;
//   if(is_left == 1)
//     tail = real_len - 1;
//   else  
//     tail = width - 1;
//   // _putc(head + '0');
//   // _putc(tail + '0');
//   char temp;
//   while(tail > head) {
//     temp = dst[head];
//     dst[head] = dst[tail];
//     dst[tail] = temp;
//     head++;
//     tail--;
//   }

//   // _putc(width + '0');
//   return width;
// }

// int transferString(char *dst, char *str,int width, int is_left) {
//   int len = 0;
//   char * p = str;
//   while(*p != '\0') {
//     len++;
//     p++;
//   }
//   //实际输出宽度
//   if(width < len)
//     width = len;
//   int i = 0;
//   //左对齐在尾部填充空白字符
//   if(is_left == 1) {
//     for( ; i < len; i++)
//       dst[i] = str[i];
//     for( ; i < width; i++)
//       dst[i] = ' ';
//   }
//   //右对齐则在前部填充空白字符
//   else {
//     for( ; i < width - len; i++)
//       dst[i] = ' ';
//     for( ; i < width; i++)
//       dst[i] = str[i - width + len];
//   }
//   return width;
// }


// int printf(const char *fmt, ...) {
//   va_list vap;
//   va_start(vap, fmt); //指向可变参数表中的第一个参数 
//   char outBuf[MAX_LEN] = {'\0'};
//   int length = vsprintf(outBuf, fmt, vap); //使用vsprintf将输出结果写入outBuf
//   for(size_t i = 0; outBuf[i] != '\0'; i++) {
//     _putc(outBuf[i]);
//   }
//   va_end(vap);
//   return length;
// }

// int vsprintf(char *out, const char *fmt, va_list ap) {
//   char temp_str[MAX_LEN];
//   int length = 0;
//   char *p = fmt;
//   while(1) {
//     //遍历fmt，直至字符串尾或%,其余字符直接存入out
//     while((*p != '\0') && (*p != '%')) {
//       out[length] = *p;
//       length++;
//       p++;
//     }
//     //已遍历至队尾
//     if(*p == '\0') {
//       break;
//     }
//     p++; //指针p指向%的后一位置
//     /*
//       对于C中的printf函数
//       %d ： 十进制有符号整数
//       %u ： 十进制无符号整数
//       %s ： 字符串
//       %c ： 字符
//       ****************************
//       eg : 
//         %3d表示输出三位整型数， 不够三位右对齐
//         %8s表示输出8个字符的字符串，不够8个字符右对齐。
//         %04d表示在输出一个小于4位的数值时，将在前面补0使其总宽度为4位。
//         %-7d 表示输出7位整数左对齐
//         %-10s 表示输出10个字符左对齐
//    */
//     int width = 0;      //输出宽度
//     int is_left = 0;    //是否为左对齐
//     int neg = 0;        //负数
//     char fill_char = ' '; //用于填充的字符，可能为‘ ’或‘0’,默认为‘ ’

//     /* 判断对齐方式及填充方式 */
//     if(*p == '0') {
//       /* 说明使用0填充 */
//       fill_char = '0';
//       p++;
//     }
//     else if(*p == '-') {
//       /* 说明左对齐 */
//       is_left = 1;
//       p++;
//     }

//     /* 判断输出宽度 */
//     while(is_digit(*p) == 1) {
//       width = width * 10 + ((*p) - '0');
//       p++;
//     }

//     /* 判断输出类型 */
//     int val;
//     char *s;
//     char c;
//     //在判断出输出类型后，便将对应的输出字符串写入temp_str,最后再写入目标out
//     int temp_length = 0; //存储temp_str当前的有效字符串长度
//     switch (*p)
//     {
//       case 'd' :
//         val = va_arg(ap, int);
//         if(val < 0) {
//           neg = 1;
//           val = -val;
//         }
//         temp_length = transferNumber(temp_str, val, neg, width, is_left,fill_char);
//         break;
//       case 'u' :
//         val = va_arg(ap, int);
//         temp_length = transferNumber(temp_str, val, 0, width, is_left,fill_char);
//         break;
//       case 's' :
//         s = (char *)va_arg(ap, char *);
//         temp_length = transferString(temp_str, s, width, is_left);
//         break;
//       case 'c' :
//         c = (char)va_arg(ap, int);
//         temp_length = transferChar(temp_str, c, width, is_left);
//         break;
//       case '\0' :
//         p--;
//         break;
//       default:
        
//         break;
//     }
//     p++; //p一定要自增，否则会将%d中的d存入目标字符串
//     //将temp_str中存储的转换后的字符串写入最终out
//     for(val = 0; val < temp_length; val++) {
//       out[length] = temp_str[val];
//       length++;
//     }
//   }
//   out[length] = '\0';
//   return length;
// }

// int sprintf(char *out, const char *fmt, ...) {
//   va_list vap;
//   va_start(vap, fmt); //指向可变参数表中的第一个参数 
//   int length = vsprintf(out, fmt, vap);
//   va_end(vap);
//   return length;
// }

// int snprintf(char *out, size_t n, const char *fmt, ...) {
//   return 0;
// }

// #endif
