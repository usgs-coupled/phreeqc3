// Surface.cxx: implementation of the cxxSurface class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)
#endif

#include "Utils.h"   // define first
#include "Surface.h"
#include "SurfComp.h"
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include <cassert>     // assert
#include <algorithm>   // std::sort 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxSurface::cxxSurface()
        //
        // default constructor for cxxSurface 
        //
: cxxNumKeyword()
{
        diffuse_layer           = false;
        edl                     = false;
        only_counter_ions       = false;
        donnan                  = false;
        thickness               = 0.0;
}

cxxSurface::cxxSurface(struct surface *surface_ptr)
        //
        // constructor for cxxSurface from struct surface
        //
: 
cxxNumKeyword()

{
        int i;

        this->set_description(surface_ptr->description);
        n_user                       = surface_ptr->n_user;      
        n_user_end                   = surface_ptr->n_user_end;  
        diffuse_layer                = (surface_ptr->diffuse_layer == TRUE); 
        edl                          = (surface_ptr->edl == TRUE); 
        only_counter_ions            = (surface_ptr->only_counter_ions == TRUE); 
        donnan                       = (surface_ptr->donnan == TRUE); 
        thickness                    = surface_ptr->thickness; 
        // Surface components
        for (i = 0; i < surface_ptr->count_comps; i++) {
                cxxSurfComp ec(&(surface_ptr->comps[i]));
                surfComps.push_back(ec);
        }
        // Surface charge
        for (i = 0; i < surface_ptr->count_charge; i++) {
                cxxSurfCharge ec(&(surface_ptr->charge[i]));
                surfCharges.push_back(ec);
        }
}

cxxSurface::~cxxSurface()
{
}

bool cxxSurface::get_related_phases()
{       
        for (std::list<cxxSurfComp>::const_iterator it = this->surfComps.begin(); it != this->surfComps.end(); ++it) {
                if (it->get_phase_name() == NULL) continue;
                return(true);
        }
        return(false);
}

bool cxxSurface::get_related_rate()
{       
        for (std::list<cxxSurfComp>::const_iterator it = this->surfComps.begin(); it != this->surfComps.end(); ++it) {
                if (it->get_rate_name() == NULL) continue;
                return(true);
        }
        return(false);
}

struct surface *cxxSurface::cxxSurface2surface()
        //
        // Builds a surface structure from instance of cxxSurface 
        //
{
        struct surface *surface_ptr = surface_alloc();
        
        surface_ptr->description                 = this->get_description();  
        surface_ptr->n_user                      = this->n_user;             
        surface_ptr->n_user_end                  = this->n_user_end;         
        surface_ptr->new_def                     = FALSE;  
                surface_ptr->diffuse_layer               = this->diffuse_layer;
        surface_ptr->edl                         = this->edl;
        surface_ptr->only_counter_ions           = this->only_counter_ions;                
        surface_ptr->donnan                      = this->donnan;  
        surface_ptr->thickness                   = this->thickness;             
        surface_ptr->debye_units                 = 1.0;          
        surface_ptr->solution_equilibria         = FALSE;
        surface_ptr->n_solution                  = -2;
        surface_ptr->related_phases              = (int) this->get_related_phases();
        surface_ptr->related_rate                = (int) this->get_related_rate();
        surface_ptr->transport                   = FALSE;

        // Surface comps
        surface_ptr->count_comps = this->surfComps.size();
        surface_ptr->comps = (struct surface_comp *) free_check_null(surface_ptr->comps);
        surface_ptr->comps = cxxSurfComp::cxxSurfComp2surface_comp(this->surfComps);

        // Surface charge
        surface_ptr->count_charge = this->surfCharges.size();
        surface_ptr->charge = (struct surface_charge *) free_check_null(surface_ptr->charge);
        surface_ptr->charge = cxxSurfCharge::cxxSurfCharge2surface_charge(this->surfCharges);

        return(surface_ptr);
}

void cxxSurface::dump_xml(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing surface message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // Surface element and attributes
        s_oss << indent0;
        s_oss << "<surface " << std::endl;

        s_oss << indent1;
        s_oss << "diffuse_layer=\"" << this->diffuse_layer << "\"" << std::endl;

        s_oss << indent1;
        s_oss << "edl=\"" << this->edl << "\"" << std::endl;

        s_oss << indent1;
        s_oss << "only_counter_ions=\"" << this->only_counter_ions << "\"" << std::endl;

        s_oss << indent1;
        s_oss << "donnan=\"" << this->donnan << "\"" << std::endl;

        s_oss << indent1;
        s_oss << "thickness=\"" << this->thickness << "\"" << std::endl;

        // surface component structures
        s_oss << indent1;
        s_oss << "<component " << std::endl;
        {
        for (std::list<cxxSurfComp>::const_iterator it = this->surfComps.begin(); it != this->surfComps.end(); ++it) {
                it->dump_xml(s_oss, indent + 2);
        }
        }
        // surface charge structures
        s_oss << indent1;
        s_oss << "<charge_component " << std::endl;
        for (std::list<cxxSurfCharge>::const_iterator it = surfCharges.begin(); it != surfCharges.end(); ++it) {
                it->dump_xml(s_oss, indent + 2);
        }

        return;
}

void cxxSurface::dump_raw(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing surface message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // Surface element and attributes
        s_oss << indent0;
        s_oss << "SURFACE_RAW       " << this->n_user  << " " << this->description << std::endl;

        s_oss << indent1;
        s_oss << "-diffuse_layer " << this->diffuse_layer << std::endl;

        s_oss << indent1;
        s_oss << "-edl " << this->edl << std::endl;

        s_oss << indent1;
        s_oss << "-only_counter_ions " << this->only_counter_ions << std::endl;

        s_oss << indent1;
        s_oss << "-donnan " << this->donnan << std::endl;

        s_oss << indent1;
        s_oss << "-thickness " << this->thickness << std::endl;

        // surfComps structures
        for (std::list<cxxSurfComp>::const_iterator it = surfComps.begin(); it != surfComps.end(); ++it) {
                s_oss << indent1;
                s_oss << "-component" << std::endl;
                it->dump_raw(s_oss, indent + 2);
        }
        // surface charge structures
                {
        for (std::list<cxxSurfCharge>::const_iterator it = surfCharges.begin(); it != surfCharges.end(); ++it) {
                s_oss << indent1;
                s_oss << "-charge_component " << std::endl;
                it->dump_raw(s_oss, indent + 2);
        }
                }

        return;
}

void cxxSurface::read_raw(CParser& parser)
{
        static std::vector<std::string> vopts;
        if (vopts.empty()) {
                vopts.reserve(15);
                vopts.push_back("diffuse_layer");             // 0 
                vopts.push_back("edl");                       // 1 
                vopts.push_back("only_counter_ions");         // 2 
                vopts.push_back("donnan");                    // 3 
                vopts.push_back("thickness");                 // 4 
                vopts.push_back("component");                 // 5
                vopts.push_back("charge_component");          // 6 
        }                                                     
                                                              
        std::istream::pos_type ptr;                           
        std::istream::pos_type next_char;                     
        std::string token;                                     
        int opt_save;
        bool useLastLine(false);

        // Read surface number and description
        this->read_number_description(parser);

        opt_save = CParser::OPT_ERROR;
        bool diffuse_layer_defined(false); 
        bool edl_defined(false); 
        bool only_counter_ions_defined(false); 
        bool donnan_defined(false); 
        bool thickness_defined(false); 

        for (;;)
        {
                int opt;
                if (useLastLine == false) {
                        opt = parser.get_option(vopts, next_char);
                } else {
                        opt = parser.getOptionFromLastLine(vopts, next_char);
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
                        parser.error_msg("Unknown input in SURF_COMP_RAW keyword.", CParser::OT_CONTINUE);
                        parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
                        useLastLine = false;
                        break;

                case 0: // diffuse_layer
                        if (!(parser.get_iss() >> this->diffuse_layer))
                        {
                                this->diffuse_layer = false;
                                parser.incr_input_error();
                                parser.error_msg("Expected boolean value for diffuse_layer.", CParser::OT_CONTINUE);
                        }
                        diffuse_layer_defined = true;
                        useLastLine = false;
                        break;

                case 1: // edl
                        if (!(parser.get_iss() >> this->edl))
                        {
                                this->edl = false;
                                parser.incr_input_error();
                                parser.error_msg("Expected boolean value for edl.", CParser::OT_CONTINUE);
                        }
                        edl_defined = true;
                        useLastLine = false;
                        break;

                case 2: // only_counter_ions
                        if (!(parser.get_iss() >> this->only_counter_ions))
                        {
                                this->only_counter_ions = false;
                                parser.incr_input_error();
                                parser.error_msg("Expected boolean value for only_counter_ions.", CParser::OT_CONTINUE);
                        }
                        only_counter_ions_defined = true;
                        useLastLine = false;
                        break;

                case 3: // donnan
                        if (!(parser.get_iss() >> this->donnan))
                        {
                                this->donnan = false;
                                parser.incr_input_error();
                                parser.error_msg("Expected boolean value for donnan.", CParser::OT_CONTINUE);
                        }
                        donnan_defined = true;
                        useLastLine = false;
                        break;

                case 4: // thickness
                        if (!(parser.get_iss() >> this->thickness))
                        {
                                this->thickness = 0.0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for thickness.", CParser::OT_CONTINUE);
                        }
                        thickness_defined = true;
                        useLastLine = false;
                        break;

                case 5: // component
                        {
                                cxxSurfComp ec;
                                ec.read_raw(parser);
                                this->surfComps.push_back(ec);
                        }
                        useLastLine = true;
                        break;

                case 6: // charge_component
                        {
                                cxxSurfCharge ec;
                                ec.read_raw(parser);
                                this->surfCharges.push_back(ec);
                        }
                        useLastLine = true;
                        break;
                }

                if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD) break;
        }
        // members that must be defined
        if (diffuse_layer_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Diffuse_layer not defined for SURFACE_RAW input.", CParser::OT_CONTINUE);
        }
        if (edl_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Edl not defined for SURFACE_RAW input.", CParser::OT_CONTINUE);
        }
        if (only_counter_ions_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Only_counter_ions not defined for SURFACE_RAW input.", CParser::OT_CONTINUE);
        }
        if (donnan_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Donnan not defined for SURFACE_RAW input.", CParser::OT_CONTINUE);
        }
        if (thickness_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Thickness not defined for SURFACE_RAW input.", CParser::OT_CONTINUE);
        }
}
