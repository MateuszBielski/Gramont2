#ifndef OneModelMock_H
#define OneModelMock_H
#include "onemodel.h"

class OneModelMock : public OneModel
{
private:
    bool m_getModelDataUsed= false;
public:
    OneModelMock();
    virtual ModelData& GetModelData() override;

    bool GetModelDataUsed();
//    spTextureForModel MyTexture();
protected:
//    ModelData data;
//    spTextureForModel m_texture = nullptr;

};

#endif // OneModel_H
