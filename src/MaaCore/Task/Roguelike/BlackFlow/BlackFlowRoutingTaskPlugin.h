#pragma once

#include "BlackFlowTaskPluginBase.h"

namespace asst::blackflow
{
class BlackFlowRoutingTaskPlugin final : public BlackFlowTaskPluginBase
{
public:
    using BlackFlowTaskPluginBase::BlackFlowTaskPluginBase;

    virtual bool verify(AsstMsg msg, const json::value& details) const override;
    virtual void reset_in_run_variables() override;

protected:
    virtual bool _run() override;

private:
    // _run() 是 MAA 工作线程上的最外层：AbstractTask::run() 与 AbstractTask::callback() 全链路无 catch，
    // 异常漏出去就是线程上的未捕获异常 -> std::terminate -> 整个 MAA 进程死。
    // 真正的逻辑放 run_impl()，_run() 只当异常闸门。
    bool run_impl();

    enum class PendingWork
    {
        None,
        ObserveAndPlan,
        ResumePendingMove,
    };

    mutable PendingWork m_pending = PendingWork::None;
    bool m_page_recovery_attempted = false;
};
} // namespace asst::blackflow
