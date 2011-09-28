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
#include "phreeqc_class.h"
class cxxSolution;
class cxxExchange;
class cxxGasPhase;
class cxxKinetics;
class cxxPPassemblage;
class cxxSSassemblage;
class cxxSurface;
class cxxReaction;
class cxxTemperature;
class cxxMix;

class cxxSystem: public PHRQ_base 
{
public:
	cxxSystem(PHRQ_io *io=NULL);
	~cxxSystem(void);
	void Initialize(void);
	void setSolution(cxxSolution * entity)
	{
		this->solution = entity;
	} 
	void setExchange(cxxExchange * entity)
	{
		this->exchange = entity;
	} 
	void setPPassemblage(cxxPPassemblage * entity)
	{
		this->ppassemblage = entity;
	} 
	void setGasPhase(cxxGasPhase * entity)
	{
		this->gasphase = entity;
	} 
	void setSSassemblage(cxxSSassemblage * entity)
	{
		this->ssassemblage = entity;
	} 
	void setKinetics(cxxKinetics * entity)
	{
		this->kinetics = entity;
	} 
	void setSurface(cxxSurface * entity)
	{
		this->surface = entity;
	} 
	void setMix(cxxMix * entity)
	{
		this->mix = entity;
	} 
	void setReaction(cxxReaction * entity)
	{
		this->reaction = entity;
	} 
	void setTemperature(cxxTemperature * entity)
	{
		this->temperature = entity;
	} 
	void totalize(PHREEQC_PTR_ARG);
	cxxNameDouble &getTotals(void)
	{
		return this->totals;
	}
	
private:
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
	cxxNameDouble totals;
};


#endif // !defined(SYSTEM_H_INCLUDED)
