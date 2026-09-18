#include "crypto_password_verifier.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

bool CryptoPasswordVerifier::CheckHash( const std::string& passwd, const std::string& reference )
{
    if ( passwd.empty() )
    {
        return false;
    }
    std::string checkHash = CreateHash( passwd );
    return checkHash == reference;
}

std::string CryptoPasswordVerifier::CreateHash( const std::string& passwd )
{
    unsigned char hash[ SHA256_DIGEST_LENGTH ];
    SHA256_CTX sha256;
    SHA256_Init( &sha256 );
    SHA256_Update( &sha256, passwd.c_str(), passwd.size() );
    SHA256_Final( hash, &sha256 );

    std::stringstream ss;
    for ( int i = 0; i < SHA256_DIGEST_LENGTH; i++ )
    {
        ss << std::hex << std::setw( 2 ) << std::setfill( '0' ) << static_cast< int >( hash[ i ] );
    }
    return ss.str();
}