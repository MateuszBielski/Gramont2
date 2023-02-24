#ifndef Selectable_H
#define Selectable_H

#include <iostream>
#include <memory>

class Selectable
{
private:
    unsigned int vaoselect = 0;
    int uniqueId = -1;
public:
    unsigned int& getVAOforSelect();
    int getUniqueId();
    void setUniqueId(unsigned int);
protected:

};
using spSelectable = std::shared_ptr<Selectable>;
using upSelectable = std::unique_ptr<Selectable>;
#endif // Selectable_H
