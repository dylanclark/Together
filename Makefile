# set project directory one level above the Makefile directory. $(CURDIR) is a
# GNU make variable containing the path to the current working directory
PROJDIR := $(realpath $(CURDIR)/..)
SOURCEDIR := $(realpath $(CURDIR)/code)
BUILDDIR := $(PROJDIR)/build

# executable name
TARGET = together

# show commands?
VERBOSE = FALSE

# create the list of directories. update this when new source files are added
DIRS = main characters engine levels menus objects sound states textures
SOURCEDIRS = $(foreach dir, $(DIRS), $(addprefix $(SOURCEDIR)/src/, $(dir)))
TARGETDIRS = $(foreach dir, $(DIRS), $(addprefix $(BUILDDIR)/object-files/, $(dir)))

# Generate the GCC includes parameters by adding -I before each source folder
INCLUDES = $(addsuffix /include, $(SOURCEDIR))

# add this list to VPATH, the place make will look for the source files
VPATH = $(SOURCEDIRS)

# Create a list of *.c sources in DIRS
SOURCES = $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.cpp))

# Define objects for all sources
OBJS := $(subst $(SOURCEDIR),$(BUILDDIR),$(subst /src/,/object-files/,$(SOURCES:.cpp=.o)))

# Name the compiler
CC = g++

# compiler flags
CFLAGS = -lstdc++ -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# OS specific part
ifeq ($(OS),Windows_NT)
	RM = del /F /Q
	RMDIR = -RMDIR /S /Q
	MKDIR = -mkdir
	ERRIGNORE = 2>NUL || true
	SEP=\\
else
	RM = rm -rf
	RMDIR = rm -rf
	MKDIR = mkdir -p
	ERRIGNORE = 2>/dev/null
	SEP=/
endif

# Remove space after separator
PSEP = $(strip $(SEP))

# Hide or not the calls depending of VERBOSE
ifeq ($(VERBOSE),TRUE)
	HIDE =
else
	HIDE = @
endif

# Define the function that will generate each rule
define generateRules
$(1)/%.o: %.cpp
	@echo Building $$(subst $$(SOURCEDIR)/src/,,$$<)
	$(HIDE)$(CC) -c -g -I$$(INCLUDES) -I/usr/local/include -o $$(subst /,$$(PSEP),$$@) $$(subst /,$$(PSEP),$$<) -MMD
endef

# indicate to make which targets are not files
.PHONY: all ../build/resources resources clean directories

all: directories $(BUILDDIR)/$(TARGET) ../build/resources

$(BUILDDIR)/$(TARGET): $(OBJS)
	$(HIDE)echo Linking $(subst $(BUILDDIR)/,build/,$@)
	$(HIDE)$(CC) $(OBJS) -o $@ $(CFLAGS)

# Generate rules
$(foreach targetdir, $(TARGETDIRS), $(eval $(call generateRules, $(targetdir))))

directories:
	$(HIDE)$(MKDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)

../build/resources: resources
	$(HIDE)rsync -rupE resources ../build/

# Remove all objects, dependencies and executable files generated during the build
clean:
	$(HIDE)$(RMDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)
	$(HIDE)$(RM) $(TARGET) $(ERRIGNORE)
	@echo Cleaning done!
