#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_p(char *args);
static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static int cmd_clear(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */

  { "si","Let the program step through N instructions and suspend execution. When N is not given, the default is 1", cmd_si },
  { "info", "[r]: print register state. [w]: print monitoring point information", cmd_info },
  { "x", "calculate the value of the expression EXPR, using the result as the starting memory address and outputting a contiguous N 4 bytes in hexadecimal form", cmd_x },
  { "p", "the value of the EXPR is calculated, and the operation of the EXPR supports the expression evaluation section in the  debug", cmd_p },
  { "w", "[-b] add breakpoint function [-w] no breakpoint function. When the value of the expression EXPR changes, the execution of the program is suspended", cmd_w },
  { "d", "delete a monitoring point with an ordinal number N", cmd_d },
  { "clear", "clear screen", cmd_clear },

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_clear(char *args){
	system("cls");
	return 0;
}

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf(c_green c_bold "%s - %s\n" c_normal, cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf(c_green c_bold "%s - %s\n" c_normal, cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf(c_red "Unknown command '%s'\n" c_normal, arg);
  }
  return 0;
}

static int cmd_si(char *args){
	int n;
	if(args == NULL)
		n = 1;
	else
		n = atoi(args);
	cpu_exec(n);
	return 0;
}

static int cmd_info(char *args){
	if(args == NULL){
		printf(c_green c_bold "%s - %s\n" c_normal,cmd_table[4].name, cmd_table[4].description);
		return 0;
	}
	if(*args != 'r' && *args != 'w'){
		puts(c_red c_bold "info SUBCMD；"c_normal c_red " no subcmd specified (register or watchpoint)\n" c_normal);
		return 1;
	}
	if(strcmp(args,"r") == 0){
		printf(c_bold c_yellow "reg		hex\t\tdec\n" c_normal);
		printf(c_bold "EAX		0x%08x | %d\n", cpu.eax, cpu.eax);
		printf("ECX		0x%08x | %d\n", cpu.ecx, cpu.ecx);
		printf("EDX		0x%08x | %d\n", cpu.edx, cpu.edx);
		printf("EBX		0x%08x | %d\n", cpu.ebx, cpu.ebx);
		printf("ESP		0x%08x | %d\n", cpu.esp, cpu.esp);
		printf("EBP		0x%08x | %d\n", cpu.ebp, cpu.ebp);
		printf("ESI		0x%08x | %d\n", cpu.esi, cpu.esi);
		printf("EDI		0x%08x | %d\n", cpu.edi, cpu.edi);
		printf("EIP		0x%08x | %d\n" c_normal, cpu.eip, cpu.eip);
	}
	else if(strcmp(args,"w") == 0){
		show_wp();
	}
	return 0;		
}

static int cmd_x(char *args){
	//parse args
	if(args == NULL){
		puts(c_red c_bold "too few arguments\n" c_normal);
		return 1;
	}
	char *sN = strtok(args, " ");
	if(sN == NULL){
		puts(c_red "too few arguments\n" c_normal);
		return 1;
	}
	char *sEXPR = strtok(NULL, " ");
	if(sEXPR == NULL){
		puts(c_red "too few arguments\n" c_normal);
		return 1;
	}
	if(strtok(NULL, " ") != NULL){
		puts(c_red "too many arguments\n" c_normal);
	    return 1;
	}

	//convert
	int N = atoi(sN);
	bool success=true;
	//vaddr_t Addr = 0x100000;
	vaddr_t Addr = expr(sEXPR, &success);
	if(!success){

		return 1;
	}

	//print
	int i;
	//for(i = 0; i<N; ++i){
	//	printf("0x%08x:	", Addr + i*4);
	//	int k;
	//	for(k =0; k < 4; ++k){
	//		printf("0x%02x	", pmem[Addr+i*4+k]);
	//	}
	//	printf("\n");
	//}
	printf(c_bold "addr\t\t" c_yellow "Big-Endian\t\t" c_cyan "Little-Endian\n" c_normal);
	for(i = 0;i < N; ++i){
		uint32_t data = vaddr_read(Addr + i*4, 4);
		printf(c_bold "0x%08x:\t" c_normal, Addr + i*4);
		printf(c_yellow "0x%08x\t\t" c_normal, data);
		for(int j=0; j<4;++j){
			printf(c_bold c_cyan "0x%02x " c_normal, data & 0xff);
			data = data >> 8;
		}
		printf("\n");
	}
	return 0;
}

static int cmd_p(char *args){
	if(args == NULL){
		printf(c_green c_bold "%s - %s\n" c_normal,cmd_table[6].name, cmd_table[6].description);
		return 0;
	}
	else{
		bool success = true;
		uint32_t result = expr(args, &success);

		if(success){
			printf(c_green c_bold "result = %d | 0x%08x\n" c_normal, result, result);
		}
		else{
			printf(c_red c_bold "error in calculating EXPR\n" c_normal);
		}
	}
	return 0;
}

static int cmd_w(char *args){
	if(args == NULL){
		printf(c_green c_bold "%s - %s\n" c_normal,cmd_table[7].name, cmd_table[7].description);
		return 0;
	}
	char *b = strtok(NULL, " ");
	char *exp = strtok(NULL, " ");
	bool bk = false;
//	printf("b = %s\texp = %s\n", b, exp);
	bk = strcmp(b, "-b") == 0 ? true:false;
	if(strcmp(b, "-b") != 0 && strcmp(b, "-w") != 0){
		printf(c_red c_bold "parameter error\n" c_normal);
		printf(c_green c_bold "%s - %s\n" c_normal,cmd_table[7].name, cmd_table[7].description);
		return 0;
	}

	bool success;
	uint32_t result = expr(exp, &success);

	if(success){
		new_wp(exp, result, bk);
		if(!bk){
			printf(c_green c_bold "added a watchpoint: %s = %d | 0x%x\n" c_normal, exp, result, result);
		}
		else{
		printf(c_green c_bold "added a breakpoint: %s = %d | 0x%x\n" c_normal, exp, result, result);
		}
	}
	else{
		printf(c_red c_bold "error in calculating EXPR\n" c_normal);
	}
	return 0;
}

static int cmd_d(char *args){
	if(args == NULL){
		printf(c_green c_bold "%s - %s\n" c_normal,cmd_table[8].name, cmd_table[8].description);
		return 0;
	}
	else{
		int no = atoi(args);
		if(no >= 0 && no < 32){
			free_wp(no);
		}
		else{
			printf(c_red c_bold "input error\n" c_normal);
			return 1;
		}
	}
	return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf(c_red "Unknown command '%s'\n" c_normal, cmd); }
  }
}
