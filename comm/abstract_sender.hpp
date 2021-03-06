#pragma once

#include "base/message.hpp"

namespace minips {

    class AbstractSender {
    public:
        virtual ~AbstractSender() {}

        virtual void Start() = 0;

        virtual void Send() = 0;

        virtual void Stop() = 0;
    };

}  // namespace minips
