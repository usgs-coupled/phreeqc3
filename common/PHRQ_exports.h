#ifndef INC_PHRQ_EXPORTS_H
#define INC_PHRQ_EXPORTS_H


#if defined(_WINDLL) && defined(IPhreeqc_EXPORTS)
#  define IPQ_DLL_EXPORT __declspec(dllexport)
#else
#  define IPQ_DLL_EXPORT
#endif

#endif // INC_PHRQ_EXPORTS_H
