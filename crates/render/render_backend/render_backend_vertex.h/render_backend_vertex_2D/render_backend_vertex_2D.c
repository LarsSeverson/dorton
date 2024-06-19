#include "./render_backend_vertex_2D.h"

#define VERTEX_2D_ATTRIBUTES_SIZE 2

VertexInputBindingDescription get_vertex_2D_binding_description(u32 binding, VertexInputRate input_rate)
{
  VertexInputBindingDescription binding_description = {0};
  binding_description.binding = binding;
  binding_description.stride = sizeof(RenderBackendVertex2D);
  binding_description.input_rate = input_rate;

  return binding_description;
}

VertexInputAttributeDescription *get_vertex_2D_attribute_descriptions(u32 binding)
{
  VertexInputAttributeDescription attribute_descriptions[VERTEX_2D_ATTRIBUTES_SIZE] = {};

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

RenderBackendVertex2D vertex_2D_create()
{
  RenderBackendVertex2D vertex = {0};
  vertex.get_binding_description = get_vertex_2D_binding_description;
  vertex.get_attribute_descriptions = get_vertex_2D_attribute_descriptions;

  return vertex;
}