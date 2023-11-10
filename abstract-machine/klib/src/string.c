#include <klib-macros.h>
#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s)
{
    if(s==NULL){
        return 0;
    }
    size_t len = 0;
    while(s[len]!='\0')
    {
        len++;
    }
    return len;
    //panic("Not implemented");
}

char *strcpy(char *dst, const char *src)
{
    if(dst==NULL || src==NULL){
        return dst;
    }
    size_t i=0;
    while(src[i]!='\0')
    {
        dst[i]=src[i];
        i++;
    }
    return dst;
    //panic("Not implemented");
}

char *strncpy(char *dst, const char *src, size_t n)
{
    if(dst==NULL || src==NULL){
        return dst;
    }
    size_t i=0;
    while(src[i]!='\0' && i<n)
    {
        dst[i]=src[i];
        i++;
    }
    for(;i<n;i++){
        dst[i]='\0';
    }
    return dst;
   // panic("Not implemented");
}

char *strcat(char *dst, const char *src)
{
    size_t len=strlen(dst);
    size_t i=0;
    while(src[i]!='\0')
    {
        dst[len+i]=src[i];
        i++;
    }
    dst[len+i]='\0';
    return dst;
   // panic("Not implemented");
}

int strcmp(const char *s1, const char *s2)
{
    if(s1==NULL || s2==NULL){
        return 0;
    }
    size_t i=0;
    while(s1[i]!='\0' && s2[i]!='\0')
    {
        if(s1[i]!=s2[i]){
            return s1[i]-s2[i];
        }
        i++;
    }
    return s1[i]-s2[i];
    //panic("Not implemented");
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    if(s1==NULL || s2==NULL){
        return 0;
    }
    size_t i=0;
    while(s1[i]!='\0' && s2[i]!='\0' && i<n)
    {
        if(s1[i]!=s2[i]){
            return s1[i]-s2[i];
        }
        i++;
    }
    if(i==n){
        return 0;
    }
    return s1[i]-s2[i];
    //panic("Not implemented");
}

void *memset(void *s, int c, size_t n)
{
    if(s==NULL){
        return s;
    }
    size_t i=0;
    while(i<n)
    {
        ((unsigned char*)s)[i]=c;
        i++;
    }
    return s;
    //panic("Not implemented");
}

void *memmove(void *dst, const void *src, size_t n)
{
    if(dst==NULL||src==NULL||dst==src||n==0){
        return dst;
    }
    size_t i=0;
    if(dst<src)
    {
        while(i<n)
        {
            ((unsigned char*)dst)[i]=((unsigned char*)src)[i];
            i++;
        }
    }
    else
    {
        while(i<n)
        {
            ((unsigned char*)dst)[n-i-1]=((unsigned char*)src)[n-i-1];
            i++;
        }
    }
    return dst;
    //panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n)
{
    if(out==NULL||in==NULL||out==in||n==0){
        return out;
    }
    size_t i=0;
    while(i<n)
    {
        ((unsigned char*)out)[i]=((unsigned char*)in)[i];
        i++;
    }
    return out;
    //panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    if(s1==NULL||s2==NULL||n==0){
        return 0;
    }
    size_t i=0;
    while(i<n && ((unsigned char*)s1)[i]!='\0' && ((unsigned char*)s2)[i]!='\0')
    {
        if(((unsigned char*)s1)[i]!=((unsigned char*)s2)[i]){
            return ((unsigned char*)s1)[i]-((unsigned char*)s2)[i];
        }
        i++;
    }
    if(i==n){
        return 0;
    }
    return ((unsigned char*)s1)[i]-((unsigned char*)s2)[i];
    //panic("Not implemented");
}

#endif
