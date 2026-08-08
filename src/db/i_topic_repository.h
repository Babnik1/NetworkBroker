#pragma once
/// @file i_topic_repisitory.h
///
/// Интерфейс базы данных топиков.
///


#include "fwd.h"
#include <unordered_map>
#include <unordered_set>
#include <memory>


/// @brief Абстрактный класс базы данных топиков.
class ITopicRepository
{
public:

    /// @brief Конструктор.
    ITopicRepository() = default;

    /// @brief Сохранение клиента в топик в БД.
    /// @param[in] topic Название топика.
    /// @param[in] id ID клиента.
    /// @return false - неудача, true - успех.
    virtual bool SaveTopic( const Topic& topic, const ClientId id ) = 0;

    /// @brief Загрузка топиков из БД.
    /// @return Мапа топиков и их клиентов.
    virtual std::unordered_map< Topic, std::unordered_set< ClientId > > LoadTopics() = 0;

    /// @brief Создание топика.
    /// @param[in] topic Название топика.
    /// @param[in] id ID клиента.
    /// @details Сразу подписывает клиента, создавшего топик.
    /// @return false - неудача, true - успех.
    virtual bool CreateTopic( const Topic& topic, const ClientId id ) = 0;

    /// @brief Удаление клиента из топика.
    /// @param[in] topic Название топика.
    /// @param[in] id ID клиента.
    /// @return false - неудача, true - успех.
    virtual bool RemoveFromTopic( const Topic& topic, const ClientId id ) = 0;
};

using ITopicRepositoryPtr = std::unique_ptr< ITopicRepository >;