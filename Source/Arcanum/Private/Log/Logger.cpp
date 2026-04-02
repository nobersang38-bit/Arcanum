#include "Log/Logger.h"

FLoggerWorker::FLoggerWorker()
{
	Thread = FRunnableThread::Create(this, TEXT("LoggerAsyncThread"), 0, TPri_BelowNormal);
}

FLoggerWorker::~FLoggerWorker()
{
	StopWorker();
}

void FLoggerWorker::Enqueue(const FString& Message, ELogLevel Level)
{
	LogQueue.Enqueue({ Message, Level });
}

void FLoggerWorker::StopWorker()
{
	bStopThread = true;

	if (Thread) {
		Thread->WaitForCompletion();
		delete Thread;
		Thread = nullptr;
	}
}

void FLoggerWorker::Stop()
{
	bStopThread = true;
}

uint32 FLoggerWorker::Run()
{
	while (!bStopThread) {
		FQueuedLogMessage LogMessage;

		if (LogQueue.Dequeue(LogMessage))
		{
			const FString LogEntry = FString::Printf(TEXT("%s [%s] %s\n"),
				*GetTimestamp(),
				*LogLevelToString(LogMessage.Level),
				*LogMessage.Message);

			FScopeLock ScopeLock(&CriticalSection);
			FFileHelper::SaveStringToFile(
				LogEntry,
				*GetLogFilePath(),
				FFileHelper::EEncodingOptions::ForceUTF8,
				&IFileManager::Get(),
				FILEWRITE_Append
			);
		}
		else
		{
			FPlatformProcess::Sleep(0.01f);
		}
	}

	FQueuedLogMessage LogMessage;
	while (LogQueue.Dequeue(LogMessage))
	{
		const FString LogEntry = FString::Printf(TEXT("%s [%s] %s\n"),
			*GetTimestamp(),
			*LogLevelToString(LogMessage.Level),
			*LogMessage.Message);

		FScopeLock ScopeLock(&CriticalSection);
		FFileHelper::SaveStringToFile(
			LogEntry,
			*GetLogFilePath(),
			FFileHelper::EEncodingOptions::ForceUTF8,
			&IFileManager::Get(),
			FILEWRITE_Append
		);
	}

	return 0;
}

FString FLoggerWorker::GetTimestamp() const
{
	return FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"));
}

FString FLoggerWorker::GetLogFilePath() const
{
	FString FolderPath = FPaths::ProjectLogDir() + TEXT("AsyncLogs/");
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!PlatformFile.DirectoryExists(*FolderPath))
	{
		PlatformFile.CreateDirectoryTree(*FolderPath);
	}

	FString DateString = FDateTime::Now().ToString(TEXT("%Y-%m-%d"));
	return FolderPath + TEXT("Log_") + DateString + TEXT(".txt");
}

FString FLoggerWorker::LogLevelToString(ELogLevel Level) const
{
	switch (Level)
	{
	case ELogLevel::Debug:    return TEXT("[DEBUG] ");
	case ELogLevel::Warning:  return TEXT("[WARNING] ");
	case ELogLevel::Error:    return TEXT("[ERROR] ");
	case ELogLevel::Critical: return TEXT("[CRITICAL] ");
	default:                  return TEXT("[UNKNOWN] ");
	}
}
