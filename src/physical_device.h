#pragma once

#include "core.h"

namespace VKCPP
{

struct PhysicalDevice
{
public:
    static PFN_vkGetPhysicalDeviceFeatures                    GetFeatures;
	static PFN_vkGetPhysicalDeviceFormatProperties            GetFormatProperties;
	static PFN_vkGetPhysicalDeviceImageFormatProperties       GetImageFormatProperties;
	static PFN_vkGetPhysicalDeviceMemoryProperties            GetMemoryProperties;
	static PFN_vkGetPhysicalDeviceProperties                  GetProperties;
	static PFN_vkGetPhysicalDeviceQueueFamilyProperties       GetQueueFamilyProperties;
	static PFN_vkGetPhysicalDeviceSparseImageFormatProperties GetSparseImageFormatProperties;

    using Primitive = VkPhysicalDevice;

public:
    VKCPP_OPERATOR_HANDLE()
    PhysicalDevice(Primitive other) :
        handle{ other }
    {

    }

public: /* inline */
    void swap(PhysicalDevice &other)
    {
        std::swap(handle, other.handle);
    }

    void get_features(VkPhysicalDeviceFeatures *pFeatures) const
    {
        GetFeatures(handle, pFeatures);
    }

    void get_format_properties(VkFormat format, VkFormatProperties *pFormatProperties) const
    {
        GetFormatProperties(handle, format, pFormatProperties);
    }

    void get_image_format_properties(VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties *pImageFormatProperties) const
    {
        GetImageFormatProperties(handle, format, type, tiling, usage, flags, pImageFormatProperties);
    }

    void get_memory_properties(VkPhysicalDeviceMemoryProperties *pMemoryProperties) const
    {
        GetMemoryProperties(handle, pMemoryProperties);
    }

    void get_properties(VkPhysicalDeviceProperties *pProperties) const
    {
        GetProperties(handle, pProperties);
    }

    void get_queue_family_properties(uint32_t *pQueueFamilyPropertyCount, VkQueueFamilyProperties *pQueueFamilyProperties) const
    {
        GetQueueFamilyProperties(handle, pQueueFamilyPropertyCount, pQueueFamilyProperties);
    }

    void get_sparse_image_format_properties(VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t *pPropertyCount, VkSparseImageFormatProperties *pProperties) const
    {
        GetSparseImageFormatProperties(handle, format, type, samples, usage, tiling, pPropertyCount, pProperties);
    }

protected:
    Primitive handle = VK_NULL_HANDLE;
};

}
