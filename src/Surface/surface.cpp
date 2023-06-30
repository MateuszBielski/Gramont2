#include "surface.h"
#include "iostream"
#include <deque>
#include <vector>

using namespace std;

Surface::Surface(GLuint segmentX,GLuint segmentY,float sizeX,float sizeY):
    m_segmentX(segmentX),m_segmentY(segmentY),m_sizeX(sizeX),m_sizeY(sizeY)
{
    data.nuPoints = (segmentX + 1 ) * ( segmentY + 1);
    //vert coordinates
    GLfloat * m_verts = new GLfloat[3 * data.nuPoints];
    GLfloat * m_TexCoord = new GLfloat[2 * data.nuPoints];
    float distX = sizeX /segmentX;
    float distY = sizeY /segmentY;
    float x0 = -sizeX/2.0f, y0 = -sizeY/2.0f, z0 = 0.0f;
    float xn = x0, yn = y0;
    float tex_xn = 0.0f, tex_yn = 0.0f;

    float tex_distX = 1.0f / segmentX;
    float tex_distY = 1.0f / segmentY;

    for(int ny = 0 ; ny <= segmentY ; ny++) {
        yn = y0 + ny * distY;
        tex_yn = ny * tex_distY;
        for(int nx = 0 ; nx <= segmentX ; nx++) {
            xn = x0 + nx * distX;
            tex_xn = nx * tex_distX;

            int n = 3 * (ny * (segmentX + 1) + nx);
            int tex_n = 2 * (ny * (segmentX + 1) + nx);
            m_verts[n] = xn;
            m_verts[n + 1] = yn;
            m_verts[n + 2] = z0;
            m_TexCoord[tex_n] = tex_xn;
            m_TexCoord[tex_n + 1] = tex_yn;
//            cout<<"\n"<<n/3<<". "<<xn<<" "<<yn;
        }
//        cout<<"\n";
    }
    /***/
//    m_verts[3*(data.nuPoints - 2) + 2] = 50.0f; //one before last - penultimate
    /**/
    //indices
    data.nuIndices = 2 * (segmentX + 1) * segmentY + 2 * (segmentY - 1);//two addictional for each indirect row of points
    //https://www.learnopengles.com/android-lesson-eight-an-introduction-to-index-buffer-objects-ibos/
    GLuint * m_indices = new GLuint[data.nuIndices];

    GLuint n = 0,a,b;
    for(int iy = 0 ; iy < segmentY ; iy++) {
        if(iy > 0)
            m_indices[n++] = (segmentX + 1) * iy;//repeat first from next row
        for(int ix = 0 ; ix <= segmentX ; ix++) {
            int in = ix + (segmentX + 1) * iy;
            a = n++, b = n++;
            m_indices[a] = in;
            m_indices[b] = in + (segmentX + 1);
//            cout<<m_indices[a]<<" "<<m_indices[b]<<" ";
        }
        if(iy < segmentY -1)
            m_indices[n++] = m_indices[b];//repeat last
    }
    //normals
    data.nuNormals = data.nuPoints;
    GLfloat * m_normals = new GLfloat[3 * data.nuNormals];
    for(int n = 0 ; n < data.nuNormals ; n++) {
        m_normals[n * 3 ] = 0.0f;
        m_normals[n * 3 + 1] = 0.0f;
        m_normals[n * 3 + 2] = 1.0f;
    }
    //colours
    data.nuColours = data.nuPoints;//if in shaders abandoned "flat" keyword
    GLfloat * m_colours = new GLfloat[4 * data.nuColours];
    for(int c = 0 ; c < data.nuColours * 4 ; c++)m_colours[c] = 1.0f;
    data.verts = m_verts;
    data.indices = m_indices;
    data.normals = m_normals;
    data.colours = m_colours;

//    cout<<"\nnuIndices: "<<data.nuIndices<<endl;
//    for(short i = 0 ; i < data.nuIndices ; i++)cout<<data.indices[i]<<" ";
//    cout<<endl;

    m_texture = make_shared<TextureForModel>();
    m_texture->texCoord = m_TexCoord;
    m_texture->nuTexCoord = data.nuPoints;
}
Surface::~Surface()
{
    auto Release = [](auto& ptr) {
        if(ptr)delete [] ptr;
        ptr = nullptr;
    };
    Release(data.verts);
    Release(data.normals);
    Release(data.indices);
    Release(data.colours);
    Release(data.tangents);
    Release(data.bitangents);
    Release(m_texture->texCoord);
}

const vector<GLuint> Surface::IndicesAdjacentToPoint(GLuint index)
{
    deque<GLint> indices(4,-1);
    GLuint verticalEdgeCheck = index % (m_segmentX + 1);
    GLuint horizontalEdgeCheck = (index - verticalEdgeCheck) / (m_segmentX + 1);
    bool noTop = true, noLeft = true;
    if(verticalEdgeCheck < m_segmentX)
        indices[0] = index + 1;

    if(horizontalEdgeCheck < m_segmentY) {
        indices[1] = index + 1 + m_segmentX;
        noTop = false;
    }

    if(verticalEdgeCheck > 0) {
        indices[2] = index - 1;
        noLeft = false;
    }

    if(horizontalEdgeCheck > 0) {
        indices[3] = index - 1 - m_segmentX;
    }

    if (noTop || noLeft) {

        unsigned short which = 0;
        auto elem = indices[which];
        while(elem > -1) {
            auto newElem = elem;
            indices.push_back(newElem);
            indices[which] = -1;
            elem = indices[++which];
        }
    }

    vector<GLuint> result;

    for(auto& i : indices) {
        if(i > -1) {
            GLuint elem = i;
            result.push_back(elem);
        }
    }
    return result;
}
GLuint Surface::PointOppositeTo(GLuint index)
{
    if (index >= (m_segmentX + 1) * (m_segmentY + 1))return 0;
    GLuint cpRow, cpCol;
    GLuint resultRow, resultCol;

    cpCol = index % (m_segmentX + 1);
    cpRow = (index - cpCol) / (m_segmentX + 1);
    resultCol = cpCol + 1;
    resultRow = cpRow + 1;
    if(cpCol == m_segmentX) resultCol = cpCol - 1;
    if(cpRow == m_segmentY) resultRow = cpRow - 1;

    GLuint resultPointIndex = resultRow * (m_segmentX + 1) + resultCol;
    return resultPointIndex;
}
vec3 Surface::ResultantNormalOnePoint(const GLuint point,const vector<GLuint>& adjacent, const float * verts)
{
    //definition to erase

    GLushort i = 0,nuNormals = adjacent.size() - 1;
    std::vector<vec3> normals(nuNormals);
    GLuint px0 = point * 3,py0 = point * 3 + 1, pz0 = point * 3 + 2;
    GLuint px1, py1, pz1, px2, py2, pz2;
    vec3 v0(verts[px0],verts[py0],verts[pz0]),normal(0.0f,0.0f,0.0f);
    for(i ; i < nuNormals ; i++) {
        px1 = adjacent[i] * 3;
        py1 = adjacent[i] * 3 + 1;
        pz1 = adjacent[i] * 3 + 2;
        px2 = adjacent[i + 1] * 3;
        py2 = adjacent[i + 1] * 3 + 1;
        pz2 = adjacent[i + 1] * 3 + 2;

//        verts[]
        vec3 v1 = vec3(verts[px1] - verts[px0],verts[py1] - verts[py0],verts[pz1] - verts[pz0]);
        vec3 v2 = vec3(verts[px2] - verts[px0],verts[py2] - verts[py0],verts[pz2] - verts[pz0]);
        vec3 res = glm::cross(v1,v2);
        normals[i] = res;
    }
    for(i = 0 ; i < nuNormals ; i++) {
        normal.x += normals[i].x;
        normal.y += normals[i].y;
        normal.z += normals[i].z;
    }
    normal.x /= nuNormals;
    normal.y /= nuNormals;
    normal.z /= nuNormals;
    int c = 3;
    return glm::normalize(normal);
}
vec3 Surface::ResultantNormalOnePoint(const GLuint point, const vector<GLuint>& adjacent)
{
    auto verts = data.verts;
    GLushort i = 0,nuNormals = adjacent.size() - 1, inext;
    if(adjacent.size() == 4)nuNormals = 4;
    std::vector<vec3> normals(nuNormals);
    GLuint px0 = point * 3,py0 = point * 3 + 1, pz0 = point * 3 + 2;
    GLuint px1, py1, pz1, px2, py2, pz2;
    vec3 v0(verts[px0],verts[py0],verts[pz0]),normal(0.0f,0.0f,0.0f);
    for(i ; i < nuNormals ; i++) {
        px1 = adjacent[i] * 3;
        py1 = adjacent[i] * 3 + 1;
        pz1 = adjacent[i] * 3 + 2;
        inext = (i == 3) ? 0 : i + 1;
        px2 = adjacent[inext] * 3;
        py2 = adjacent[inext] * 3 + 1;
        pz2 = adjacent[inext] * 3 + 2;

        vec3 v1 = vec3(verts[px1] - verts[px0],verts[py1] - verts[py0],verts[pz1] - verts[pz0]);
        vec3 v2 = vec3(verts[px2] - verts[px0],verts[py2] - verts[py0],verts[pz2] - verts[pz0]);
        vec3 res = glm::cross(v1,v2);
        normals[i] = res;
    }
    for(i = 0 ; i < nuNormals ; i++) {
        normal.x += normals[i].x;
        normal.y += normals[i].y;
        normal.z += normals[i].z;
    }
    normal.x /= nuNormals;
    normal.y /= nuNormals;
    normal.z /= nuNormals;
    return glm::normalize(normal);
}

void Surface::CalculateResultantNormalForAllPoints()
{
    if(data.nuNormals != data.nuPoints) return;
    if(!data.normals)return;
    delete [] data.normals;
    GLfloat * temp = new GLfloat[data.nuNormals * 3];
    for(GLuint i = 0 ; i < data.nuPoints ; i++) {
        auto adjacent = IndicesAdjacentToPoint(i);
        auto normal = ResultantNormalOnePoint(i,adjacent);
        temp[i * 3] = normal.x;
        temp[i * 3 + 1] = normal.y;
        temp[i * 3 + 2] = normal.z;
    }
    data.normals = temp;
}
float Surface::AveragedCoordinateAngle(const GLuint point, const vector<GLuint>& adjacent, const float* texCoord)
{
    //metoda daje kąt średni, można użyć innych metod, np.:
    //średnia ważona w zależności od długości ramienia,
    //lub miks obu ze współczynnikiem udziału
    float u, v, _v, thisAngle, thisAngle2, resultantAngle = 0;

    GLuint i = 0, nuAdjacents = adjacent.size(), cu0 = point * 2, cv0 = point * 2 + 1;
    GLuint cu1, cv1, cu2, cv2;

    for(i ; i < nuAdjacents ; i++) {
        cu1 = adjacent[i] * 2;
        cv1 = adjacent[i] * 2 + 1;
        u = texCoord[cu1] - texCoord[cu0];
        v = texCoord[cv1] - texCoord[cv0];
        if(v*v > u*u) {
            _v = v;
            v = - u;
            u = _v;
        }
        thisAngle = atan(v/u);
        resultantAngle += thisAngle;
    }
    resultantAngle /= nuAdjacents;
    return resultantAngle;
}
mat2x3 Surface::RotationOfOriginalTangentAndBitangent(float planeRotationAngle, vec3& normal)
{
    vec3 tangent(1.0, 0.0, 0.0);
    vec3 bitangent(0.0, 1.0, 0.0);
    vec3 zAxis(0.0, 0.0, 1.0);
//    quat planeRotation(planeRotationAngle,zAxis) ;//ten konstruktor nie robi tego co następna linijka
    quat planeRotation(angleAxis(planeRotationAngle,zAxis)) ;//obrót na płaszczyźnie
    quat rotationFromNormal(zAxis,normal);//pierwotnie normal,zAxis 

    quat finalRotation = planeRotation * rotationFromNormal;//pierwotnie planeRotation * rotationFromNormal potem: rotationFromNormal * planeRotation
    tangent = finalRotation * tangent;
    bitangent = finalRotation * bitangent;

    tangent = glm::normalize(tangent);
    bitangent = glm::normalize(bitangent);
    mat2x3 result;
    result[0] = tangent;
    result[1] = bitangent;
    return result;
}

bool Surface::CalculateTangentAndBitangentForAllPointsBasedOn(TextureForModel& tex)
{
    if(tex.nuTexCoord != data.nuPoints) return false;
    data.nuTangents = tex.nuTexCoord;
    data.nuBitangents = tex.nuTexCoord;
    float * tangents = new float[3 * data.nuTangents];
    float * bitangents = new float[3 * data.nuTangents];
    unsigned xn, yn, zn;
    for (unsigned p = 0; p < data.nuTangents; p++) {
        auto& adjacent = IndicesAdjacentToPoint(p);
        float planeRotatnionAngle = AveragedCoordinateAngle(p,adjacent,tex.texCoord);
        xn = p * 3;
        yn = p * 3 + 1;
        zn = p * 3 + 2;
        vec3 normal(data.normals[xn], data.normals[yn], data.normals[zn]);
        mat2x3 tanBitan = RotationOfOriginalTangentAndBitangent(planeRotatnionAngle,normal);
//        mat2x3 tanBitan = ResultantTangentAndBitangentOnePoint(p,adjacent,tex.texCoord);
        tangents[xn] = tanBitan[0].x;
        tangents[yn] = tanBitan[0].y;
        tangents[zn] = tanBitan[0].z;
        bitangents[xn] = tanBitan[1].x;
        bitangents[yn] = tanBitan[1].y;
        bitangents[zn] = tanBitan[1].z;
    }
    if(data.tangents) delete [] data.tangents;
    if(data.bitangents) delete [] data.bitangents;
    data.tangents = tangents;
    data.bitangents = bitangents;
    return true;
}
void Surface::SetZcoordinateForOnePoint(GLuint index, float z_coord)
{
    if(!data.verts)return;
    GLfloat * temp = new GLfloat[data.nuPoints * 3];
    memcpy(temp,data.verts,sizeof(float) * 3 * data.nuPoints);
    temp[index * 3 + 2] = z_coord;
    delete [] data.verts;
    data.verts = temp;
}
