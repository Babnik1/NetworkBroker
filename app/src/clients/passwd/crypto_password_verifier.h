#pragma once

/// @file crypto_password_verifier.h
///
/// @brief Обработка хеша пароля.
///

#include "i_password_verifier.h"
#include "fwd.h"

/// @brief Класс обработки хешей паролей.
class CryptoPasswordVerifier : public IPasswordVerifier
{
public:
    /// @brief Конструктор.
    CryptoPasswordVerifier() = default;

    /// @copydoc IPasswordVerifier::CheckHash
    virtual bool CheckHash( const std::string& passwd, const std::string& reference );

    /// @copydoc IPasswordVerifier::CreateHash
    virtual std::string CreateHash( const std::string& passwd );

};
