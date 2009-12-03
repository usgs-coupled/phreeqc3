#if !defined(SYSTEM_H_INCLUDED)
#define SYSTEM_H_INCLUDED
#include "Solution.h"
#include "Exchange.h"
#include "GasPhase.h"
#include "cxxKinetics.h"
#include "PPassemblage.h"
#include "SSassemblage.h"
#include "Surface.h"
#include "cxxMix.h"
#include "Reaction.h"
#include "Temperature.h"
class cxxSystem 
{
  public:cxxSystem(void);
  public: ~cxxSystem(void);
	 void Initialize(void);
	 void setSolution(cxxSolution * entity)
	{
		this->solution = entity;
	} void setExchange(cxxExchange * entity)
	{
		this->exchange = entity;
	} void setPPassemblage(cxxPPassemblage * entity)
	{
		this->ppassemblage = entity;
	} void setGasPhase(cxxGasPhase * entity)
	{
		this->gasphase = entity;
	} void setSSassemblage(cxxSSassemblage * entity)
	{
		this->ssassemblage = entity;
	} void setKinetics(cxxKinetics * entity)
	{
		this->kinetics = entity;
	} void setSurface(cxxSurface * entity)
	{
		this->surface = entity;
	} void setMix(cxxMix * entity)
	{
		this->mix = entity;
	} void setReaction(cxxReaction * entity)
	{
		this->reaction = entity;
	} void setTemperature(cxxTemperature * entity)
	{
		this->temperature = entity;
	} void totalize();
	
#ifdef ORCHESTRA
	void ORCH_components();
	void ORCH_write(std::ostream & chemistry_dat, std::ostream & input_dat,
					 std::ostream & output_dat);
	void ORCH_write_chemistry_water(std::ostream & chemistry_dat);
	void ORCH_write_chemistry_primary(std::ostream & chemistry_dat);
	void ORCH_write_chemistry_total_O_H(std::ostream & chemistry_dat);
	void ORCH_write_output_vars(std::ostream & outstream);
	void ORCH_write_input(std::ostream & input_dat);
	
#endif	/*  */
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
	cxxNameDouble orch_totals;
};


#endif // !defined(SYSTEM_H_INCLUDED)
