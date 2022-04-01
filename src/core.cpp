#include "core.h"
#include "instance.h"

#ifdef _WIN32
#include "Windows.h"
#else
#include <dlfcn.h>
#endif

namespace VKCPP
{

VkResult InitVKContext()
{
#ifdef _WIN32
    HMODULE module = LoadLibraryA("vulkan-1.dll");
    if (!module)
    {
      	return VK_ERROR_INITIALIZATION_FAILED;
    }

    Instance::GetProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(module, "vkGetInstanceProcAddr");
#else
  	void *module = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
	if (!module)
    {
        module = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
    }
	if (!module)
    {
      	return VK_ERROR_INITIALIZATION_FAILED;
    }

	Instance::GetProcAddr = (PFN_vkGetInstanceProcAddr)dlsym(module, "vkGetInstanceProcAddr");
#endif

	return VK_SUCCESS;
}

}
