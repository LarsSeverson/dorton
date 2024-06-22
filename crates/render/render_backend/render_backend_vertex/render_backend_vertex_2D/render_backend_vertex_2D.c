#include "render_backend_vertex_2D.h"

#include "render/render_backend/render_backend_vertex/render_backend_vertex.h"

#include <stdlib.h>
#include <string.h>

#define VERTEX_2D_ATTRIBUTES_SIZE 2

VkVertexInputBindingDescription get_vertex_2D_binding_description(u32 binding, VertexInputRate input_rate)
{
  VkVertexInputBindingDescription binding_description = {0};
  binding_description.binding = binding;
  binding_description.stride = sizeof(RenderBackendVertex2D);
  binding_description.inputRate = input_rate;

  return binding_description;
}

VkVertexInputAttributeDescription *get_vertex_2D_attribute_descriptions(u32 binding)
{
  VkVertexInputAttributeDescription *attribute_descriptions = (VkVertexInputAttributeDescription *)calloc(VERTEX_2D_ATTRIBUTES_SIZE, sizeof(VkVertexInputAttributeDescription));

  attribute_descriptions[0].binding = binding;
  attribute_descriptions[0].location = 0;
  attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attribute_descriptions[0].offset = offsetof(RenderBackendVertex2D, position);

  attribute_descriptions[1].binding = binding;
  attribute_descriptions[1].location = 1;
  attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attribute_descriptions[1].offset = offsetof(RenderBackendVertex2D, color);

  return attribute_descriptions;
}

RenderBackendVertex2D *render_backend_create_vertex_2D(RenderBackendVertex *vertex, void *vertex_data)
{
  RenderBackendVertex2D *vertex_2D = (RenderBackendVertex2D *)calloc(1, sizeof(RenderBackendVertex2D));
  memcpy(vertex_2D, vertex_data, sizeof(RenderBackendVertex2D));

  vertex->byte_size = sizeof(RenderBackendVertex2D);
  vertex->get_binding_description = get_vertex_2D_binding_description;
  vertex->get_attribute_descriptions = get_vertex_2D_attribute_descriptions;

  return vertex_2D;
}
