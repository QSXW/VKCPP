#include "device.h"

namespace VKCPP
{

PFN_vkCreateDevice                       Device::Create;
PFN_vkEnumerateDeviceExtensionProperties Device::EnumerateExtensionProperties;
PFN_vkEnumerateDeviceLayerProperties     Device::EnumerateLayerProperties;
PFN_vkGetDeviceProcAddr                  Device::GetProcAddr;

}
