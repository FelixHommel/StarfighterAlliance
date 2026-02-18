#include "utility/BlockingQueue.hpp"
#include "utility/details/Threading.hpp"

#include <gtest/gtest.h>

#include <atomic>
#include <barrier>
#include <chrono>
#include <thread>
#include <vector>

namespace sfa::testing
{

/// \brief Test the features of the \ref BlockingQueue.
///
/// \author Felix Hommel
/// \date 2/11/2026
class BlockingQueueTest : public ::testing::Test
{
public:
    BlockingQueueTest() = default;
    ~BlockingQueueTest() override = default;

    BlockingQueueTest(const BlockingQueueTest&) = delete;
    BlockingQueueTest& operator=(const BlockingQueueTest&) = delete;
    BlockingQueueTest(BlockingQueueTest&&) = delete;
    BlockingQueueTest& operator=(BlockingQueueTest&&) = delete;

protected:
    BlockingQueue<int> m_queue;
};

/// \brief Test data processing with multiple producers and multiple consumers.
///
/// Given producers and consumers the queue should not skip over any insertions and process all elements in order.
TEST_F(BlockingQueueTest, MultipleProducersMultipleConsumers)
{
    constexpr int NUM_PRODUCERS{ 3 };
    constexpr int NUM_CONSUMERS{ 3 };
    constexpr int ITEMS_PER_PRODUCER{ 100 };
    constexpr int TOTAL_ITEMS{ NUM_PRODUCERS * ITEMS_PER_PRODUCER };
    constexpr int EXPECTED_SUM{ (TOTAL_ITEMS * (TOTAL_ITEMS - 1)) / 2 };

    std::atomic<int> consumed{ 0 };
    std::atomic<int> sum{ 0 };

    std::barrier start_barrier{ NUM_PRODUCERS + NUM_CONSUMERS };

    std::vector<threading::thread_t> producers;
    for(int p{ 0 }; p < NUM_PRODUCERS; ++p)
    {
        producers.emplace_back([&, p]() {
            start_barrier.arrive_and_wait();

            for(int i{ 0 }; i < ITEMS_PER_PRODUCER; ++i)
                m_queue.push((p * ITEMS_PER_PRODUCER) + i);
        });
    }

    std::vector<threading::thread_t> consumers;
    for(int c{ 0 }; c < NUM_CONSUMERS; ++c)
    {
        consumers.emplace_back([&]() {
            start_barrier.arrive_and_wait();

            while(const auto v{ m_queue.waitAndPop() })
            {
                sum.fetch_add(v.value(), std::memory_order_relaxed);
                consumed.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    producers.clear();

    m_queue.close();
    consumers.clear();

    EXPECT_EQ(consumed, TOTAL_ITEMS);

    EXPECT_EQ(sum, EXPECTED_SUM);
}

/// \brief Test the closing behavior of the queue.
///
/// When the queue is closed while not having any values, all threads that are waiting for the queue to fill should
/// stop waiting and return false.
TEST_F(BlockingQueueTest, CloseWhileWatching)
{
    constexpr auto SLEEP_DELAY{ 100 };

    std::atomic<bool> consumerExited{ false };

    threading::thread_t consumer{ [&]() {
        const auto v{ m_queue.waitAndPop() };
        EXPECT_FALSE(v.has_value());

        consumerExited = true;
    } };

    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DELAY));

    m_queue.close();

    consumer.join();
    EXPECT_TRUE(consumerExited);
}

/// \brief Test the non blocking version of the pop method.
///
/// When the queue is empty and the non-blocking pop method is used, the tryPop method should return false to indicate
/// that no value was available.
TEST_F(BlockingQueueTest, TryPopNonBlockingOnEmptyQueue)
{
    const auto v{ m_queue.tryPop() };

    EXPECT_FALSE(v.has_value());
}

/// \brief Test the non blocking version of the pop method.
///
/// When the queue is not empty and the non-blocking pop method is used, the tryPop method should return true to indicate
/// that the value passed to the function is populated properly.
TEST_F(BlockingQueueTest, TryPopNonBlockingOnNonEmptyQueue)
{
    constexpr auto DATA{ 42 };

    m_queue.push(DATA);

    if(const auto v{ m_queue.tryPop() })
        EXPECT_EQ(DATA, v.value());
    else
        ASSERT_TRUE(v.has_value());
}

} // namespace sfa::testing

