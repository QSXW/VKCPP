#pragma once

#include <string>
#include <vector>

#include "core.h"
#include "IObject.h"
#include "physical_device.h"

namespace VKCPP
{

struct Device : public IObject
{
public:
    static PFN_vkCreateDevice                       Create;
    static PFN_vkDestroyDevice                      Destroy;
    static PFN_vkEnumerateDeviceExtensionProperties EnumerateExtensionProperties;
    static PFN_vkEnumerateDeviceLayerProperties     EnumerateLayerProperties;
    static PFN_vkGetDeviceProcAddr                  GetProcAddr;
    static PFN_vkDeviceWaitIdle                     WaitIdle;

    struct CreateInfo : public VkDeviceCreateInfo
    {
        CreateInfo()
        {
            VKCPP_DERIVED_FROM(DEVICE)
        }
    };

    using Description = CreateInfo;

    using Primitive = VkDevice;
    VKCPP_OPERATOR_HANDLE()

public:
    Device(PhysicalDevice *physical_device, CreateInfo *create_info, const VkAllocationCallbacks *pAllocator = nullptr);

    ~Device();

    uint32_t get_queue_family_index(VkQueueFlags request) const;

    void generate_queue_create_info(std::vector<VkDeviceQueueCreateInfo> &queue_create_infos);

public: /* inline */
    VKCPP_DELETE_COPY(Device)

    void swap(Device &other)
    {
        std::swap(handle, other.handle);
    }

    VkResult wait_idle() const
    {
        return WaitIdle(handle);
    }

protected:
    PhysicalDevice *physical_device;
};

}
