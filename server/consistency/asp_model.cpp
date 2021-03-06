#include "server/consistency/asp_model.hpp"
#include "glog/logging.h"
#include <base/utils.hpp>
#include <base/node.hpp>

namespace minips {

    ASPModel::ASPModel(uint32_t model_id, std::unique_ptr<AbstractStorage> &&storage_ptr,
                       ThreadsafeQueue<Message> *reply_queue)
            : model_id_(model_id), reply_queue_(reply_queue) {
        this->storage_ = std::move(storage_ptr);
    }

    void ASPModel::Clock(Message &msg) {
        progress_tracker_.AdvanceAndGetChangedMinClock(msg.meta.sender);
    }

    void ASPModel::Add(Message &msg) {
        CHECK(progress_tracker_.CheckThreadValid(msg.meta.sender));
        storage_->Add(msg);
    }

    void ASPModel::Get(Message &msg) {
        CHECK(progress_tracker_.CheckThreadValid(msg.meta.sender));
        reply_queue_->Push(storage_->Get(msg));
    }

    int ASPModel::GetProgress(int tid) {
        return this->progress_tracker_.GetProgress(tid);
    }

    void ASPModel::ResetWorker(Message &msg) {
        CHECK_EQ(msg.data.size(), 1);

        third_party::SArray<uint32_t> sArray;
        sArray = msg.data[0];
        std::vector<uint32_t> vec = SArrayToVector<uint32_t >(sArray);
        this->progress_tracker_.Init(vec);

        Message reply_msg;
        reply_msg.meta.model_id = model_id_;
        reply_msg.meta.recver = msg.meta.sender;
        reply_msg.meta.flag = Flag::kResetWorkerInModel;
        reply_queue_->Push(reply_msg);
    }

    void ASPModel::Dump(Message &msg) {}

    void ASPModel::Restore() {}

    void ASPModel::Update(int failed_node_id, std::vector<Node> &nodes, third_party::Range &range) {}

}  // namespace minips
