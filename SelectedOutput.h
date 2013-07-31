#if !defined(SELECTEDOUTPUT_H_INCLUDED)
#define SELECTEDOUTPUT_H_INCLUDED
#include <string>				// std::string
#include <vector>
#include <map>
#include "NumKeyword.h"
class SelectedOutput:public cxxNumKeyword
{
public:
	SelectedOutput(int n=1, PHRQ_io *io=NULL);
	~SelectedOutput(void);
	void Reset(bool tf);

	std::vector< std::pair< std::string, void * > > & Get_totals(void)           {return this->totals;}
	std::vector< std::pair< std::string, void * > > & Get_molalities(void)       {return this->molalities;}
	std::vector< std::pair< std::string, void * > > & Get_activities(void)       {return this->activities;}
	std::vector< std::pair< std::string, void * > > & Get_pure_phases(void)      {return this->pure_phases;}
	std::vector< std::pair< std::string, void * > > & Get_si(void)               {return this->si;}
	std::vector< std::pair< std::string, void * > > & Get_gases(void)            {return this->gases;}
	std::vector< std::pair< std::string, void * > > & Get_s_s(void)              {return this->s_s;}
	std::vector< std::pair< std::string, void * > > & Get_kinetics(void)         {return this->kinetics;}
	std::vector< std::pair< std::string, void * > > & Get_isotopes(void)         {return this->isotopes;}
	std::vector< std::pair< std::string, void * > > & Get_calculate_values(void) {return this->calculate_values;}

	std::string & Get_file_name(void)                          {return this->file_name;}
	bool Get_new_def(void)                                     {return this->new_def;}
	bool Get_sim(void)                                         {return this->sim;}
	bool Get_state(void)                                       {return this->state;}
	bool Get_soln(void)                                        {return this->soln;}
	bool Get_dist(void)                                        {return this->dist;}
	bool Get_time(void)                                        {return this->time;}
	bool Get_step(void)                                        {return this->step;}
	bool Get_rxn(void)                                         {return this->rxn;}
	bool Get_temp(void)                                        {return this->temp;}
	bool Get_ph(void)                                          {return this->ph;}
	bool Get_pe(void)                                          {return this->pe;}
	bool Get_alk(void)                                         {return this->alk;}
	bool Get_mu(void)                                          {return this->mu;}
	bool Get_water(void)                                       {return this->water;}
	bool Get_high_precision(void)                              {return this->high_precision;}
	bool Get_user_punch(void)                                  {return this->user_punch;}
	bool Get_charge_balance(void)                              {return this->charge_balance;}
	bool Get_percent_error(void)                               {return this->percent_error;}
	bool Get_inverse(void)                                     {return this->inverse;}
	bool Get_punch(void)                                       {return this->punch;}
	bool Get_active(void)                                      {return this->active;}

	void Set_file_name(int i);
	void Set_file_name(std::string s)                          {this->file_name = s;}
	void Set_new_def(bool tf)                                  {this->new_def = tf;}
	void Set_punch_ostream(std::ostream * os)                  {this->punch_ostream = os;}
	void Set_sim(bool tf)                                      {this->sim = tf;}
	void Set_state(bool tf)                                    {this->state = tf;}
	void Set_soln(bool tf)                                     {this->soln = tf;}
	void Set_dist(bool tf)                                     {this->dist = tf;}
	void Set_time(bool tf)                                     {this->time = tf;}
	void Set_step(bool tf)                                     {this->step = tf;}
	void Set_rxn(bool tf)                                      {this->rxn = tf;}
	void Set_temp(bool tf)                                     {this->temp = tf;}
	void Set_ph(bool tf)                                       {this->ph = tf;}
	void Set_pe(bool tf)                                       {this->pe = tf;}
	void Set_alk(bool tf)                                      {this->alk = tf;}
	void Set_mu(bool tf)                                       {this->mu = tf;}
	void Set_water(bool tf)                                    {this->water = tf;}
	void Set_high_precision(bool tf)                           {this->high_precision = tf;}
	void Set_user_punch(bool tf)                               {this->user_punch = tf;}
	void Set_charge_balance(bool tf)                           {this->charge_balance = tf;}
	void Set_percent_error(bool tf)                            {this->percent_error = tf;}
	void Set_inverse(bool tf)                                  {this->inverse = tf;}
	void Set_punch(bool tf)                                    {this->punch = tf;}
	void Set_active(bool tf)                                   {this->active = tf;}

	std::string file_name;
	std::ostream * punch_ostream;
	bool active;
	bool new_def;
	bool user_punch_new_def;
	std::vector< std::pair< std::string, void * > > totals;
	std::vector< std::pair< std::string, void * > > molalities;
	std::vector< std::pair< std::string, void * > > activities;
	std::vector< std::pair< std::string, void * > > pure_phases;
	std::vector< std::pair< std::string, void * > > si;
	std::vector< std::pair< std::string, void * > > gases;
	std::vector< std::pair< std::string, void * > > s_s;
	std::vector< std::pair< std::string, void * > > kinetics;
	std::vector< std::pair< std::string, void * > > isotopes;
	std::vector< std::pair< std::string, void * > > calculate_values;
	bool sim;
	bool state;
	bool soln;
	bool dist;
	bool time;
	bool step;
	bool rxn;
	bool temp;
	bool ph;
	bool pe;
	bool alk;
	bool mu;
	bool water;
	bool high_precision;
	bool user_punch;
	bool charge_balance;
	bool percent_error;
	bool inverse;
	bool punch;
};
#endif // !defined(SELECTEDOUTPUT_H_INCLUDED)