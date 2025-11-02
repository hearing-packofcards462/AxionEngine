// // nvrhi::BindingLayoutHandle layout = device->createBindingLayout({
// //     .bindings = {
// //         { 0, nvrhi::ResourceType::Texture_SRV, 1, nvrhi::ShaderType::Pixel },
// //         { 1, nvrhi::ResourceType::Sampler, 1, nvrhi::ShaderType::Pixel }
// //     }
// // });

// // nvrhi::BindingSetHandle set = device->createBindingSet({
// //     .bindings = {
// //         { 0, myTexture },
// //         { 1, mySampler }
// //     }
// // }, layout);

// //REFFFF

// // | Vulkan Concept                 | DX12 Concept               | NVRHI Equivalent                           | Notes                                        |
// // | ------------------------------ | -------------------------- | ------------------------------------------ | -------------------------------------------- |
// // | `VkDescriptorSetLayout`        | Root Signature slot layout | `nvrhi::BindingLayout`                     | Describes what’s bound where                 |
// // | `VkDescriptorSet`              | Descriptor tables          | `nvrhi::BindingSet`                        | Holds actual SRVs/UAVs/samplers              |
// // | `VkDescriptorPool`             | Descriptor heap allocator  | Internal to device                         | Hidden                                       |
// // | `VkImageView` / `VkBufferView` | Descriptors (SRV/UAV)      | Created automatically per resource         | Cached inside `nvrhi::ITexture` or `IBuffer` |
// // | `VkPipelineLayout`             | Root Signature             | `nvrhi::Pipeline` (links layout + shaders) | Automatically built from layout              |

// // device->createTexture(desc, nvrhi::TextureFlags::Bindless);

// // struct DescriptorBinding {
// //     uint32_t binding;
// //     enum class Type { TextureSRV, TextureUAV, Sampler, ConstantBuffer };
// //     uint32_t count;
// // };

// // struct DescriptorLayoutDesc {
// //     std::vector<DescriptorBinding> bindings;
// // };

// // class DescriptorLayout {
// // public:
// //     virtual ~DescriptorLayout() = default;
// // };

// // class DescriptorSet {
// // public:
// //   std::unordered_map<uint32_t, ResourceHandle> resources;
// //     virtual ~DescriptorSet() = default;
// //     virtual void update(uint32_t binding, Texture* texture) = 0;
// // };

// // class Device {
// // public:
// //     virtual Texture* createTexture(const TextureDesc& desc) = 0;
// //     virtual DescriptorLayout* createDescriptorLayout(const DescriptorLayoutDesc& desc) = 0;
// //     virtual DescriptorSet* createDescriptorSet(DescriptorLayout* layout) = 0;
// // };

// // D3D12_DESCRIPTOR_RANGE ranges[MAX_BINDINGS];
// // D3D12_ROOT_PARAMETER params[MAX_BINDINGS];
// // for (each binding) {
// //     ranges[i].RangeType = toD3D12DescriptorRangeType(binding.type);
// //     ranges[i].NumDescriptors = 1;
// //     ranges[i].BaseShaderRegister = binding.binding;
// //     ranges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

// //     params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
// //     params[i].DescriptorTable.NumDescriptorRanges = 1;
// //     params[i].DescriptorTable.pDescriptorRanges = &ranges[i];
// //     params[i].ShaderVisibility = toD3D12Visibility(binding.stages);
// // }
// #pragma once
// #include "Raiko/Graphics/RHI/UniformArena.h"
// #include "DX12Device.h"
// #include <vector>

// RAIKO_NAMESPCE_BEGIN
// namespace Graphics::RHI {

// class DX12UniformArena : public IUniformArena
// {
// public:
//     // device - your DX12 device wrapper
//     // resources - DX12Device::Resources used to allocate descriptors
//     DX12UniformArena(const ComPtr<ID3D12Device2>& device,
//                      DX12Device::Resources& resources,
//                      const UniformArenaDesc& desc);

//     ~DX12UniformArena() override;

//     const UniformArenaDesc& getDesc() const override { return _desc; }

//     void update(uint32_t frameIndex, uint32_t objectIndex, const void* data, size_t size) override;
//     BindDescriptor getDescriptor(uint32_t frameIndex, uint32_t objectIndex) const override;
//     uint64_t getGPUAddressOffset(uint32_t frameIndex, uint32_t objectIndex) const override;

// private:
//     void createBackingBuffer();
//     void createDescriptors();

// private:
//     ComPtr<ID3D12Device2> _device;
//     DX12Device::Resources* _resources; // pointer to your resource/descriptor heaps

//     UniformArenaDesc _desc;
//     size_t _alignedElementSize = 0;
//     size_t _perFrameSize = 0;
//     size_t _totalSize = 0;

//     // The backing buffer is CPU-visible (upload heap)
//     ComPtr<ID3D12Resource> _backingBuffer;
//     // Mapped CPU pointer
//     uint8_t* _mappedPtr = nullptr;

//     // Descriptor handles: cpu and gpu arrays sized [frames * maxObjects]
//     std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> _cpuCBVHandles;
//     std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> _gpuCBVHandles;

//     UINT _descriptorStride = 0; // descriptor increment size in bytes
// };

// } // namespace Graphics::RHI
// RAIKO_NAMESPCE_END
// #include "DX12UniformArena.h"
// #include "DX12Utils.h" // for AlignUp, ThrowIfFailed, etc.
// #include <d3dx12.h>
// #include <fmt/format.h>

// RAIKO_NAMESPCE_BEGIN
// namespace Graphics::RHI {

// DX12UniformArena::DX12UniformArena(const ComPtr<ID3D12Device2>& device,
//                                    DX12Device::Resources& resources,
//                                    const UniformArenaDesc& desc)
//     : _device(device), _resources(&resources), _desc(desc)
// {
//     AXION_LOG_ASSERT(desc.elementSize > 0, Logger::Module::RHI, "UniformArena elementSize must be > 0");

//     // align to 256 bytes for DX12 CBV
//     _alignedElementSize = Math::AlignUp(desc.elementSize, (size_t)256);
//     _perFrameSize = _alignedElementSize * desc.maxObjectsPerFrame;
//     _totalSize = _perFrameSize * desc.framesInFlight;

//     createBackingBuffer();
//     createDescriptors();

//     // set readable debug name
//     if (!_desc.debugName.empty()) {
//         std::string name = fmt::format("{} (UniformArena)", _desc.debugName);
//         _backingBuffer->SetName(std::wstring(name.begin(), name.end()).c_str());
//     }
// }

// DX12UniformArena::~DX12UniformArena()
// {
//     if (_backingBuffer && _mappedPtr)
//         _backingBuffer->Unmap(0, nullptr);
//     _mappedPtr = nullptr;
//     _backingBuffer.Reset();
// }

// void DX12UniformArena::createBackingBuffer()
// {
//     // create upload heap buffer
//     CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
//     auto bufDesc = CD3DX12_RESOURCE_DESC::Buffer(_totalSize);

//     DX_CHECK(_device->CreateCommittedResource(
//         &heapProps,
//         D3D12_HEAP_FLAG_NONE,
//         &bufDesc,
//         D3D12_RESOURCE_STATE_GENERIC_READ,
//         nullptr,
//         IID_PPV_ARGS(&_backingBuffer)));

//     // Map persistently
//     CD3DX12_RANGE readRange(0, 0);
//     void* p = nullptr;
//     DX_CHECK(_backingBuffer->Map(0, &readRange, &p));
//     _mappedPtr = reinterpret_cast<uint8_t*>(p);
// }

// void DX12UniformArena::createDescriptors()
// {
//     const uint32_t totalSlots = _desc.framesInFlight * _desc.maxObjectsPerFrame;
//     _cpuCBVHandles.resize(totalSlots);
//     _gpuCBVHandles.resize(totalSlots);

//     // assume resources.heapCBV is a CPU/GPU descriptor heap manager with allocateCPU/allocateGPU
//     // If you only have one combined heap, use that (CBV/SRV/UAV heap).
//     // We'll use resources.heapSRV (which in your code was the CBV/SRV/UAV heap) as allocator.
//     // You must provide allocation functions in DX12Device::Resources:
//     //   D3D12_CPU_DESCRIPTOR_HANDLE allocateCPU(); D3D12_GPU_DESCRIPTOR_HANDLE allocateGPU();
//     // For brevity I assume .heapSRV has allocateCPU() and allocateGPU().

//     _descriptorStride = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

//     for (uint32_t f = 0; f < _desc.framesInFlight; ++f)
//     {
//         for (uint32_t obj = 0; obj < _desc.maxObjectsPerFrame; ++obj)
//         {
//             uint32_t idx = f * _desc.maxObjectsPerFrame + obj;

//             // allocate CPU handle
//             D3D12_CPU_DESCRIPTOR_HANDLE cpu = _resources->heapSRV.allocateCPU();
//             // compute GPU handle by offset from heap start if allocateGPU() not available
//             // If your heap manager provides a GPU allocate, use it. We'll compute relative GPU handle by index:
//             D3D12_GPU_DESCRIPTOR_HANDLE gpu = _resources->heapSRV.getGPUHandleForCPU(cpu); // <-- implement in your heap

//             _cpuCBVHandles[idx] = cpu;
//             _gpuCBVHandles[idx] = gpu;

//             // create CBV for that slot (each CBV points to a different offset)
//             D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
//             cbvDesc.BufferLocation = _backingBuffer->GetGPUVirtualAddress() + (uint64_t)(f * _perFrameSize + obj * _alignedElementSize);
//             cbvDesc.SizeInBytes = (UINT)_alignedElementSize;

//             _device->CreateConstantBufferView(&cbvDesc, cpu);
//         }
//     }
// }

// void DX12UniformArena::update(uint32_t frameIndex, uint32_t objectIndex, const void* data, size_t size)
// {
//     AXION_LOG_ASSERT(frameIndex < _desc.framesInFlight, Logger::Module::RHI, "frameIndex out of range");
//     AXION_LOG_ASSERT(objectIndex < _desc.maxObjectsPerFrame, Logger::Module::RHI, "objectIndex out of range");
//     AXION_LOG_ASSERT(size <= _desc.elementSize, Logger::Module::RHI, "size > elementSize");

//     const size_t offset = (size_t)frameIndex * _perFrameSize + (size_t)objectIndex * _alignedElementSize;
//     uint8_t* dst = _mappedPtr + offset;
//     memcpy(dst, data, size);

//     // No memory barrier needed for upload heap when CPU writes and GPU reads GENERIC_READ.
// }

// BindDescriptor DX12UniformArena::getDescriptor(uint32_t frameIndex, uint32_t objectIndex) const
// {
//     AXION_LOG_ASSERT(frameIndex < _desc.framesInFlight, Logger::Module::RHI, "frameIndex out of range");
//     AXION_LOG_ASSERT(objectIndex < _desc.maxObjectsPerFrame, Logger::Module::RHI, "objectIndex out of range");

//     uint32_t idx = frameIndex * _desc.maxObjectsPerFrame + objectIndex;

//     BindDescriptor out{};
//     out.cpuPtr = (void*)_cpuCBVHandles[idx].ptr; // CPU descriptor pointer encoded as ptr
//     out.gpuHandle = _gpuCBVHandles[idx].ptr;

//     return out;
// }

// uint64_t DX12UniformArena::getGPUAddressOffset(uint32_t frameIndex, uint32_t objectIndex) const
// {
//     uint64_t addr = _backingBuffer->GetGPUVirtualAddress() + (uint64_t)frameIndex * _perFrameSize + (uint64_t)objectIndex * _alignedElementSize;
//     return addr;
// }

// } // namespace Graphics::RHI
// RAIKO_NAMESPCE_END
