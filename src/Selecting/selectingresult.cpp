#include "selecting.h"
#include <memory>


bool SelectingResult::selectingDone()
{
	return m_selectingDone;
}
SelectingResult::SelectingResult():m_selectingDone(false)
{
	selected = std::make_shared<Selectable>();
}
SelectingResult::SelectingResult(spSelectable sel):m_selectingDone(true)
{
	selected = sel;
}
spSelectable SelectingResult::getSelected()
{
    return selected;
}
