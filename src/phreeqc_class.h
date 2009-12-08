#if !defined PHREEQC_CLASS
#define P_INSTANCE
#define P_INSTANCE_COMMA
#define P_INSTANCE_POINTER
#define PHREEQC_PTR_ARG
#define PHREEQC_PTR_ARG_COMMA
#else
#define P_INSTANCE p_instance
#define P_INSTANCE_COMMA p_instance,
#define P_INSTANCE_POINTER p_instance->
#define PHREEQC_PTR_ARG Phreeqc *p_instance
#define PHREEQC_PTR_ARG_COMMA Phreeqc *p_instance,
#endif

//#include "Phreeqc_class.h"