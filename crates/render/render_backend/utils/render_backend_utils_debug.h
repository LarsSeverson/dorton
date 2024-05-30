#include "../../render_types.inl"

#include "../../../dorton_util/darray/darray.h"

DResult get_debug_extensions(DArray *required_extensions, u32 *extensions_count);
DResult get_debug_layers(DArray *required_validation_layers, u32 *required_validation_layers_count);

DResult vk_debug_create(VkInstance *instance, VkAllocationCallbacks *allocator, VkDebugUtilsMessengerEXT *debug_messenger);