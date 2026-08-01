#include <memory>
#include <string>
#include "rc.h"

struct Configs
{
    std::string nameFileLog;
    short port;
};

class Configurator
{
public:
    Configurator( std::string configFile );

    ReturnCodes ReadConfig();

    Configs GetConfigs() const;

private:
    Configs configs_;
    std::string configFile_;

    void SetConfigs( const Configs configs );
};

using ConfiguratorPtr = std::unique_ptr< Configurator >;