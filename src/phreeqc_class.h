#ifndef _INC_PHREEQC_CLASS_H
#define _INC_PHREEQC_CLASS_H
#if !defined PHREEQC_CLASS
#define P_INSTANCE
#define P_INSTANCE_COMMA
#define P_INSTANCE_POINTER
#define PHREEQC_PTR_ARG
#define PHREEQC_PTR_ARG_COMMA
#define PHREEQC_THIS
#define PHREEQC_THIS_COMMA
#define PHREEQC_NAME_SPACE ::
#else
#define P_INSTANCE p_instance
#define P_INSTANCE_COMMA p_instance,
#define P_INSTANCE_POINTER p_instance->
#define PHREEQC_PTR_ARG Phreeqc *p_instance
#define PHREEQC_PTR_ARG_COMMA Phreeqc *p_instance,
#define PHREEQC_THIS this
#define PHREEQC_THIS_COMMA this,
#define PHREEQC_NAME_SPACE Phreeqc::
#endif
#endif /* _INC_PHREEQC_CLASS_H */
//#include "Phreeqc_class.h"