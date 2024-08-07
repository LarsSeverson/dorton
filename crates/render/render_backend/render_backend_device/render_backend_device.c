#include "render_backend_device.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"
#include "./render_backend_physical_device.h"
#include "./render_backend_logical_device.h"

DResult render_backend_create_device(RenderBackend *backend)
{
  if (render_backend_pick_physical_device(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  if (render_backend_create_logical_device(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  return D_SUCCESS;
}

DResult render_backend_destroy_device(RenderBackend *backend)
{
  render_backend_destroy_logical_device(backend);
  return D_SUCCESS;
}
