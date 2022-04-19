#include "device.h"

namespace VKCPP
{

PFN_vkCreateDevice                       Device::Create;
PFN_vkDestroyDevice                      Device::Destroy;
PFN_vkEnumerateDeviceExtensionProperties Device::EnumerateExtensionProperties;
PFN_vkEnumerateDeviceLayerProperties     Device::EnumerateLayerProperties;
PFN_vkGetDeviceProcAddr                  Device::GetProcAddr;
PFN_vkDeviceWaitIdle                     Device::WaitIdle;

Device::Device(PhysicalDevice *physical_device, CreateInfo *create_info, const VkAllocationCallbacks *pAllocator) :
    handle{ VK_NULL_HANDLE },
    physical_device{ physical_device }
{
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    auto &queue_family_properties = physical_device->get_queue_family_properties();
    uint32_t props_count = (uint32_t)queue_family_properties.size();

    queue_create_infos.resize(props_count, { VKCPP_STYPE(DEVICE_QUEUE) });
    std::vector<std::vector<float>> queueProps(props_count);

    for (uint32_t index = 0; index < props_count; index++)
    {
        const VkQueueFamilyProperties &prop = queue_family_properties[index];
        queueProps[index].resize(prop.queueCount, 0.5f);
        if (get_queue_family_index(VK_QUEUE_GRAPHICS_BIT) == index)
        {
            queueProps[index][0] = 1.0f;
        }

        queue_create_infos[index].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_infos[index].pNext            = nullptr;
        queue_create_infos[index].queueFamilyIndex = index;
        queue_create_infos[index].queueCount       = prop.queueCount;
        queue_create_infos[index].pQueuePriorities = queueProps[index].data();
    }

    create_info->queueCreateInfoCount = (uint32_t)queue_create_infos.size();
    create_info->pQueueCreateInfos    = queue_create_infos.data();

    VKCPP_ASSERT(Create(*physical_device, create_info, pAllocator, &handle));
}

Device::~Device()
{
    VKCPP_ASSERT(wait_idle());
    if (handle != VK_NULL_HANDLE)
    {
        Destroy(handle, nullptr);
    }
}

uint32_t Device::get_queue_family_index(VkQueueFlags request) const
{
    VkQueueFlags discard = 0;
    const auto &queueFamilyProperties = physical_device->get_queue_family_properties();

    switch (request)
    {
    case VK_QUEUE_COMPUTE_BIT:
        discard |= VK_QUEUE_GRAPHICS_BIT;
        break;

    case VK_QUEUE_TRANSFER_BIT:
        discard |= (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);

    default:
        break;
    }

    for (size_t i = 0; i < queueFamilyProperties.size(); i++)
    {
        auto queueFlags = queueFamilyProperties[i].queueFlags;
        if (queueFlags & request && !(queueFlags & discard))
        {
            return (uint32_t)i;
            break;
        }
    }

    return 0;
}

}
