#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "Misc/DateTime.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/CriticalSection.h"
#include "Containers/Queue.h"

// ----------------------------------------------------------
// Logger.h
// ----------------------------------------------------------

/**
 * @brief 로그 레벨 정의
 */
enum class ELogLevel : uint8
{
	Debug,      /**< 디버그용 상세 로그 */
	Info,       /**< 일반 정보 로그 */
	Warning,    /**< 경고성 로그 */
	Error,      /**< 오류 로그 */
	Critical    /**< 치명적 오류 로그 */
};

/**
 * @brief 큐에 담길 로그 메시지 구조체
 */
struct FQueuedLogMessage
{
	FString Message;   /**< 실제 로그 메시지 */
	ELogLevel Level;   /**< 로그의 심각도 레벨 */
};

/**
 * @brief 백그라운드 스레드에서 로그를 파일로 비동기 저장하는 워커 클래스
 *
 * 이 클래스는 로그 메시지를 큐에 저장하고,
 * 별도 스레드에서 주기적으로 파일로 기록합니다.
 * MPSC(Multi-Producer Single-Consumer) 큐 구조를 사용해
 * 다중 스레드 환경에서도 안전하게 동작합니다.
 */
class FLoggerWorker : public FRunnable
{
public:
	FLoggerWorker();
	virtual ~FLoggerWorker() override;

	/**
	 * @brief 로그 메시지를 큐에 추가
	 * @param Message 로그 내용
	 * @param Level 로그 레벨
	 */
	void Enqueue(const FString& Message, ELogLevel Level);

	/**
	 * @brief 스레드 종료 요청
	 */
	void StopWorker();

private:
	// FRunnable 인터페이스 구현부
	virtual bool Init() override { return true; }
	virtual uint32 Run() override;
	virtual void Stop() override;

	/**
	 * @brief 오늘 날짜 기준 로그 파일 경로 반환
	 * @return 예: "Saved/Logs/2025-11-12.log"
	 */
	FString GetLogFilePath() const;

	/**
	 * @brief 현재 시간 문자열 반환
	 * @return 예: "[22:45:13]"
	 */
	FString GetTimestamp() const;

	/**
	 * @brief 로그 레벨을 문자열로 변환
	 * @param Level 로그 레벨
	 * @return 예: "Debug", "Warning", "Error"
	 */
	FString LogLevelToString(ELogLevel Level) const;

private:
	TQueue<FQueuedLogMessage, EQueueMode::Mpsc> LogQueue;  /**< 로그 메시지를 담는 큐 */
	FThreadSafeBool bStopThread = false;                   /**< 스레드 종료 요청 플래그 */
	FRunnableThread* Thread = nullptr;                     /**< 실제 실행 스레드 포인터 */
	FCriticalSection CriticalSection;                      /**< 파일 접근 동기화를 위한 뮤텍스 */
};

// ----------------------------------------------------------

/**
 * @brief 전체 시스템에서 사용하는 싱글톤 Logger 클래스
 *
 * - `UE_LOG`로 콘솔/에디터 로그 출력
 * - 동시에 비동기 워커(FLoggerWorker)를 통해 파일로 저장
 *
 * @usage
 * ```cpp
 * Logger::Get().Log(TEXT("게임이 시작되었습니다."), ELogLevel::Debug);
 * Logger::Get().Log(TEXT("AI 캐릭터가 플레이어를 인식했습니다."), ELogLevel::Warning);
 * Logger::Get().Log(TEXT("Vehicle 컴포넌트 초기화 실패!"), ELogLevel::Error);
 * ```
 */
class ARCANUM_API Logger
{
public:
	/**
	 * @brief 싱글톤 인스턴스 반환
	 */
	static Logger& Get()
	{
		static Logger Instance;
		return Instance;
	}

	/**
	 * @brief 로그 기록
	 * @param Message 출력할 메시지
	 * @param Level 로그 레벨 (기본값: Debug)
	 */
	void Log(const FString& Message, ELogLevel Level = ELogLevel::Debug)
	{
		switch (Level)
		{
		case ELogLevel::Debug:    UE_LOG(LogTemp, Display, TEXT("%s"), *Message); break;
		case ELogLevel::Warning:  UE_LOG(LogTemp, Warning, TEXT("%s"), *Message); break;
		case ELogLevel::Error:
		case ELogLevel::Critical: UE_LOG(LogTemp, Error, TEXT("%s"), *Message); break;
		default: break;
		}

		// 파일 쓰기는 비동기 큐에 추가
		Worker.Enqueue(Message, Level);
	}

	/**
	 * @brief 로그 명시적 종료
	 */
	static void Shutdown()
	{
		Get().Worker.StopWorker();
	}
private:
	Logger() {}
	~Logger() { Worker.StopWorker(); }

private:
	FLoggerWorker Worker; /**< 백그라운드 로그 처리용 워커 */
};