#include "render_backend_utils_draw.h"

DResult render_backend_predraw_process_result(RenderBackend *backend, VkResult result)
{
  if (result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    if (render_backend_recreate_swap_chain(backend) != D_SUCCESS)
    {
      return D_FATAL;
    }

    if (render_backend_components_recreate_framebuffers(backend, &backend->components) != D_SUCCESS)
    {
      return D_ERROR;
    }
  }
  else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
  {
    return D_ERROR;
  }

  return D_SUCCESS;
}

DResult render_backend_postdraw_process_result(RenderBackend *backend, VkResult result)
{
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || backend->resized)
  {
    backend->resized = true;

    if (render_backend_recreate_swap_chain(backend) != D_SUCCESS)
    {
      return D_FATAL;
    }

    if (render_backend_components_recreate_framebuffers(backend, &backend->components) != D_SUCCESS)
    {
      return D_ERROR;
    }
  }

  return D_SUCCESS;
}