#ifndef _INC_PBasic_H
#define _INC_PBasic_H
#define forloop         0
#define whileloop       1
#define gosubloop       2

#define checking	true
#define varnamelen      20
#define maxdims	 4

typedef unsigned char boolean;
typedef char varnamestring[varnamelen + 1];
typedef char string255[256];
#define MAX_LINE 4096
#define FileNotFound     10
#define FileNotOpen      13
#define FileWriteError   38
#define BadInputFormat   14
#define EndOfFile        30

#define tokvar               0
#define toknum               1
#define tokstr               2
#define toksnerr             3
#define tokplus              4
#define tokminus             5
#define toktimes             6
#define tokdiv               7
#define tokup                8
#define toklp                9
#define tokrp               10
#define tokcomma            11
#define toksemi             12
#define tokcolon            13
#define tokeq               14
#define toklt               15
#define tokgt               16
#define tokle               17
#define tokge               18
#define tokne               19
#define tokand              20
#define tokor               21
#define tokxor              22
#define tokmod              23
#define toknot              24
#define toksqr              25
#define toksqrt             26
#define toksin              27
#define tokcos              28
#define toktan              29
#define tokarctan           30
#define toklog              31
#define tokexp              32
#define tokabs              33
#define toksgn              34
#define tokstr_             35
#define tokval              36
#define tokchr_             37
#define tokasc              38
#define toklen              39
#define tokmid_             40
#define tokpeek             41
#define tokrem              42
#define toklet              43
#define tokprint            44
#define tokinput            45
#define tokgoto             46
#define tokif               47
#define tokend              48
#define tokstop             49
#define tokfor              50
#define toknext             51
#define tokwhile            52
#define tokwend             53
#define tokgosub            54
#define tokreturn           55
#define tokread             56
#define tokdata             57
#define tokrestore          58
#define tokgotoxy           59
#define tokon               60
#define tokdim              61
#define tokpoke             62
#define toklist             63
#define tokrun              64
#define toknew              65
#define tokload             66
#define tokmerge            67
#define toksave             68
#define tokbye              69
#define tokdel              70
#define tokrenum            71
#define tokthen             72
#define tokelse             73
#define tokto               74
#define tokstep             75
#define toktc               76
#define tokm0               77
#define tokm                78
#define tokparm             79
#define tokact              80
#define tokmol              81
#define tokla               82
#define toklm               83
#define toksr               84
#define toksi               85
#define toktot              86
#define toktk               87
#define toktime             88
#define toklog10            89
#define toksim_time         90
#define tokequi             91
#define tokgas              92
#define tokpunch            93
#define tokkin              94
#define toks_s              95
#define tokmu               96
#define tokalk              97
#define tokrxn              98
#define tokdist             99
#define tokmisc1           100
#define tokmisc2           101
#define tokedl             102
#define tokstep_no         103
#define toksim_no          104
#define toktotal_time      105
#define tokput             106
#define tokget             107
#define tokcharge_balance  109
#define tokpercent_error   110
#if defined PHREEQ98 || MULTICHART
#define tokgraph_x         111
#define tokgraph_y         112
#define tokgraph_sy        113
#endif
#define tokcell_no         114
#define tokexists          115
#define toksurf            116
#define toklk_species      117
#define toklk_named        118
#define toklk_phase        119
#define toksum_species     120
#define toksum_gas         121
#define toksum_s_s         122
#define tokcalc_value      123
#define tokdescription     124
#define toksys             125
#define tokinstr           126
#define tokltrim           127
#define tokrtrim           128
#define toktrim            129
#define tokpad             130
#define tokchange_por      131
#define tokget_por         132
#define tokosmotic         133
#define tokchange_surf     134
#define tokporevolume      135
#define toksc              136
#define tokgamma           137
#define toklg              138
/* VP: Density Start */
#define tokrho             139
/* VP: Density End */
#define tokcell_volume      140
#define tokcell_pore_volume 141
#define tokcell_porosity    142
#define tokcell_saturation  143
#if defined MULTICHART
#define tokplot_xy          144
#endif
#define toktotmole          145
#define tokiso              146
#define tokiso_unit         147
#define toktotmol           148
#define toktotmoles         149
#define tokeol_             150
#define tokceil             151
#define tokfloor            152
#define tokphase_formula    153
#define toklist_s_s         154
#define tokpr_p             155
#define tokpr_phi           156
#define tokgas_p            157
#define tokgas_vm           158
/* Header file for code generated by "p2c", the Pascal-to-C translator */

/* "p2c"  Copyright (C) 1989, 1990, 1991 Free Software Foundation.
 * By Dave Gillespie, daveg@csvax.cs.caltech.edu.  Version 1.20.
 * This file may be copied, modified, etc. in any way.  It is not restricted
 * by the licence agreement accompanying p2c itself.
 */

#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include <setjmp.h>
#include "phrqtype.h"
#include "PHRQ_base.h"
#include "global_structures.h"
typedef struct varrec
{
	varnamestring name;
	struct varrec *next;
	long dims[maxdims];
	char numdims;
	boolean stringvar;
	union
	{
		struct
		{
			LDBLE *arr;
			LDBLE *val, rv;
		} U0;
		struct
		{
			char **sarr;
			char **sval, *sv;
		} U1;
	} UU;
} varrec;
typedef struct tokenrec
{
	struct tokenrec *next;
	int kind;
	union
	{
		struct varrec *vp;
		LDBLE num;
		char *sp;
		char snch;
	} UU;
#ifdef PHREEQCI_GUI
	size_t n_sz;
	char *sz_num;
#endif
} tokenrec;
typedef struct linerec
{
	long num, num2;
	tokenrec *txt;
	char inbuf[MAX_LINE];
	struct linerec *next;
} linerec;
typedef struct valrec
{
	boolean stringval;
	union
	{
		LDBLE val;
		char *sval;
	} UU;
} valrec;

typedef struct looprec
{
	struct looprec *next;
	linerec *homeline;
	tokenrec *hometok;
	int kind;
	union
	{
		struct
		{
			varrec *vp;
			LDBLE max, step;
		} U0;
	} UU;
} looprec;

/*  variables for exec: */
struct LOC_exec
{
	boolean gotoflag, elseflag;
	tokenrec *t;
};

class Phreeqc;

class PBasic: public PHRQ_base
{
public:
	PBasic(Phreeqc *ptr, PHRQ_io *phrq_io=NULL);
	~PBasic();
typedef struct __p2c_jmp_buf
{
	struct __p2c_jmp_buf *next;
	jmp_buf jbuf;
} __p2c_jmp_buf;


/* Warning: The following will not work if setjmp is used simultaneously.
   This also violates the ANSI restriction about using vars after longjmp,
   but a typical implementation of longjmp will get it right anyway. */


# define TRY(x)         do { __p2c_jmp_buf __try_jb;  \
			     __try_jb.next = __top_jb;  \
			     if (!setjmp((__top_jb = &__try_jb)->jbuf)) {
# define RECOVER(x)	__top_jb = __try_jb.next; } else {
# define RECOVER2(x,L)  __top_jb = __try_jb.next; } else {  \
			     { L: __top_jb = __try_jb.next; }
# define ENDTRY(x)      } } while (0)

#define SETBITS  32

#define Const

#define P2PP(x)      ()
#define PV()       ()
typedef char *Anyptr;

#define Register    register	/* Register variables */
#define char        char		/* Characters (not bytes) */

	// Methods
	int free_dim_stringvar(varrec *varbase);
	void exec(void);
	int basic_renumber(char *commands, void **lnbase, void **vbase, void **lpbase);
	void restoredata(void);
	void clearloops(void);
	void clearvar(varrec * v);
	void clearvars(void);
	char * numtostr(char * Result, LDBLE n);
	void parse(char * inbuf, tokenrec ** buf);
	void listtokens(FILE * f, tokenrec * buf);
	void disposetokens(tokenrec ** tok);
	void parseinput(tokenrec ** buf);
	void errormsg(const char * s);
	void snerr(const char * s);
	void tmerr(const char * s);
	void badsubscr(void);
	LDBLE realfactor(struct LOC_exec *LINK);
	char * strfactor(struct LOC_exec * LINK);
	char *stringfactor(char * Result, struct LOC_exec *LINK);
	long intfactor(struct LOC_exec *LINK);
	LDBLE realexpr(struct LOC_exec *LINK);
	char * strexpr(struct LOC_exec * LINK);
	char * stringexpr(char * Result, struct LOC_exec * LINK);
	long intexpr(struct LOC_exec *LINK);
	void require(int k, struct LOC_exec *LINK);
	void skipparen(struct LOC_exec *LINK);
	varrec * findvar(struct LOC_exec *LINK);
	valrec factor(struct LOC_exec *LINK);
	valrec upexpr(struct LOC_exec * LINK);
	valrec term(struct LOC_exec * LINK);
	valrec sexpr(struct LOC_exec * LINK);
	valrec relexpr(struct LOC_exec * LINK);
	valrec andexpr(struct LOC_exec * LINK);
	valrec expr(struct LOC_exec *LINK);
	void checkextra(struct LOC_exec *LINK);
	boolean iseos(struct LOC_exec *LINK);
	void skiptoeos(struct LOC_exec *LINK);
	linerec * findline(long n);
	linerec * mustfindline(long n);
	void cmdend(struct LOC_exec *LINK);
	void cmdnew(struct LOC_exec *LINK);
	void cmdlist(struct LOC_exec *LINK);
	void cmdload(boolean merging, char * name, struct LOC_exec *LINK);
	void cmdrun(struct LOC_exec *LINK);
	void cmdsave(struct LOC_exec *LINK);
	void cmdput(struct LOC_exec *LINK);
	void cmdchange_por(struct LOC_exec *LINK);
	void cmdchange_surf(struct LOC_exec *LINK);
	void cmdbye(void);
	void cmddel(struct LOC_exec *LINK);
	void cmdrenum(struct LOC_exec *LINK);
	void cmdprint(struct LOC_exec *LINK);
	void cmdpunch(struct LOC_exec *LINK);
	#if defined PHREEQ98 || defined MULTICHART
	void cmdgraph_x(struct LOC_exec *LINK);
	void cmdgraph_y(struct LOC_exec *LINK);
	void cmdgraph_sy(struct LOC_exec *LINK);
	#endif
	#if defined MULTICHART
	void cmdplot_xy(struct LOC_exec *LINK);
	#endif
	void cmdlet(boolean implied, struct LOC_exec *LINK);
	void cmdgoto(struct LOC_exec *LINK);
	void cmdif(struct LOC_exec *LINK);
	void cmdelse(struct LOC_exec *LINK);
	boolean skiploop(int up, int dn, struct LOC_exec *LINK);
	void cmdfor(struct LOC_exec *LINK);
	void cmdnext(struct LOC_exec *LINK);
	void cmdwhile(struct LOC_exec *LINK);
	void cmdwend(struct LOC_exec *LINK);
	void cmdgosub(struct LOC_exec *LINK);
	void cmdreturn(struct LOC_exec *LINK);
	void cmdread(struct LOC_exec *LINK);
	void cmddata(struct LOC_exec *LINK);
	void cmdrestore(struct LOC_exec *LINK);
	void cmdgotoxy(struct LOC_exec *LINK);
	void cmdon(struct LOC_exec *LINK);
	void cmddim(struct LOC_exec *LINK);
	void cmdpoke(struct LOC_exec *LINK);
int basic_main(char *commands);
void cmd_initialize(void);
void cmd_free(void);
int basic_compile(char *commands, void **lnbase, void **vbase, void **lpbase);
int basic_run(char *commands, void *lnbase, void *vbase, void *lpbase);
int basic_init(void);
#ifdef PHREEQ98
void GridChar(char *s, char *a);
#endif
int sget_logical_line(char **ptr, int *l, char *return_line);
void PASCAL_MAIN(int argc, char **argv);
long my_labs(long x);
Anyptr my_memmove(Anyptr d, Const Anyptr s, size_t n);
Anyptr my_memcpy(Anyptr d, Const Anyptr s, size_t n);
int my_memcmp(Const Anyptr s1, Const Anyptr s2, size_t n);
Anyptr my_memset(Anyptr d, int c, size_t n);
int my_toupper(int c);
int my_tolower(int c);
long ipow(long a, long b);
char * strsub(register char *ret, register char *s, register int pos,
	   register int len);
int strpos2(char *s, register char *pat, register int pos);
int strcicmp(register char *s1, register char *s2);
char * strltrim(register char *s);
char * strrtrim(register char *s);
void strmove(register int len, register char *s, register int spos,
		register char *d, register int dpos);
void strinsert(register char *src, register char *dst, register int pos);
int P_peek(FILE * f);
int P_eof(void);
int P_eoln(FILE * f);
void P_readpaoc(FILE * f, char *s, int len);
void P_readlnpaoc(FILE * f, char *s, int len);
long P_maxpos(FILE * f);
char * P_trimname(register char * fn, register int len);
long memavail(void);
long maxavail(void);
long * P_setunion(register long *d, register long *s1, register long *s2);
long * P_setint(register long *d, register long *s1, register long *s2);
long * P_setdiff(register long *d, register long *s1, register long *s2);
long * P_setxor(register long *d, register long *s1, register long *s2);
long * P_addset(register long *s, register unsigned val);
long * P_addsetr(register long *s, register unsigned v1, register unsigned v2);
long * P_remset(register long *s, register unsigned val);
int P_setequal(register long *s1, register long *s2);
int P_subset(register long *s1, register long *s2);
long * P_setcpy(register long *d, register long *s);
long * P_expset(register long *d, register long s);
long P_packset(register long *s);	
int _OutMem(void);
int _CaseCheck(void);
int _NilCheck(void);
static char * _ShowEscape(char *buf, int code, int ior, char *prefix);
int _Escape(int code);
int _EscIO(int code);


protected:
	Phreeqc * PhreeqcPtr;
	char *inbuf;
	linerec *linebase;
	varrec *varbase;
	looprec *loopbase;
	long curline;
	linerec *stmtline, *dataline;
	tokenrec *stmttok, *datatok, *buf;
	boolean exitflag;
	long EXCP_LINE;
	HashTable *command_hash_table;
	struct const_key *command;
	int NCMDS;
	Anyptr __MallocTemp__;
	int P_argc;
	char **P_argv;
	int P_escapecode;
	int P_ioresult;
	__p2c_jmp_buf *__top_jb;

	//int max_line;
};


/* Following defines are suitable for the HP Pascal operating system */

#endif /* _INC_PBasic_H */
