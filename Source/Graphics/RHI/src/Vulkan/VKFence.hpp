// //Use TIMELINE SEMPAHORES, THEY PERFECTLY MATCH DX12 API

// // VulkanFence.h
// #pragma once
// #include "Nigiri/RHI/IFence.h"
// #include <vulkan/vulkan.h>

// NIGIRI_NAMESPCE_BEGIN
// namespace RHI {

// class VulkanFence final : public IFence {
// public:
//     VulkanFence(VkDevice device)
//         : _device(device) {
//         VkSemaphoreTypeCreateInfo timelineCreateInfo = {
//             VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO
//         };
//         timelineCreateInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
//         timelineCreateInfo.initialValue  = 0;

//         VkSemaphoreCreateInfo createInfo = {
//             VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
//         };
//         createInfo.pNext = &timelineCreateInfo;

//         VkResult res = vkCreateSemaphore(_device, &createInfo, nullptr, &_timelineSemaphore);
//         assert(res == VK_SUCCESS && "Failed to create timeline semaphore");
//     }

//     ~VulkanFence() override {
//         if (_timelineSemaphore)
//             vkDestroySemaphore(_device, _timelineSemaphore, nullptr);
//     }

//     void signal(uint64_t value = 0) override {
//         VkSemaphoreSignalInfo info = { VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO };
//         info.semaphore = _timelineSemaphore;
//         info.value = value;
//         vkSignalSemaphore(_device, &info);
//     }

//     void wait(uint64_t value = UINT64_MAX) override {
//         VkSemaphoreWaitInfo info = { VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO };
//         info.semaphoreCount = 1;
//         info.pSemaphores = &_timelineSemaphore;
//         info.pValues = &value;
//         vkWaitSemaphores(_device, &info, UINT64_MAX);
//     }

//     bool isSignaled(uint64_t value = 0) const override {
//         uint64_t completed = 0;
//         vkGetSemaphoreCounterValue(_device, _timelineSemaphore, &completed);
//         return completed >= value;
//     }

//     uint64_t getCompletedValue() const override {
//         uint64_t completed = 0;
//         vkGetSemaphoreCounterValue(_device, _timelineSemaphore, &completed);
//         return completed;
//     }

//     VkSemaphore getNative() const { return _timelineSemaphore; }

// private:
//     VkDevice     _device = VK_NULL_HANDLE;
//     VkSemaphore  _timelineSemaphore = VK_NULL_HANDLE;
// };

// } // namespace RHI
// NIGIRI_NAMESPCE_END
