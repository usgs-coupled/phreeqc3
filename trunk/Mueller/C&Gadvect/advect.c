#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <IPhreeqc.h>

int id;
int vt[7];
double dv[7];
char sv[7][100];
char buffer[100];

void ExtractWrite(int cell)
{
	VAR v;
	int j;
	VarInit(&v);
	for (j = 0; j < 7; ++j) {
		GetSelectedOutputValue(id, 1, j, &v);
		vt[j] = v.type;
		switch (vt[j]) {
		case TT_DOUBLE:
			dv[j] = v.dVal;
			sprintf(sv[j], "%23.15e", v.dVal);
			break;
		case TT_STRING:
			strcpy(sv[j], v.sVal);
			break;
		}
		VarClear(&v);
	}
	printf("Cell %d \n\tpH: %4.2f\tSR(calcite): %4.2f\n", cell, dv[5], dv[6]);
}

void EHandler(void)
{
	OutputErrorString(id);
	exit(EXIT_FAILURE);	
}

const char *ConCat(const char *str1, const char *str2)
{
	strcpy(buffer, str1);
	return strcat(buffer, str2);
}

int main(void)
{
	/* Create module, load database, define initial conditions and selected output */
	id = CreateIPhreeqc();
	if (LoadDatabase(id, "phreeqc.dat") != 0) EHandler();
	if (RunFile(id, "ic") != 0) EHandler();

	/* Run cell 1, extract/write result */
	if (RunString(id, "RUN_CELLS; -cells; 1; END") != 0) EHandler();
	ExtractWrite(1);

	/* Advect cell 1 solution to cell 2, run cell 2, extract/write results */
	AccumulateLine(id, ConCat("SOLUTION_MODIFY 2",         ""   ));
	AccumulateLine(id, ConCat("   -cb      ",              sv[0]));
	AccumulateLine(id, ConCat("   -total_h ",              sv[1]));
	AccumulateLine(id, ConCat("   -total_o ",              sv[2]));
	AccumulateLine(id, ConCat("   -totals  ",              ""   ));
	AccumulateLine(id, ConCat("      C     ",              sv[3]));
	AccumulateLine(id, ConCat("      Ca    ",              sv[4]));
	AccumulateLine(id, ConCat("RUN_CELLS; -cells; 2; END", ""   ));
	if (RunAccumulated(id) != 0) EHandler();
	ExtractWrite(2);

	/* Destroy module */
	if (DestroyIPhreeqc(id) != IPQ_OK) EHandler();
	exit(EXIT_SUCCESS);
}