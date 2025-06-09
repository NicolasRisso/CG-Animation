#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

// Definições específicas para Windows para evitar conflitos de headers
#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX  // Evita conflitos com min/max do Windows
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN  // Reduz inclusões do Windows.h
    #endif
    #include <windows.h>
#endif

#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <functional>
#include <chrono>
#include <future> // Para std::future
#include <map>    // Para std::map

// Incluir thread após windows.h para evitar conflitos
#include <thread>

/**
 * @class ThreadPool
 * @brief Classe para gerenciamento de pool de threads para processamento paralelo
 */
class ThreadPool {
public:
    /**
     * @brief Construtor
     * @param numThreads Número de threads no pool
     */
    ThreadPool(size_t numThreads) : m_stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            m_workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(m_queueMutex);
                        m_condition.wait(lock, [this] { 
                            return m_stop || !m_tasks.empty(); 
                        });
                        
                        if (m_stop && m_tasks.empty()) {
                            return;
                        }
                        
                        task = std::move(m_tasks.front());
                        m_tasks.pop();
                    }
                    
                    task();
                }
            });
        }
    }
    
    /**
     * @brief Destrutor
     */
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_stop = true;
        }
        
        m_condition.notify_all();
        
        for (std::thread &worker : m_workers) {
            worker.join();
        }
    }
    
    /**
     * @brief Adiciona uma tarefa ao pool
     * @param f Função a ser executada
     * @param args Argumentos da função
     * @return Resultado futuro da execução
     */
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
        using return_type = typename std::invoke_result<F, Args...>::type;
        
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        std::future<return_type> result = task->get_future();
        
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            if (m_stop) {
                throw std::runtime_error("enqueue em ThreadPool parado");
            }
            
            m_tasks.emplace([task]() { (*task)(); });
        }
        
        m_condition.notify_one();
        return result;
    }
    
private:
    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
    
    std::mutex m_queueMutex;
    std::condition_variable m_condition;
    bool m_stop;
};

/**
 * @class FrameProcessor
 * @brief Classe para processamento paralelo de frames
 */
class FrameProcessor {
public:
    /**
     * @brief Construtor
     * @param numThreads Número de threads para processamento
     */
    FrameProcessor(size_t numThreads = 0) 
        : m_threadPool(numThreads > 0 ? numThreads : std::thread::hardware_concurrency()) {
    }
    
    /**
     * @brief Processa frames em paralelo
     * @param totalFrames Número total de frames
     * @param frameProcessor Função de processamento para cada frame
     */
    void processFrames(int totalFrames, std::function<void(int)> frameProcessor) {
        std::vector<std::future<void>> results;
        
        // Enfileirar tarefas para cada frame
        for (int i = 0; i < totalFrames; ++i) {
            results.emplace_back(
                m_threadPool.enqueue(frameProcessor, i)
            );
        }
        
        // Aguardar conclusão de todas as tarefas
        for (auto& result : results) {
            result.get();
        }
    }
    
private:
    ThreadPool m_threadPool;
};

/**
 * @class PerformanceMonitor
 * @brief Classe para monitoramento de desempenho
 */
class PerformanceMonitor {
public:
    /**
     * @brief Inicia medição de tempo
     * @param operationName Nome da operação
     */
    void startOperation(const std::string& operationName) {
        m_startTimes[operationName] = std::chrono::high_resolution_clock::now();
    }
    
    /**
     * @brief Finaliza medição de tempo
     * @param operationName Nome da operação
     * @return Duração em milissegundos
     */
    double endOperation(const std::string& operationName) {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto startTime = m_startTimes[operationName];
        
        std::chrono::duration<double, std::milli> duration = endTime - startTime;
        
        m_operationTimes[operationName] = duration.count();
        return duration.count();
    }
    
    /**
     * @brief Imprime relatório de desempenho
     */
    void printReport() {
        std::cout << "=== Relatório de Desempenho ===" << std::endl;
        
        for (const auto& pair : m_operationTimes) {
            std::cout << pair.first << ": " << pair.second << " ms" << std::endl;
        }
        
        std::cout << "===============================" << std::endl;
    }
    
private:
    std::map<std::string, std::chrono::time_point<std::chrono::high_resolution_clock>> m_startTimes;
    std::map<std::string, double> m_operationTimes;
};

/**
 * @class MemoryManager
 * @brief Classe para gerenciamento otimizado de memória
 */
class MemoryManager {
public:
    /**
     * @brief Aloca buffer de memória
     * @param size Tamanho do buffer em bytes
     * @return Ponteiro para o buffer alocado
     */
    static void* allocateBuffer(size_t size) {
        // Usar alinhamento de memória para melhor desempenho
        void* ptr = nullptr;
        
#ifdef _WIN32
        ptr = _aligned_malloc(size, 16);
#else
        if (posix_memalign(&ptr, 16, size) != 0) {
            ptr = nullptr;
        }
#endif
        
        if (!ptr) {
            throw std::bad_alloc();
        }
        
        return ptr;
    }
    
    /**
     * @brief Libera buffer de memória
     * @param ptr Ponteiro para o buffer
     */
    static void freeBuffer(void* ptr) {
        if (ptr) {
#ifdef _WIN32
            _aligned_free(ptr);
#else
            free(ptr);
#endif
        }
    }
    
    /**
     * @brief Classe para gerenciamento automático de buffer
     */
    template<typename T>
    class Buffer {
    public:
        /**
         * @brief Construtor
         * @param size Número de elementos
         */
        Buffer(size_t size) : m_size(size) {
            m_data = static_cast<T*>(allocateBuffer(size * sizeof(T)));
        }
        
        /**
         * @brief Destrutor
         */
        ~Buffer() {
            freeBuffer(m_data);
        }
        
        /**
         * @brief Acesso aos dados
         * @return Ponteiro para os dados
         */
        T* data() { return m_data; }
        
        /**
         * @brief Acesso aos dados (const)
         * @return Ponteiro constante para os dados
         */
        const T* data() const { return m_data; }
        
        /**
         * @brief Tamanho do buffer
         * @return Número de elementos
         */
        size_t size() const { return m_size; }
        
    private:
        T* m_data;
        size_t m_size;
        
        // Proibir cópia
        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;
    };
};

#endif // OPTIMIZATION_H
