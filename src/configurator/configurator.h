/// @file configurator.h
///
/// Загрузчик конфигурации. Объявление.
///

#include <memory>
#include <string>
#include "rc.h"



/// @brief Структура конфигурации.
struct Configs
{
    std::string nameFileLog;
    std::string clientDb;
    std::string topicDb;
    std::string loglevel;
    short port;
};

/// @brief Класс конфигуратора.
class Configurator
{
public:
    /// @brief Конструктор.
    /// @param[in] configFile Путь к конфигурации.
    Configurator( std::string configFile );

    /// @brief Чтение конфигурации из файла.
    /// @return Возвращает код выполнения. 0 = успех.
    ConfigCodes ReadConfig();

    /// @brief Получить конфигурацию.
    /// @return Возвращает структуру конфигурации.
    Configs GetConfigs() const;

private:
    Configs configs_;               /// Структура конфигурации.
    std::string configFile_;        /// Путь к конфигурационному файлу.

    /// @brief Установка значения конфигурации.
    /// @param[in] configs Структура конфигурации.
    void SetConfigs( const Configs configs );
};

using ConfiguratorPtr = std::unique_ptr< Configurator >;
