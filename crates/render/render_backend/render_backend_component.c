#include "render_backend_component.h"

#include "logger.h"

#include "./render_backend.h"

DResult render_backend_create_component(RenderBackend *backend, RenderBackendComponent *component, RenderBackendComponentInfo *component_info)
{
  DINFO("Creating backend component:");

  *component = (RenderBackendComponent){0};

  if (component_info->render_pass_info == NULL || component_info->framebuffer_info == NULL || component_info->command_buffers_info == NULL || component_info->pipeline_info == NULL)
  {
    DERROR("\tA component info was NULL.");
    return D_ERROR;
  }

  if (render_backend_create_render_pass(backend, &component->render_pass, &component_info->render_pass_info) != D_SUCCESS)
  {
    DERROR("\tCould not create component.");
    return D_ERROR;
  }

  DINFO("\tRender pass created.");

  if (render_backend_create_framebuffers(backend, &component->framebuffers, &component_info->framebuffer_info) != D_SUCCESS)
  {
    DERROR("\tCould not create component.");
    return D_ERROR;
  }

  DINFO("Framebuffers created.");

  if (render_backend_create_command_buffers(backend, &component->command_buffers, &component_info->command_buffers_info) != D_SUCCESS)
  {
    DERROR("\tCould not create component.");
    return D_ERROR;
  }

  DINFO("Command buffers created.");

  if (render_backend_create_pipeline(backend, &component->pipeline, &component_info->pipeline_info) != D_SUCCESS)
  {
    DERROR("\tCould not create component.");
    return D_ERROR;
  }

  DINFO("Pipeline created.");

  return D_SUCCESS;
}

DResult render_backend_destroy_component(RenderBackend *backend, RenderBackendComponent *component)
{

  render_backend_destroy_pipeline(backend, &component->pipeline);
  render_backend_destroy_command_buffers(backend, &component->command_buffers);
  render_backend_destroy_framebuffers(backend, &component->framebuffers);
  render_backend_destroy_render_pass(backend, &component->render_pass);

  return D_SUCCESS;
}
