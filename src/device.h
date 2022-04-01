#pragma once

#include <string>
#include <vector>

#include "core.h"
#include "IObject.h"

namespace VKCPP
{

struct Device : public IObject
{
public:
    static PFN_vkCreateDevice                       Create;
    static PFN_vkEnumerateDeviceExtensionProperties EnumerateExtensionProperties;
    static PFN_vkEnumerateDeviceLayerProperties     EnumerateLayerProperties;
    static PFN_vkGetDeviceProcAddr                  GetProcAddr;

    using Primitive   = VkDevice;

    struct CreateInfo : public VkDeviceCreateInfo
    {
        CreateInfo()
        {
            VKCPP_DERIVED_FROM(DEVICE)
        }
    };

    using Description = CreateInfo;

public:
    VKCPP_OPERATOR_HANDLE()

public: /* inline */
    void swap(Device &other)
    {
        std::swap(handle, other.handle);
    }

protected:
    Primitive handle;
};

}
