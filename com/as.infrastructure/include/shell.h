/**MARTEN_HEADER**/


#ifndef SHELL_H_
#define SHELL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/queue.h>

typedef int (*ShellFuncT)(int argc, char *argv[]);

typedef struct ShellCmd {
	ShellFuncT func;
	int   argMin;	/* excluding cmd, from 0 -> */
	int   argMax;   /* excluding cmd, from 0 -> */
	const char *cmd;
	const char *shortDesc;
	const char *longDesc;
	TAILQ_ENTRY(ShellCmd) cmdEntry;
} ShellCmdT;

#if defined(__GNUC__)
#define SHELL_CONST const
#define SHELL_CMD_EXPORT(cmd) \
	const ShellCmdT* const __attribute__((section("SSymTab"))) cmd##Ref = &cmd; 
#else
#define SHELL_CONST
#define SHELL_CMD_EXPORT(cmd)
#endif
int SHELL_AddCmd(ShellCmdT *shellCmd);
int SHELL_RunCmd(const char *cmdArgs, int *cmdRv );
int SHELL_Init( void );
int SHELL_Mainloop( void );

#define SHELL_E_OK				0
#define SHELL_E_CMD_TOO_LONG	1
#define SHELL_E_CMD_IS_NULL		2
#define SHELL_E_NO_SUCH_CMD		3
#if defined(__LINUX__) || defined(__WINDOWS__)
#define SHELL_printf  PRINTF
#define SHELL_puts    PRINTF
#define SHELL_putc(c)
#else
#define SHELL_printf  printf
#define SHELL_puts    puts
#define SHELL_putc(c) printf("%c", c)
#endif

void SHELL_input(char c);

#ifdef __cplusplus
}
#endif

#endif /* SHELL_H_ */
