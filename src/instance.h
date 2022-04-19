#pragma once

#include <string>
#include <vector>

#include "core.h"
#include "IObject.h"
#include "physical_device.h"

namespace VKCPP
{

struct Instance : public IObject
{
private:
    static std::vector<FuncPair> basic_funcs_;
    static std::vector<FuncPair> utils_funcs_;

public:
    static PFN_vkGetInstanceProcAddr                  GetProcAddr;
    static PFN_vkCreateInstance                       Create;
    static PFN_vkDestroyInstance                      Destroy;
    static PFN_vkEnumerateInstanceExtensionProperties EnumerateExtensionProperties;
    static PFN_vkEnumerateInstanceExtensionProperties EnumerateLayerProperties;
    static PFN_vkEnumerateInstanceExtensionProperties EnumerateVersion;
    static PFN_vkEnumeratePhysicalDevices             EnumeratePhysicalDevices;
    static PFN_vkCreateDebugUtilsMessengerEXT         CreateDebugUtilsMessengerEXT;
    static PFN_vkCreateDebugReportCallbackEXT         CreateDebugReportCallbackEXT;
    static PFN_vkDestroyDebugUtilsMessengerEXT        DestroyDebugUtilsMessengerEXT;
    static PFN_vkDestroyDebugReportCallbackEXT        DestroyDebugReportCallbackEXT;

    static void Init();

    struct CreateInfo : public VkInstanceCreateInfo
    {
        CreateInfo()
        {
            VKCPP_DERIVED_FROM(INSTANCE)
        }
    };

    using Description = CreateInfo;

    using Primitive   = VkInstance;
    VKCPP_OPERATOR_HANDLE()

public:
    Instance(const std::string &name, const std::vector<const char *> &requestExtension, const std::vector<const char *> &requestLayers, uint32_t apiVersion = VK_API_VERSION_1_2);

    Instance(const CreateInfo &createInfo, const VkAllocationCallbacks *pAllocator = nullptr);

    ~Instance();

    bool listen_debug_utils_messenger(PFN_vkDebugUtilsMessengerCallbackEXT callback = nullptr);

    bool listen_debug_report_callback(PFN_vkDebugReportCallbackEXT callback = nullptr);

    /*
        @brief: managed internal, don't release what you got.
    */
    PhysicalDevice *get_physical_device(uint32_t index = 0);

public: /* inline */
    VKCPP_DELETE_COPY(Instance)

    Instance() :
        handle{ VK_NULL_HANDLE }
    {

    }

    Instance(Primitive other) :
        handle{ other }
    {

    }

    Instance(Instance &&other) :
        handle{ VK_NULL_HANDLE }
    {
        other.swap(*this);
    }

    Instance &operator=(Instance &&other)
    {
        Instance{std::move(*this)}.swap(other);
        return *this;
    }

    void swap(Instance &other)
    {
        std::swap(handle, other.handle);
        std::swap(physical_devics, other.physical_devics);
    }

    template <class T>
    T get_func(const std::string &name) const
    {
        return reinterpret_cast<T>(GetProcAddr(handle, name.c_str()));
    }

    VkResult create(const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, VkDebugUtilsMessengerEXT *pMessenger, const VkAllocationCallbacks *pAllocator = nullptr)
    {
        return CreateDebugUtilsMessengerEXT(handle, pCreateInfo, pAllocator, pMessenger);
    }

    void destroy(VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks *pAllocator = nullptr)
    {
        DestroyDebugUtilsMessengerEXT(handle, messenger, pAllocator);
    }

    VkResult create(const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, VkDebugReportCallbackEXT *pCallback, const VkAllocationCallbacks* pAllocator = nullptr)
    {
        return CreateDebugReportCallbackEXT(handle, pCreateInfo, pAllocator, pCallback);
    }

    void destroy(VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator = nullptr)
    {
        DestroyDebugReportCallbackEXT(handle, callback, pAllocator);
    }

    VkResult enumerate_physical_devices(uint32_t *pCount, VkPhysicalDevice *pPhysicalDevices)
    {
        return EnumeratePhysicalDevices(handle, pCount, pPhysicalDevices);
    }

private:
    void load_func_(std::vector<FuncPair> &funcs);

    void on_loaded_();

    void init_physical_devices_();

protected:
    std::vector<MonoRef<PhysicalDevice>> physical_devics;

    VkDebugUtilsMessengerEXT debug_utils_messenger = VK_NULL_HANDLE;

    VkDebugReportCallbackEXT debug_report = VK_NULL_HANDLE;
};

}
