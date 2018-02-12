#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

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

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
			case TK_NOTYPE: break;

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

uint32_t expr(char *e, bool *success) {
  int i = 0;
  for(i = 0; i < nr_token; ++i){
	  printf("tokens[%d] = %s\n", i, tokens[i].str);
  }

  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  TODO();

  return 0;
}
