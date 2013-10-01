#include "SelectedOutput.h"

SelectedOutput::SelectedOutput(int n, PHRQ_io *io)
:	cxxNumKeyword(io)
{
	// file_name
	this->Set_file_name(n);

	// punch_ostream
	this->punch_ostream = NULL;

	// state vars
	this->active             = true;
	this->new_def            = false;
	this->user_punch_new_def = false;
	this->have_punch_name    = false;

	// as-is vars
	//
	this->user_punch       = true;
	this->high_precision   = false;
	this->inverse          = true;

	this->sim              = true;
	this->state            = true;
	this->soln             = true;
	this->dist             = true;
	this->time             = true;

	this->step             = true;
	this->ph               = true;
	this->pe               = true;
	this->rxn              = false;
	this->temp             = false;

	this->alk              = false;
	this->mu               = false;
	this->water            = false;
	this->charge_balance   = false;
	this->percent_error    = false;

	// as-is set flags
	//
	this->set_user_punch     = false;
	this->set_high_precision = false;
	this->set_inverse        = false;

	this->set_sim            = false;
	this->set_state          = false;
	this->set_soln           = false;
	this->set_dist           = false;
	this->set_time           = false;

	this->set_step           = false;
	this->set_ph             = false;
	this->set_pe             = false;
	this->set_rxn            = false;
	this->set_temp           = false;

	this->set_alk            = false;
	this->set_mu             = false;
	this->set_water          = false;
	this->set_charge_balance = false;
	this->set_percent_error  = false;
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
	// as-is vars
	sim            = value;
	state          = value;
	soln           = value;
	dist           = value;
	time           = value;

	step           = value;
	ph             = value;
	pe             = value;
	rxn            = value;
	temp           = value;

	alk            = value;
	mu             = value;
	water          = value;
	charge_balance = value;
	percent_error  = value;
}

void
SelectedOutput::Set_file_name(int n)
{
	std::ostringstream os;
	os << "selected_output_" << n << ".sel";
	file_name = os.str();
}
