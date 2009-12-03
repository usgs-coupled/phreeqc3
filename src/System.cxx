#include "System.h"
#include <algorithm>			// std::replace 
extern void ORCH_write_chemistry_species(std::ostream & chemistry_dat);

cxxSystem::cxxSystem(void) 
{
	
this->solution = NULL;
	
this->exchange = NULL;
	
this->ppassemblage = NULL;
	
this->gasphase = NULL;
	
this->ssassemblage = NULL;
	
this->kinetics = NULL;
	
this->surface = NULL;
	
this->mix = NULL;
	
this->reaction = NULL;
	
this->temperature = NULL;

} 

cxxSystem::~cxxSystem(void) 
{

} 

void
cxxSystem::Initialize(void) 
{
	
this->solution = NULL;
	
this->exchange = NULL;
	
this->ppassemblage = NULL;
	
this->gasphase = NULL;
	
this->ssassemblage = NULL;
	
this->kinetics = NULL;
	
this->surface = NULL;
	
this->mix = NULL;
	
this->reaction = NULL;
	
this->temperature = NULL;

} 
void
cxxSystem::totalize(void) 
{
	
		//initialize
		this->totals.clear();
	
		//add solution
		if (this->solution != NULL)
		
	{
		
char token[MAX_LENGTH];
		
strcpy(token, "O");
		
this->totals[token] = this->solution->get_total_o();
		
strcpy(token, "H");
		
this->totals[token] = this->solution->get_total_h();
		
strcpy(token, "Charge");
		
this->totals[token] = this->solution->get_cb();
		
this->totals.add_extensive(this->solution->get_totals(), 1.0);
	
}
	
if (this->exchange != NULL)
		
	{
		
this->exchange->totalize();
		
this->totals.add_extensive(this->exchange->get_totals(), 1.0);
	
}
	
if (this->ppassemblage != NULL)
		
	{
		
this->ppassemblage->totalize();
		
this->totals.add_extensive(this->ppassemblage->get_totals(), 1.0);
	
}
	
if (this->gasphase != NULL)
		
	{
		
this->gasphase->totalize();
		
this->totals.add_extensive(this->gasphase->get_totals(), 1.0);
	
}
	
if (this->ssassemblage != NULL)
		
	{
		
this->ssassemblage->totalize();
		
this->totals.add_extensive(this->ssassemblage->get_totals(), 1.0);
	
}
	
if (this->surface != NULL)
		
	{
		
this->ssassemblage->totalize();
		
this->totals.add_extensive(this->surface->get_totals(), 1.0);
	
}
	

		//Need to handle the following 3 reactions:
		//
		//this->kinetics = NULL;
		//this->mix = NULL;
		//this->reaction = NULL;
		//this->temperature = NULL;
		//this->totals.dump_raw(std::cerr, 1);
		return;

}


#ifdef ORCHESTRA
	void
cxxSystem::ORCH_write(std::ostream & chemistry_dat, std::ostream & input_dat,
					  std::ostream & output_dat) 
{
	
		//
		// chemistry file
		//
		this->totalize();
	
		// calculate orchestra components from PHREEQC totals
		this->ORCH_components();
	
		// write definitions for chemistry
		chemistry_dat << std::
		endl << "@class: species_reactions () {" << std::endl;
	
this->ORCH_write_chemistry_water(chemistry_dat);
	
this->ORCH_write_chemistry_primary(chemistry_dat);
	
this->ORCH_write_chemistry_total_O_H(chemistry_dat);
	
ORCH_write_chemistry_species(chemistry_dat);
	

		// add definitions for pure phases
		if (this->ppassemblage != NULL)
		this->ppassemblage->ORCH_write_chemistry(chemistry_dat);
	

		// add definitions for other PHREEQC entities here
		
		// finish up
		chemistry_dat << std::endl << "}" << std::endl;
	
		//
		// input file
		//
		input_dat << std::endl << "@class: input_file_data () {" << std::endl;
	
this->ORCH_write_input(input_dat);
	
input_dat << std::endl << "}" << std::endl;
	
		//
		// output file
		//
		output_dat << std::endl << "Output_every: 1" << std::endl;
	
output_dat << "Var:";
	
this->ORCH_write_output_vars(output_dat);
	
		// add definitions for pure phases
		if (this->ppassemblage != NULL)
		this->ppassemblage->ORCH_write_output_vars(output_dat);
	

		//finish up
		output_dat << std::endl;

};


void
cxxSystem::ORCH_write_chemistry_water(std::ostream & chemistry_dat) 
{
	
		//
		//  Write water entities
		//
		chemistry_dat << std::
		endl << "//********* The water entities" << std::endl;
	

		//  e-, total hydrogen
		chemistry_dat << "@entity(e-, diss, 0)" << std::endl;
	
chemistry_dat << "@Calc: (1, \"e-.act = 10.^(-pe)\")" << std::endl;
	
chemistry_dat << "@solve (pe, 1e-6, lin, 1, e-.liter,  1e-14)" << std::
		endl;
	

		//  H+, charge balance
		chemistry_dat << "@Calc: (1, \"H+.act = 10.^(-pH)\")" << std::endl;
	
chemistry_dat << "@solve (pH, 1e-6, lin, 1, H+.liter,  1e-14)" << std::
		endl;
	

		// H2O
		chemistry_dat << "@entity(" << s_h2o->
		name << ", diss, 55.506)" << std::endl;
	
chemistry_dat << std::endl;

} 

void

cxxSystem::ORCH_write_chemistry_primary(std::ostream & chemistry_dat) 
{
	
chemistry_dat << std::endl << "//********* The primary species" << std::
		endl;
	
		//
		//  Write other master species definitions, i.e. primary entities
		//
		chemistry_dat << "@species(H+, 1)" << std::endl;
	
for (cxxNameDouble::iterator iter = this->totals.begin();
		  iter != this->totals.end(); ++iter)
		
	{
		
std::string name(iter->first);
		
if (name == "H(1)" || name == "E" || name == "H" || name == "O"
			 || name == "Charge")
			continue;
		
struct element *elt;
		
char *element_name = string_hsave(name.c_str());
		
elt = element_store(element_name);
		
assert(elt != NULL);
		
struct species *s_ptr;
		
s_ptr = elt->master->s;
		
assert(s_ptr != NULL);
		
chemistry_dat << "@species(" << s_ptr->name << ", " << s_ptr->
			z << ")" << std::endl;
		
			// regular mole balance
			chemistry_dat << "@primary_entity(" << s_ptr->
			name << ", 1e-9, liter, 1.0e-9)" << std::endl;
	
} 
chemistry_dat << std::endl;

} 

void

cxxSystem::ORCH_write_chemistry_total_O_H(std::ostream & chemistry_dat) 
{
	
chemistry_dat << std::
		endl << "//********* Define total hydrogen and oxygen" << std::endl;
	

		// Write H total equation
		chemistry_dat << "@var: total_hydrogen 0" << std::endl;
	
chemistry_dat << "@calc: (5, \"total_hydrogen = 0";
	
int i;
	
for (i = 0; i < count_s_x; i++)
		
	{
		
			// write in terms of orchestra components
			if (s_x[i]->primary != NULL
				|| (s_x[i]->secondary != NULL
					&& s_x[i]->secondary->in == TRUE))
			
		{
			
if (s_x[i]->h != 0)
				
			{
				
chemistry_dat << "+";
				
if (s_x[i]->h != 1)
					
				{
					
chemistry_dat << s_x[i]->h << "*";
				
}
				
chemistry_dat << "{" << s_x[i]->name << ".liter}";
			
}
		
}
	
}
	
chemistry_dat << "\")" << std::endl;
	

		// Write O total equation
		chemistry_dat << "@var: total_oxygen 0" << std::endl;
	
chemistry_dat << "@calc: (5, \"total_oxygen = 0";
	
for (i = 0; i < count_s_x; i++)
		
	{
		
if (s_x[i]->o != 0)
			
		{
			
				// write in terms of orchestra components
				if (s_x[i]->primary != NULL
					|| (s_x[i]->secondary != NULL
						&& s_x[i]->secondary->in == TRUE))
				
			{
				
chemistry_dat << "+";
				
if (s_x[i]->o != 1)
					
				{
					
chemistry_dat << s_x[i]->o << "*";
				
}
				
chemistry_dat << "{" << s_x[i]->name << ".liter}";
			
}
		
}
	
}
	
chemistry_dat << "\")" << std::endl;
	
chemistry_dat << std::endl;

}

void
cxxSystem::ORCH_write_input(std::ostream & input_dat) 
{
	


//
//  Write orchestra input file info
//
		std::ostringstream headings, data;
	
data.precision(DBL_DIG - 1);
	
headings << "var:   ";
	
data << "data:  ";
	

headings << "tempc\t";
	
data << this->solution->get_tc() << "\t";
	

headings << "pH\t";
	
data << this->solution->get_ph() << "\t";
	

headings << "pe\t";
	
data << this->solution->get_pe() << "\t";
	

headings << "H2O.act\t";
	
data << 1 << "\t";
	

headings << "I\t";
	
data << this->solution->get_mu() << "\t";
	

for (cxxNameDouble::iterator iter = this->orch_totals.begin();
		   iter != this->orch_totals.end(); ++iter)
		
	{
		
headings << iter->first << ".liter" << "\t";
		
data << iter->second << "\t";
	
}
	

		//  activity estimate
		for (cxxNameDouble::iterator iter = this->totals.begin();
			 iter != this->totals.end(); ++iter)
		
	{
		
if (iter->first == "O" || iter->first == "Charge")
			continue;
		
std::string master_name;
		
struct master *master_ptr;
		
master_ptr = master_bsearch(iter->first);
		
assert(master_ptr != NULL);
		

cxxNameDouble ma = this->solution->get_master_activity();
		
cxxNameDouble::iterator it = ma.find(iter->first);
		
if (it == ma.end())
			
		{
			
it = ma.find(master_ptr->s->secondary->elt->name);
		
}
		
headings << master_ptr->s->name << ".act\t";
		
if (it != ma.end())
			
		{
			
data << pow(10., it->second) << "\t";
		
}
		else
			
		{
			
data << 1e-9 << "\t";
		
}
	
}
	

		// Isotopes
		//s_oss << "-Isotopes" << std::endl;
		/*
		   {
		   for (std::list<cxxSolutionIsotope>::const_iterator it = this->isotopes.begin(); it != isotopes.end(); ++it) {
		   it->dump_raw(s_oss, indent + 2);
		   }
		   }
		 */ 
		
		// Write data to string
		input_dat << headings.str() << std::endl;
	
input_dat << data.str() << std::endl;
	

return;

}


void
cxxSystem::ORCH_components(void) 
{
	
		// translate from H, O, charge to H+tot, e-tot, and H2Otot
		cxxNameDouble::iterator it;
	
cxxNameDouble temp_totals;
	
		//
		// Set names in orch_totals to master species names
		//
		this->orch_totals.clear();
	
temp_totals = this->totals;
	
temp_totals.erase("H");
	
temp_totals.erase("O");
	
temp_totals.erase("Charge");
	
		//temp_totals.dump_raw(std::cerr, 1);
		for (it = temp_totals.begin(); it != temp_totals.end(); it++)
		
	{
		
struct element *elt_ptr;
		
elt_ptr = element_store(it->first);
		
assert(elt_ptr != NULL);
		
struct master *master_ptr;
		
master_ptr = elt_ptr->primary;
		
assert(master_ptr != NULL);
		
double coef = master_ptr->coef;
		
if (master_ptr->coef == 0)
			
		{
			
coef = 1;
		
}
		
this->orch_totals[master_ptr->s->name] = it->second / coef;
	
}
	
		//
		// Calculate h2otot
		//
		it = this->totals.find("O");
	
assert(it != this->totals.end());
	
double h2otot = it->second;
	
		// subtract O in master species
		for (it = temp_totals.begin(); it != temp_totals.end(); it++)
		
	{
		
struct element *elt_ptr;
		
elt_ptr = element_store(it->first);
		
struct master *master_ptr;
		
master_ptr = elt_ptr->primary;
		
h2otot -= it->second * master_ptr->s->o;
	
} 
		//
		// Calculate htot
		//
		it = this->totals.find("H");
	
assert(it != this->totals.end());
	
double htot = it->second - 2 * h2otot;
	
		// subtract O in master species
		for (it = temp_totals.begin(); it != temp_totals.end(); it++)
		
	{
		
struct element *elt_ptr;
		
elt_ptr = element_store(it->first);
		
struct master *master_ptr;
		
master_ptr = elt_ptr->primary;
		
htot -= it->second * master_ptr->s->h;
	
} 
		//
		// Calculate etot
		//
		it = this->totals.find("Charge");
	
assert(it != this->totals.end());
	
double etot = -it->second + htot;
	
		// subtract O in master species
		for (it = temp_totals.begin(); it != temp_totals.end(); it++)
		
	{
		
struct element *elt_ptr;
		
elt_ptr = element_store(it->first);
		
struct master *master_ptr;
		
master_ptr = elt_ptr->primary;
		
etot += it->second * master_ptr->s->z;
	
} 
		//
		// store h2otot, htot, etot in orch_totals
		//
		this->orch_totals["H2O"] = h2otot;
	
this->orch_totals["H+"] = htot;
	
this->orch_totals["e-"] = etot;
	
this->orch_totals.dump_raw(std::cerr, 1);

} 
void

cxxSystem::ORCH_write_output_vars(std::ostream & outstream) 
{
	
outstream << "\tnr_iter";
	
		//
		//  Serialize solution
		//
		outstream << "\tstart_solution";
	
		//tc
		outstream << "\ttempc";
	
		//pH
		outstream << "\tpH";
	
		//pe
		outstream << "\tpe";
	
		//mu
		outstream << "\tI";
	
		//ah2o
		outstream << "\tH2O.act";
	
		//total_h;
		outstream << "\ttotal_hydrogen";
	
		//total_o;
		outstream << "\ttotal_oxygen";
	
		//cb
		outstream << "\tchargebalance";
	
		//mass_water;
		outstream << "\tH2O.con";
	
		//total_alkalinity;
		outstream << "\tAlkalinity";
	
		//orchestra master variables
		outstream << "\tH+.diss";
	
outstream << "\te-.diss";
	
outstream << "\tH2O.diss";
	
		//
		// Write totals
		for (cxxNameDouble::iterator it = this->orch_totals.begin();
			 it != this->orch_totals.end(); it++)
		
	{
		
if (it->first == "H+" || it->first == "H2O" || it->first == "e-")
			continue;
		
outstream << "\t" << it->first << ".diss";
	
}
	
outstream << "\tend_totals";
	
for (cxxNameDouble::iterator it = this->orch_totals.begin();
		  it != this->orch_totals.end(); it++)
		
	{
		
if (it->first == "H+" || it->first == "H2O" || it->first == "e-")
			continue;
		
outstream << "\t" << it->first << ".act";
	
}
	
outstream << "\tend_master_activities";
	
		//
		// Write all species activities and concentrations
		//
	int i;
	
for (i = 0; i < count_s_x; i++)
	{
		
std::string name(s_x[i]->name);
		
std::replace(name.begin(), name.end(), '(', '[');
		
std::replace(name.begin(), name.end(), ')', ']');
		
outstream << "\t" << name.c_str() << ".act" << "\t" << name.
			c_str() << ".con";
	
}
	
outstream << "\tend_species";

}


#endif	/* 
 */
