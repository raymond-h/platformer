CFLAGS = $(INCLUDES)
CXXFLAGS =-O3 -Wall -Wno-sign-compare -Wno-narrowing -std=c++11 $(DEFINES) $(INCLUDES)
CXX = LANG=C g++

MKDIR=mkdir -p

src_to_obj = $(patsubst $(1)/%.c,$(BIN_DIR)/$(1)/%.o,$(wildcard $(1)/*.c)) $(patsubst $(1)/%.cpp,$(BIN_DIR)/$(1)/%.o,$(wildcard $(1)/*.cpp))
src_to_deps = $(patsubst $(1)/%.c,$(DEP_DIR)/$(1)/%.d,$(wildcard $(1)/*.c)) $(patsubst $(1)/%.cpp,$(DEP_DIR)/$(1)/%.d,$(wildcard $(1)/*.cpp))

OBJS =  $(foreach dir,$(SRC_DIRS),$(call src_to_obj,$(dir)))
DEPS = $(foreach dir,$(SRC_DIRS),$(call src_to_deps,$(dir)))

TILEDPP_PREFIX = D:/Eclipse-Workspace/TiledPP
TILEDPP_LIB_DEBUG = $(TILEDPP_PREFIX)/Debug
TILEDPP_LIB = $(TILEDPP_PREFIX)/Release
TILEDPP_INCLUDE = $(TILEDPP_PREFIX)/src

INCLUDES =-I. -Ilibs/include -I$(TILEDPP_INCLUDE)

LIBS = -Llibs/lib -L$(TILEDPP_LIB_DEBUG) -lTiledPP -ltinyxml2 -lticppd -lz $(SDL_LIBS) $(BOOST_LIBS)

SDL_LIBS = -lSDL -lSDL_image

BOOST_LIBS = -lboost_filesystem-mgw47-mt-sd-1_52 -lboost_thread-mgw47-mt-sd-1_52 \
		-lboost_system-mgw47-mt-sd-1_52 -lboost_chrono-mgw47-mt-sd-1_52

### Parameters
TARGET = platformer.exe

DEFINES = -D__GXX_EXPERIMENTAL_CXX0X__ -DBOOST_THREAD_USE_LIB -DRES_DIR=\"$(RES_DIR)\" -DMAPS_DIR=\"$(MAPS_DIR)\"

# Folders
SRC_DIRS = src src/util
BIN_DIR = bin
DEP_DIR = dep

RES_DIR = res
MAPS_DIR = maps

.PHONY: all clean clean-bin clean-dep rebuild run package-all src-archive dist

all: $(TARGET)

$(TARGET): $(OBJS)
	@$(MKDIR) $(@D)
	$(CXX) -o $@ $^ -static-libgcc -static-libstdc++ $(LIBS)

clean: clean-bin clean-dep

clean-bin:
	rm -rf $(BIN_DIR) $(TARGET)

clean-dep:
	rm -rf $(DEP_DIR)

rebuild: clean all

rebuild-bin: clean-bin all

clean-exe:
	rm -rf $(TARGET)

relink: clean-exe $(TARGET)

run: $(TARGET)
	@echo -----
	@./$<

package-all: src-archive dist

src-archive: $(SRC_DIRS)/* $(RES_DIR)/* $(MAPS_DIR)/* ./*.dll Makefile
	7z a -r src.zip $^

dist: $(TARGET) $(RES_DIR)/* $(MAPS_DIR)/*.tmx ./*.dll
	7z a -r dist.zip $^

### File generation rules
# Object files
$(BIN_DIR)/%.o: %.c
	@$(MKDIR) $(@D)
	$(C) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/%.o: %.cpp
	@$(MKDIR) $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Dependency files
$(DEP_DIR)/%.d: %.c
	@$(MKDIR) $(@D)
	$(C) $(CFLAGS) -MM -MT"$(patsubst %.c,$(BIN_DIR)/%.o,$<)" -MT"$@" -MF"$@" $<

$(DEP_DIR)/%.d: %.cpp
	@$(MKDIR) $(@D)
	$(CXX) $(CXXFLAGS) -MM -MT"$(patsubst %.cpp,$(BIN_DIR)/%.o,$<)" -MT"$@" -MF"$@" $<

# DEPENDENCIES
ifneq ($(MAKECMDGOALS),clean)
  -include $(DEPS)
endif