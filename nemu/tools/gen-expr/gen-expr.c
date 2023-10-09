/***************************************************************************************
 * Copyright (c) 2014-2022 Zihao Yu, Nanjing University
 *
 * NEMU is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
    "#include <stdio.h>\n"
    "int main() { "
    "  unsigned result = %s; "
    "  printf(\"%%u\", result); "
    "  return 0; "
    "}";
static uint32_t choose(uint32_t n)
{
    return rand() % n;
}
void gen_num()
{
    uint32_t x=choose(100);
    char s[20];
    sprintf(s,"%u",x);
    strcat(buf,s);

    //printf("%s\n",buf);
    return;
}
void gen_rand_op()
{
    int x=choose(4);
    switch (x){
    case 0:
        strcat(buf,"+");
        break;
    case 1:
        strcat(buf,"-");
        break;
    case 2:
        strcat(buf,"*");
        break;
    default:
        strcat(buf,"/");
        break;
    }
    return ;
}
static void gen(char *c)
{
    strcat(buf,c);
    return ;
}
static void gen_rand_expr(int x)
{
    //buf[0] = '\0';
    if(x>10){
        gen_num();
        return ;
    }
    switch(choose(3)){
        case 0:gen_num();break;
        case 1:gen("(");gen_rand_expr(x+1);gen(")");break;
        default:gen_rand_expr(x+1);gen_rand_op();gen_rand_expr(x+1);break;
    }
    return ;
}

int main(int argc, char *argv[])
{
    int seed = time(0);
    srand(seed);
    // gen_rand_expr(0);
    // printf("%s\n",buf);
    int loop = 1;
    if (argc > 1) {
        sscanf(argv[1], "%d", &loop);
    }
    int i;
    for (i = 0; i < loop; i++) {
        buf[0]='\0';
        gen_rand_expr(0);

        sprintf(code_buf, code_format, buf);

        FILE *fp = fopen("/tmp/.code.c", "w");
        assert(fp != NULL);
        fputs(code_buf, fp);
        fclose(fp);

        int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
        if (ret != 0)
            continue;

        fp = popen("/tmp/.expr", "r");
        assert(fp != NULL);

        int result;
        ret = fscanf(fp, "%d", &result);
        pclose(fp);

        printf("%u %s\n", result, buf);
    }
    return 0;
}
