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

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <memory/paddr.h>
#include <regex.h>
enum {
    TK_NOTYPE = 256,
    TK_EQ,
    TK_NUM,
    TK_HEX,
    TK_REG,
    TK_NEQ,
    TK_AND,
    TK_NEG,
    TK_DEREF,
    /* TODO: Add more token types */

};

static struct rule {   
    const char *regex;
    int token_type;
} rules[] = {

    /* TODO: Add more rules.
     * Pay attention to the precedence level of different rules.
     */

    {" +", TK_NOTYPE}, // spaces
    {"\\+", '+'},      // plus
    {"==", TK_EQ},     // equal
    {"/", '/'},
    {"-", '-'},
    {"\\*", '*'},
    {"\\(", '('},
    {"\\)", ')'},
    {"0[xX][0-9|A-F|a-f]+", TK_HEX},
    {"[0-9]+", TK_NUM},
    {"\\$[$|0-9|a-z|A-Z]+", TK_REG},
    {"!=", TK_NEQ},
    {"&&", TK_AND},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
    int i;
    char error_msg[128];
    int ret;

    for (i = 0; i < NR_REGEX; i++) {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if (ret != 0) {
            regerror(ret, &re[i], error_msg, 128);
            panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
        }
    }
}

typedef struct token {
    int type;
    char str[32];
} Token;

static Token tokens[65536] __attribute__((used)) = {};
static int nr_token __attribute__((used)) = 0;

static bool make_token(char *e)
{
    int position = 0;
    int i;
    regmatch_t pmatch;

    nr_token = 0;

    while (e[position] != '\0') {
        /* Try all rules one by one. */
        for (i = 0; i < NR_REGEX; i++) {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;

                Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
                    i, rules[i].regex, position, substr_len, substr_len, substr_start);

                position += substr_len;
                if (rules[i].token_type == TK_NOTYPE) {
                    continue;
                }
                /* TODO: Now a new token is recognized with rules[i]. Add codes
                 * to record the token in the array `tokens'. For certain types
                 * of tokens, some extra actions should be performed.
                 */
                //				nr_token++;
                tokens[nr_token].type = rules[i].token_type;

                switch (rules[i].token_type) {
                case TK_NUM:
                    strncpy(tokens[nr_token].str, substr_start, substr_len);
                    tokens[nr_token].str[substr_len] = '\0';
                    break;
                case TK_HEX:
                    strncpy(tokens[nr_token].str, substr_start, substr_len);
                    tokens[nr_token].str[substr_len] = '\0';
                    break;
                case TK_REG:
                    strncpy(tokens[nr_token].str, substr_start, substr_len);
                    tokens[nr_token].str[substr_len] = '\0';
                    break;
                default:
                    break;
                }
                ++nr_token;
                break;
            }
        }

        /*	for(i=0;i<nr_token;i++){
                printf("%d %s\n",tokens[i].type,tokens[i].str);
        }*/

        if (i == NR_REGEX) {
            printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
            return false;
        }
    }
    // printf("sd\n");

    return true;
}

bool check_parentheses(int p, int q)
{
    int left_bracket = 0, right_bracket = 0;
    int f1 = 1, f2 = 1, i = 0; // f1 true|flase f2 good|bad
                               // 0 true 1 false 2 bad expr
    for (i = p; i <= q; i++) {
        switch (tokens[i].type) {
        case '(':
            left_bracket++;
            break;
        case ')':
            right_bracket++;
            if (left_bracket == right_bracket) {
                if (i != q)
                    f1 = 0;
            }
            if (right_bracket > left_bracket)
                f2 = 0;
            break;
        default:
            break;
        }
    }

    if (left_bracket != right_bracket)
        f2 = 0;

    if (!f2) {
        printf("Wrong brackets\n");
        assert(0);
    }

    if (tokens[p].type == '(' && tokens[q].type == ')' && f1)
        return 1;
    else
        return 0;
}
static bool is_operator(int type)
{
    switch (type) {
    case TK_HEX:
    case TK_NUM:
    case TK_REG:
    case ')':
        return 0;
        break;
    default:
        return 1;
        break;
    }
}
static bool is_unary_op(int type)
{
    switch (type) {
    case TK_DEREF:
    case TK_NEG:
        return 1;
        break;
    default:
        return 0;
        break;
    }
}
// int update_op(int i,int op)
// {

// }
static int find_mainop(int p, int q)
{
    int i = 0, pri = 1000, op = -1; // 1 && 2 == != 3 +- 4 */ 5 NEG DEREF
    int num = 0;
    for (i = p; i <= q; i++) {
        if (tokens[i].type == '(')
            num++;
        if (tokens[i].type == ')')
            num--;
        if (num != 0)
            continue;
        switch (tokens[i].type) {
        case TK_AND:
            pri = 1;
            op = i;
            break;
        case TK_EQ:
        case TK_NEQ:
            if (pri >= 2) {
                pri = 2;
                op = i;
            }
            break;
        case '+':
        case '-':
            if (pri >= 3) {
                pri = 3;
                op = i;
            }
            break;
        case '*':
        case '/':
            if (pri >= 4) {
                pri = 4;
                op = i;
            }
            break;
        case TK_NEG:
        case TK_DEREF:
            if (pri >= 5) {
                pri = 5;
                if (op == -1)
                    op = i;
            }
        }
    }
    return op;
}
uint32_t eval(int p, int q)
{
    if (p > q) {
        assert(0);
        return -1;
    } else if (p == q) {
        // number
        switch (tokens[p].type) {
        case TK_NUM:
            return atoi(tokens[p].str);
            break;
        case TK_HEX: {
            uint32_t x = 0;
            int t = 0;
            t = sscanf(tokens[p].str, "%x", &x);
            if (t == -1)
                assert(0);
            return x;
        }

        //    break;
        case TK_REG: {
            bool f = 0;
            uint32_t x = 0;
            x = isa_reg_str2val(tokens[p].str, &f);
            if (!f) {
                printf("Access register ERROR\n");
                assert(0);//bugs to be fixed
            }
            return x;
        }

        //    break;
        default:
            printf("Unknown number type\n");
            //return 0;
            assert(0);
        }
    } else if (check_parentheses(p, q)) {
        return eval(p + 1, q - 1);
    } else {

        int op = 0;
        op = find_mainop(p, q);
        if (is_unary_op(tokens[op].type)) {
            if (op != p) {
                printf("Unary error\n");
                assert(0);
            }
            uint32_t val = eval(op + 1, q);
            switch (tokens[op].type) {
            case TK_DEREF:
                return paddr_read(val, 4);
                break;
            case TK_NEG:
                return -val;
            default:
                assert(0);
            }
        } else {
            uint32_t val1 = eval(p, op - 1), val2 = eval(op + 1, q);
            // printf("%u %u %c\n",val1,val2,tokens[op].type);
            switch (tokens[op].type) {
            case TK_AND:
                return val1 && val2;
            case TK_NEG:
                return val1 != val2;
            case TK_EQ:
                return val1 == val2;
            case '+':
                return val1 + val2;
            case '-':
                return val1 - val2;
            case '*':
                return val1 * val2;
            case '/':
                return val1 / val2;
            default:
                assert(0);
            }
        }
    }
}

word_t expr(char *e, bool *success)
{
    if (!make_token(e)) {
        *success = false;
        return 0;
    }

    //	printf("start evaluating\n");
    int i;
    for (i = 0; i < nr_token; i++) {
        if (tokens[i].type == '*' && ((i == 0 || is_operator(tokens[i - 1].type)))) {
            tokens[i].type = TK_DEREF;
        }
    }

    for (i = 0; i < nr_token; i++) {
        if (tokens[i].type == '-' && ((i == 0 || is_operator(tokens[i - 1].type)))) {
            tokens[i].type = TK_NEG;
        }
    }
    *success = 1;
    return eval(0, nr_token - 1);
    // printf("%u\n", eval(0, nr_token - 1));
    /* TODO: Insert codes to evaluate the expression. */
    // TODO();
}
