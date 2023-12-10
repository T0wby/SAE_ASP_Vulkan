#ifndef VARIABLES_H
#define VARIABLES_H
#include <string>
#include <Vulkan/Include/vulkan/vulkan_core.h>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm/gtx/hash.hpp>
#include <assimp/scene.h>



struct Vertex
{
	struct Position
	{
		float x, y, z;

		static Position Zero(void);
		static Position Forward(void);
		void operator += (Position a_pos);
		bool operator==(Position const& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	} position = Position::Zero();

	struct Color
	{
		float r, g, b;

		static Color Black(void);
		static Color White(void);
		static Color Red(void);
		static Color Green(void);
		static Color Blue(void);
		bool operator==(Color const& rhs) const { return r == rhs.r && g == rhs.g && b == rhs.b; }
	} color = Color::Black();
	
	struct Normal
	{
		float x, y, z;

		static Normal Zero(void);
		static Normal One(void);
		bool operator==(Normal const& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	} normal = Normal::Zero();

	struct UVCoord
	{
		float u, v;
		bool operator==(UVCoord const& rhs) const { return u == rhs.u && v == rhs.v; }
	} uv{ 0.0f,0.0f };

	static VkVertexInputBindingDescription GetBindingDescription() 
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static VkVertexInputAttributeDescription GetAttributeDescriptionPos()
	{
		VkVertexInputAttributeDescription attributeDescription{};


		attributeDescription.binding = 0;
		attributeDescription.location = 0;
		attributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription.offset = offsetof(Vertex, position);

		return attributeDescription;
	}

	static VkVertexInputAttributeDescription GetAttributeDescriptionCol()
	{
		VkVertexInputAttributeDescription attributeDescription{};

		attributeDescription.binding = 0;
		attributeDescription.location = 1;
		attributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription.offset = offsetof(Vertex, color);

		return attributeDescription;
	}

	static VkVertexInputAttributeDescription GetAttributeDescriptionNormal()
	{
		VkVertexInputAttributeDescription attributeDescription{};

		attributeDescription.binding = 0;
		attributeDescription.location = 2;
		attributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription.offset = offsetof(Vertex, normal);

		return attributeDescription;
	}

	static VkVertexInputAttributeDescription GetAttributeDescriptionUV()
	{
		VkVertexInputAttributeDescription attributeDescription{};

		attributeDescription.binding = 0;
		attributeDescription.location = 3;
		attributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescription.offset = offsetof(Vertex, uv);

		return attributeDescription;
	}
	
	inline static const int GetSize(void) { return sizeof(Vertex); }

	bool operator==(const Vertex &other) const {
		return position == other.position && color == other.color && normal == other.normal &&
			   uv == other.uv;
	}
};

struct MeshData
{
	std::vector<Vertex> vertices{};
	std::vector<uint16_t> indices{};

	static auto LoadMesh(const std::string& a_filePath)-> const aiScene*;
	static void ProcessMesh(const aiMesh* a_pMesh, const aiScene* a_pScene, MeshData& a_data);
	static void ProcessNode(const aiNode* a_pNode, const aiScene* a_pScene, MeshData& a_data);
};

#endif // !VARIABLES_H
