#ifndef _INC_SAXPHREEQC_H
#define _INC_SAXPHREEQC_H

#if defined(__cplusplus) | defined(_CPP)
extern "C"
{
#endif

	void SAX_StartSystem();
	int SAX_AddSolution(struct solution *solution_ptr);
	void SAX_EndSystem();
	int SAX_GetXMLLength();
	const char *SAX_GetXMLStr();
	int SAX_UnpackSolutions(void *pvBuffer, int buf_size);


#if defined(__cplusplus) | defined(_CPP)
}
#endif

#endif
