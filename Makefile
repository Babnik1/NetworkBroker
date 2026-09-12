MODE ?= Release

# Создание conan-профиля с правильным компилятором.
conan-profile:
	@conan profile list | grep -q "broker_profile" && echo "Conan profile 'broker_profile' already exists. Skipping creating profile..." || \
	( \
			echo "Creating broker_profile..." && \
			conan profile new broker_profile --detect && \
			conan profile update settings.compiler.libcxx=libstdc++11 broker_profile \
	)
	@echo "Setting ${MODE} mode..."
	@conan profile update settings.build_type=${MODE} broker_profile

# Загрузка и сборка зависимостей.
fetch:
	conan install . -if build -pr=broker_profile --build=missing

# Сборка проекта.
build:
	cmake -B build . && cmake --build build

# Собрать проект.
debug:
	$(eval MODE := Debug)
	@true

# Дефолтный путь сборки.
all: conan-profile fetch build

# Очистка.
clean:
	@rm -rf build
	@rm -rf Broker
	@rm -rf BrokerClient

.PHONY : conan-profile fetch build all debug