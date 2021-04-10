#include "Classes.h"
#include "Phreeqc.h"
class phase* inv_phases::phase(Phreeqc* php)
{
	int n;
	class phase* p = php->phase_bsearch(this->name, &n, FALSE);
	if (p == NULL)
	{
		std::ostringstream error_oss;
		error_oss << "Did not find inverse phase " << this->name;
		php->error_msg(error_oss.str().c_str(), false);
	}
	return p;
}