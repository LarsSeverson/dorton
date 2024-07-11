#include "render_backend_vertex_2D.h"

#include <stdlib.h>
#include <string.h>

#define VERTEX_2D_ATTRIBUTES_SIZE 2

VkVertexInputBindingDescription get_vertex_2D_binding_description(u32 binding, VertexInputRate input_rate)
{
  VkVertexInputBindingDescription binding_description = {0};
  binding_description.binding = binding;
  binding_description.stride = sizeof(RenderBackendVertex2D);
  binding_description.inputRate = (VkVertexInputRate)input_rate;

  return binding_description;
}

DArray get_vertex_2D_attribute_descriptions(u32 binding)
{
  DArray attribute_descriptions;
  darray_reserve(&attribute_descriptions, VkVertexInputAttributeDescription, VERTEX_2D_ATTRIBUTES_SIZE);

  VkVertexInputAttributeDescription *attribute_descriptions_data = (VkVertexInputAttributeDescription *)darray_data(&attribute_descriptions);

  attribute_descriptions_data[0].binding = binding;
  attribute_descriptions_data[0].location = 0;
  attribute_descriptions_data[0].format = VK_FORMAT_R32G32_SFLOAT;
  attribute_descriptions_data[0].offset = offsetof(RenderBackendVertex2D, position);

  attribute_descriptions_data[1].binding = binding;
  attribute_descriptions_data[1].location = 1;
  attribute_descriptions_data[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attribute_descriptions_data[1].offset = offsetof(RenderBackendVertex2D, color);

  return attribute_descriptions;
}
