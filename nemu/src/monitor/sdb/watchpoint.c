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

#include "sdb.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool()
{
    int i;
    for (i = 0; i < NR_WP; i++) {
        wp_pool[i].NO = i;
        // wp_pool[i].expr = NULL;
        wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
        wp_pool[i].expr_value = -1;
    }

    head = NULL;
    free_ = wp_pool;
}

WP *new_wp()
{
    if (free_ == NULL) {
        printf("NO free watchpoint\n");
        assert(0);
    }
    WP *p = free_;
    free_ = free_->next;
    p->next = head;
    head = p;
    // printf("%d\n", p->NO);
    return p;
}
void free_wp(WP *wp)
{
    WP *p = head;
    while (p->next != NULL) {
        if (p->next == wp) {
            p->next = p->next->next;
            wp->next = free_;
            free_ = wp;
            return;
        }
        p = p->next;
    }
    printf("such watchpoint is not being used\n");
    return;
}
/* TODO: Implement the functionality of watchpoint */
bool scan_watchpoint()
{
    WP *p = head;
    if (p == NULL) {
        //printf("NO using watchpoint\n");
        return 0;
    }
    bool f = 0;
    while (p != NULL) {
        bool f1 = 0;
        uint32_t x = expr(p->expr, &f1);
        if (!f1) {
            printf("Watch Point:%d , evalating ERROR\n", p->NO);
            continue;
            f = 1;
        }
        if (x != p->expr_value) {
            printf("Watch Point %d : %s\n", p->NO, p->expr);
            printf("Old value=%u\n", p->expr_value);
            printf("New value=%u\n", x);
            p->expr_value = x;
            f = 1;
        }
        p = p->next;
    }
    return f;
}

void watchpoint_display()
{
    WP *p = head;
    if (p == NULL) {
        printf("NO using watchpoint\n");
        return;
    }
    printf("Num  Expression                    Value\n");
    while (p != NULL) {
        printf("%-5d%-30s%-10u\n", p->NO, p->expr, p->expr_value);
        p = p->next;
    }
    return;
}
void free_wp_num(int no)
{
    WP *p = head;
    while (p != NULL) {
        if (p->NO == no) {
            free_wp(p);
            return;
        }
        p = p->next;
    }
    printf("NO such watchpoint\n");
    return;
}