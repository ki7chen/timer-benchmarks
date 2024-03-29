// Copyright © 2021 ichenq@gmail.com All rights reserved.
// See accompanying files LICENSE

#pragma once

#include "TimerBase.h"
#include <vector>
#include <unordered_map>

class HashedWheelBucket;
class HashedWheelTimeout;

// A simple hashed wheel timer inspired by [Netty HashedWheelTimer]
// see https://github.com/netty/netty/blob/4.1/common/src/main/java/io/netty/util/HashedWheelTimer.java
//
// timer scheduler implemented by hashed wheel
// complexity:
//      StartTimer   CancelTimer   PerTick
//       O(1)         O(1)          O(1)
//
class HashedWheelTimer : public TimerBase
{
public:
    HashedWheelTimer();
    ~HashedWheelTimer();

    TimerSchedType Type() const override
    {
        return TimerSchedType::TIMER_HASHED_WHEEL;
    }

    // start a timer after `duration` milliseconds
    int Start(uint32_t duration, TimeoutAction action) override;

    // cancel a timer
    bool Cancel(int timer_id) override;

    int Update(int64_t now = 0) override;

    int Size() const override
    {
        return (int)ref_.size();
    }

private:
    friend class HashedWheelTimeout;
    friend class HashedWheelBucket;

    int tick();

    void purge();
    void delTimeout(HashedWheelTimeout*);

    HashedWheelTimeout* allocTimeout(int id, int64_t deadline, TimeoutAction action);
    void freeTimeout(HashedWheelTimeout*);

private:
    std::vector<HashedWheelBucket*> wheel_;
    std::unordered_map<int, HashedWheelTimeout*> ref_;
    
    int ticks_ = 0;
    int64_t started_at_ = 0;
    int64_t last_time_ = 0;
};
