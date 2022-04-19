#include "physical_device.h"

namespace VKCPP
{

PFN_vkGetPhysicalDeviceFeatures                    PhysicalDevice::GetFeatures;
PFN_vkGetPhysicalDeviceFormatProperties            PhysicalDevice::GetFormatProperties;
PFN_vkGetPhysicalDeviceImageFormatProperties       PhysicalDevice::GetImageFormatProperties;
PFN_vkGetPhysicalDeviceMemoryProperties            PhysicalDevice::GetMemoryProperties;
PFN_vkGetPhysicalDeviceProperties                  PhysicalDevice::GetProperties;
PFN_vkGetPhysicalDeviceQueueFamilyProperties       PhysicalDevice::GetQueueFamilyProperties;
PFN_vkGetPhysicalDeviceSparseImageFormatProperties PhysicalDevice::GetSparseImageFormatProperties;

PhysicalDevice::PhysicalDevice(Primitive other) :
    handle{ other }
{
    uint32_t count = 0;
    get_queue_family_properties(&count, nullptr);

    queue_family_properties.resize(count);
    get_queue_family_properties(&count, queue_family_properties.data());
}

}
