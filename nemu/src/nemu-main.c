/***************************************************************************************
 * Copyright (c) 2014-2022 Zihao Yu, Nanjing University
 *
 * NEMU is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan
 *PSL v2. You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
 *KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 *NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

#include "monitor/sdb/sdb.h"
#include <common.h>
#include <readline/readline.h>
#include <stdio.h>
void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

// static char *rl_gets()
// {
//     static char *line_read = NULL;

//     if (line_read) {
//         free(line_read);
//         line_read = NULL;
//     }

//     line_read = readline(NULL);

//     return line_read;
// }
int main(int argc, char *argv[])
{
    /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
    am_init_monitor();
#else
    init_monitor(argc, argv);
#endif
    /* Start engine. */
    engine_start();

/*    char *s = rl_gets();
    bool *f = NULL;
    printf("%u\n", expr(s, f));
    return 0;
    int n=100;
    while(n--){
        uint32_t x=0;
        int t=0;
        t=scanf("%u",&x);

        char *s = rl_gets();

        bool *f = 0;
        uint32_t ans_my=expr(s, f);
        if(ans_my!=x){
            printf("%u %s\n",x,s);
            return 0;
        }
    }
    return 0;*/
    return is_exit_status_bad();
}
