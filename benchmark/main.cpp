#include <vkcpp.h>
using namespace VKCPP;

int main()
{
    std::vector<const char*> extensions = {
        { VK_EXT_DEBUG_UTILS_EXTENSION_NAME  },
    };

    std::vector<const char*> layers = {
        { "VK_LAYER_KHRONOS_validation" }
    };

    Ref<Instance> instance{ new Instance{ "VKCPP_ENGINE",  extensions, layers } };

    auto physical_device = instance->get_physical_device(0);

    VkPhysicalDeviceFeatures features;
    physical_device->get_features(&features);

    VkFormatProperties format_properties;
    physical_device->get_format_properties(VK_FORMAT_R8G8B8A8_UNORM, &format_properties);

    VkImageFormatProperties image_format_properties;
    physical_device->get_image_format_properties(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT, 0, &image_format_properties);

    VkPhysicalDeviceMemoryProperties memory_properties;
    physical_device->get_memory_properties(&memory_properties);

    VkPhysicalDeviceProperties properties;
    physical_device->get_properties(&properties);

    uint32_t count = 0;
    VkQueueFamilyProperties queue_family_properties[8] = { 0 };
    physical_device->get_queue_family_properties(&count, nullptr);
    physical_device->get_queue_family_properties(&count, queue_family_properties);

    VkSparseImageFormatProperties sparse_image_format_properties[8] = { 0 };
    physical_device->get_sparse_image_format_properties(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TYPE_2D, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_TILING_OPTIMAL, &count, sparse_image_format_properties);

    instance->listen_debug_report_callback();
    instance->listen_debug_utils_messenger();

    return 0;
}
