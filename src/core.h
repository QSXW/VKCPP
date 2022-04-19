#pragma once

#include <iostream>
#include <vulkan/vulkan.h>

namespace VKCPP
{

#define VKCPP_MAKE_VERSION(variant, major, minor, patch) VK_MAKE_API_VERSION(variant, major, minor, patch)
#define VKCPP_VERSION VKCPP_MAKE_VERSION(0, 0, 0, 1)
#define VKCPP_STYPE(T) VK_STRUCTURE_TYPE_##T##_CREATE_INFO
#define VKCPP_DERIVED_FROM(T) sType = VK_STRUCTURE_TYPE_##T##_CREATE_INFO; pNext = nullptr;
#define VKCPP_DELETE_COPY_CONSTRUCTOR(T) T(const T &) = delete;
#define VKCPP_DELETE_COPY_ASSIGNMENT(T) T &operator=(const T &) = delete;
#define VKCPP_DELETE_COPY(T) VKCPP_DELETE_COPY_CONSTRUCTOR(T) VKCPP_DELETE_COPY_ASSIGNMENT(T)

#define VKCPP_OPERATOR_HANDLE() Primitive Handle() const { return handle; } operator Primitive() const { return handle; }  protected: Primitive handle{ VK_NULL_HANDLE };

#define VKCPP_CASE(x) case x: return #x;
inline const char *Stringify(VkResult err)
{
    switch (err)
    {
        VKCPP_CASE(VK_SUCCESS)
        VKCPP_CASE(VK_NOT_READY)
        VKCPP_CASE(VK_TIMEOUT)
        VKCPP_CASE(VK_EVENT_SET)
        VKCPP_CASE(VK_EVENT_RESET)
        VKCPP_CASE(VK_INCOMPLETE)
        VKCPP_CASE(VK_ERROR_OUT_OF_HOST_MEMORY)
        VKCPP_CASE(VK_ERROR_OUT_OF_DEVICE_MEMORY)
        VKCPP_CASE(VK_ERROR_INITIALIZATION_FAILED)
        VKCPP_CASE(VK_ERROR_DEVICE_LOST)
        VKCPP_CASE(VK_ERROR_MEMORY_MAP_FAILED)
        VKCPP_CASE(VK_ERROR_LAYER_NOT_PRESENT)
        VKCPP_CASE(VK_ERROR_EXTENSION_NOT_PRESENT)
        VKCPP_CASE(VK_ERROR_FEATURE_NOT_PRESENT)
        VKCPP_CASE(VK_ERROR_INCOMPATIBLE_DRIVER)
        VKCPP_CASE(VK_ERROR_TOO_MANY_OBJECTS)
        VKCPP_CASE(VK_ERROR_FORMAT_NOT_SUPPORTED)
        VKCPP_CASE(VK_ERROR_FRAGMENTED_POOL)
        VKCPP_CASE(VK_ERROR_UNKNOWN)
        VKCPP_CASE(VK_ERROR_OUT_OF_POOL_MEMORY)
        VKCPP_CASE(VK_ERROR_INVALID_EXTERNAL_HANDLE)
        VKCPP_CASE(VK_ERROR_FRAGMENTATION)
        VKCPP_CASE(VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS)
        VKCPP_CASE(VK_ERROR_SURFACE_LOST_KHR)
        VKCPP_CASE(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR)
        VKCPP_CASE(VK_SUBOPTIMAL_KHR)
        VKCPP_CASE(VK_ERROR_OUT_OF_DATE_KHR)
        VKCPP_CASE(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR)
        VKCPP_CASE(VK_ERROR_VALIDATION_FAILED_EXT)
        VKCPP_CASE(VK_ERROR_INVALID_SHADER_NV)
        VKCPP_CASE(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT)
        VKCPP_CASE(VK_ERROR_NOT_PERMITTED_EXT)
        VKCPP_CASE(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT)
        VKCPP_CASE(VK_THREAD_IDLE_KHR)
        VKCPP_CASE(VK_THREAD_DONE_KHR)
        VKCPP_CASE(VK_OPERATION_DEFERRED_KHR)
        VKCPP_CASE(VK_OPERATION_NOT_DEFERRED_KHR)
        VKCPP_CASE(VK_PIPELINE_COMPILE_REQUIRED_EXT)
        default: return "Unknown Vulkan Error Code";
    }
}

inline void VKCPP_ASSERT(VkResult status)
{
    if (status)
    {
        auto str = Stringify(status);
        printf("Detected Vulkan error: %d{%s}\n", status, str);
        throw str;
    }
}

class Exception : public std::exception
{
public: Exception(const char *what) noexcept :
#ifdef _MSC_VER
std::exception(what, 1) { }
#elif __GNUC__
message(what) { }

    virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override
    {
        return message.c_str();
    }

    std::string message;
#endif
};

class CorruptedAPICall: public Exception
{
public:
    CorruptedAPICall(const char *what) noexcept :
        Exception(what)
    {

    }

    CorruptedAPICall(const std::string &what) noexcept :
        CorruptedAPICall(what.c_str())
    {

    }
};

#define VKCPP_THROW_WHEN(expr, x) if (!!(expr)) { throw CorruptedAPICall(x); }

using FuncPair = std::pair<void*, const char *>;

VkResult InitVKContext();

struct ApplicationInfo : public VkApplicationInfo
{
    ApplicationInfo()
    {
        sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        pNext              = nullptr;
        apiVersion         = VK_API_VERSION_1_2;
        applicationVersion = VKCPP_MAKE_VERSION(0, 0, 0, 1);
    }
};

}
