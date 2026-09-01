from conans import ConanFile, CMake

class NetworkBrokerConan( ConanFile ):
    name = "NetworkBroker"
    version = "1.0.0" # Сделать глобальную перепенную версии.
    settings = "os", "compiler", "build_type", "arch"

    generators = "cmake"

    def requirements( self ):

        # Пример добавления классической зависимости
        # self.requires("poco/1.12.4")
        self.requires( "nlohmann_json/3.11.3" )
        self.requires( "boost/1.86.0" )
        pass

    def build( self ):
        cmake = CMake( self )
        cmake.configure()
        cmake.build()