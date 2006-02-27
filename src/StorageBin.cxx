// StorageBin.cxx: implementation of the cxxStorageBin class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)
#endif

#include "Utils.h"   // define first
#include "StorageBin.h"
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include <cassert>     // assert
#include <algorithm>   // std::sort 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxStorageBin::cxxStorageBin()
        //
        // default constructor for cxxStorageBin 
        //
{
}

cxxStorageBin::~cxxStorageBin()
{
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
        //const char    ERR_MESSAGE[] = "Packing mix message: %s, element not found\n";
        s_oss.precision(DBL_DIG - 1);

	// Solutions
	
	//	for (std::map<int, cxxSolution>::const_iterator it = this->Solutions.begin(); it != this->Solutions.end(); ++it) {
	//it->second.dump_raw(s_oss, indent);
	//}
	// Exchange
	for (std::map<int, cxxExchange>::const_iterator it = this->Exchangers.begin(); it != this->Exchangers.end(); ++it) {
		it->second.dump_raw(s_oss, indent);

	}
	// Gas Phases
	for (std::map<int, cxxGasPhase>::const_iterator it = this->GasPhases.begin(); it != this->GasPhases.end(); ++it) {
		it->second.dump_raw(s_oss, indent);
	}
	// Kinetics
	for (std::map<int, cxxKinetics>::const_iterator it = this->Kinetics.begin(); it != this->Kinetics.end(); ++it) {
		it->second.dump_raw(s_oss, indent);
	}
	// PPassemblage
	for (std::map<int, cxxPPassemblage>::const_iterator it = this->PPassemblages.begin(); it != this->PPassemblages.end(); ++it) {
		it->second.dump_raw(s_oss, indent);
	}
	// SSassemblage
	for (std::map<int, cxxSSassemblage>::const_iterator it = this->SSassemblages.begin(); it != this->SSassemblages.end(); ++it) {
		it->second.dump_raw(s_oss, indent);
	}
	// Surface
	for (std::map<int, cxxSurface>::const_iterator it = this->Surfaces.begin(); it != this->Surfaces.end(); ++it) {
		it->second.dump_raw(s_oss, indent);
	}
}

void cxxStorageBin::read_raw(CParser& parser)
{
	CParser::LINE_TYPE i;
	while ((i = parser.check_line("Subroutine Read", false, true, true, true)) != CParser::LT_KEYWORD)
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

