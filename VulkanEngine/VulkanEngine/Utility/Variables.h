#ifndef VARIABLES_H
#define VARIABLES_H

#include <Vulkan/Include/vulkan/vulkan_core.h>
#include <vector>

struct Vertex
{
	/*struct // Also possible as anonymous
	{
		float x, y, z;
	} position;*/

	struct Position
	{
		float x, y, z;

		static Position Zero(void);
		static Position Forward(void);
		void operator += (Position a_pos);
	} position = Position::Zero();

	struct Color
	{
		float r, g, b;

		static Color Black(void);
		static Color White(void);
		static Color Red(void);
		static Color Green(void);
		static Color Blue(void);

	} color = Color::Black();

	struct UVCoord
	{
		float u, v;

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

	static VkVertexInputAttributeDescription GetAttributeDescriptionUV()
	{
		VkVertexInputAttributeDescription attributeDescription{};

		attributeDescription.binding = 0;
		attributeDescription.location = 2;
		attributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescription.offset = offsetof(Vertex, uv);

		return attributeDescription;
	}



	/*struct Normal
	{
		float x, y, z;

		static Normal Zero(void);
		static Normal One(void);

	} normal = Normal::Zero();
	*/
	inline static const int GetSize(void) { return 5 * sizeof(float); }
	//inline static const int GetElemSize(void) { return 3 * sizeof(float); }
};

#endif // !VARIABLES_H
