#pragma once
#include "Axion/Graphics/RHI/CommandList.h"
#include "Axion/Graphics/RHI/Common.h"
#include "Axion/Graphics/RHI/Pipeline.h"
#include "Axion/Graphics/RHI/Resource.h"
#include "Axion/Graphics/RHI/Swapchain.h"

AXION_NAMESPACE_BEGIN

namespace Graphics::RHI {

DEFINE_COM_HANDLE_FOR_TYPE( IDevice, Device )

struct Fence {
    ulong value = 0;
};

class IDevice : public IResource
{
public:
    virtual ~IDevice()                                                                                          = default;
    virtual SwapchainHandle       createSwapchain( const NativeObject& handle, const SwapchainDesc& desc = {} ) = 0;
    virtual CommandListHandle     createCommandList( const CommandListDesc& desc )                              = 0;
    virtual TextureHandle         createTexture( const TextureDesc& desc, const void* initialData = nullptr )   = 0;
    virtual BufferHandle          createBuffer( const BufferDesc& desc, const void* initialData = nullptr )     = 0;
    virtual PipelineLayoutHandle  createPipelineLayout( const PipelineLayoutDesc& desc )                        = 0;
    virtual GraphicPipelineHandle createGraphicPipeline( const GraphicPipelineDesc& desc )                      = 0;
    virtual ComputePipelineHandle createComputePipeline( const ComputePipelineDesc& desc )                      = 0;

    virtual void executeCommandLists( const std::vector<ICommandList*>& lists, QueueType workingQueue, Fence& frameFence ) = 0;
    virtual void waitForFrame( const Fence& frameFence, QueueType workingQueue )                                           = 0;
    virtual void waitForQueue( QueueType workingQueue, QueueType dstQueue )                                                = 0;
    virtual void queueWaitIdle( QueueType workingQueue, Fence& frameFence )                                                = 0;
    virtual bool waitIdle()                                                                                                = 0;

    virtual bool          queryFeatureSupport( Feature feature, void* pInfo = nullptr, size_t infoSize = 0 ) const = 0;
    virtual FormatSupport queryFormatSupport( Format format ) const                                                = 0;
    virtual API           getGraphicsAPI()                                                                         = 0;

protected:
    virtual void checkExtensions() = 0;
};

} // namespace Graphics::RHI

AXION_NAMESPACE_END
