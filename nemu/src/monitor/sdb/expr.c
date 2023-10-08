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
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,TK_NUM,

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
	{"/",'/'},
	{"-",'-'},
	{"\\*",'*'},
	{"\\(",'('},
	{"\\)",')'},
	{"[0-9]+",TK_NUM},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
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

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;
        if(rules[i].token_type == TK_NOTYPE){
					continue;
				}
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
//				nr_token++;
				tokens[nr_token].type=rules[i].token_type;

        switch (rules[i].token_type) {
					case TK_NUM:
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						tokens[nr_token].str[substr_len]='\0';
						break;
          default: break;
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

bool check_parentheses(int p,int q){
	static int left_bracket=0,right_bracket=0;
	static int f1=1,f2=1,i;//f1 true|flase f2 good|bad
											 //0 true 1 false 2 bad expr
	for(i=p;i<=q;i++){
		switch (tokens[i].type){
			case '(' :
				left_bracket++;
			case ')' :
				right_bracket++;
				if(left_bracket==right_bracket){
					if(i!=q) f1=0;
				}
				if(right_bracket>left_bracket) f2=0;
			}
	}

	if(left_bracket!=right_bracket) f2=0;

	if(!f2){
			printf("Wrong brackets\n");
			assert(0);
	}

	if(tokens[p].type=='('&&tokens[q].type==')'&&f1)
		return 1;
	else
		return 0;
}
static int find_mainop(int p,int q){
	static int i=0,pri=1000,op=0;// 1 +- 2*/
	for(i=p;i<=q;i++){
			switch (tokens[i].type){
				case '+': case '-':
					pri=1;
					op=i;
				case '*': case '/':
					if(pri>=2){
						pri=2;
						op=i;
					}
			}
	}
	return op;
}
uint32_t eval(int p,int q){
	if(p>q){
		assert(0);
		return -1;
	}
	else if(p==q){
		//number
		return atoi(tokens[p].str);
	}
	else if(check_parentheses(p,q)){
		return eval(p+1,q-1);
	}
	else{
		static int op=0;
		op=find_mainop(p,q);
		uint32_t val1=eval(p,op-1),val2=eval(op+1,q);
		switch(tokens[op].type){
			case '+':return val1+val2;
			case '-':return val1-val2;
			case '*':return val1*val2;
			case '/':return val1/val2;
			default:assert(0);
		}
	}
}
word_t expr(char *e, bool *success) {
	if (!make_token(e)) {
    *success = false;
    return 0;
  }
	

	printf("start evaling\n");
	printf("%u\n",eval(0,nr_token-1));
  /* TODO: Insert codes to evaluate the expression. */
  //TODO();

  return 0;
}
