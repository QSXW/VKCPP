#include "instance.h"
#include "device.h"
#include "physical_device.h"

namespace VKCPP
{

PFN_vkGetInstanceProcAddr                  Instance::GetProcAddr;
PFN_vkCreateInstance                       Instance::Create;
PFN_vkDestroyInstance                      Instance::Destroy;
PFN_vkEnumerateInstanceExtensionProperties Instance::EnumerateExtensionProperties;
PFN_vkEnumerateInstanceExtensionProperties Instance::EnumerateLayerProperties;
PFN_vkEnumerateInstanceExtensionProperties Instance::EnumerateVersion;
PFN_vkEnumeratePhysicalDevices             Instance::EnumeratePhysicalDevices;
PFN_vkCreateDebugUtilsMessengerEXT         Instance::CreateDebugUtilsMessengerEXT;
PFN_vkCreateDebugReportCallbackEXT         Instance::CreateDebugReportCallbackEXT;
PFN_vkDestroyDebugUtilsMessengerEXT        Instance::DestroyDebugUtilsMessengerEXT;
PFN_vkDestroyDebugReportCallbackEXT        Instance::DestroyDebugReportCallbackEXT;

std::vector<FuncPair> Instance::basic_funcs_ = {
    { &Create,                       "vkCreateInstance"                       },
    { &EnumerateExtensionProperties, "vkEnumerateInstanceExtensionProperties" },
    { &EnumerateLayerProperties,     "vkEnumerateInstanceLayerProperties"     },
    { &EnumerateVersion,             "vkEnumerateInstanceVersion"             }
};

std::vector<FuncPair> Instance::utils_funcs_ = {
    { &Destroy,                                        "vkDestroyInstance"                              },
    { &EnumeratePhysicalDevices,                       "vkEnumeratePhysicalDevices"                     },
    { &CreateDebugUtilsMessengerEXT,                   "vkCreateDebugUtilsMessengerEXT"                 },
    { &CreateDebugReportCallbackEXT,                   "vkCreateDebugReportCallbackEXT"                 },
    { &DestroyDebugUtilsMessengerEXT,                  "vkDestroyDebugUtilsMessengerEXT"                },
    { &DestroyDebugReportCallbackEXT,                  "vkDestroyDebugReportCallbackEXT"                },
    { &Device::Create,                                 "vkCreateDevice"                                 },
	{ &Device::EnumerateExtensionProperties,           "vkEnumerateDeviceExtensionProperties"           },
	{ &Device::EnumerateLayerProperties,               "vkEnumerateDeviceLayerProperties"               },
	{ &Device::GetProcAddr,                            "vkGetDeviceProcAddr"                            },
	{ &PhysicalDevice::GetFeatures,                    "vkGetPhysicalDeviceFeatures"                    },
	{ &PhysicalDevice::GetFormatProperties,            "vkGetPhysicalDeviceFormatProperties"            },
	{ &PhysicalDevice::GetImageFormatProperties,       "vkGetPhysicalDeviceImageFormatProperties"       },
	{ &PhysicalDevice::GetMemoryProperties,            "vkGetPhysicalDeviceMemoryProperties"            },
	{ &PhysicalDevice::GetProperties,                  "vkGetPhysicalDeviceProperties"                  },
	{ &PhysicalDevice::GetQueueFamilyProperties,       "vkGetPhysicalDeviceQueueFamilyProperties"       },
	{ &PhysicalDevice::GetSparseImageFormatProperties, "vkGetPhysicalDeviceSparseImageFormatProperties" },
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_utils_messenger_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                                                     VkDebugUtilsMessageTypeFlagsEXT message_type,
                                                                     const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                                                                     void *user_data)
{
    (void)message_severity;
    (void)message_type;
    (void)user_data;

    printf("%d - %s: %s\n", callback_data->messageIdNumber, callback_data->pMessageIdName, callback_data->pMessage);

    return VK_FALSE;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report_callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT type,
                                                            uint64_t object,  size_t location, int32_t message_code,
                                                            const char *layer_prefix, const char *message,void *user_data)
{
    (void)flags;
    (void)type;
    (void)object;
    (void)location;
    (void)message_code;
    (void)user_data;

    printf("layer prefix: %s, error: %s\n", layer_prefix, message);
    return VK_FALSE;
}

void Instance::Init()
{
    if (!GetProcAddr)
    {
        InitVKContext();
    }

    if (Create)
    {
        return;
    }

    Instance{}.load_func_(basic_funcs_);
}

Instance::Instance(const std::string &name, const std::vector<const char *> &requestExtension, const std::vector<const char *> &requestLayers, uint32_t version) :
    handle{ VK_NULL_HANDLE }
{
    Init();

    ApplicationInfo appInfo{};
    appInfo.pEngineName      = name.c_str();
    appInfo.pApplicationName = name.c_str();
    appInfo.apiVersion       = version;

    CreateInfo createInfo{};
    createInfo.flags                   = 0;
    createInfo.enabledExtensionCount   = static_cast<uint32_t>(requestExtension.size());
    createInfo.ppEnabledExtensionNames = requestExtension.data();
    createInfo.enabledLayerCount       = static_cast<uint32_t>(requestExtension.size());
    createInfo.ppEnabledLayerNames     = requestLayers.data();
    createInfo.pApplicationInfo        = &appInfo;

    Instance{createInfo}.swap(*this);
}

Instance::Instance(const CreateInfo &createInfo, const VkAllocationCallbacks *pAllocator) :
    handle{ VK_NULL_HANDLE }
{
    Init();
    VKCPP_ASSERT(Create(&createInfo, pAllocator, &handle));
    on_loaded_();
}

Instance::~Instance()
{
    if (debug_utils_messenger)
    {
        destroy(debug_utils_messenger);
    }
    if (debug_report)
    {
        destroy(debug_report);
    }
    if (handle)
    {
        Destroy(handle, nullptr);
        handle = VK_NULL_HANDLE;
    }
}

bool Instance::listen_debug_utils_messenger(PFN_vkDebugUtilsMessengerCallbackEXT callback)
{
    if (!CreateDebugUtilsMessengerEXT)
    {
        return false;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    createInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = callback ? callback : debug_utils_messenger_callback;

    return !create(&createInfo, &debug_utils_messenger);
}

bool Instance::listen_debug_report_callback(PFN_vkDebugReportCallbackEXT callback)
{
    if (!CreateDebugReportCallbackEXT)
    {
        return false;
    }

    VkDebugReportCallbackCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
    createInfo.pfnCallback = callback ? callback : debug_report_callback;

    return !create(&createInfo, &debug_report);
}

void Instance::load_func_(std::vector<FuncPair> &funcs)
{
    for (auto &f : funcs)
    {
        *(void**)f.first = get_func<void*>(f.second);
    }
}

void Instance::on_loaded_()
{
    load_func_(utils_funcs_);
    init_physical_devices_();
}

void Instance::init_physical_devices_()
{
    uint32_t num_physical_device = 0;
    VKCPP_ASSERT(enumerate_physical_devices(&num_physical_device, nullptr));

    std::vector<VkPhysicalDevice> physical_device_handles;
    physical_device_handles.resize(num_physical_device);
    VKCPP_ASSERT(enumerate_physical_devices(&num_physical_device, physical_device_handles.data()));

    for (auto &physical_device : physical_device_handles)
    {
        physical_devics.emplace_back(new PhysicalDevice{ physical_device });
    }
}

PhysicalDevice *Instance::get_physical_device(uint32_t index)
{
    return physical_devics[index];
}

}
