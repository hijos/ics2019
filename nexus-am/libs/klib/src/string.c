#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  int i;
  for(i=0;s[i];i++){}
  return i;
}

char *strcpy(char* dst,const char* src) {
  int i = 0;
  while(src[i]){
    dst[i] = src[i];
    i++;
  }
  assert(src[i] == '\0');
  dst[i] = 0;
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  int i = 0;
  while(src[i] && (i<n)){
    dst[i] = src[i];
    i++;
  }
  while(i<n){
    dst[i] = 0;
    i++;
  }
  return dst;
}

// char* strcat(char* dst, const char* src) {
//   strcpy((char*)(dst+strlen(dst)), src);
//   return dst;
// }

// int strcmp(const char* s1, const char* s2) {
//   int i;
//   while (s1[i] != 0 && s1[i] == s2[i]) {
//     i++;
//   }
//   return ((unsigned char *) s1)[i] - ((unsigned char *) s2)[i];
// }

// int strncmp(const char* s1, const char* s2, size_t n) {
//   int i = 0;
//   while (s1[i] != 0 && s1[i] == s2[i]) {
//     i++;
//     if (i == n) {
//       return 0; 
//     }
//   }
//   return ((unsigned char *) s1)[i] - ((unsigned char *) s2)[i];
  
// }

// void* memset(void* v,int c,size_t n) {
//   for(int i = 0; i < n;i++)
//     ((unsigned char *)v)[i] = (unsigned char)c;
//   return v;
// }

// void* memcpy(void* out, const void* in, size_t n) {
//   for(int i = 0; i < n; i++)
//     ((unsigned char*)out)[i] = ((unsigned char*)in)[i];
//   return out;
// }

// int memcmp(const void* s1, const void* s2, size_t n){
//   for(int i = 0; i < n; i++){
//       if(((unsigned char*)s1)[i] != ((unsigned char*)s2)[i]){
//           return ((unsigned char*)s1)[i] - ((unsigned char*)s2)[i];
//       }
//   }
//   return 0;
// }


// char* strncpy(char* dst, const char* src, size_t n) {
//     int i = 0;
//     while(src[i] && (i<n)){
//         dst[i] = src[i];
//         i++;
//     }
//     while(i<n){
//         dst[i] = 0;
//         i++;
//     }
//   return dst;
// }

char* strcat(char* dst, const char* src) {
    int i = strlen(dst);
    strcpy((char*)(dst+i), src);
    return dst;
}

int strcmp(const char* s1, const char* s2) {
    int i = 0;
    for(;s1[i];i++){
        if(s1[i]!=s2[i])
            break;
    }
  return (s1[i]-s2[i]);
}

int strncmp(const char* s1, const char* s2, size_t n) {
    int i = 0;
    for(;s1[i] && (i<n);i++){
        if(s1[i]!=s2[i])
            break;
    }
    if(i==n)
        return 0;
    return (s1[i]-s2[i]);
}

void* memset(void* v,int c,size_t n) {
    for(int i = 0; i < n;i++){
        ((char *)v)[i] = (char)c;
    }
    return v;
}

void* memcpy(void* out, const void* in, size_t n) {
    for(int i = 0; i < n; i++){
        ((char*)out)[i] = ((char*)in)[i];
    }
    return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
    for(int i = 0; i < n; i++){
        if(((char*)s1)[i] != ((char*)s2)[i]){
            return ((char*)s1)[i] - ((char*)s2)[i];
        }
    }
    return 0;
}

#endif

