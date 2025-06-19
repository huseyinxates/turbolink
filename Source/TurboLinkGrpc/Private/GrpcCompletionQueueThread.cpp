#include "GrpcCompletionQueueThread.h"
#include "TurboLinkGrpcManager.h"
#include "TurboLinkGrpcManager_Private.h"
#include "TurboLinkGrpcContext.h"
#include "Async/Async.h"

FGrpcCompletionQueueThread::FGrpcCompletionQueueThread(UTurboLinkGrpcManager* InManager)
: Manager(InManager)
, bStop(false)
{
}

uint32 FGrpcCompletionQueueThread::Run()
{
if (!Manager || !Manager->d || Manager->d->CompletionQueue == nullptr)
{
return 0;
}

while (!bStop)
{
void* event_tag = nullptr;
bool ok = false;
gpr_timespec deadline;
deadline.clock_type = GPR_CLOCK_MONOTONIC;
deadline.tv_nsec = 1000000; // 1ms
deadline.tv_sec = 0;

auto result = Manager->d->CompletionQueue->AsyncNext(&event_tag, &ok, deadline);
if (result == grpc::CompletionQueue::NextStatus::GOT_EVENT)
{
void* capturedTag = event_tag;
bool capturedOk = ok;
AsyncTask(ENamedThreads::GameThread, [this, capturedTag, capturedOk]()
{
Manager->HandleRpcEvent(capturedTag, capturedOk);
});
}
else if (result == grpc::CompletionQueue::NextStatus::SHUTDOWN)
{
break;
}
}
return 0;
}

void FGrpcCompletionQueueThread::Stop()
{
bStop = true;
}
