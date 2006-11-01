// StorageBin.cxx: implementation of the cxxStorageBin class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)
#endif

#include <fstream>
#include <iostream>     // std::cout std::cerr
#include "Utils.h"   // define first
#include "StorageBin.h"
#include "Solution.h"
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include "output.h"
#include <cassert>     // assert
#include <algorithm>   // std::sort 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
cxxStorageBin::cxxStorageBin()
{
        // default constructor for cxxStorageBin 
}

cxxStorageBin::~cxxStorageBin()
{
}

void cxxStorageBin::import_phreeqc(void)
        //
	// pull data out of c storage
        //
{
	int i;

	// Solutions
	for (i = 0; i < count_solution; i++) {
		Solutions[solution[i]->n_user] = cxxSolution(solution[i]);
	}

	// Exchangers
	for (i = 0; i < count_exchange; i++) {
		Exchangers[exchange[i].n_user] = cxxExchange(&exchange[i]);
	}

	// GasPhases
	for (i = 0; i < count_gas_phase; i++) {
		GasPhases[gas_phase[i].n_user] = cxxGasPhase(&gas_phase[i]);
	}

	// Kinetics
	for (i = 0; i < count_kinetics; i++) {
		Kinetics[kinetics[i].n_user] = cxxKinetics(&kinetics[i]);
	}

	// PPassemblages
	for (i = 0; i < count_pp_assemblage; i++) {
		PPassemblages[pp_assemblage[i].n_user] = cxxPPassemblage(&pp_assemblage[i]);
	}

	// SSassemblages
	for (i = 0; i < count_s_s_assemblage; i++) {
		SSassemblages[s_s_assemblage[i].n_user] = cxxSSassemblage(&s_s_assemblage[i]);
	}

	// Surfaces
	for (i = 0; i < count_surface; i++) {
		Surfaces[surface[i].n_user] = cxxSurface(&surface[i]);
	}

	// Mixes
	for (i = 0; i < count_mix; i++) {
		Mixes[mix[i].n_user] = cxxMix(&mix[i]);
	}

	// Reactions
	for (i = 0; i < count_irrev; i++) {
		Reactions[irrev[i].n_user] = cxxReaction(&irrev[i]);
	}

	// Temperatures
	for (i = 0; i < count_temperature; i++) {
		Temperatures[temperature[i].n_user] = cxxTemperature(&temperature[i]);
	}
}


#ifdef SKIP
void cxxStorageBin::dump_xml(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing mix message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // StorageBin element and attributes
        s_oss << indent0;
        s_oss << "<mix " << std::endl;

        s_oss << indent1;
        s_oss << "pitzer_mix_gammas=\"" << this->pitzer_mix_gammas << "\"" << std::endl;

        // components
        s_oss << indent1;
        s_oss << "<component " << std::endl;
        for (std::list<cxxStorageBinComp>::const_iterator it = mixComps.begin(); it != mixComps.end(); ++it) {
                it->dump_xml(s_oss, indent + 2);
        }

        return;
}
#endif

void cxxStorageBin::dump_raw(std::ostream& s_oss, unsigned int indent)const
{
	// Dump all data

        //const char    ERR_MESSAGE[] = "Packing mix message: %s, element not found\n";
        s_oss.precision(DBL_DIG - 1);

	// Solutions
	Utilities::dump_raw(Solutions, s_oss, indent);

	// Exchange
	Utilities::dump_raw(Exchangers, s_oss, indent);

	// Gas Phases
	Utilities::dump_raw(GasPhases, s_oss, indent);

	// Kinetics
	Utilities::dump_raw(Kinetics, s_oss, indent);

	// PPassemblage
	Utilities::dump_raw(PPassemblages, s_oss, indent);

	// SSassemblage
	Utilities::dump_raw(SSassemblages, s_oss, indent);

	// Surface
	Utilities::dump_raw(Surfaces, s_oss, indent);
}
void cxxStorageBin::dump_raw(std::ostream& s_oss, int n, unsigned int indent)
{
	// Dump one user number

        //const char    ERR_MESSAGE[] = "Packing mix message: %s, element not found\n";
        s_oss.precision(DBL_DIG - 1);

	// Solutions
	if (this->getSolution(n) != NULL) {
		this->getSolution(n)->dump_raw(s_oss, indent);
	}

	// Exchange
	if (this->getExchange(n) != NULL) {
		this->getExchange(n)->dump_raw(s_oss, indent);
	}

	// Gas Phases
	if (this->getGasPhase(n) != NULL) {
		this->getGasPhase(n)->dump_raw(s_oss, indent);
	}

	// Kinetics
	if (this->getKinetics(n) != NULL) {
		this->getKinetics(n)->dump_raw(s_oss, indent);
	}

	// PPassemblage
	if (this->getPPassemblage(n) != NULL) {
		this->getPPassemblage(n)->dump_raw(s_oss, indent);
	}

	// SSassemblage
	if (this->getSSassemblage(n) != NULL) {
		this->getSSassemblage(n)->dump_raw(s_oss, indent);
	}

	// Surface
	if (this->getSurface(n) != NULL) {
		this->getSurface(n)->dump_raw(s_oss, indent);
	}
}

void cxxStorageBin::read_raw(CParser& parser)
{
	CParser::LINE_TYPE i;
	while ((i = parser.check_line("StorageBin read_raw", false, true, true, true)) != CParser::LT_KEYWORD)
	{
		if (i == CParser::LT_EOF) return; // CParser::LT_EOF;
	}

	for (;;) {
		switch(parser.next_keyword())
		{
		case CParser::KT_END: 
		case CParser::KT_EOF:
		case CParser::KT_NONE:
			goto END_OF_SIMULATION_INPUT;
			break;
		/*
		  KT_SOLUTION_RAW     =  5,
		  KT_EXCHANGE_RAW     =  6,
		  KT_GASPHASE_RAW     =  7,
		  KT_KINETICS_RAW     =  8,
		  KT_PPASSEMBLAGE_RAW =  9,
		  KT_SSASSEMBLAGE_RAW =  10,
		  KT_SURFACE_RAW      =  11
		*/
		case CParser::KT_SOLUTION_RAW:
			{
				cxxSolution entity;
				entity.read_raw(parser);
				Solutions[entity.get_n_user()] = entity;
			}
			break;

		case CParser::KT_EXCHANGE_RAW:
			{
				cxxExchange entity;
				entity.read_raw(parser);
				Exchangers[entity.get_n_user()] = entity;
			}
			break;

		case CParser::KT_GASPHASE_RAW:
			{
				cxxGasPhase entity;
				entity.read_raw(parser);
				GasPhases[entity.get_n_user()] = entity;
			}
			break;

		case CParser::KT_KINETICS_RAW:
			{
				cxxKinetics entity;
				entity.read_raw(parser);
				Kinetics[entity.get_n_user()] = entity;
			}
			break;

		case CParser::KT_PPASSEMBLAGE_RAW:
			{
				cxxPPassemblage entity;
				entity.read_raw(parser);
				PPassemblages[entity.get_n_user()] = entity;
			}
			break;

		case CParser::KT_SSASSEMBLAGE_RAW:
			{
				cxxSSassemblage entity;
				entity.read_raw(parser);
				SSassemblages[entity.get_n_user()] = entity;
			}
			break;

		case CParser::KT_SURFACE_RAW:
			{
				cxxSurface entity;
				entity.read_raw(parser);
				Surfaces[entity.get_n_user()] = entity;
			}
			break;

		default:
			break;
		}
	}

END_OF_SIMULATION_INPUT:
	return; //CParser::LT_OK;
}

void cxxStorageBin::add(struct system * system_ptr)
        //
        // add data from a system structure
        //
{

	// Solutions
	if (system_ptr->solution != NULL) {
		this->Solutions[system_ptr->solution->n_user] = cxxSolution(system_ptr->solution);
	}

	// Exchangers
	if (system_ptr->exchange != NULL) {
		this->Exchangers[system_ptr->exchange->n_user] = cxxExchange(system_ptr->exchange);
	}

	// GasPhases
	if (system_ptr->gas_phase != NULL) {
		this->GasPhases[system_ptr->gas_phase->n_user] = cxxGasPhase(system_ptr->gas_phase);
	}

	// Kinetics
	if (system_ptr->kinetics != NULL) {
		this->Kinetics[system_ptr->kinetics->n_user] = cxxKinetics(system_ptr->kinetics);
	}

	// PPassemblages
	if (system_ptr->pp_assemblage != NULL) {
		this->PPassemblages[system_ptr->pp_assemblage->n_user] = cxxPPassemblage(system_ptr->pp_assemblage);
	}

	// SSassemblages
	if (system_ptr->s_s_assemblage != NULL) {
		this->SSassemblages[system_ptr->s_s_assemblage->n_user] = cxxSSassemblage(system_ptr->s_s_assemblage);
	}

	// Surfaces
	if (system_ptr->surface != NULL) {
		this->Surfaces[system_ptr->surface->n_user] = cxxSurface(system_ptr->surface);
	}
}

void cxxStorageBin::cxxStorageBin2phreeqc(int n)
        //
        // copy data fromphreeqc storage to storage bin
        //
{
	//std::ostringstream oss;

	// Solutions
	{
		
		std::map <int, cxxSolution>::iterator it = this->Solutions.find(n);
		if (it != this->Solutions.end()){
			solution[0] = (it->second).cxxSolution2solution();
			solution[0]->n_user = n;
			solution[0]->n_user_end = n;
			count_solution++;
		} else {
			error_msg("cxxSolution not found in system2phreeqc", STOP);
		}
	}

	// Exchangers
	{
		std::map <int, cxxExchange>::iterator it = this->Exchangers.find(n);
		if ( it != this->Exchangers.end()) {
			struct exchange *exchange_ptr = (it->second).cxxExchange2exchange();
			exchange_copy(exchange_ptr, &exchange[0], n);
			count_exchange++;
			exchange_free(exchange_ptr);
			exchange_ptr = (struct exchange *) free_check_null(exchange_ptr);
		} 
	}

	// GasPhases
	{
		std::map <int, cxxGasPhase>::iterator it = this->GasPhases.find(n);
		if ( it != this->GasPhases.end()) {
			struct gas_phase *gas_phase_ptr = (it->second).cxxGasPhase2gas_phase();
			gas_phase_copy(gas_phase_ptr, &gas_phase[0], n);
			count_gas_phase++;
			gas_phase_free(gas_phase_ptr);
			gas_phase_ptr = (struct gas_phase *) free_check_null(gas_phase_ptr);
		} 
	}

	// Kinetics
	{
		std::map <int, cxxKinetics>::iterator it = this->Kinetics.find(n);
		if ( it != this->Kinetics.end()) {
			struct kinetics *kinetics_ptr = (it->second).cxxKinetics2kinetics();
			kinetics_copy(kinetics_ptr, &kinetics[0], n);
			count_kinetics++;
			kinetics_free(kinetics_ptr);
			kinetics_ptr = (struct kinetics *) free_check_null(kinetics_ptr);
		} 
	}

	// PPassemblages
	{
		std::map <int, cxxPPassemblage>::iterator it = this->PPassemblages.find(n);
		if ( it != this->PPassemblages.end()) {
			struct pp_assemblage *pp_assemblage_ptr = (it->second).cxxPPassemblage2pp_assemblage();
			pp_assemblage_copy(pp_assemblage_ptr, &pp_assemblage[0], n);
			count_pp_assemblage++;
			pp_assemblage_free(pp_assemblage_ptr);
			pp_assemblage_ptr = (struct pp_assemblage *) free_check_null(pp_assemblage_ptr);
		} 
	}

	// SSassemblages
	{
		std::map <int, cxxSSassemblage>::iterator it = this->SSassemblages.find(n);
		if ( it != this->SSassemblages.end()) {
			struct s_s_assemblage *s_s_assemblage_ptr = (it->second).cxxSSassemblage2s_s_assemblage();
			s_s_assemblage_copy(s_s_assemblage_ptr, &s_s_assemblage[0], n);
			count_s_s_assemblage++;
			s_s_assemblage_free(s_s_assemblage_ptr);
			s_s_assemblage_ptr = (struct s_s_assemblage *) free_check_null(s_s_assemblage_ptr);
		} 
	}

	// Surfaces
	{
		std::map <int, cxxSurface>::iterator it = this->Surfaces.find(n);
		if ( it != this->Surfaces.end()) {
			struct surface *surface_ptr = (it->second).cxxSurface2surface();
			surface_copy(surface_ptr, &surface[0], n);
			count_surface++;
			surface_free(surface_ptr);
			surface_ptr = (struct surface *) free_check_null(surface_ptr);
		} 
	}
	//std::cerr << oss.str();

}
void cxxStorageBin::phreeqc2cxxStorageBin(int n)
        //
        // copy data fromphreeqc storage to storage bin
        //
{
	int pos;

	// Solutions
	{
		solution_bsearch(n, &pos, TRUE);
		this->Solutions[n] = cxxSolution(solution[pos]);
	}

	// Exchangers
	{
		if (exchange_bsearch(n, &pos) != NULL) {
			this->Exchangers[n] = cxxExchange(&(exchange[pos]));
		}
	}

	// GasPhases
	{
		if (gas_phase_bsearch(n, &pos) != NULL) {
			this->GasPhases[n] = cxxGasPhase(&(gas_phase[pos]));
		}
	}

	// Kinetics
	{
		if (kinetics_bsearch(n, &pos) != NULL) {
			this->Kinetics[n] = cxxKinetics(&(kinetics[pos]));
		}
	}

	// PPassemblages
	{
		if (pp_assemblage_bsearch(n, &pos) != NULL) {
			this->PPassemblages[n] = cxxPPassemblage(&(pp_assemblage[pos]));
		}
	}

	// SSassemblages
	{
		if (s_s_assemblage_bsearch(n, &pos) != NULL) {
			this->SSassemblages[n] = cxxSSassemblage(&(s_s_assemblage[pos]));
		}
	}

	// Surfaces
	{
		if (surface_bsearch(n, &pos) != NULL) {
			this->Surfaces[n] = cxxSurface(&(surface[pos]));
		}
	}
}

void cxxStorageBin::remove(int n)
{
	// Solution
	this->Solutions.erase(n);

	// Exchanger
	this->Exchangers.erase(n);

	// GasPhase
	this->GasPhases.erase(n);

	// Kinetics
	this->Kinetics.erase(n);

	// PPassemblage
	this->PPassemblages.erase(n);

	// SSassemblage
	this->SSassemblages.erase(n);

	// Surface
	this->Surfaces.erase(n);
}

cxxSolution *cxxStorageBin::mix_cxxSolutions(cxxMix &mixmap)

{
/*
 *   mixes solutions based on cxxMix structure, returns new solution
 *   return solution must be freed by calling method
 */
	double intensive, extensive;
	cxxSolution *cxxsoln_ptr, *cxxsoln_ptr1;
/*
 *   Zero out global solution data
 */
	cxxsoln_ptr = new cxxSolution(0.0);
/*
 *   Determine sum of mixing fractions
 */	
	extensive = 0.0;
	
	std::map<int, double> *mixcomps = mixmap.comps();
	
	std::map<int, double>::const_iterator it;
	for (it = mixcomps->begin(); it != mixcomps->end(); it++) {
		extensive += it->second;
	}
/*
 *   Add solutions 
 */	
	for (it = mixcomps->begin(); it != mixcomps->end(); it++) {
		cxxsoln_ptr1 = &((this->Solutions.find(it->first))->second);
		if (cxxsoln_ptr1 == NULL) {
			sprintf(error_string, "Solution %d not found in mix_cxxSolutions.", it->first);
			error_msg(error_string, CONTINUE);
			input_error++;
			return(NULL);
		} 
		intensive = it->second/extensive;
		cxxsoln_ptr->add(*cxxsoln_ptr1, intensive, it->second);
	} 
	return(cxxsoln_ptr);
}

struct system *cxxStorageBin::cxxStorageBin2system(int n)
        //
        // make a system from storagebin
        //
{
	struct system *system_ptr = (struct system *) PHRQ_malloc(sizeof(struct system));
	if (system_ptr  == NULL) malloc_error();

	// Solutions
	
	if (this->getSolution(n) != NULL) {
		system_ptr->solution = (this->getSolution(n))->cxxSolution2solution();
	} else {
		system_ptr->solution = NULL;
	}

	// Exchangers
	if (this->getExchange(n) != NULL) {
		system_ptr->exchange = (this->getExchange(n))->cxxExchange2exchange();
	} else {
		system_ptr->exchange = NULL;
	}

	// GasPhases
	if (this->getGasPhase(n) != NULL) {
		system_ptr->gas_phase = (this->getGasPhase(n))->cxxGasPhase2gas_phase();
	} else {
		system_ptr->gas_phase = NULL;
	}

	// Kinetics
	if (this->getKinetics(n) != NULL) {
		system_ptr->kinetics = (this->getKinetics(n))->cxxKinetics2kinetics();
	} else {
		system_ptr->kinetics = NULL;
	}

	// PPassemblages
	if (this->getPPassemblage(n) != NULL) {
		system_ptr->pp_assemblage = (this->getPPassemblage(n))->cxxPPassemblage2pp_assemblage();
	} else {
		system_ptr->pp_assemblage = NULL;
	}

	// SSassemblages
	if (this->getSSassemblage(n) != NULL) {
		system_ptr->s_s_assemblage = (this->getSSassemblage(n))->cxxSSassemblage2s_s_assemblage();
	} else {
		system_ptr->s_s_assemblage = NULL;
	}

	// Surfaces
	if (this->getSurface(n) != NULL) {
		system_ptr->surface = (this->getSurface(n))->cxxSurface2surface();
	} else {
		system_ptr->surface = NULL;
	}
	return system_ptr;
}
#ifdef USE_MPI
#include <mpi.h>
void cxxStorageBin::mpi_send(int n, int task_number)
{
        //
        // Send data for system n to task_number
        //
	std::vector<int> ints;
	std::vector<double> doubles;
	
	// Solution
	if (this->getSolution(n) != NULL) {
		ints.push_back(1);
		this->getSolution(n)->mpi_pack(ints, doubles);
	} else {
		ints.push_back(0);
	}
	//std::cerr << "Packed Solution" << std::endl;

	// Exchanger
	if (this->getExchange(n) != NULL) {
		ints.push_back(1);
		this->getExchange(n)->mpi_pack(ints, doubles);
	} else {
		ints.push_back(0);
	}
	//std::cerr << "Packed Exchange" << std::endl;

	// GasPhase
	if (this->getGasPhase(n) != NULL) {
		ints.push_back(1);
		this->getGasPhase(n)->mpi_pack(ints, doubles);
	} else {
		ints.push_back(0);
	}
	//std::cerr << "Packed GasPhase" << std::endl;

	// Kinetics
	if (this->getKinetics(n) != NULL) {
		ints.push_back(1);
		this->getKinetics(n)->mpi_pack(ints, doubles);
	} else {
		ints.push_back(0);
	}
	//std::cerr << "Packed Kinetics" << std::endl;

	// PPassemblages
	if (this->getPPassemblage(n) != NULL) {
		ints.push_back(1);
		this->getPPassemblage(n)->mpi_pack(ints, doubles);
	} else {
		ints.push_back(0);
	}
	//std::cerr << "Packed PPassemblage" << std::endl;

	// SSassemblages
	if (this->getSSassemblage(n) != NULL) {
		ints.push_back(1);
		this->getSSassemblage(n)->mpi_pack(ints, doubles);
	} else {
		ints.push_back(0);
	}
	//std::cerr << "Packed SSassemblage" << std::endl;

	// Surfaces
	if (this->getSurface(n) != NULL) {
		ints.push_back(1);
		this->getSurface(n)->mpi_pack(ints, doubles);
	} else {
		ints.push_back(0);
	}
	//std::cerr << "Packed Surface" << std::endl;

	// Pack data
	int max_size = 0;
	int member_size = 0;
	MPI_Pack_size((int) ints.size() + 10, MPI_INT, MPI_COMM_WORLD, &member_size);
	max_size += member_size;
	MPI_Pack_size((int) doubles.size(), MPI_DOUBLE, MPI_COMM_WORLD, &member_size);
	max_size += member_size + 10;
	void *buffer = PHRQ_malloc(max_size);
	if (buffer == NULL) malloc_error();
	
	// Convert to arrays
	int i = (int) ints.size();
	//int int_array[i];
	int d = (int) doubles.size();
	//double double_array[d];
	/*
	for (int j = 0; j < i; j++) {
		int_array[j] = ints[j];
		//std::cerr << "Sending ints " << j << " value " << ints[j] << std::endl;
	}
	for (int j = 0; j < d; j++) {
		double_array[j] = doubles[j];
		//std::cerr << "Sending doubles " << j << " value " << doubles[j] << std::endl;
	}
	*/
	/*
	 *   Send message to processor
	 */
	int position = 0;
	MPI_Send(&max_size, 1, MPI_INT, task_number, 0, MPI_COMM_WORLD);
	MPI_Pack(&i, 1, MPI_INT, buffer, max_size, &position, MPI_COMM_WORLD);
	//MPI_Pack(&int_array, i, MPI_INT, buffer, max_size, &position, MPI_COMM_WORLD);
	MPI_Pack(&(ints.front()), i, MPI_INT, buffer, max_size, &position, MPI_COMM_WORLD);
	MPI_Pack(&d, 1, MPI_INT, buffer, max_size, &position, MPI_COMM_WORLD);
	//MPI_Pack(&double_array, d, MPI_DOUBLE, buffer, max_size, &position, MPI_COMM_WORLD);
	if (d > 0) {
		MPI_Pack(&(doubles.front()), d, MPI_DOUBLE, buffer, max_size, &position, MPI_COMM_WORLD);
	}
	std::cerr << "Packed 4" << std::endl;
	MPI_Send(buffer, position, MPI_PACKED, task_number, 0, MPI_COMM_WORLD);

	buffer = (void *) free_check_null(buffer);
}
/* ---------------------------------------------------------------------- */
void cxxStorageBin::mpi_recv(int task_number)
/* ---------------------------------------------------------------------- */
{
	MPI_Status mpi_status;
/*
 *   Malloc space for a buffer
 */
	int max_size;
        // buffer size

	MPI_Recv(&max_size, 1, MPI_INT, task_number, 0, MPI_COMM_WORLD, &mpi_status);
	void *buffer = PHRQ_malloc(max_size);
	if (buffer == NULL) malloc_error();
	/*
	 *   Recieve system
	 */
	MPI_Recv(buffer, max_size, MPI_PACKED, task_number, 0, MPI_COMM_WORLD, &mpi_status);
 	int position = 0;
	int msg_size;
 	MPI_Get_count(&mpi_status, MPI_PACKED, &msg_size);

	/* Unpack ints */
	int count_ints;
 	MPI_Unpack(buffer, msg_size, &position, &count_ints, 1, MPI_INT, MPI_COMM_WORLD);
	int *ints = new int[count_ints];
 	MPI_Unpack(buffer, msg_size, &position, ints, count_ints, MPI_INT, MPI_COMM_WORLD);

	/* Unpack doubles */
	int count_doubles;
 	MPI_Unpack(buffer, msg_size, &position, &count_doubles, 1, MPI_INT, MPI_COMM_WORLD);
	double *doubles = new double[count_doubles];
	if (count_doubles > 0) {
 		MPI_Unpack(buffer, msg_size, &position, doubles, count_doubles, MPI_DOUBLE, MPI_COMM_WORLD);
	}
	buffer = free_check_null(buffer);
#ifdef SKIP
	for (int j = 0; j < count_ints; j++) {
		std::cerr << "Recving ints " << j << " value " << ints[j] << std::endl;
	}
	for (int j = 0; j < count_doubles; j++) {
		std::cerr << "Recving doubles " << j << " value " << doubles[j] << std::endl;
	}
#endif
	/*
	 *   Make list of list of ints and doubles from solution structure
	 *   This list is not the complete structure, but only enough
	 *   for batch-reaction, advection, and transport calculations
	 */
	int i = 0;
	int d = 0;

	// Solution
	if (ints[i++] != 0) {
		cxxSolution entity;
		entity.mpi_unpack(ints, &i, doubles, &d);
		this->setSolution(entity.get_n_user(), &entity);
	}
	//std::cerr << "Unpacked Solution" << std::endl;
	
	// Exchanger
	if (ints[i++] != 0) {
		cxxExchange entity;
		entity.mpi_unpack(ints, &i, doubles, &d);
		this->setExchange(entity.get_n_user(), &entity);
	}
	//std::cerr << "Unpacked Exchange" << std::endl;

	// GasPhase
	if (ints[i++] != 0) {
		cxxGasPhase entity;
		entity.mpi_unpack(ints, &i, doubles, &d);
		this->setGasPhase(entity.get_n_user(), &entity);
	}
	//std::cerr << "Unpacked GasPhase" << std::endl;

	// Kinetics
	if (ints[i++] != 0) {
		cxxKinetics entity;
		entity.mpi_unpack(ints, &i, doubles, &d);
		this->setKinetics(entity.get_n_user(), &entity);
	}
	//std::cerr << "Unpacked Kinetics" << std::endl;

	// PPassemblage
	if (ints[i++] != 0) {
		cxxPPassemblage entity;
		entity.mpi_unpack(ints, &i, doubles, &d);
		this->setPPassemblage(entity.get_n_user(), &entity);
	}
	//std::cerr << "Unpacked PPassemblage" << std::endl;

	// SSassemblage
	if (ints[i++] != 0) {
		cxxSSassemblage entity;
		entity.mpi_unpack(ints, &i, doubles, &d);
		this->setSSassemblage(entity.get_n_user(), &entity);
	}
	//std::cerr << "Unpacked SSassemblage" << std::endl;

	// Surfaces
	if (ints[i++] != 0) {
		cxxSurface entity;
		entity.mpi_unpack(ints, &i, doubles, &d);
		this->setSurface(entity.get_n_user(), &entity);
	}
	//std::cerr << "Unpacked Surface" << std::endl;

	delete[] ints;
	delete[] doubles;
}
#endif

cxxExchange *cxxStorageBin::mix_cxxExchange(cxxMix &mixmap)

{
/*
 *   mixes exchangers based on cxxMix structure, returns new exchanger
 *   return exchanger must be freed by calling method
 */
	cxxExchange *new_exch_ptr, *old_exch_ptr;
/*
 *   Zero out global solution data
 */
	new_exch_ptr = new cxxExchange();

	std::map<int, double>::const_iterator it_mix;
	std::map<int, double> *mixcomps = mixmap.comps();

	// Pitzer_exchange_gammas
	it_mix = mixcomps->begin();
	old_exch_ptr = &((this->Exchangers.find(it_mix->first))->second);
	if (old_exch_ptr == NULL) {
		sprintf(error_string, "Exchange %d not found in mix_cxxExchange.", it_mix->first);
		error_msg(error_string, CONTINUE);
		input_error++;
		return(NULL);
	} 
	new_exch_ptr->set_pitzer_exchange_gammas(old_exch_ptr->get_pitzer_exchange_gammas());
/*
 *   Make list of ExchComps
 */	
	std::vector<cxxExchComp> ec_vector;
	std::vector<double> f_vector;
	//
	// make list of all exchange components and their mix fractions
	//
	for (it_mix = mixcomps->begin(); it_mix != mixcomps->end(); it_mix++) {
		old_exch_ptr = &((this->Exchangers.find(it_mix->first))->second);
		if (old_exch_ptr == NULL) {
			sprintf(error_string, "Exchange %d not found in mix_cxxExchange.", it_mix->first);
			error_msg(error_string, CONTINUE);
			input_error++;
			return(NULL);
		} 
		//  Add exchange components to vector ec_vector
		std::list<cxxExchComp>::const_iterator it_ec;
		std::list<cxxExchComp> &eclist = old_exch_ptr->get_exchComps();
		for (it_ec = eclist.begin(); it_ec != eclist.end(); it_ec++) {
			f_vector.push_back(it_mix->second);
			//cxxExchComp ec = *it_ec;
			//ec_vector.push_back(ec);
			ec_vector.push_back(*it_ec);
		}
	} 
	//
	// Process list to make mixture
	//
	char *current_formula = ec_vector.begin()->get_formula();
	while (current_formula != NULL) {
		
		std::vector<cxxExchComp> ec_subvector;
		std::vector<double> f_subvector;
		std::vector<cxxExchComp>::iterator it_ec = ec_vector.begin();
		std::vector<double>::iterator it_f = f_vector.begin();
		current_formula = NULL;
		for ( ; it_ec != ec_vector.end(); it_ec++) {
			if (*it_f != 0) {
				if (current_formula == NULL) current_formula = it_ec->get_formula();
				if (it_ec->get_formula() == current_formula) {
					ec_subvector.push_back(*it_ec);
					f_subvector.push_back(*it_f);
					*it_f = 0;
					//ec_vector.erase(it_ec);
					//f_vector.erase(it_f);
				}
			}
			it_f++;
		}
		//
		//  mix ec_subvector to make
		// one exchange component
		//
		if (current_formula != NULL) {
			cxxExchComp new_comp(ec_subvector, f_subvector);
			new_exch_ptr->get_exchComps().push_back(new_comp);
		}
	}
	/*
	std::ostringstream oss;
	new_exch_ptr->dump_raw(oss, 0);
	std::cerr << oss.str();
	*/
	return(new_exch_ptr);
}
