#ifndef CUBE_H
#define CUBE_H
#include "../GameObject.h"
#include "../../Components/Mesh.h"
#include "../../Utility/Variables.h"

class CCube : public CGameObject
{
public:
    CCube() = default;
    CCube(const CCube&) = default;
    CCube(CCube&&) = default;
    CCube& operator= (const CCube&) = default;
    CCube& operator= (CCube&&) = default;
    ~CCube() = default;


	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Draw(void);

    //TODO: Add a way to get a certain Component via template in the gameobject?
    virtual std::vector<Vertex>& GetMeshVertexData(void);
    virtual std::vector<uint16_t>& GetMeshIndiceData(void);

private:
    std::shared_ptr<CMesh> m_pMesh{ nullptr };

};
#endif
