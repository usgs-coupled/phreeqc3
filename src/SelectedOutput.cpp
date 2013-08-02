#include "SelectedOutput.h"

SelectedOutput::SelectedOutput(int n, PHRQ_io *io)
:	cxxNumKeyword(io)
{
	this->Set_file_name(n);
	this->punch_ostream = NULL;
	this->active = true;
	this->new_def = false;
	this->user_punch_new_def = false;
	//std::map<std::string, void *> totals;
	//std::map<std::string, void *> molalities;
	//std::map<std::string, void *> activities;
	//std::map<std::string, void *> pure_phases;
	//std::map<std::string, void *> si;
	//std::map<std::string, void *> gases;
	//std::map<std::string, void *> s_s;
	//std::map<std::string, void *> kinetics;
	//std::map<std::string, void *> isotopes;
	//std::map<std::string, void *> calculate_values;
	//std::map<std::string, void *> kinetics;
	//std::map<std::string, void *> kinetics;
	this->inverse          = true;
	this->sim              = true;
	this->state            = true;
	this->soln             = true;
	this->dist             = true;
	this->time             = true;
	this->step             = true;
	this->rxn              = false;
	this->temp             = false;
	this->ph               = true;
	this->pe               = true;
	this->alk              = false;
	this->mu               = false;
	this->water            = false;
	this->high_precision   = false;
	this->user_punch       = true;
	this->charge_balance   = false;
	this->percent_error    = false;

}


SelectedOutput::~SelectedOutput(void)
{
	if (this->punch_ostream != NULL)
	{
		delete this->punch_ostream;
	}
	this->punch_ostream = NULL;
}
void
SelectedOutput::Reset(bool value)
{
	sim = value;
	state = value;
	soln = value;
	dist = value;
	time = value;
	step = value;
	ph = value;
	pe = value;
	rxn = value;
	temp = value;
	alk = value;
	mu = value;
	water = value;
	charge_balance = value;
	percent_error = value;
}
void
SelectedOutput::Set_file_name(int n)
{
	std::ostringstream os;
	os << "selected_output_" << n << ".sel";
	file_name = os.str();
}
