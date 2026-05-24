PROJECT_NAME = inventory-api
NPROCS ?= $(shell nproc)
CLANG_FORMAT ?= clang-format
DOCKER_IMAGE ?= ghcr.io/userver-framework/ubuntu-24.04-userver:latest
CMAKE_OPTS ?=
DOCKER_ARGS = $(shell /bin/test -t 0 && /bin/echo -it || echo)
PRESETS ?= debug release

.PHONY: all
all: test-debug test-release

$(addprefix cmake-, $(PRESETS)): cmake-%:
	cmake --preset $* $(CMAKE_OPTS)

$(addsuffix /CMakeCache.txt, $(addprefix build-, $(PRESETS))): build-%/CMakeCache.txt:
	$(MAKE) cmake-$*

$(addprefix build-, $(PRESETS)): build-%: build-%/CMakeCache.txt
	cmake --build build-$* -j $(NPROCS) --target $(PROJECT_NAME)

$(addprefix test-, $(PRESETS)): test-%: build-%/CMakeCache.txt
	cmake --build build-$* -j $(NPROCS)
	cd build-$* && ((test -t 1 && GTEST_COLOR=1 PYTEST_ADDOPTS="--color=yes" ctest -V) || ctest -V)

$(addprefix start-, $(PRESETS)): start-%:
	cmake --build build-$* -v --target start-$(PROJECT_NAME)

$(addprefix clean-, $(PRESETS)): clean-%:
	cmake --build build-$* --target clean

.PHONY: dist-clean
dist-clean:
	rm -rf build*
	rm -rf tests/__pycache__/
	rm -rf tests/.pytest_cache/
	rm -rf .ccache
	rm -rf .vscode/.cache
	rm -rf .vscode/compile_commands.json

$(addprefix install-, $(PRESETS)): install-%: build-%
	cmake --install build-$* -v --component $(PROJECT_NAME)

.PHONY: install
install: install-release

.PHONY: format
format:
	find src -name '*pp' -type f | xargs $(CLANG_FORMAT) -i
	find tests -name '*.py' -type f | xargs autopep8 -i

$(addprefix docker-cmake-, $(PRESETS)) $(addprefix docker-build-, $(PRESETS)) $(addprefix docker-test-, $(PRESETS)) $(addprefix docker-clean-, $(PRESETS)): docker-%:
	docker run $(DOCKER_ARGS) \
		--network=host \
		-v $$PWD:$$PWD \
		-w $$PWD \
		$(DOCKER_IMAGE) \
		env CCACHE_DIR=$$PWD/.ccache \
		    HOME=$$HOME \
		    $$PWD/run_as_user.sh $(shell /bin/id -u) $(shell /bin/id -g) make $*