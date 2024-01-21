#include "DefaultScene.h"
#include <physx/PxRigidStatic.h>
#include <physx/foundation/PxMathUtils.h>

void CDefaultScene::Initialize()
{
	CScene::Initialize();
	InitGameObjects();
}

void CDefaultScene::InitGameObjects()
{
	if(!m_vGameObjects.empty()) return;
	auto cube = CCube::CreateGameObject(m_pDevice);
	m_pCube = std::make_shared<CCube>(std::move(cube));
	m_pCube->Initialize();
	m_vGameObjects.push_back(std::move(m_pCube));

	cube = CCube::CreateGameObject(m_pDevice);
	m_pCube2 = std::make_shared<CCube>(std::move(cube));
	m_pCube2->Initialize();
	m_pCube2->SetPosition(glm::vec3(1.0f, 1.0f,-2.0f));
	m_pCube2->SetRotation(glm::vec3(100.0f, 55.0f,128.0f));
	m_vGameObjects.push_back(std::move(m_pCube2));

	auto floor = CQuad::CreateGameObject(m_pDevice);
	m_pFloor = std::make_shared<CQuad>(std::move(floor));
	m_pFloor->Initialize();
	m_pFloor->SetPosition(glm::vec3(0.0f, -0.5f,0.0f));
	m_pFloor->SetScale(glm::vec3(5.0f, 0.0f,5.0f));
	m_vGameObjects.push_back(std::move(m_pFloor));

	auto light = CGameObject::CreateGameObject(m_pDevice);
	m_pLightObject = std::make_shared<CGameObject>(std::move(light));
	m_pLightObject->SetPosition(glm::vec3(2.0f,3.0f,0.0f));
	m_vGameObjects.push_back(std::move(m_pLightObject));
	
	auto loaded = CLoadedVase::CreateGameObject(m_pDevice);
	m_pVaseLoad = std::make_shared<CLoadedVase>(std::move(loaded));
	m_pVaseLoad->Initialize();
	m_pVaseLoad->SetPosition(glm::vec3(0.0f, 0.5f,0.0f));
	m_pVaseLoad->SetRotation(glm::vec3(110.0f, 0.0f,0.0f));
	m_pVaseLoad->SetScale(glm::vec3(1.7f, 1.7f,1.7f));
	m_vGameObjects.push_back(std::move(m_pVaseLoad));

	const physx::PxShapeFlags shapeFlags = physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eSCENE_QUERY_SHAPE | physx::PxShapeFlag::eSIMULATION_SHAPE;
	physx::PxMaterial* materialPtr = nullptr;
	physx::PxVec3 floorPos = physx::PxVec3(0.0f, -0.5f, 0.0f);

	//plane rigid static
	//physx::PxRigidStatic* rigidStatic = m_physics.createRigidStatic(physx::PxTransformFromPlaneEquation(physx::PxPlane(floorPos, 0.f)));
	//{
	//	physx::PxShape* shape = m_physics.createShape(physx::PxPlaneGeometry(), &materialPtr, 1, true, shapeFlags);
	//	rigidStatic->attachShape(*shape);
	//	shape->release(); // this way shape gets automatically released with actor
	//}

}

void CDefaultScene::Initialize(VkCommandBuffer a_commandBuffer)
{
	CScene::Initialize(a_commandBuffer);
}

void CDefaultScene::Update(const double& a_dDeltaTime)
{
	m_vGameObjects[1]->SetRotation(glm::vec3(1.0f + static_cast<float>(glfwGetTime()),static_cast<float>(glfwGetTime()), 0.0f));
	CScene::Update(a_dDeltaTime);
}

void CDefaultScene::Draw(void)
{
	CScene::Draw();
}

void CDefaultScene::Draw(const DrawInformation& a_drawInformation)
{
	CScene::Draw(a_drawInformation);
}

void CDefaultScene::Finalize(void)
{
	CScene::Finalize();
}

UniformBufferObject& CDefaultScene::CreateUniformBuffer()
{
	UniformBufferObject ubo{};
	ubo.model = glm::mat4(1.0f);
	ubo.view = m_pCamera->GetViewMatrix(m_pCameraObject->GetPosition());
	ubo.proj = m_pCamera->GetProjectionMatrix();
	ubo.proj[1][1] *= -1;
	ubo.lightPosition = m_vGameObjects[3]->GetPosition();
	ubo.lightColor = glm::vec4(1.0f);
	ubo.camPos = m_pCameraObject->GetPosition();

	return ubo;
}

