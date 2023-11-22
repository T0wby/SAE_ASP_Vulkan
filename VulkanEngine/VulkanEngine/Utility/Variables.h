#ifndef VARIABLES_H
#define VARIABLES_H

#include <Vulkan/Include/vulkan/vulkan_core.h>

struct Vertex
{
	/*struct // Also possible as anonymous
	{
		float x, y, z;
	} position;*/

	struct Position
	{
		float x, y;

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

	static VkVertexInputBindingDescription GetBindingDescription() 
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(float) * 5;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static VkVertexInputAttributeDescription GetAttributeDescriptionPos()
	{
		VkVertexInputAttributeDescription attributeDescription{};


		attributeDescription.binding = 0;
		attributeDescription.location = 0;
		attributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescription.offset = 0;

		return attributeDescription;
	}

	static VkVertexInputAttributeDescription GetAttributeDescriptionCol()
	{
		VkVertexInputAttributeDescription attributeDescription{};

		attributeDescription.binding = 0;
		attributeDescription.location = 1;
		attributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription.offset = sizeof(float) * 2;

		return attributeDescription;
	}

	/*struct Normal
	{
		float x, y, z;

		static Normal Zero(void);
		static Normal One(void);

	} normal = Normal::Zero();

	struct UVCoord
	{
		float u, v;

	} uv{ 0.0f,0.0f };
	*/
	inline static const int GetSize(void) { return 5 * sizeof(float); }
	//inline static const int GetElemSize(void) { return 3 * sizeof(float); }
};

#endif // !VARIABLES_H
