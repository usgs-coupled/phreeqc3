#if !defined(SYSTEM_H_INCLUDED)
#define SYSTEM_H_INCLUDED
//#include "Solution.h"
//#include "Exchange.h"
//#include "GasPhase.h"
//#include "cxxKinetics.h"
//#include "PPassemblage.h"
//#include "SSassemblage.h"
//#include "Surface.h"
//#include "cxxMix.h"
//#include "Reaction.h"
//#include "Temperature.h"
#include "NameDouble.h"
#include "PHRQ_base.h"
#include "Phreeqc_class.h"
class cxxSolution;
class cxxExchange;
class cxxGasPhase;
class cxxKinetics;
class cxxPPassemblage;
class cxxSSassemblage;
class cxxSurface;
class cxxReaction;
class cxxTemperature;
class cxxPressure;
class cxxMix;

class cxxSystem: public PHRQ_base 
{
public:
	cxxSystem(PHRQ_io *io=NULL);
	~cxxSystem(void);
	void Initialize(void);
	void Set_Solution(cxxSolution * entity)
	{
		this->solution = entity;
	} 
	void Set_Exchange(cxxExchange * entity)
	{
		this->exchange = entity;
	} 
	void Set_PPassemblage(cxxPPassemblage * entity)
	{
		this->ppassemblage = entity;
	} 
	void Set_GasPhase(cxxGasPhase * entity)
	{
		this->gasphase = entity;
	} 
	void Set_SSassemblage(cxxSSassemblage * entity)
	{
		this->ssassemblage = entity;
	} 
	void Set_Kinetics(cxxKinetics * entity)
	{
		this->kinetics = entity;
	} 
	void Set_Surface(cxxSurface * entity)
	{
		this->surface = entity;
	} 
	void Set_Mix(cxxMix * entity)
	{
		this->mix = entity;
	} 
	void Set_Reaction(cxxReaction * entity)
	{
		this->reaction = entity;
	} 
	void Set_Temperature(cxxTemperature * entity)
	{
		this->temperature = entity;
	} 
	void Set_Pressure(cxxPressure * entity)
	{
		this->pressure = entity;
	} 
	void totalize(PHREEQC_PTR_ARG);
	cxxNameDouble &Get_Totals(void)
	{
		return this->totals;
	}
	
protected:
	cxxSolution * solution;
	cxxExchange * exchange;
	cxxPPassemblage * ppassemblage;
	cxxGasPhase * gasphase;
	cxxSSassemblage * ssassemblage;
	cxxKinetics * kinetics;
	cxxSurface * surface;
	cxxMix * mix;
	cxxReaction * reaction;
	cxxTemperature * temperature;
	cxxPressure * pressure;
	cxxNameDouble totals;
};


#endif // !defined(SYSTEM_H_INCLUDED)
