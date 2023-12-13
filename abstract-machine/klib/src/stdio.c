#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void itoa(int num,char* str,int radix){
    char index[]="0123456789ABCDEF";
    unsigned unum;
    int i=0,j,k;
    if(radix==10 && num<0){
        unum=(unsigned)-num;
        str[i++]='-';
    }
    else{
        unum=(unsigned)num;
    }
    do{
        str[i++]=index[unum%(unsigned)radix];
        unum/=radix;
    }while(unum);
    str[i]='\0';
    if(str[0]=='-'){
        k=1;
    }
    else{
        k=0;
    }
    char temp;
    for(j=k;j<=(i-1)/2;j++){
        temp=str[j];
        str[j]=str[i-1+k-j];
        str[i-1+k-j]=temp;
    }
}


int vsprintf(char *out, const char *fmt, va_list ap)
{
    int i=0;
    while(i<strlen(fmt)){
        if(fmt[i]!='%'){
            *out=fmt[i];
            out++;
            i++;
        }
        else{
            i++;
            switch(fmt[i]){
                case 'd':{
                    int num=va_arg(ap,int);
                    char str[100];
                    itoa(num,str,10);
                    strcpy(out,str);
                    out+=strlen(str);
                    i++;
                    break;
                }
                case 'x':{
                    int num=va_arg(ap,int);
                    char str[100];
                    itoa(num,str,16);
                    strcpy(out,str);
                    out+=strlen(str);
                    i++;
                    break;
                }
                case 's':{
                    char* str=va_arg(ap,char*);
                    strcpy(out,str);
                    out+=strlen(str);
                    i++;
                    break;
                }
                case 'c':{
                    char ch=va_arg(ap,int);
                    *out=ch;
                    out++;
                    i++;
                    break;  
                }
                default:{
                    *out=fmt[i];
                    out++;
                    i++;
                    break;
                }
            }
        }
    }
    *out='\0';
    return strlen(out);
    //panic("Not implemented");
}
int printf(const char *fmt, ...)
{
    char output[4096];
    va_list ap;
    va_start(ap,fmt);
    vsprintf(output,fmt,ap);
    va_end(ap);
    int i=0;
    while(output[i]!='\0'){
        putch(output[i]);
        i++;
    }
    return strlen(output);
}

int sprintf(char *out, const char *fmt, ...)
{
    va_list ap;
    va_start(ap,fmt);
    vsprintf(out,fmt,ap);
    va_end(ap);
    return strlen(out);
    //panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...)
{
    panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap)
{
    panic("Not implemented");
}

#endif
