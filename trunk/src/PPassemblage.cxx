// PPassemblage.cxx: implementation of the cxxPPassemblage class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <cassert>				// assert
#include <algorithm>			// std::sort

#include "Utils.h"				// define first
#include "Phreeqc.h"
#include "PPassemblage.h"
#include "cxxMix.h"
#include "phqalloc.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxPPassemblage::cxxPPassemblage(PHRQ_io * io)
	//
	// default constructor for cxxPPassemblage 
	//
:	cxxNumKeyword(io)
{
	eltList.type = cxxNameDouble::ND_ELT_MOLES;
}

cxxPPassemblage::cxxPPassemblage(const std::map < int,
								 cxxPPassemblage > &entities, cxxMix & mix,
								 int l_n_user, PHRQ_io * io):
cxxNumKeyword(io)
{
	this->n_user = this->n_user_end = l_n_user;
	eltList.type = cxxNameDouble::ND_ELT_MOLES;
//
//   Mix
//
	const std::map < int, LDBLE >&mixcomps = mix.Get_mixComps();
	std::map < int, LDBLE >::const_iterator it;
	for (it = mixcomps.begin(); it != mixcomps.end(); it++)
	{
		if (entities.find(it->first) != entities.end())
		{
			const cxxPPassemblage *entity_ptr =
				&(entities.find(it->first)->second);
			this->add(*entity_ptr, it->second);
		}
	}
}

cxxPPassemblage::~cxxPPassemblage()
{
}

void
cxxPPassemblage::dump_xml(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing PPassemblage message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// PPassemblage element and attributes
	s_oss << indent0;
	s_oss << "<EQUILIBRIUM_PHASES " << "\n";

	// eltList
	this->eltList.dump_xml(s_oss, indent + 1);

	// ppAssemblageComps
	s_oss << indent1;
	s_oss << "<pure_phases " << "\n";
	for (std::map < std::string, cxxPPassemblageComp >::const_iterator it =
		 pp_assemblage_comps.begin(); it != pp_assemblage_comps.end(); ++it)
	{
		(*it).second.dump_xml(s_oss, indent + 2);
	}
}

void
cxxPPassemblage::dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out) const
{
	//const char    ERR_MESSAGE[] = "Packing PPassemblage message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// PPassemblage element and attributes
	s_oss << indent0;
	int n_user_local = (n_out != NULL) ? *n_out : this->n_user;
	s_oss << "EQUILIBRIUM_PHASES_RAW       " << n_user_local << " " << this->
		description << "\n";

	// eltList

	s_oss << indent1;
	s_oss << "-eltList       " << "\n";
	this->eltList.dump_raw(s_oss, indent + 2);

	// ppAssemblagComps
	for (std::map < std::string, cxxPPassemblageComp >::const_iterator it =
		 pp_assemblage_comps.begin(); it != pp_assemblage_comps.end(); ++it)
	{
		s_oss << indent1;
		s_oss << "-component" << "\n";
		(*it).second.dump_raw(s_oss, indent + 2);
	}
}

void
cxxPPassemblage::read_raw(CParser & parser, bool check)
{
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(15);
		vopts.push_back("eltlist");	// 0
		vopts.push_back("component");	// 1
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);

	// Read PPassemblage number and description
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
			opt = parser.getOptionFromLastLine(vopts, next_char, true);
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
						  PHRQ_io::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), PHRQ_io::OT_CONTINUE);
			useLastLine = false;
			break;

		case 0:				// eltList
			if (this->eltList.read_raw(parser, next_char) !=
				CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.
					error_msg("Expected element name and moles for totals.",
							  PHRQ_io::OT_CONTINUE);
			}
			opt_save = 0;
			break;

		case 1:				// component
			{
				cxxPPassemblageComp ec(this->Get_io());

				// preliminary read
				parser.set_accumulate(true);
				ec.read_raw(parser, false);
				parser.set_accumulate(false);
				std::istringstream is(parser.get_accumulated());
				CParser reread(is, this->Get_io());
				reread.set_echo_file(CParser::EO_NONE);
				reread.set_echo_stream(CParser::EO_NONE);
				if (this->pp_assemblage_comps.find(ec.Get_name()) != this->pp_assemblage_comps.end())
				{
					cxxPPassemblageComp & comp = this->pp_assemblage_comps.find(ec.Get_name())->second;
					comp.read_raw(reread, false);
				}
				else
				{
					cxxPPassemblageComp ppComp1(this->Get_io());
					ppComp1.read_raw(reread, false);
					std::string str(ppComp1.Get_name());
					this->pp_assemblage_comps[str] = ppComp1;
				}
			}
			useLastLine = true;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	// members that must be defined
}

#ifdef USE_MPI
/* ---------------------------------------------------------------------- */
void
cxxPPassemblage::mpi_pack(std::vector < int >&ints,
						  std::vector < LDBLE >&doubles)
/* ---------------------------------------------------------------------- */
{
	/* int n_user; */
	ints.push_back(this->n_user);
	ints.push_back((int) this->ppAssemblageComps.size());
	for (std::map < std::string, cxxPPassemblageComp >::iterator it =
		 this->ppAssemblageComps.begin(); it != this->ppAssemblageComps.end();
		 it++)
	{
		(*it).second.mpi_pack(ints, doubles);
	}
	this->eltList.mpi_pack(ints, doubles);
}

/* ---------------------------------------------------------------------- */
void
cxxPPassemblage::mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd)
/* ---------------------------------------------------------------------- */
{
	int i = *ii;
	int d = *dd;
	/* int n_user; */
	this->n_user = ints[i++];
	this->n_user_end = this->n_user;
	this->description = " ";

	int count = ints[i++];
	this->ppAssemblageComps.clear();
	for (int n = 0; n < count; n++)
	{
		cxxPPassemblageComp ppc;
		ppc.mpi_unpack(ints, &i, doubles, &d);
		std::string str(ppc.get_name());
		this->ppAssemblageComps[str] = ppc;
	}
	this->eltList.mpi_unpack(ints, &i, doubles, &d);
	*ii = i;
	*dd = d;
}
#endif

void
cxxPPassemblage::totalize(PHREEQC_PTR_ARG)
{
	this->totals.clear();
	// component structures
	for (std::map < std::string, cxxPPassemblageComp >::iterator it =
		 pp_assemblage_comps.begin(); it != pp_assemblage_comps.end(); ++it)
	{
		(*it).second.totalize(P_INSTANCE);
		this->totals.add_extensive((*it).second.Get_totals(), 1.0);
	}
	return;
}

void
cxxPPassemblage::add(const cxxPPassemblage & addee, LDBLE extensive)
		//
		// Add to existing ppassemblage to "this" ppassemblage
		//
{
	if (extensive == 0.0)
		return;
	//std::list<cxxPPassemblageComp> ppAssemblageComps;
	for (std::map < std::string, cxxPPassemblageComp >::const_iterator itadd = addee.pp_assemblage_comps.begin();
		 itadd != addee.pp_assemblage_comps.end(); ++itadd)
	{
		bool found = false;
		for (std::map < std::string, cxxPPassemblageComp >::iterator it =
			 this->pp_assemblage_comps.begin();
			 it != this->pp_assemblage_comps.end(); ++it)
		{
			if ((*it).second.Get_name() == itadd->second.Get_name())
			{
				(*it).second.add((*itadd).second, extensive);
				found = true;
				break;
			}
		}
		if (!found)
		{
			cxxPPassemblageComp entity = (*itadd).second;
			entity.multiply(extensive);
			std::string str(entity.Get_name());
			this->pp_assemblage_comps[str] = entity;
		}
	}
	//cxxNameDouble eltList;
	this->eltList.add_extensive(addee.eltList, extensive);
}

cxxPPassemblageComp * cxxPPassemblage::
Find(const std::string name_in)
{
	std::string name(name_in);
	Utilities::str_tolower(name);

	cxxPPassemblageComp * comp = NULL;
	std::map<std::string, cxxPPassemblageComp>::iterator it;
	it = this->pp_assemblage_comps.begin();
	for ( ; it != this->pp_assemblage_comps.end(); it++)
	{
		std::string pname(it->first);
		Utilities::str_tolower(pname);
		if (name == pname)
		{
			comp = &it->second;
			break;
		}
	}
	return comp;
}