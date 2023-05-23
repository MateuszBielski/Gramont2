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
    unsigned int VAO = 0;
    //jeśli będą inne, kolejne vao to można zrobić:
//    vector<unsigned int> vaos;
//    void setWhichVao(param);
    unsigned int& getVao();
    unsigned int * getVaoPtr();
protected:
    ModelData data;
    spTextureForModel m_texture = nullptr;

};

using spOneModel = std::shared_ptr<OneModel>;

#endif // OneModel_H
