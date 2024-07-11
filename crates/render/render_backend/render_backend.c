#include "render_backend.h"

#include "logger.h"

#include "./utils/render_backend_utils_debug.h"
#include "./utils/render_backend_utils_draw.h"

DResult render_backend_create_instance(RenderBackend *backend)
{
  VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
  app_info.pApplicationName = backend->app_title;
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "Dorton Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo inst_create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
  inst_create_info.pApplicationInfo = &app_info;

  DArray required_extensions;
  darray_create(&required_extensions, const char *);

  u32 extensions_count = 0;
  const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);

  for (int i = 0; i < extensions_count; ++i)
  {
    darray_push(&required_extensions, glfw_extensions[i]);
  }

#if defined(_DEBUG)
  get_debug_extensions(&required_extensions, &extensions_count);
#endif

  inst_create_info.enabledExtensionCount = extensions_count;
  inst_create_info.ppEnabledExtensionNames = darray_data(&required_extensions);

  DArray required_validation_layers;
  u32 required_validation_layers_count = 0;

#if defined(_DEBUG)
  get_debug_layers(&required_validation_layers, &required_validation_layers_count);
#endif

  inst_create_info.enabledLayerCount = required_validation_layers_count;
  inst_create_info.ppEnabledLayerNames = darray_data(&required_validation_layers);

  VkResult result = vkCreateInstance(&inst_create_info, backend->vulkan_context.allocator, &backend->vulkan_context.instance);

  darray_destroy(&required_extensions);
#if defined(_DEBUG)
  darray_destroy(&required_validation_layers);
#endif

  if (result != D_SUCCESS)
  {
    DFATAL("Unable to create render instance.");
    return D_FATAL;
  }

  return D_SUCCESS;
}

DResult render_backend_create_surface(RenderBackend *backend)
{
  if (glfwCreateWindowSurface(backend->vulkan_context.instance, backend->window->window_inner, backend->vulkan_context.allocator, &backend->vulkan_context.surface) != D_SUCCESS)
  {
    DFATAL("Unable to create render surface.");
    return D_ERROR;
  }

  return D_SUCCESS;
}

DResult render_backend_create(RenderBackend *backend, RenderBackendCreateInfo *create_info)
{
  backend->app_title = create_info->app_title;
  backend->window = create_info->window;

  DINFO("Creating render backend:");
  if (render_backend_create_instance(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend instance created.");

  if (render_backend_create_surface(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend surface created.");

  if (render_backend_create_device(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend device created.");

  if (render_backend_create_swap_chain(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend swap chain created.");

  if (render_backend_create_components(backend, &backend->components) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("Backend components created.");

  if (render_backend_create_fences(backend, &backend->in_flight_fences, backend->swap_chain.max_frames_in_flight) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend fences created.");

  if (render_backend_create_semaphores(backend, &backend->image_available_semaphores, backend->swap_chain.max_frames_in_flight) != D_SUCCESS)
  {
    return D_FATAL;
  }

  if (render_backend_create_semaphores(backend, &backend->render_finished_semaphores, backend->swap_chain.max_frames_in_flight) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend semaphores created.");

  CommandPoolInfo transfer_command_pool_info = {0};
  transfer_command_pool_info.queue_family_index = backend->device.transfer_family;
  transfer_command_pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

  if (render_backend_create_command_pool(backend, &backend->transfer_command_pool, &transfer_command_pool_info) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend transfer command pool created.");

  backend->current_frame = 0;

  DINFO("Render backend created.");

  return D_SUCCESS;
}

DResult render_backend_destroy(RenderBackend *backend)
{
  vkDeviceWaitIdle(backend->device.logical_device);

  if (backend->vulkan_context.debug_messenger)
  {
    PFN_vkDestroyDebugUtilsMessengerEXT vk_debugger_destroy_func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(backend->vulkan_context.instance, "vkDestroyDebugUtilsMessengerEXT");
    vk_debugger_destroy_func(backend->vulkan_context.instance, backend->vulkan_context.debug_messenger, backend->vulkan_context.allocator);
  }

  render_backend_destroy_components(backend, &backend->components);

  // Transfer command pool
  render_backend_destroy_command_pool(backend, &backend->transfer_command_pool);

  // Semaphores
  render_backend_destroy_semaphores(backend, &backend->image_available_semaphores);
  render_backend_destroy_semaphores(backend, &backend->render_finished_semaphores);

  // Fences
  render_backend_destroy_fences(backend, &backend->in_flight_fences);

  // Swap chain
  render_backend_destroy_swap_chain(backend);

  // Device (Physical / Logical)
  render_backend_destroy_device(backend);

  // Surface
  vkDestroySurfaceKHR(backend->vulkan_context.instance, backend->vulkan_context.surface, backend->vulkan_context.allocator);

  // Instance
  vkDestroyInstance(backend->vulkan_context.instance, backend->vulkan_context.allocator);

  return D_SUCCESS;
}

DResult render_backend_draw(RenderBackend *backend, RenderPacket packet)
{
  RenderBackendDrawPacket draw_packet = {0};

  if (render_backend_begin_frame(backend, &draw_packet) != D_SUCCESS)
  {
    DERROR("Error beginning frame.");
    goto render_backend_draw_error;
  }

  if (render_backend_process_frame(backend, &draw_packet) != D_SUCCESS)
  {
    DERROR("Error processing frame.");
    goto render_backend_draw_error;
  }

  if (render_backend_draw_frame(backend, &draw_packet) != D_SUCCESS)
  {
    DERROR("Error drawing frame.");
    goto render_backend_draw_error;
  }

  if (render_backend_end_frame(backend, &draw_packet) != D_SUCCESS)
  {
    DERROR("Error ending frame.");
    goto render_backend_draw_error;
  }

  return D_SUCCESS;

render_backend_draw_error:

  darray_destroy(&draw_packet.draw_command_buffers);

  return D_ERROR;
}

DResult render_backend_begin_frame(RenderBackend *backend, RenderBackendDrawPacket *draw_packet)
{
  if (render_backend_wait_for_fences(backend, &backend->in_flight_fences, backend->current_frame, 1, UINT64_MAX) != D_SUCCESS)
  {
    return D_ERROR;
  }

  u32 image_index = 0;
  RenderBackendSemaphore *image_available_semaphore = semaphores_get(&backend->image_available_semaphores, backend->current_frame);
  VkResult result = vkAcquireNextImageKHR(backend->device.logical_device, backend->swap_chain.swap_chain_inner, UINT64_MAX, image_available_semaphore->semaphore_inner, NULL, &image_index);

  if (render_backend_predraw_process_result(backend, result) != D_SUCCESS)
  {
    return D_ERROR;
  }

  if (render_backend_reset_fences(backend, &backend->in_flight_fences, backend->current_frame, 1) != D_SUCCESS)
  {
    return D_ERROR;
  }

  draw_packet->image_index = image_index;
  draw_packet->current_frame = backend->current_frame;
  darray_create(&draw_packet->draw_command_buffers, VkCommandBuffer);

  return D_SUCCESS;
}

DResult render_backend_process_frame(RenderBackend *backend, RenderBackendDrawPacket *draw_packet)
{
  if (render_backend_process_components(backend, &backend->components, draw_packet) != D_SUCCESS)
  {
    DERROR("Error processing components.");
    return D_ERROR;
  }

  u32 command_buffer_count = (u32)darray_size(&draw_packet->draw_command_buffers);

  if (command_buffer_count == 0)
  {
    DWARN("Command buffer count is 0.");
    return D_IGNORED;
  }

  RenderBackendSemaphore *image_available_semaphore = semaphores_get(&backend->image_available_semaphores, draw_packet->current_frame);
  VkSemaphore wait_semaphores[] = {image_available_semaphore->semaphore_inner};

  VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

  RenderBackendSemaphore *render_finished_semaphore = semaphores_get(&backend->render_finished_semaphores, draw_packet->current_frame);
  VkSemaphore signal_sempahores[] = {render_finished_semaphore->semaphore_inner};

  VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = wait_semaphores;
  submit_info.pWaitDstStageMask = wait_stages;
  submit_info.commandBufferCount = command_buffer_count;
  submit_info.pCommandBuffers = (VkCommandBuffer *)darray_data(&draw_packet->draw_command_buffers);
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = signal_sempahores;

  VkFence in_flight_fence = fences_get(&backend->in_flight_fences, draw_packet->current_frame)->fence_inner;
  if (vkQueueSubmit(backend->device.graphics_queue, 1, &submit_info, in_flight_fence) != VK_SUCCESS)
  {
    DERROR("Error submitting queue.");
    return D_ERROR;
  }

  return D_SUCCESS;
}

DResult render_backend_draw_frame(RenderBackend *backend, RenderBackendDrawPacket *draw_packet)
{
  RenderBackendSemaphore *render_finished_semaphore = semaphores_get(&backend->render_finished_semaphores, draw_packet->current_frame);
  VkSemaphore signal_sempahores[] = {render_finished_semaphore->semaphore_inner};

  VkSwapchainKHR swap_chains[] = {backend->swap_chain.swap_chain_inner};

  VkPresentInfoKHR present_info = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = signal_sempahores;
  present_info.swapchainCount = 1;
  present_info.pSwapchains = swap_chains;
  present_info.pImageIndices = &draw_packet->image_index;

  VkResult result = vkQueuePresentKHR(backend->device.present_queue, &present_info);

  if (render_backend_postdraw_process_result(backend, result) != D_SUCCESS)
  {
    return D_ERROR;
  }

  return D_SUCCESS;
}

DResult render_backend_end_frame(RenderBackend *backend, RenderBackendDrawPacket *draw_packet)
{
  darray_destroy(&draw_packet->draw_command_buffers);

  backend->current_frame = (backend->current_frame + 1) % backend->swap_chain.max_frames_in_flight;

  return D_SUCCESS;
}

DResult render_backend_add_component(RenderBackend *backend, RenderBackendComponentInfo *component_info)
{
  if (render_backend_components_push(backend, &backend->components, component_info) != D_SUCCESS)
  {
    return D_ERROR;
  }

  return D_SUCCESS;
}

DResult render_backend_add_empty_component(RenderBackend *backend)
{
  if (render_backend_components_push_empty(backend, &backend->components) != D_SUCCESS)
  {
    return D_ERROR;
  }

  return D_SUCCESS;
}

DResult render_backend_resize(RenderBackend *backend, i32 width, i32 height)
{
  backend->resized = true;

  return D_SUCCESS;
}
