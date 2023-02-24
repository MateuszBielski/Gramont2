#include "selectable.h"


unsigned int& Selectable::getVAOforSelect()
{
	return vaoselect;
}
int Selectable::getUniqueId()
{
	return uniqueId;
}
void Selectable::setUniqueId(unsigned int id)
{
	uniqueId = id;
}
