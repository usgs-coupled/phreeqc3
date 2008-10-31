// SAXPhreeqc.cpp
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif

#include <float.h>				// DBL_DIG
#include <stdio.h>				// sprintf
#include <wctype.h>				// iswspace

#include <cassert>				// assert
//#include <strstream>                       // std::ostrstream
#include <sstream>
#include <iostream>				// std::cerr
#ifdef SKIP
#endif
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif


#include <xercesc/framework/StdInInputSource.hpp>
#include <xercesc/framework/MemoryManager.hpp>
#include <xercesc/framework/XMLGrammarPool.hpp>
#include <xercesc/internal/XMLGrammarPoolImpl.hpp>
#include <xercesc/internal/MemoryManagerImpl.hpp>
#include <xercesc/validators/common/Grammar.hpp>

//#include <xercesc/parsers/SAXParser.hpp>           // SAXParser
#include <xercesc/sax/AttributeList.hpp>	// AttributeList
#include <xercesc/util/XMLUniDefs.hpp>	// Unicode definitions
#include <xercesc/framework/MemBufInputSource.hpp>	// MemBufInputSource
#include <xercesc/framework/MemoryManager.hpp>
#include <xercesc/internal/MemoryManagerImpl.hpp>
#include <xercesc/util/PlatformUtils.hpp>	// XMLPlatformUtils::getCurrentMillis
#include <xercesc/util/XMLUni.hpp>

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>

#include "SAXPhreeqc.h"			// SAX_ functions
#include "SaxPhreeqcHandlers.h"	// SaxPhreeqcHandlers

//XERCES_CPP_NAMESPACE_USE
#define xns XERCES_CPP_NAMESPACE

///static char buffer[300000];
///static std::ostrstream s_oss(buffer, 300000);        // must never go out of scope
static
	std::ostringstream
	s_oss;						// must never go out of scope
static bool
	s_bSysIsOpen = false;		// must never go out of scope

#include <math.h>
// extern routines
#include "phqalloc.h"
#include "global.h"
#include "phrqproto.h"
#include "output.h"
#ifdef SKIP
int
conc_init(struct conc *conc_ptr);
void *
free_check_null(void *ptr);
int
pe_data_store(struct pe_data **pe, const char *token);
struct phase *
phase_bsearch(char *ptr, int *j, int print);
struct solution *
solution_alloc(void);
struct solution *
solution_bsearch(int k, int *n, int print);
int
solution_free(struct solution *solution_ptr);
void
space(void **ptr, int i, int *max, int struct_size);
char *
string_duplicate(const char *token);
char *
string_hsave(const char *str);
int
error_msg(const char *err_str, const int stop);
struct master *
master_bsearch(const char *ptr);
void
malloc_error(void);
#endif
//}


class
	Initializer
{
  public:
	Initializer()
	{
#if defined(_DEBUG)
		int
			tmpDbgFlag;

		/*
		 * Set the debug-heap flag to keep freed blocks in the
		 * heap's linked list - This will allow us to catch any
		 * inadvertent use of freed memory
		 */
		tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
		//tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
		tmpDbgFlag |=
			_CRTDBG_LEAK_CHECK_DF;
		//tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
		_CrtSetDbgFlag(tmpDbgFlag);
#endif

		xns::XMLPlatformUtils::Initialize();
	}
};

static Initializer
	sInit;						// initialize xerces once
static SaxPhreeqcHandlers
	s_handler;					// one and only instance

void
SAX_StartSystem()
{

	assert(!s_bSysIsOpen);		// system already open and has not been closed

	// init stream
	//s_oss.freeze(false);
	s_oss.seekp(0);

	// write stream
	// s_oss << "<system>";
	s_oss << "<?xml version=\"1.0\" ?>";
	s_oss << "<phast_state nx=\"2\">";
	s_oss << " <system system_number=\"1\">";
	s_bSysIsOpen = true;

}

char *
stringify_null(char *string)
{
	if (string == NULL)
		return (string_hsave(""));
	return (string);
}

int
SAX_AddSolution(struct solution *solution_ptr)
{
	//const char    ERR_MESSAGE[] = "Packing solution message: %s, element not found\n";
	int
		i,
		newd;
	assert(s_bSysIsOpen);		// must call SAX_StartSystem first
	s_oss.precision(DBL_DIG - 1);
	// Solution element and attributes
	newd = solution_ptr->new_def;
	s_oss << "  <solution " << std::endl;
	s_oss << "   soln_new_def=\"" << solution_ptr->
		new_def << "\"" << std::endl;
	s_oss << "   soln_n_user=\"" << solution_ptr->
		n_user << "\" " << std::endl;
	s_oss << "   soln_n_user_end=\"" << solution_ptr->
		n_user_end << "\"" << std::endl;
	s_oss << "   soln_description=\"" << solution_ptr->
		description << "\"" << std::endl;
	s_oss << "   soln_tc=\"" << solution_ptr->tc << "\"" << std::endl;
	s_oss << "   soln_ph=\"" << solution_ptr->ph << "\"" << std::endl;
	s_oss << "   soln_solution_pe=\"" << solution_ptr->
		solution_pe << "\"" << std::endl;
	s_oss << "   soln_mu=\"" << solution_ptr->mu << "\"" << std::endl;
	s_oss << "   soln_ah2o=\"" << solution_ptr->ah2o << "\"" << std::endl;
	s_oss << "   soln_density=\"" << solution_ptr->
		density << "\"" << std::endl;
	s_oss << "   soln_total_h=\"" << solution_ptr->
		total_h << "\"" << std::endl;
	s_oss << "   soln_total_o=\"" << solution_ptr->
		total_o << "\"" << std::endl;
	s_oss << "   soln_cb=\"" << solution_ptr->cb << "\"" << std::endl;
	s_oss << "   soln_mass_water=\"" << solution_ptr->
		mass_water << "\"" << std::endl;
	s_oss << "   soln_total_alkalinity=\"" << solution_ptr->
		total_alkalinity << "\"" << std::endl;
	//s_oss << "   soln_total_co2=\"" << solution_ptr->total_co2 << "\"" << std::endl;
	s_oss << "   soln_units=\"" << solution_ptr->units << "\"" << std::endl;
	s_oss << "   soln_default_pe=\"" << solution_ptr->
		default_pe << "\"" << std::endl;
	s_oss << "   soln_count_master_activity=\"" << solution_ptr->
		count_master_activity << "\"" << std::endl;
	s_oss << "   soln_count_isotopes=\"" << solution_ptr->
		count_isotopes << "\"" << std::endl;
	s_oss << "   soln_count_species_gamma=\"" << solution_ptr->
		count_species_gamma << "\">" << std::endl;
	// end of solution attributes
	// pe structures
	for (i = 0; solution_ptr->pe[i].name != NULL; i++)
	{
		s_oss << "   <soln_pe soln_pe_name=\"" << solution_ptr->pe[i].
			name << "\"/>" << std::endl;
	}
	// soln_total conc structures
	for (i = 0; solution_ptr->totals[i].description != NULL; i++)
	{
		struct conc *
			c = &(solution_ptr->totals[i]);
		s_oss << "   <soln_total " << std::endl;
		s_oss << "    conc_desc=\"" << c->description << "\"" << std::endl;
		s_oss << "    conc_moles=\"" << c->moles << "\"" << std::endl;
		if (newd == TRUE)
		{
			s_oss << "    conc_input_conc=\"" << c->
				input_conc << "\"" << std::endl;
			if (c->units != NULL)
				s_oss << "        conc_units=\"" << c->
					units << "\"" << std::endl;
			if (c->equation_name != NULL)
			{
				s_oss << "    conc_equation_name=\"" << stringify_null(c->
																	   equation_name)
					<< "\"" << std::endl;
				s_oss << "    conc_phase_si=\"" << c->
					phase_si << "\"" << std::endl;
			}
			if (c->as != NULL)
				s_oss << "        conc_as=\"" << stringify_null(c->
																as) << "\"" <<
					std::endl;
			s_oss << "    conc_gfw=\"" << c->gfw << "\"" << std::endl;
		}
		s_oss << "    conc_n_pe=\"" << c->n_pe << "\"" << std::endl;
		s_oss << "   />" << std::endl;
	}
	// master_activity, master_activity structure

	for (i = 0; i < solution_ptr->count_master_activity; i++)
	{
		s_oss << "   <soln_m_a m_a_desc=\"" << stringify_null(solution_ptr->
															  master_activity
															  [i].
															  description) <<
			"\" m_a_la=\"" << solution_ptr->master_activity[i].
			la << "\"/>" << std::endl;
	}
	/*
	   if (solution_ptr->count_master_activity > 0) {
	   s_oss << "      <soln_m_a m_a_list=\"" << std::endl;
	   for (i=0; i < solution_ptr->count_master_activity; i++) {
	   if (solution_ptr->master_activity[i].description != NULL) {
	   s_oss << stringify_null(solution_ptr->master_activity[i].description) << " ";
	   s_oss << solution_ptr->master_activity[i].la << std::endl;
	   } else {
	   s_oss << "null 0.0" << std::endl;
	   }
	   }
	   s_oss << "\"/>" << std::endl;
	   }
	 */
	// species_gamma, mater_activity structure
	for (i = 0; i < solution_ptr->count_species_gamma; i++)
	{
		s_oss << "   <soln_s_g m_a_desc=\"" << stringify_null(solution_ptr->
															  species_gamma
															  [i].
															  description) <<
			"\" m_a_la=\"" << solution_ptr->species_gamma[i].
			la << "\"/>" << std::endl;
	}
	// isotopes, isotope structure
	for (i = 0; solution_ptr->count_isotopes; i++)
	{
		s_oss << "   <soln_isotope " << std::endl;
		s_oss << "    iso_isotope_number=\"" << solution_ptr->isotopes[i].
			isotope_number << "\"" << std::endl;
		s_oss << "    iso_elt_name=\"" << solution_ptr->isotopes[i].
			elt_name << "\"" << std::endl;
		s_oss << "    iso_isotope_name=\"" << solution_ptr->isotopes[i].
			isotope_name << "\"" << std::endl;
		s_oss << "    iso_total=\"" << solution_ptr->isotopes[i].
			total << "\"" << std::endl;
		s_oss << "    iso_ratio=\"" << solution_ptr->isotopes[i].
			ratio << "\"" << std::endl;
		s_oss << "    iso_ratio_uncertainty=\"" << solution_ptr->isotopes[i].
			ratio_uncertainty << "\"" << std::endl;
		s_oss << "    iso_coef=\"" << solution_ptr->isotopes[i].
			coef << "\"" << std::endl;
	}
	// End of solution
	s_oss << "  </solution>" << std::endl;

	return (OK);
}

void
SAX_EndSystem()
{
	assert(s_bSysIsOpen);		// must call SAX_StartSystem first

	s_oss << " </system>" << std::endl;
	s_oss << "</phast_state>" << std::endl;
	s_oss << '\0';

	s_bSysIsOpen = false;
	//delete[] s_handler;
	return;
}

int
SAX_GetXMLLength()
{
	assert(!s_bSysIsOpen);		// must call SAX_EndSystem first
	//return s_oss.pcount();
	return s_oss.str().size();
}

const char *
SAX_GetXMLStr()
{
	assert(!s_bSysIsOpen);		// must call SAX_EndSystem first
	return s_oss.str().c_str();
}

void
SAX_cleanup()
{
	//delete s_handler;
	//s_oss.freeze(false);
}

// utility routines

int
XMLCh2Int(const XMLCh * const attValue)
{
	char *
		string = xns::XMLString::transcode(attValue);
	int
		i = strtol(string, NULL, 10);
	xns::XMLString::release(&string);
	return i;
	//return (xns::XMLString::parseInt(attValue));
}

double
XMLCh2Double(const XMLCh * const attValue)
{
	char *
		string = xns::XMLString::transcode(attValue);
	double
		d = strtod(string, NULL);
	xns::XMLString::release(&string);
	return d;
}

char *
XMLCh2String(const XMLCh * const attValue)
{
	char *
		string = xns::XMLString::transcode(attValue);
	char *
		s = string_duplicate(string);
	xns::XMLString::release(&string);
	return s;
}

char *
XMLCh_hsave(const XMLCh * const attValue, bool allow_null)
{
	char *
		string = xns::XMLString::transcode(attValue);
	char *
		s = string_hsave(string);
	if (allow_null && strlen(s) == 0)
		s = NULL;
	xns::XMLString::release(&string);
	return s;
}


int
SAX_UnpackSolutions(void *pvBuffer, int buf_size)
{
	//  Create MemBufferInputSource from the buffer containing the XML
	//  statements.
	xns::MemBufInputSource memBufIS((const XMLByte *) pvBuffer, buf_size,
									"solution_id", false);
	fprintf(stderr, "%s", (char *) pvBuffer);
	return 0;
	//
	//  Create a SAX2 parser object.
	//
	xns::SAX2XMLReader * parser = xns::XMLReaderFactory::createXMLReader();
	parser->setFeature(xns::XMLUni::fgXercesSchemaFullChecking, false);
	parser->setFeature(xns::XMLUni::fgSAX2CoreNameSpaces, false);
	parser->setFeature(xns::XMLUni::fgXercesSchema, false);
	parser->setFeature(xns::XMLUni::fgXercesIdentityConstraintChecking,
					   false);
	//
	//  Create the handler object and install it as the document and error
	//  handler for the parser. Then parse the MemBufferInputSource and 
	//  catch any exceptions that propogate out
	//
	try
	{
		unsigned long
			duration = 0;
		parser->setContentHandler(&s_handler);
		parser->setErrorHandler(&s_handler);
		int
			t = 0;
		for (; t < 1000; ++t)
		{
			const unsigned long
				startMillis = xns::XMLPlatformUtils::getCurrentMillis();
			parser->parse(memBufIS);
			const unsigned long
				endMillis = xns::XMLPlatformUtils::getCurrentMillis();
			duration += endMillis - startMillis;
		}
		std::cerr << "\nSaxParse time = " << duration << " millis\n";
	}
	catch(const xns::SAXException & toCatch)
	{
		char *
			psz = xns::XMLString::transcode(toCatch.getMessage());
		input_error++;
		sprintf(error_string, "SAX_UnpackSolutions: %s\n", psz);
		error_msg(error_string, CONTINUE);
		xns::XMLString::release(&psz);
		return ERROR;
	}
	catch(const xns::XMLException & toCatch)
	{
		char *
			psz = xns::XMLString::transcode(toCatch.getMessage());
		input_error++;
		sprintf(error_string, "SAX_UnpackSolutions: %s\n", psz);
		error_msg(error_string, CONTINUE);
		xns::XMLString::release(&psz);
		return ERROR;
	}
	catch(...)
	{
		input_error++;
		sprintf(error_string, "SAX_UnpackSolutions: %s\n",
				"Unknown error occured.");
		error_msg(error_string, CONTINUE);
		return ERROR;
	}

	return (OK);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// element names

SaxPhreeqcHandlers::SaxPhreeqcHandlers():eltType(typeNULL), attType(attNULL), totals(), acts(),
solution_ptr(NULL)
{
	int
		i;

	int
		count_elementInfo,
		count_attributeInfo;
	struct mapElementInfo
	{
		char *
			key;
		enum elementType
			type;
	};
	struct mapAttributeInfo
	{
		enum attributeType
			type;
		char *
			key;
	};
	struct mapElementInfo
		elementInfo[] = {
		{"phast_state", typePHAST_STATE},
		{"system", typeSYSTEM},
		{"solution", typeSOLUTION},
		{"soln_pe", typeSOLN_PE},
		{"soln_total", typeSOLN_TOTAL},
		{"soln_m_a", typeSOLN_MASTER_ACTIVITY},
		{"soln_isotope", typeSOLN_ISOTOPE},
		{"soln_s_g", typeSOLN_SPECIES_GAMMA}
	};
	count_elementInfo = sizeof(elementInfo) / sizeof(struct mapElementInfo);
	struct mapAttributeInfo
		attributeInfo[] = {
		// Solution structure
		{attSOLN_new_def, "soln_new_def"},
		{attSOLN_n_user, "soln_n_user"},
		{attSOLN_n_user_end, "soln_n_user_end"},
		{attSOLN_description, "soln_description"},
		{attSOLN_tc, "soln_tc"},
		{attSOLN_ph, "soln_ph"},
		{attSOLN_solution_pe, "soln_solution_pe"},
		{attSOLN_mu, "soln_mu"},
		{attSOLN_ah2o, "soln_ah2o"},
		{attSOLN_density, "soln_density"},
		{attSOLN_total_h, "soln_total_h"},
		{attSOLN_total_o, "soln_total_o"},
		{attSOLN_cb, "soln_cb"},
		{attSOLN_mass_water, "soln_mass_water"},
		{attSOLN_total_alkalinity, "soln_total_alkalinity"},
		{attSOLN_total_co2, "soln_total_co2"},
		{attSOLN_units, "soln_units"},
		{attSOLN_default_pe, "soln_default_pe"},
		{attSOLN_count_master_activity, "soln_count_master_activity"},
		{attSOLN_count_isotopes, "soln_count_isotopes"},
		{attSOLN_count_species_gamma, "soln_count_species_gamma"},
		{attSOLN_PE_name, "soln_pe_name"},
		// master_activity structure
		{attM_A_description, "m_a_desc"},
		{attM_A_la, "m_a_la"},
		{attM_A_list, "m_a_list"},
		// isotope structure
		{attISO_isotope_number, "iso_isotope_number"},
		{attISO_elt_name, "iso_elt_name"},
		{attISO_isotope_name, "iso_isotope_name"},
		{attISO_total, "iso_total"},
		{attISO_ratio, "iso_ratio"},
		{attISO_ratio_uncertainty, "iso_ratio_uncertainty"},
		{attISO_x_ratio_uncertainty, "iso_x_ratio_uncertainty"},
		{attISO_coef, "iso_coef"},
		// conc structure
		{attCONC_description, "conc_desc"},
		{attCONC_moles, "conc_moles"},
		{attCONC_input_conc, "conc_input_conc"},
		{attCONC_units, "conc_units"},
		{attCONC_equation_name, "conc_equation_name"},
		{attCONC_phase_si, "conc_phase_si"},
		{attCONC_n_pe, "conc_n_pe"},
		{attCONC_as, "conc_as"},
		{attCONC_gfw, "conc_gfw"},

	};

	count_attributeInfo =
		sizeof(attributeInfo) / sizeof(struct mapAttributeInfo);
	for (i = 0; i < count_elementInfo; i++)
	{
		// memory freed in destructor
		this->mapXMLCh2Type[xns::XMLString::transcode(elementInfo[i].key)] =
			elementInfo[i].type;
	}
	for (i = 0; i < count_attributeInfo; i++)
	{
		// memory freed in destructor
		this->
			mapXMLCh2AttType[xns::XMLString::
							 transcode(attributeInfo[i].key)] =
			attributeInfo[i].type;
	}

	this->totals.reserve(50);
	this->acts.reserve(50);
}

SaxPhreeqcHandlers::~SaxPhreeqcHandlers()
{
	std::map < const XMLCh *, elementType, XMLCH_LESS >::iterator it =
		this->mapXMLCh2Type.begin();
	for (; it != this->mapXMLCh2Type.end(); ++it)
	{
		XMLCh *p = (XMLCh *) it->first;
		xns::XMLString::release(&p);
	}
	this->mapXMLCh2Type.clear();
	std::map < const XMLCh *, attributeType, XMLCH_LESS >::iterator ita =
		this->mapXMLCh2AttType.begin();
	for (; ita != this->mapXMLCh2AttType.end(); ++ita)
	{
		XMLCh *p = (XMLCh *) ita->first;
		xns::XMLString::release(&p);
	}
	this->mapXMLCh2AttType.clear();

}

// -----------------------------------------------------------------------
//  Implementations of the SAX DocumentHandler interface
// -----------------------------------------------------------------------

void
SaxPhreeqcHandlers::endDocument()
{
}

void
SaxPhreeqcHandlers::endElement(const XMLCh * const uri,
							   const XMLCh * const name,
							   const XMLCh * const qname)
{
	switch (this->mapXMLCh2Type[name])
	{
	case typeSOLUTION:
		// solution is finished now copy into solutions array
		{
			int n;
			// copy vector of conc's to solution
			this->solution_ptr->totals =
				(struct conc *) PHRQ_realloc(this->solution_ptr->totals,
											 (size_t) (this->totals.size() +
													   1) *
											 sizeof(struct conc));
			std::copy(this->totals.begin(), this->totals.end(),
					  this->solution_ptr->totals);
			this->solution_ptr->totals[this->totals.size()].description =
				NULL;
			this->totals.clear();
			assert(this->totals.size() == 0);

			// copy vector of master_activities's to solution
			this->solution_ptr->master_activity =
				(struct master_activity *) free_check_null(this->
														   solution_ptr->
														   master_activity);
			if (this->acts.size() > 0)
			{
				this->solution_ptr->master_activity =
					(struct master_activity *) PHRQ_realloc(this->
															solution_ptr->
															master_activity,
															(size_t) (this->
																	  acts.
																	  size())
															*
															sizeof(struct
																   master_activity));
				std::copy(this->acts.begin(), this->acts.end(),
						  this->solution_ptr->master_activity);
			}
			this->solution_ptr->count_master_activity = this->acts.size();
			this->acts.clear();
			assert(this->acts.size() == 0);

			// copy vector of s_gamma's to solution
			this->solution_ptr->species_gamma =
				(struct master_activity *) free_check_null(this->
														   solution_ptr->
														   species_gamma);
			if (this->s_gammas.size() > 0)
			{
				this->solution_ptr->species_gamma =
					(struct master_activity *) PHRQ_realloc(this->
															solution_ptr->
															species_gamma,
															(size_t) (this->
																	  s_gammas.
																	  size())
															*
															sizeof(struct
																   master_activity));
				std::copy(this->s_gammas.begin(), this->s_gammas.end(),
						  this->solution_ptr->species_gamma);
			}
			this->solution_ptr->count_species_gamma = this->s_gammas.size();
			this->s_gammas.clear();
			assert(this->s_gammas.size() == 0);

			// copy vector of isotopes's to solution
			this->solution_ptr->isotopes =
				(struct isotope *) free_check_null(this->solution_ptr->
												   isotopes);
			if (this->isotopes.size() > 0)
			{
				this->solution_ptr->isotopes =
					(struct isotope *) PHRQ_realloc(this->solution_ptr->
													isotopes,
													(size_t) (this->isotopes.
															  size()) *
													sizeof(struct isotope));
				std::copy(this->isotopes.begin(), this->isotopes.end(),
						  this->solution_ptr->isotopes);
			}
			this->solution_ptr->count_isotopes = this->isotopes.size();
			this->isotopes.clear();
			assert(this->isotopes.size() == 0);


			// store solution for now
			if (solution_bsearch(this->solution_ptr->n_user, &n, FALSE) !=
				NULL)
			{
				solution_free(solution[n]);
				solution[n] = this->solution_ptr;
			}
			else
			{
				n = count_solution++;
				if (count_solution >= max_solution)
				{
					space((void **) &(solution), count_solution,
						  &max_solution, sizeof(struct solution *));
				}
				solution[n] = this->solution_ptr;
			}
			this->solution_ptr = NULL;
		}
		break;
	default:
		break;
	}
}

void
SaxPhreeqcHandlers::characters(const XMLCh * const chars,
							   const unsigned int length)
{
	// skip whitespace

	//XMLCh* pChar = (XMLCh*)chars;
	//while(pChar && iswspace(*pChar)) ++pChar;
	//if (*pChar)
	// {
/*
    switch(this->eltType)
	{
		case typeSOLN_MASTER_ACTIVITY:
			//xns::BaseRefVectorOf<XMLCh> *arg = xns::XMLString::tokenizeString(chars);
			for ( i = 0; i < arg->size() - 1; i+=2 ) {
				struct master_activity *ma = new master_activity();
				ma->description = XMLCh_hsave( arg->elementAt(i), true);
				ma->la = XMLCh2Double( arg->elementAt(i + 1));
				this->acts.push_back(*ma);
			}
			//struct master_activity *ma = new master_activity();
			//ma->description = NULL;
			//this->acts.push_back(*ma);
			break;
	}
	*/

}

void
SaxPhreeqcHandlers::ignorableWhitespace(const XMLCh * const chars,
										const unsigned int length)
{
}

void
SaxPhreeqcHandlers::processingInstruction(const XMLCh * const target,
										  const XMLCh * const data)
{
}

void
SaxPhreeqcHandlers::startDocument()
{
}

void
SaxPhreeqcHandlers::startElement(const XMLCh * const uri,
								 const XMLCh * const name,
								 const XMLCh * const qname,
								 const xns::Attributes & attributes)
{
	//const char ERR_MSG[] = "Unpacking solution message: %s, element not found\n";
	char *string;

	int i;

	string = xns::XMLString::transcode(name);
	this->eltType = this->mapXMLCh2Type[name];
	xns::XMLString::release(&string);
	switch (this->eltType)
	{
	case typePHAST_STATE:
		XMLCh * x;
		x = xns::XMLString::transcode("nx");
		i = XMLCh2Int(attributes.getValue(x));
		xns::XMLString::release(&x);
		break;
	case typeSOLUTION:
		assert(this->solution_ptr == NULL);
		assert(this->totals.size() == 0);
		assert(this->acts.size() == 0);

		// allocate space for solution
		this->solution_ptr = solution_alloc();

		// process attributes for solution
		processSolutionAttributes(attributes);
		break;
	case typeSOLN_PE:
		assert(this->solution_ptr->pe != NULL);
		// store pe, no need to clean up at end of solution
		if ((attributes.getLength() >= 1)
			&& (this->mapXMLCh2AttType[attributes.getLocalName(0)] ==
				attSOLN_PE_name))
		{
			string =
				xns::XMLString::transcode(attributes.
										  getValue((unsigned int) 0));
			pe_data_store(&(this->solution_ptr->pe), string);
			xns::XMLString::release(&string);
		}
		else
		{
			++input_error;
			sprintf(error_string, "No attribute data for SOLN_PE.\n");
			error_msg(error_string, CONTINUE);
		}
		break;
	case typeSOLN_TOTAL:
		{
			// store in c, push_back on totals
			// need to copy and clean up at end of </solution>
			struct conc c;
			processSolutionTotalAttributes(attributes, &c);
			this->totals.push_back(c);
		}
		break;
	case typeSOLN_MASTER_ACTIVITY:
		{
			// store in ma, push_back on acts
			// need to copy and clean up at end of </solution>
			struct master_activity ma;
			processMasterActivityAttributes(attributes, &ma);
			this->acts.push_back(ma);
			//processMasterActivityAttributes(attributes, &this->acts);
		}
		break;
	case typeSOLN_SPECIES_GAMMA:
		{
			// store in ma, push_back on s_gammas
			// need to copy and clean up at end of </solution>
			struct master_activity ma;
			processMasterActivityAttributes(attributes, &ma);
			this->s_gammas.push_back(ma);
		}
		break;
	case typeSOLN_ISOTOPE:
		{
			// store in iso, push_back on isotopes
			// need to copy and clean up at end of </solution>
			struct isotope iso;
			processIsotopeAttributes(attributes, &iso);
			this->isotopes.push_back(iso);
		}
		break;

	default:
		break;
	}
}
int
SaxPhreeqcHandlers::processSolutionAttributes(const xns::
											  Attributes & attributes)
{
	const char ERR_MSG[] =
		"Unpacking solution attributes: %s, attribute not found\n";
	unsigned int i;
	char *string;
	attributeType attType;
	assert(this->eltType == typeSOLUTION);
	assert(this->solution_ptr != NULL);

	// Get attribute name, map to attribute type, process

	for (i = 0; i < attributes.getLength(); i++)
	{
		attType = this->mapXMLCh2AttType[attributes.getLocalName(i)];
		switch (attType)
		{
		case attSOLN_new_def:
			this->solution_ptr->new_def = XMLCh2Int(attributes.getValue(i));
			break;
		case attSOLN_n_user:
			this->solution_ptr->n_user = XMLCh2Int(attributes.getValue(i));
			break;
		case attSOLN_n_user_end:
			this->solution_ptr->n_user_end =
				XMLCh2Int(attributes.getValue(i));
			break;
		case attSOLN_description:
			this->solution_ptr->description =
				XMLCh2String(attributes.getValue(i));
			break;
		case attSOLN_tc:
			this->solution_ptr->tc = XMLCh2Double(attributes.getValue(i));
			break;
		case attSOLN_ph:
			this->solution_ptr->ph = XMLCh2Double(attributes.getValue(i));
			break;
		case attSOLN_solution_pe:
			this->solution_ptr->solution_pe =
				XMLCh2Double(attributes.getValue(i));
			break;
		case attSOLN_mu:
			this->solution_ptr->mu = XMLCh2Double(attributes.getValue(i));
			break;
		case attSOLN_ah2o:
			this->solution_ptr->ah2o = XMLCh2Double(attributes.getValue(i));
			break;
		case attSOLN_density:
			this->solution_ptr->density =
				XMLCh2Double(attributes.getValue(i));
			break;
		case attSOLN_total_h:
			this->solution_ptr->total_h =
				XMLCh2Double(attributes.getValue(i));
			break;
		case attSOLN_total_o:
			this->solution_ptr->total_o =
				XMLCh2Double(attributes.getValue(i));
			break;
		case attSOLN_cb:
			this->solution_ptr->cb = XMLCh2Double(attributes.getValue(i));
			break;
		case attSOLN_mass_water:
			this->solution_ptr->mass_water =
				XMLCh2Double(attributes.getValue(i));
			break;
		case attSOLN_total_alkalinity:
			this->solution_ptr->total_alkalinity =
				XMLCh2Double(attributes.getValue(i));
			break;
			//case attSOLN_total_co2:
			//this->solution_ptr->total_co2 = XMLCh2Double(attributes.getValue(i));
			//break;
		case attSOLN_units:
			this->solution_ptr->units =
				XMLCh_hsave(attributes.getValue(i), TRUE);
			break;
		case attSOLN_default_pe:
			this->solution_ptr->default_pe =
				XMLCh2Int(attributes.getValue(i));
			break;
		case attSOLN_count_master_activity:
			this->solution_ptr->count_master_activity =
				XMLCh2Int(attributes.getValue(i));
			break;
		case attSOLN_count_isotopes:
			this->solution_ptr->count_isotopes =
				XMLCh2Int(attributes.getValue(i));
			break;
		case attSOLN_count_species_gamma:
			this->solution_ptr->count_species_gamma =
				XMLCh2Int(attributes.getValue(i));
			break;
		default:
			++input_error;
			string = xns::XMLString::transcode(attributes.getLocalName(i));
			sprintf(error_string, ERR_MSG, string);
			error_msg(error_string, CONTINUE);
			xns::XMLString::release(&string);
			break;
		}
	}
	return 0;
}

int
SaxPhreeqcHandlers::processSolutionTotalAttributes(const xns::
												   Attributes & attributes,
												   struct conc *c)
{
	const char ERR_MSG[] =
		"Unpacking solution totals attributes: %s, attribute not found\n";
	unsigned int i;
	char *string;
	conc_init(c);
	attributeType attType;
	assert(this->eltType == typeSOLN_TOTAL);


	// Get attribute name, map to attribute type, process

	for (i = 0; i < attributes.getLength(); i++)
	{
		attType = this->mapXMLCh2AttType[attributes.getLocalName(i)];
		switch (attType)
		{
		case attCONC_description:
			c->description = XMLCh_hsave(attributes.getValue(i), TRUE);
			break;
		case attCONC_moles:
			c->moles = XMLCh2Double(attributes.getValue(i));
			break;
		case attCONC_input_conc:
			c->input_conc = XMLCh2Double(attributes.getValue(i));
			break;
		case attCONC_units:
			c->units = XMLCh_hsave(attributes.getValue(i), TRUE);
			break;
		case attCONC_equation_name:
			c->equation_name = XMLCh_hsave(attributes.getValue(i), TRUE);
			c->phase = NULL;
			break;
		case attCONC_phase_si:
			c->phase_si = XMLCh2Double(attributes.getValue(i));
			break;
		case attCONC_n_pe:
			c->n_pe = XMLCh2Int(attributes.getValue(i));
			break;
		case attCONC_as:
			c->as = XMLCh_hsave(attributes.getValue(i), TRUE);
			break;
		case attCONC_gfw:
			c->gfw = XMLCh2Double(attributes.getValue(i));
			break;

		default:
			++input_error;
			string = xns::XMLString::transcode(attributes.getLocalName(i));
			sprintf(error_string, ERR_MSG, string);
			error_msg(error_string, CONTINUE);
			xns::XMLString::release(&string);
			break;
		}
	}
	return (OK);
}

int
SaxPhreeqcHandlers::processMasterActivityAttributes(const xns::
													Attributes & attributes,
													struct master_activity
													*ma)
//int SaxPhreeqcHandlers::processMasterActivityAttributes(const xns::Attributes& attributes, std::vector<struct master_activity> *v)
{
	int i;
	char *string;
	const char ERR_MSG[] =
		"Unpacking master activity attributes: %s, attribute not found\n";
	ma->description = NULL;
	ma->la = 0.0;
	attributeType attType;
	for (i = 0; i < (int) attributes.getLength(); i++)
	{
		attType = this->mapXMLCh2AttType[attributes.getLocalName(i)];

		switch (attType)
		{

		case attM_A_description:
			ma->description = XMLCh_hsave(attributes.getValue(i), TRUE);
			break;
		case attM_A_la:
			ma->la = XMLCh2Double(attributes.getValue(i));
			break;
			/*
			   case attM_A_list:
			   {
			   struct master_activity ma;
			   xns::BaseRefVectorOf<XMLCh> *arg = xns::XMLString::tokenizeString(attributes.getValue((unsigned int) 0));

			   for ( i = 0; i < arg->size(); i+=2 ) {
			   ma.description = XMLCh_hsave( arg->elementAt(i), TRUE);
			   if (strcmp(ma.description, "null") == 0) {
			   ma.description = NULL;
			   }
			   ma.la = XMLCh2Double( arg->elementAt(i+1));
			   //this->acts.push_back(ma);
			   (*v).push_back(ma);
			   }
			   }
			   break;
			 */
		default:
			++input_error;
			string = xns::XMLString::transcode(attributes.getLocalName(i));
			sprintf(error_string, ERR_MSG, string);
			error_msg(error_string, CONTINUE);
			xns::XMLString::release(&string);
			break;
		}
	}
	return (OK);
}

int
SaxPhreeqcHandlers::processIsotopeAttributes(const xns::
											 Attributes & attributes,
											 struct isotope *iso)
{
	int i;
	char *string;
	const char ERR_MSG[] =
		"Unpacking isotope attributes: %s, attribute not found\n";

	iso->primary = iso->master = NULL;
	iso->elt_name = iso->isotope_name = NULL;
	attributeType attType;
	for (i = 0; i < (int) attributes.getLength(); i++)
	{
		attType = this->mapXMLCh2AttType[attributes.getLocalName(i)];
		switch (attType)
		{
		case attISO_isotope_number:
			iso->isotope_number = XMLCh2Int(attributes.getValue(i));
			break;
		case attISO_elt_name:
			iso->elt_name = XMLCh_hsave(attributes.getValue(i), TRUE);
			break;
		case attISO_isotope_name:
			iso->isotope_name = XMLCh_hsave(attributes.getValue(i), TRUE);
			break;
		case attISO_total:
			iso->total = XMLCh2Double(attributes.getValue(i));
			break;
		case attISO_ratio:
			iso->ratio = XMLCh2Double(attributes.getValue(i));
			break;
		case attISO_ratio_uncertainty:
			iso->ratio_uncertainty = XMLCh2Double(attributes.getValue(i));
			break;
		case attISO_coef:
			iso->coef = XMLCh2Double(attributes.getValue(i));
			break;

		default:
			++input_error;
			string = xns::XMLString::transcode(attributes.getLocalName(i));
			sprintf(error_string, ERR_MSG, string);
			error_msg(error_string, CONTINUE);
			xns::XMLString::release(&string);
			break;
		}
	}
	return (OK);
}
