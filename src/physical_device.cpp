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

}
