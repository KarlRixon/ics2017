#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NEQ,

  /* TODO: Add more token types */
  
  ADD, SUB, MUL, DIV,
  LAND, LOR, LNOT,
  OP, CP,
  HEX_NUM, DEC_NUM,
  REG_NAME,
  NEGTIVE,
  INDICATOR,

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {"\\+", ADD},         // plus
  {"-", SUB},			// minus or negtive
  {"\\*", MUL},			// multiply or indicator
  {"/", DIV},			// divide
  
  {" +", TK_NOTYPE},    // spaces
  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},		// not equal

  {"&&", LAND},			// logic and
  {"\\|\\|", LOR},		// logic or
  {"!", LNOT},			// logic not

  {"\\(", OP},			// open parenthesis
  {"\\)", CP},			// close parenthesis
  
  {"0[xX][0-9a-fA-F]+", HEX_NUM},	//hexadecimal number
  {"[0-9]+", DEC_NUM},		// decimal(natural) number

  {"\\$e(ax|bx|cx|dx|sp|bp|si|di|ip)", REG_NAME},	//register

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

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

Token tokens[32];
int nr_token;

static bool check_neg(int t){
	int p = tokens[t].type;
	if(p == ADD || p == SUB || p == MUL || p == DIV || p == OP || p == NEGTIVE || p == INDICATOR)
		return true;
	else
		return false;
}

static bool check_ind(int t){
	int p = tokens[t].type;
	if(p == ADD || p == SUB || p == MUL || p == DIV || p == OP || p == NEGTIVE || p == INDICATOR || p == LAND || p == LOR || p == LNOT)
		return true;
	else
		return false;
}

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
	  //printf("tring %d : %s\n", i, rules[i].regex);
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0){
		if(nr_token > 31){
		   printf(c_red c_bold "too many tokens\n" c_normal);
		   nr_token = 0;
		   return false;
		}
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        //Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
        //    i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
			case TK_NOTYPE: 
				continue;

			case SUB:
				if(nr_token == 0 || check_neg(nr_token - 1)){
					tokens[nr_token].type = NEGTIVE;
				}
				else{
					tokens[nr_token].type = rules[i].token_type;
				}
				break;
			
			case MUL:
				if(nr_token == 0 || check_ind(nr_token - 1)){
					tokens[nr_token].type = INDICATOR;
				}
				else{
					tokens[nr_token].type = rules[i].token_type;
				}
				break;

			case HEX_NUM:
			case DEC_NUM:
			case REG_NAME:
				if(substr_len > 31){
					printf(c_red c_bold "value too long\n" c_normal);
					return false;
				}
				strncpy(tokens[nr_token].str, substr_start, substr_len);
				tokens[nr_token].str[substr_len] = '\0';
				tokens[nr_token].type = rules[i].token_type;
				break;

			default: tokens[nr_token].type = rules[i].token_type;
					 break;
        }
		++nr_token;
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

uint32_t eval(int p, int q);
bool check_parentheses(int p, int q);
int find_dominant_operator(int p, int q);
int priority(int i);
void expr_test();
void show_tokens();

uint32_t expr(char *e, bool *success) {
  //int i = 0;
  //for(i = 0; i < nr_token; ++i){
  //  printf("tokens[%d] = %s\n", i, tokens[i].str);
  //}

  if(strcmp(e, "test") == 0){
	  expr_test();
  }

  if (!make_token(e)) {
	printf(c_red "make tokens failed\n" c_normal);
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  //TODO();
  else{
	  //show_tokens();
	  *success = true;
	  return eval(0, nr_token - 1);
  }

  return 0;
}

bool check_parentheses(int p, int q){
	int i, par = 0;
	if(tokens[p].type != OP || tokens[q].type != CP)
		return false;
	for(i = p; i < q; ++i){
		if(tokens[i].type == OP)
			++par;
		else if(tokens[i].type == CP)
			--par;
		if(par == 0 && i < q)
			return false;
	}
	return true;
}

int find_dominant_operator(int p, int q){
	int i,tp;
	int pri = -1, max = 0, dom = -1;

	for(i = p; i<+q;++i){
		if(tokens[i].type == OP){
			++i;
			while(tokens[i].type != CP)
				++i;
			++i;
			if(i > q)
				break;
		}
		tp = tokens[i].type;
		if(tp == HEX_NUM || tp == DEC_NUM || tp == REG_NAME)
			continue;
		pri = priority(i);
		if(pri > max && pri != -1){
			max = pri;
			dom = i;
		}
	}
	if(pri == -1 || dom == -1){
		printf("pri = %d, dom = %d\n", pri, dom);
		show_tokens();
		printf(c_red c_bold "cannot find dominant\n" c_normal);
		assert(0);
	}
	return dom;	
}

int priority(int i){
	switch(tokens[i].type){
		case LOR:
			return 12;
		case LAND:
			return 11;
		case TK_EQ:
		case TK_NEQ:
			return 7;
		case ADD:
		case SUB:

				printf("1111111111111111111\n");
			return 4;
		case MUL:
		case DIV:
			return 3;
		case LNOT:
		case INDICATOR:
		case NEGTIVE:
			return 2;
		default:
			return -1;
	}
}

uint32_t eval(int p, int q){
	if(p > q){
		panic(c_red c_bold "bad expression" c_normal);
	}
	else if(p == q){
		int type = tokens[p].type;
		uint32_t t = 0;
		switch(type){
			case DEC_NUM:
				return atoi(tokens[p].str);
			case HEX_NUM:
				sscanf(tokens[p].str + 2, "%x", &t);
				return t;
			case REG_NAME:
				if(strcmp(tokens[p].str, "$eax") == 0)
					return cpu.eax;
				else if(strcmp(tokens[p].str, "$ebx") == 0)
					return cpu.ebx;
				else if(strcmp(tokens[p].str, "$ecx") == 0)
					return cpu.ecx;
				else if(strcmp(tokens[p].str, "$edx") == 0)
					return cpu.edx;
				else if(strcmp(tokens[p].str, "$esp") == 0)
					return cpu.esp;
				else if(strcmp(tokens[p].str, "$ebp") == 0)
					return cpu.ebp;
				else if(strcmp(tokens[p].str, "$esi") == 0)
					return cpu.esi;
				else if(strcmp(tokens[p].str, "$edi") == 0)
					return cpu.edi;
				else if(strcmp(tokens[p].str, "$eip") == 0)
					return cpu.eip;
				else
					assert(0);
			default:
				assert(0);
		}
	}
	else if(check_parentheses(p, q) == true){
		return eval(p + 1, q - 1);
	}
	else{
		int op = find_dominant_operator(p, q);
		// check monadic operator first
		if(tokens[op].type == NEGTIVE)
			return -1 * eval(op + 1, q);
		if(tokens[op].type == INDICATOR)
			return vaddr_read(eval(op + 1, q), 4);
		if(tokens[op].type == LNOT)
			return !eval(op + 1, q);

		int val1 = eval(p, op - 1);
		int val2 = eval(op + 1, q);

		switch(tokens[op].type){
			case ADD:
				return val1 + val2;
			case SUB:
				return val1 - val2;
			case MUL:
				return val1 * val2;
			case DIV:
				return val1 / val2;
			case LAND:
				return val1 && val2;
			case LOR:
				return val1 || val2;
			case TK_EQ:
				return val1 == val2;
			case TK_NEQ:
				return val1 != val2;
			default:
				assert(0);
		}
	}
	return 1;
}

void show_tokens(){
	int i;
	for(i = 0; i < nr_token; ++i){
		printf("%d: %d %s\n", i, tokens[i].type, tokens[i].str);
	}
}

void expr_test(){
#define N 20
	int i;
	bool success = true;
	char exprs[][N] = {
		"1+2", "1-2", "1*2", "1/2",
		"1 == 2", "1 != 2",
		"1&&1", "1&&0", "0&&1", "0&&0",
		"1||1", "1||0", "0||1", "0||0",
		"!1", "!0", "!0x1", "!0x0",
		"-1", "(-1*(5-6)+3)/2"
	};
	int res[N] = {
		3, -1, 2, 0,
		0, 1,
		1, 0, 0, 0,
		1, 1, 1, 0,
		0, 1, 0, 1,
		-1, 2
	};
	for(i = 0; i < N; ++i){
		Log("test: %s = %d", exprs[i], res[i]);
		if(res[i] != expr(exprs[i], &success)){
			printf(c_red c_bold "evaluate result: %d\nshould be: %d\n" c_normal, expr(exprs[i], &success), res[i]);
			return;
		}
	}
}
