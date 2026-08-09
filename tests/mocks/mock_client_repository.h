#pragma once

#include "db/i_client_repository.h"

class MockClientRepository : public IClientRepository
{
public:
    std::vector< Client > LoadClients() override
    {
        return clients_;
    }

    bool SaveClient( const Client& client ) override
    {
        if ( !saveResult_ )
        {
            return false;
        }

        clients_.push_back( client );
        return true;
    }

    bool DeleteClient( ClientId id ) override
    {
        if ( !deleteResult_ )
        {
            return false;
        }

        auto it = std::remove_if(
            clients_.begin(),
            clients_.end(),
            [ id ]( const Client& client )
            {
                return client.GetId() == id;
            } );

        if ( it == clients_.end() )
        {
            return false;
        }

        clients_.erase( it, clients_.end() );
        return true;
    }

    void SetSaveResult( bool result )
    {
        saveResult_ = result;
    }

    void SetDeleteResult( bool result )
    {
        deleteResult_ = result;
    }

    void AddClient( const Client& client )
    {
        clients_.push_back( client );
    }

private:
    std::vector< Client > clients_;
    bool saveResult_ = true;
    bool deleteResult_ = true;
};