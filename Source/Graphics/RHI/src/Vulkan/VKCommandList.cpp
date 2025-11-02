// class VulkanCommandList final : public ICommandList
// {
// public:
//     VulkanCommandList(VkDevice device, QueueType queueType, uint numFrames) {
//         _cmdPools.resize(numFrames);
//         _cmdBuffers.resize(numFrames);

//         VkCommandPoolCreateInfo poolInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
//         poolInfo.queueFamilyIndex = getQueueFamilyIndex(queueType);
//         poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

//         for (uint i = 0; i < numFrames; ++i)
//         {
//             vkCreateCommandPool(device, &poolInfo, nullptr, &_cmdPools[i]);

//             VkCommandBufferAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
//             allocInfo.commandPool        = _cmdPools[i];
//             allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//             allocInfo.commandBufferCount = 1;

//             vkAllocateCommandBuffers(device, &allocInfo, &_cmdBuffers[i]);
//         }
//     }

//     void begin() override {
//         VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
//         vkBeginCommandBuffer(_cmdBuffers[_currentFrame], &beginInfo);
//     }

//     void end() override {
//         vkEndCommandBuffer(_cmdBuffers[_currentFrame]);
//     }

//     void setCurrentFrame(uint index) override { _currentFrame = index; }

//     void resourceBarrier() override {
//         // TODO: Implement later with VkImageMemoryBarrier / VkBufferMemoryBarrier
//     }

//     NativeObject getNativeObject(ObjectType objectType) override {
//         switch (objectType)
//         {
//             case ObjectTypes::Vulkan_CommandBuffer:
//                 return NativeObject(objectType, _cmdBuffers[_currentFrame]);
//             case ObjectTypes::Vulkan_CommandPool:
//                 return NativeObject(objectType, _cmdPools[_currentFrame]);
//             default:
//                 // return nullptr;
//         }
//     }

// private:
//     std::vector<VkCommandPool>   _cmdPools;
//     std::vector<VkCommandBuffer> _cmdBuffers;
//     uint                         _currentFrame = 0;
// };
