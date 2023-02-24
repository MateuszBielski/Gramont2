#ifndef OneModel_H
#define OneModel_H
#include "modeldata.h"
#include "textureformodel.h"
#include "transformable.h"
#include "selectable.h"


class OneModel : public Transformable, public Selectable
{
private:
public:
    virtual ModelData& GetModelData();
    spTextureForModel MyTexture();
protected:
    ModelData data;
    spTextureForModel m_texture = nullptr;

};

using spOneModel = std::shared_ptr<OneModel>;

#endif // OneModel_H
