#pragma once

#include "fwd.h"

/// @file i_password_verifier.h
/// 
/// @brief Интерфейс обработки хеша пароля.
///


/// @brief Абстрактный класс обработки хеша пароля.
class IPasswordVerifier 
{
public:
    /// @brief Конструктор.
    IPasswordVerifier() = default;

    /// @brief Проверка хеша пароля для пользователя.
    /// @param[in] passwd Строка пароля пользователя.
    /// @param[in] reference Референс хеша.
    /// @return true - успех, false - неудачно.
    virtual bool CheckHash( const std::string& passwd, const std::string& reference ) = 0;

    /// @brief Создание хеша пароля.
    /// @param[in] passwd Строка пароля.
    /// @return Хеш пароля.
    virtual std::string CreateHash( const std::string& passwd ) = 0;

};