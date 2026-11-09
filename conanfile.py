from conans import ConanFile, CMake

class NetworkBrokerConan( ConanFile ):
    name = "NetworkBroker"
    version = "1.0.0" # @todo Сделать глобальную переменную версии.
    settings = "os", "compiler", "build_type", "arch"

    generators = "cmake"

    def configure( self ):
        self.options[ "boost" ].without_test = True

    def requirements( self ):
        self.requires( "bzip2/1.0.8" )
        self.requires( "nlohmann_json/3.11.3" )
        self.requires( "boost/1.86.0" )
        self.requires( "gtest/1.15.0")
        self.requires( "openssl/3.3.2" )
        pass

    def build( self ):
        cmake = CMake( self )
        cmake.configure()
        cmake.build()