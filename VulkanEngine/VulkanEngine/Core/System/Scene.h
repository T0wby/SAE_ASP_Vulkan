#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include "../../GameObjects/GameObject.h"
#include "../../GameObjects/Primitives/Cube.h"
#include "../../Utility/Variables.h"

class CScene
{
public:

    CScene() = default;
    CScene(const CScene&) = default;
    CScene(CScene&&) = default;
    CScene& operator= (const CScene&) = default;
    CScene& operator= (CScene&&) = default;
    virtual ~CScene() = default;

    void AddGameObject(std::shared_ptr<CGameObject> a_gameObject);
    void RemoveGameObject(std::shared_ptr<CGameObject> a_gameObject);

    std::shared_ptr<CGameObject> GetGameObject(const int& a_iIndex);
    auto GetSceneVertexCount(void) const -> const int;
    auto GetSceneFirstVertex(void) const -> const std::vector<Vertex>;
    auto GetSceneIndicesCount(void) const -> const int;
    auto GetSceneFirstIndice(void) const -> const std::vector<uint16_t>;

    virtual void Initialize(void);
    virtual void Update(void);
    virtual void Draw(void);
    virtual void Finalize(void);

protected:
    void CreateGameObjects(void);
    std::shared_ptr<CCube> m_cube{ nullptr };
    std::vector<std::shared_ptr<CGameObject>> m_vGameObjects{};

};
#endif
