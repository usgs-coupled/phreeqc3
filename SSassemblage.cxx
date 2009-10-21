// SSassemblage.cxx: implementation of the cxxSSassemblage class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif

#include "Utils.h"				// define first
#include "SSassemblage.h"
#include "SSassemblageSS.h"
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include <cassert>				// assert
#include <algorithm>			// std::sort

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxSSassemblage::cxxSSassemblage()
	//
	// default constructor for cxxSSassemblage 
	//
:	cxxNumKeyword()
{
}

cxxSSassemblage::cxxSSassemblage(struct s_s_assemblage * s_s_assemblage_ptr)
	//
	// constructor for cxxSSassemblage from struct SSassemblage
	//
:
cxxNumKeyword()
{
	int i;
	this->set_description(s_s_assemblage_ptr->description);
	n_user = s_s_assemblage_ptr->n_user;
	n_user_end = s_s_assemblage_ptr->n_user_end;
	for (i = 0; i < s_s_assemblage_ptr->count_s_s; i++)
	{
		cxxSSassemblageSS ssSS(&(s_s_assemblage_ptr->s_s[i]));
		ssAssemblageSSs.push_back(ssSS);
	}
}
cxxSSassemblage::cxxSSassemblage(const std::map < int,
								 cxxSSassemblage > &entities, cxxMix & mix,
								 int n_user):
cxxNumKeyword()
{
	this->n_user = this->n_user_end = n_user;
	//std::list<cxxSSassemblageSS> ssAssemblageSSs;
//
//   Mix
//
	std::map < int, double >*mixcomps = mix.comps();
	std::map < int, double >::const_iterator it;
	for (it = mixcomps->begin(); it != mixcomps->end(); it++)
	{
		if (entities.find(it->first) != entities.end())
		{
			const cxxSSassemblage *entity_ptr =
				&(entities.find(it->first)->second);
			this->add(*entity_ptr, it->second);
		}
	}
}

cxxSSassemblage::~cxxSSassemblage()
{
}

struct s_s_assemblage *
cxxSSassemblage::cxxSSassemblage2s_s_assemblage()
		//
		// Builds a s_s_assemblage structure from instance of cxxSSassemblage 
		//
{
	struct s_s_assemblage *s_s_assemblage_ptr = s_s_assemblage_alloc();

	s_s_assemblage_ptr->description = this->get_description();
	s_s_assemblage_ptr->n_user = this->n_user;
	s_s_assemblage_ptr->n_user_end = this->n_user_end;
	s_s_assemblage_ptr->new_def = FALSE;
	s_s_assemblage_ptr->count_s_s = (int) this->ssAssemblageSSs.size();
	s_s_assemblage_ptr->s_s =
		cxxSSassemblageSS::cxxSSassemblageSS2s_s(this->ssAssemblageSSs);
	return (s_s_assemblage_ptr);
}

#ifdef SKIP
void
cxxSSassemblage::dump_xml(std::ostream & s_oss, unsigned int indent) const const
{
	//const char    ERR_MESSAGE[] = "Packing SSassemblage message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// SSassemblage element and attributes
	s_oss << indent0;
	s_oss << "<EQUILIBRIUM_PHASES " << std::endl;

	// eltList
	this->eltList.dump_xml(s_oss, indent + 1);

	// ssAssemblageSSs
	s_oss << indent1;
	s_oss << "<pure_phases " << std::endl;
	for (std::list < cxxSSassemblageSS >::const_iterator it =
		 ssAssemblageSSs.begin(); it != ssAssemblageSSs.end(); ++it)
	{
		it->dump_xml(s_oss, indent + 2);
	}
}
#endif
void
cxxSSassemblage::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing SSassemblage message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// SSassemblage element and attributes
	s_oss << indent0;
	s_oss << "SOLID_SOLUTIONS_RAW       " << this->n_user << " " << this->
		description << std::endl;

	// ssAssemblageSSs
	for (std::list < cxxSSassemblageSS >::const_iterator it =
		 ssAssemblageSSs.begin(); it != ssAssemblageSSs.end(); ++it)
	{
		s_oss << indent1;
		s_oss << "-solid_solution" << std::endl;
		it->dump_raw(s_oss, indent + 2);
	}
}

void
cxxSSassemblage::read_raw(CParser & parser, bool check)
{
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(10);
		vopts.push_back("solid_solution");	// 0
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);

	// Read SSassemblage number and description
	this->read_number_description(parser);

	opt_save = CParser::OPT_ERROR;

	for (;;)
	{
		int opt;
		if (useLastLine == false)
		{
			opt = parser.get_option(vopts, next_char);
		}
		else
		{
			opt = parser.getOptionFromLastLine(vopts, next_char);
		}
		if (opt == CParser::OPT_DEFAULT)
		{
			opt = opt_save;
		}
		switch (opt)
		{
		case CParser::OPT_EOF:
			break;
		case CParser::OPT_KEYWORD:
			break;
		case CParser::OPT_DEFAULT:
		case CParser::OPT_ERROR:
			opt = CParser::OPT_EOF;
			parser.
				error_msg("Unknown input in EQUILIBRIUM_PHASES_RAW keyword.",
						  CParser::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			useLastLine = false;
			break;

		case 0:				// solid_solution
			{
				cxxSSassemblageSS ssSS;
				ssSS.read_raw(parser);
				this->ssAssemblageSSs.push_back(ssSS);
			}
			useLastLine = true;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
}

#ifdef USE_MPI
/* ---------------------------------------------------------------------- */
void
cxxSSassemblage::mpi_pack(std::vector < int >&ints,
						  std::vector < double >&doubles)
/* ---------------------------------------------------------------------- */
{
	/* int n_user; */
	ints.push_back(this->n_user);
	ints.push_back((int) this->ssAssemblageSSs.size());
	for (std::list < cxxSSassemblageSS >::iterator it =
		 this->ssAssemblageSSs.begin(); it != this->ssAssemblageSSs.end();
		 it++)
	{
		it->mpi_pack(ints, doubles);
	}
}

/* ---------------------------------------------------------------------- */
void
cxxSSassemblage::mpi_unpack(int *ints, int *ii, double *doubles, int *dd)
/* ---------------------------------------------------------------------- */
{
	int i = *ii;
	int d = *dd;
	/* int n_user; */
	this->n_user = ints[i++];
	this->n_user_end = this->n_user;
	this->description = " ";

	int count = ints[i++];
	this->ssAssemblageSSs.clear();
	for (int n = 0; n < count; n++)
	{
		cxxSSassemblageSS ssc;
		ssc.mpi_unpack(ints, &i, doubles, &d);
		this->ssAssemblageSSs.push_back(ssc);
	}
	*ii = i;
	*dd = d;
}
#endif

void
cxxSSassemblage::totalize()
{
	this->totals.clear();
	// component structures
	for (std::list < cxxSSassemblageSS >::iterator it =
		 ssAssemblageSSs.begin(); it != ssAssemblageSSs.end(); ++it)
	{
		it->totalize();
		this->totals.add_extensive(it->get_totals(), 1.0);
	}
	return;
}

void
cxxSSassemblage::add(const cxxSSassemblage & addee, double extensive)
		//
		// Add to existing ssassemblage to "this" ssassemblage
		//
{
	if (extensive == 0.0)
		return;

	for (std::list < cxxSSassemblageSS >::const_iterator itadd =
		 addee.ssAssemblageSSs.begin(); itadd != addee.ssAssemblageSSs.end();
		 ++itadd)
	{
		bool found = false;
		for (std::list < cxxSSassemblageSS >::iterator it =
			 this->ssAssemblageSSs.begin(); it != this->ssAssemblageSSs.end();
			 ++it)
		{
			if (it->get_name() == itadd->get_name())
			{
				it->add((*itadd), extensive);
				found = true;
				break;
			}
		}
		if (!found)
		{
			cxxSSassemblageSS entity = *itadd;
			entity.multiply(extensive);
			this->ssAssemblageSSs.push_back(entity);
		}
	}
}
