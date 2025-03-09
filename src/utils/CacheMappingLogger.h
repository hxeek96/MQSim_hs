#ifndef CACHE_MAPPING_LOGGER_H
#define CACHE_MAPPING_LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <cstdint>

namespace Utils
{
    class CacheMappingLogger 
    {
    public:
        static CacheMappingLogger& GetInstance() {
            static CacheMappingLogger instance;
            return instance;
        }

        ~CacheMappingLogger() {
            if (logFile.is_open()) {
                logFile.close();
            }
        }

        void Initialize(const std::string& filename = "cache_mapping_log.txt") {
            std::lock_guard<std::mutex> lock(mutex);
            if (logFile.is_open()) {
                logFile.close();
            }
            logFile.open(filename, std::ios::out | std::ios::trunc);
            if (!logFile.is_open()) {
                std::cerr << "Failed to open log file: " << filename << std::endl;
            }
        }

        template<typename StreamIdType, typename LPAType, typename PPAType>
        void LogDirtyEntry(uint64_t time, StreamIdType stream_id, LPAType lpa, PPAType new_ppa, PPAType prev_ppa) {
            std::lock_guard<std::mutex> lock(mutex);
            if (logFile.is_open()) {
                logFile << "Time [" << time << "]: Dirty Entry: LPA " << lpa << " → PPA " << new_ppa << " (Prev PPA " << prev_ppa << ")" << std::endl;
            }
        }

        template<typename StreamIdType, typename LPAType, typename PPAType>
        void LogEvictedEntry(uint64_t time, StreamIdType stream_id, LPAType lpa, PPAType ppa) {
            std::lock_guard<std::mutex> lock(mutex);
            if (logFile.is_open()) {
                logFile << "Time [" << time << "]: Evicted Entry: LPA " << lpa << " → PPA " << ppa << std::endl;
            }
        }

        // 로그 파일을 CSV 형식으로 내보내는 기능을 나중에 추가할 수 있음
        void ExportToCSV(const std::string& filename = "cache_mapping_log.csv") {
            // 이 기능은 필요에 따라 구현할 수 있음
        }

    private:
        CacheMappingLogger() {}  // Private 생성자로 싱글톤 패턴
        CacheMappingLogger(const CacheMappingLogger&) = delete;
        CacheMappingLogger& operator=(const CacheMappingLogger&) = delete;

        std::ofstream logFile;
        std::mutex mutex;  // 멀티스레딩 환경에서 로깅 동기화를 위한 뮤텍스
    };
}

#endif // CACHE_MAPPING_LOGGER_H 