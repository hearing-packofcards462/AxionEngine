#pragma once
#include "Axion/Common/Math.h"
#include "Axion/Graphics/RHI/Common.h"
#include "Axion/Graphics/RHI/Resource.h"

AXION_NAMESPACE_BEGIN

namespace Graphics::RHI {

struct DescriptorBinding {
    uint           binding = 0; // register(t#, b#, s#, etc.)
    DescriptorType type;
    ShaderStage    stageMask = ShaderStage::Vertex | ShaderStage::Pixel;
    uint           arraySize = 1;
};

struct DescriptorLayoutDesc {
    std::vector<DescriptorBinding> bindings;
};

// class IDescriptorSet: public IResource
// {
// public:
//     std::unordered_map<uint32_t, ResourceHandle> resources;
//     virtual ~DescriptorSet()                                  = default;
//     virtual void update( uint32_t binding, Texture* texture ) = 0;
// };

} // namespace Graphics::RHI

AXION_NAMESPACE_END

