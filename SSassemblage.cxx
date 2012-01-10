// SSassemblage.cxx: implementation of the cxxSSassemblage class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <float.h>
#include <cassert>				// assert
#include <algorithm>			// std::sort

#include "Utils.h"				// define first
#include "Phreeqc.h"
#include "SSassemblage.h"
#include "SSassemblageSS.h"
#include "cxxMix.h"
#include "phqalloc.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxSSassemblage::cxxSSassemblage(PHRQ_io * io)
	//
	// default constructor for cxxSSassemblage 
	//
:	cxxNumKeyword(io)
{
}

cxxSSassemblage::cxxSSassemblage(struct ss_assemblage * s_s_assemblage_ptr, PHRQ_io * io)
	//
	// constructor for cxxSSassemblage from struct SSassemblage
	//
:
cxxNumKeyword(io)
{
	int i;
	this->Set_description(s_s_assemblage_ptr->description);
	n_user = s_s_assemblage_ptr->n_user;
	n_user_end = s_s_assemblage_ptr->n_user_end;
	for (i = 0; i < s_s_assemblage_ptr->count_s_s; i++)
	{
		cxxSSassemblageSS ssSS(&(s_s_assemblage_ptr->s_s[i]), this->Get_io());
		std::string str(ssSS.Get_name());
		ssAssemblageSSs[str] = ssSS;
	}
}
cxxSSassemblage::cxxSSassemblage(const std::map < int,
								 cxxSSassemblage > &entities, cxxMix & mix,
								 int l_n_user, PHRQ_io * io):
cxxNumKeyword(io)
{
	this->n_user = this->n_user_end = l_n_user;
	//std::list<cxxSSassemblageSS> ssAssemblageSSs;
//
//   Mix
//
	const std::map < int, double >&mixcomps = mix.Get_mixComps();
	std::map < int, double >::const_iterator it;
	for (it = mixcomps.begin(); it != mixcomps.end(); it++)
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
	s_oss << "<EQUILIBRIUM_PHASES " << "\n";

	// eltList
	this->eltList.dump_xml(s_oss, indent + 1);

	// ssAssemblageSSs
	s_oss << indent1;
	s_oss << "<pure_phases " << "\n";
	for (std::list < cxxSSassemblageSS >::const_iterator it =
		 ssAssemblageSSs.begin(); it != ssAssemblageSSs.end(); ++it)
	{
		it->dump_xml(s_oss, indent + 2);
	}
}
#endif
void
cxxSSassemblage::dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out) const
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
	int n_user_local = (n_out != NULL) ? *n_out : this->n_user;
	s_oss << "SOLID_SOLUTIONS_RAW       " << n_user_local << " " << this->description << "\n";

	// ssAssemblageSSs
	for (std::map < std::string, cxxSSassemblageSS >::const_iterator it =
		 ssAssemblageSSs.begin(); it != ssAssemblageSSs.end(); ++it)
	{
		s_oss << indent1;
		s_oss << "-solid_solution" << "\n";
		(*it).second.dump_raw(s_oss, indent + 2);
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
				error_msg("Unknown input in SOLID_SOLUTIONS_RAW or SOLID_SOLUTIONS_MODIFY keyword.",
						  PHRQ_io::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), PHRQ_io::OT_CONTINUE);
			useLastLine = false;
			break;

		case 0:				// solid_solution
			{
				cxxSSassemblageSS ec(this->Get_io());

				// preliminary read
				parser.set_accumulate(true);
				ec.read_raw(parser, false);
				parser.set_accumulate(false);
				std::istringstream is(parser.get_accumulated());
				CParser reread(is, this->Get_io());
				reread.set_echo_file(CParser::EO_NONE);
				reread.set_echo_stream(CParser::EO_NONE);
				if (this->ssAssemblageSSs.find(ec.Get_name()) != this->ssAssemblageSSs.end())
				{
					cxxSSassemblageSS & ec1 = this->ssAssemblageSSs.find(ec.Get_name())->second;
					ec1.read_raw(reread, false);
				}
				else
				{
					cxxSSassemblageSS ec1(this->Get_io());
					ec1.read_raw(reread, false);
					std::string str(ec1.Get_name());
					this->ssAssemblageSSs[str] = ec1;
				}
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
	for (std::map < std::string, cxxSSassemblageSS >::iterator it =
		 this->ssAssemblageSSs.begin(); it != this->ssAssemblageSSs.end();
		 it++)
	{
		(*it).second.mpi_pack(ints, doubles);
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
		std::string str(ssc.get_name());
		this->ssAssemblageSSs[str] = ssc;
	}
	*ii = i;
	*dd = d;
}
#endif

void
cxxSSassemblage::totalize(PHREEQC_PTR_ARG)
{
	this->totals.clear();
	// component structures
	for (std::map < std::string, cxxSSassemblageSS >::iterator it =
		 ssAssemblageSSs.begin(); it != ssAssemblageSSs.end(); ++it)
	{
		(*it).second.totalize(P_INSTANCE);
		this->totals.add_extensive((*it).second.Get_totals(), 1.0);
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

	for (std::map < std::string, cxxSSassemblageSS >::const_iterator itadd =
		 addee.ssAssemblageSSs.begin(); itadd != addee.ssAssemblageSSs.end();
		 ++itadd)
	{
		std::map < std::string, cxxSSassemblageSS >::iterator it =
			this->ssAssemblageSSs.find((*itadd).first);
		if (it != this->ssAssemblageSSs.end())
		{
			(*it).second.add((*itadd).second, extensive);
		}
		else
		{
			cxxSSassemblageSS entity = (*itadd).second;
			entity.multiply(extensive);
			std::string str(entity.Get_name());
			this->ssAssemblageSSs[str] = entity;
		}
	}
}

