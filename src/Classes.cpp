#include "Classes.h"
#include "Phreeqc.h"
class phase* phase_name_wrapper::phase(Phreeqc* php)
{
	int n;
	class phase* p = php->phase_bsearch(this->Get_phase_name(), &n, FALSE);
	if (p == NULL)
	{
		std::ostringstream error_oss;
		error_oss << "Did not find phase " << this->phase_name;
		php->error_msg(error_oss.str().c_str(), false);
	}
	return p;
}