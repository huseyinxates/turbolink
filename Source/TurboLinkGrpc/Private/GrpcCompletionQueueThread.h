#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"

class UTurboLinkGrpcManager;

class FGrpcCompletionQueueThread : public FRunnable
{
public:
FGrpcCompletionQueueThread(UTurboLinkGrpcManager* InManager);

virtual uint32 Run() override;
virtual void Stop() override;

private:
UTurboLinkGrpcManager* Manager;
FThreadSafeBool bStop;
};
