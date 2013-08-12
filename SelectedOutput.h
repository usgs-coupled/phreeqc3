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

	inline std::vector< std::pair< std::string, void * > > & Get_totals(void)           {return this->totals;}
	inline std::vector< std::pair< std::string, void * > > & Get_molalities(void)       {return this->molalities;}
	inline std::vector< std::pair< std::string, void * > > & Get_activities(void)       {return this->activities;}
	inline std::vector< std::pair< std::string, void * > > & Get_pure_phases(void)      {return this->pure_phases;}
	inline std::vector< std::pair< std::string, void * > > & Get_si(void)               {return this->si;}
	inline std::vector< std::pair< std::string, void * > > & Get_gases(void)            {return this->gases;}
	inline std::vector< std::pair< std::string, void * > > & Get_s_s(void)              {return this->s_s;}
	inline std::vector< std::pair< std::string, void * > > & Get_kinetics(void)         {return this->kinetics;}
	inline std::vector< std::pair< std::string, void * > > & Get_isotopes(void)         {return this->isotopes;}
	inline std::vector< std::pair< std::string, void * > > & Get_calculate_values(void) {return this->calculate_values;}

	inline std::string & Get_file_name(void)                          {return this->file_name;}
	inline bool Get_new_def(void)                                     {return this->new_def;}
	inline bool Get_sim(void)                                         {return this->sim;}
	inline bool Get_state(void)                                       {return this->state;}
	inline bool Get_soln(void)                                        {return this->soln;}
	inline bool Get_dist(void)                                        {return this->dist;}
	inline bool Get_time(void)                                        {return this->time;}
	inline bool Get_step(void)                                        {return this->step;}
	inline bool Get_rxn(void)                                         {return this->rxn;}
	inline bool Get_temp(void)                                        {return this->temp;}
	inline bool Get_ph(void)                                          {return this->ph;}
	inline bool Get_pe(void)                                          {return this->pe;}
	inline bool Get_alk(void)                                         {return this->alk;}
	inline bool Get_mu(void)                                          {return this->mu;}
	inline bool Get_water(void)                                       {return this->water;}
	inline bool Get_high_precision(void)                              {return this->high_precision;}
	inline bool Get_user_punch(void)                                  {return this->user_punch;}
	inline bool Get_charge_balance(void)                              {return this->charge_balance;}
	inline bool Get_percent_error(void)                               {return this->percent_error;}
	inline bool Get_inverse(void)                                     {return this->inverse;}
	//inline bool Get_punch(void)                                       {return this->punch;}
	inline bool Get_active(void)                                      {return this->active;}
	inline bool Get_have_punch_name(void)                             {return this->have_punch_name;}
	inline std::ostream* Get_punch_ostream(void)                      {return this->punch_ostream;}

	inline void Set_file_name(int i);
	inline void Set_file_name(std::string s)                          {this->file_name = s;}
	inline void Set_new_def(bool tf)                                  {this->new_def = tf;}
	inline void Set_punch_ostream(std::ostream * os)                  {this->punch_ostream = os;}
	inline void Set_sim(bool tf)                                      {this->sim = tf;}
	inline void Set_state(bool tf)                                    {this->state = tf;}
	inline void Set_soln(bool tf)                                     {this->soln = tf;}
	inline void Set_dist(bool tf)                                     {this->dist = tf;}
	inline void Set_time(bool tf)                                     {this->time = tf;}
	inline void Set_step(bool tf)                                     {this->step = tf;}
	inline void Set_rxn(bool tf)                                      {this->rxn = tf;}
	inline void Set_temp(bool tf)                                     {this->temp = tf;}
	inline void Set_ph(bool tf)                                       {this->ph = tf;}
	inline void Set_pe(bool tf)                                       {this->pe = tf;}
	inline void Set_alk(bool tf)                                      {this->alk = tf;}
	inline void Set_mu(bool tf)                                       {this->mu = tf;}
	inline void Set_water(bool tf)                                    {this->water = tf;}
	inline void Set_high_precision(bool tf)                           {this->high_precision = tf;}
	inline void Set_user_punch(bool tf)                               {this->user_punch = tf;}
	inline void Set_charge_balance(bool tf)                           {this->charge_balance = tf;}
	inline void Set_percent_error(bool tf)                            {this->percent_error = tf;}
	inline void Set_inverse(bool tf)                                  {this->inverse = tf;}
	//inline void Set_punch(bool tf)                                    {this->punch = tf;}
	inline void Set_active(bool tf)                                   {this->active = tf;}
	inline void Set_have_punch_name(bool tf)                          {this->have_punch_name = tf;}

protected:
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
	bool have_punch_name;
};
#endif // !defined(SELECTEDOUTPUT_H_INCLUDED)
