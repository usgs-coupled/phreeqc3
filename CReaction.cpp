#include "global_structures.h"
#include "CReaction.h"
CReaction::CReaction(void)
{
	for (size_t i = 0; i < MAX_LOG_K_INDICES; i++) this->logk[i] = 0.0;
	for (size_t i = 0; i < 3; i++) this->dz[i] = 0.0;
}
CReaction::CReaction(size_t ntoken)
{
	for (size_t i = 0; i < MAX_LOG_K_INDICES; i++) this->logk[i] = 0.0;
	for (size_t i = 0; i < 3; i++) this->dz[i] = 0.0;
	this->token.resize(ntoken);
}
void  CReaction::Set_logk(double* d)
{
	for (size_t i = 0; i < MAX_LOG_K_INDICES; i++)logk[i] = d[i];
}
void   CReaction::Set_dz(double* d)
{
	for (size_t i = 0; i < 3; i++) dz[i] = d[i];
}
