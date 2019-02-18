########## defaut ##########

CC      = g++ --std=c++14 -fPIC -no-pie
DEBUG   = -g -Wall
CFLAGS  = -c
CFLAGS_NO_C  = -Wall $(DEBUG)
LFLAGS  = -Wall

#-I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include
################################################################################
# ROOT Libs and links
################################################################################
GLIB         = `pkg-config --cflags --libs glib-2.0`
ROOT_LINKS   = `root-config --cflags --glibs`
ROOFIT       = -lRooFit -lHtml -lMinuit -lRooFitCore -lRooStats
ROOT_LIBS    = -L${ROOTSYS}/lib -lTreePlayer -lCore
ROOT_INCLUDE = -I${ROOTSYS}/include
BOOST_LIB    = -L/usr/include/boost/property_tree

################################################################################
# top level folder path
################################################################################
BASE_PATH = /home/kuro/Codes/HGTD_BetaScope/BetaScopeAnalysis
MAIN_DIR = ./main
BIN_DIR = ./bin

################################################################################
# Waveform Analysis Dir & Src
################################################################################
WAVEFORM_ANALYSIS_INCLUDE_DIR = ./WaveformAnalysis/include
WAVEFORM_ANALYSIS_SRC_DIR = ./WaveformAnalysis/src
WAVEFORM_ANALYSIS_BUILD_DIR = ./WaveformAnalysis/build
WAVEFORM_ANALYSIS_SRC_FILES = $(wildcard $(WAVEFORM_ANALYSIS_SRC_DIR)/*.cpp)
WAVEFORM_ANALYSIS_BUILD_FILES = $(patsubst $(WAVEFORM_ANALYSIS_SRC_DIR)/%.cpp, $(WAVEFORM_ANALYSIS_BUILD_DIR)/%.o, $(WAVEFORM_ANALYSIS_SRC_FILES))

################################################################################
# BetaScope Dir & Src
################################################################################
BETA_SCOPE_INCLUDE_DIR = BetaScope/include
BETA_SCOPE_SRC_DIR = BetaScope/src
BETA_SCOPE_BUILD_DIR = BetaScope/build
BETA_SCOPE_SRC_FILES = $(wildcard $(BETA_SCOPE_SRC_DIR)/*.cpp)
BETA_SCOPE_BUILD_FILES = $(patsubst $(BETA_SCOPE_SRC_DIR)/%.cpp, $(BETA_SCOPE_BUILD_DIR)/%.o, $(BETA_SCOPE_SRC_FILES))

################################################################################
# BetaScope configuration parser Dir & Src
################################################################################
BETA_CONFIG_PARSER_INCLUDE_DIR = BetaScope_ConfigParser/include
BETA_CONFIG_PARSER_SRC_DIR = BetaScope_ConfigParser/src
BETA_CONFIG_PARSER_BUILD_DIR = BetaScope_ConfigParser/build
BETA_CONFIG_PARSER_SRC_FILES = $(wildcard $(BETA_CONFIG_PARSER_SRC_DIR)/*.cpp)
BETA_CONFIG_PARSER_BUILD_FILES = $(patsubst $(BETA_CONFIG_PARSER_SRC_DIR)/%.cpp, $(BETA_CONFIG_PARSER_BUILD_DIR)/%.o, $(BETA_CONFIG_PARSER_SRC_FILES))


################################################################################
# Custom function
################################################################################
define PRINT_BUILD
	@printf "\n \033[0;33m Builidng $(1) \e[0m\n"
endef
#@printf "\033[0;33m=%.0s\e[0m" {1..100}
#@printf "\n \033[0;33m Builidng $@ \e[0m"


#===============================================================================
# Make exe

all:$(BIN_DIR)/generateBetaConfig.exe \
		$(BIN_DIR)/simpleBetaRun.exe

SIMPLE_BETA_RUN_OBJ = $(WAVEFORM_ANALYSIS_BUILD_FILES)
SIMPLE_BETA_RUN_OBJ += $(BASE_PATH)/$(BETA_SCOPE_BUILD_DIR)/libTimeWindow_Dict.so
SIMPLE_BETA_RUN_OBJ += $(BETA_CONFIG_PARSER_BUILD_FILES)
SIMPLE_BETA_RUN_OBJ += $(BETA_SCOPE_BUILD_FILES)
$(BIN_DIR)/simpleBetaRun.exe: $(MAIN_DIR)/main_simpleBetaRun.cpp $(SIMPLE_BETA_RUN_OBJ)
	$(call PRINT_BUILD,$@)
	$(CC) $(DEBUG) $(LFLAGS) $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(GLIB) $(BOOST_LIB)
	@printf "\n"

$(BIN_DIR)/generateBetaConfig.exe: $(MAIN_DIR)/main_generateBetaConfig.cpp $(BETA_CONFIG_PARSER_BUILD_FILES)
	$(call PRINT_BUILD,$@)
	$(CC) $(DEBUG) $(LFLAGS) $^ -o $@ $(GLIB) $(BOOST_LIB)
	@printf "\n"


#===============================================================================


#*******************************************************************************
# Build Waveform Analsyis object files.
#*******************************************************************************
$(WAVEFORM_ANALYSIS_BUILD_DIR)/%.o: $(WAVEFORM_ANALYSIS_SRC_DIR)/%.cpp
	$(call PRINT_BUILD,$@)
	$(CC) $(DEBUG) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)
	@printf "\n"


#*******************************************************************************
# Build BetaScope custom shared library.
#*******************************************************************************
$(BASE_PATH)/$(BETA_SCOPE_BUILD_DIR)/TimeWindow_Dict.cxx:
	$(call PRINT_BUILD,$(BASE_PATH)/$(BETA_SCOPE_BUILD_DIR)/TimeWindow_Dict.cxx)
	rootcint -f $(BASE_PATH)/$(BETA_SCOPE_BUILD_DIR)/TimeWindow_Dict.cxx -c $(BASE_PATH)/$(BETA_SCOPE_INCLUDE_DIR)/TimeWindow.hpp $(BASE_PATH)/$(BETA_SCOPE_INCLUDE_DIR)/LinkDef.h

$(BASE_PATH)/$(BETA_SCOPE_BUILD_DIR)/TimeWindow.o: $(BASE_PATH)/$(BETA_SCOPE_SRC_DIR)/TimeWindow.cxx
	$(CC) $(DEBUG) $(CFLAGS) $< -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)
	@printf "\n"

$(BASE_PATH)/$(BETA_SCOPE_BUILD_DIR)/TimeWindow_Dict.o: $(BASE_PATH)/$(BETA_SCOPE_BUILD_DIR)/TimeWindow_Dict.cxx
	$(CC) $(DEBUG) $(CFLAGS) $< -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)
	@printf "\n"

$(BASE_PATH)/$(BETA_SCOPE_BUILD_DIR)/libTimeWindow_Dict.so: $(BASE_PATH)/$(BETA_SCOPE_BUILD_DIR)/TimeWindow_Dict.o $(BASE_PATH)/$(BETA_SCOPE_BUILD_DIR)/TimeWindow.o
	$(CC) -shared $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)
	@printf "\033[0;33m $@ is created \e[0m \n"


#*******************************************************************************
# Build BetaScope object files.
#*******************************************************************************
$(BETA_SCOPE_BUILD_DIR)/%.o: $(BETA_SCOPE_SRC_DIR)/%.cpp
	$(call PRINT_BUILD,$@)
	$(CC) $(DEBUG) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)
	@printf "\n"

#*******************************************************************************
# Build BetaScope config parser object files.
#*******************************************************************************
$(BETA_CONFIG_PARSER_BUILD_DIR)/%.o: $(BETA_CONFIG_PARSER_SRC_DIR)/%.cpp
	$(call PRINT_BUILD,$@)
	$(CC) $(DEBUG) -c $^ -o $@ $(GLIB) $(BOOST_LIB)
	@printf "\n"













#========= find all the content in a dirctory ==========

$(BUILD)$(EXTRACT_CONTENT_FROM_DIRECTORY)extract_content_from_directory.o: $(SRC)$(EXTRACT_CONTENT_FROM_DIRECTORY)$(CPP)main_extract_content_from_directory.cpp
	$(CC) $(DEBUG) $(CFLAGS) $< -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(GLIB)

EXTRACT_CONTENT_FROM_DIRECTORY_DEP := $(BUILD)$(EXTRACT_CONTENT_FROM_DIRECTORY)extract_content_from_directory.o


#================================================================================================
#================================================================================================

#========= beta scope analysis configuration ==========

$(BUILD)$(GLIB_CONFIG)ini_reader.o: $(SRC)$(GLIB_CONFIG)$(CPP)ini_reader.cpp
	$(CC) $(DEBUG) $(CFLAGS) $< -o $@ $(GLIB)

GLIB_CONFIG_DEP += $(BUILD)$(GLIB_CONFIG)ini_reader.o

$(BIN)create_beta_config: $(SRC)$(GLIB_CONFIG)$(CPP)main_create_beta_config.cpp $(GLIB_CONFIG_DEP)
	$(CC) $(LFLAGS) $^ -o $@ $(GLIB)


#================================================================================================
#================================================================================================

#========= ini reader with boost lib ==========

$(BIN)ini_reader_boost: $(SRC)$(INI_BOOST)$(CPP)ini_reader_with_boost.cpp
	$(CC) $(LFLAGS) $^ -o $@ $(GLIB) $(BOOST_LIB)


#================================================================================================
#================================================================================================

#========= generate measuremnt information template ==========

$(BUILD)$(MEASUREMENT_INFO)generate_run_info.o: $(SRC)$(MEASUREMENT_INFO)$(CPP)generate_run_info.cpp
	$(CC) $(DEBUG) $(CFLAGS) $< -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)

$(BUILD)$(MEASUREMENT_INFO)measurement_info.o: $(SRC)$(MEASUREMENT_INFO)$(CPP)measurement_info.cpp
	$(CC) $(DEBUG) $(CFLAGS) $< -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)

measurement_info_dep += $(BUILD)$(MEASUREMENT_INFO)generate_run_info.o
measurement_info_dep += $(BUILD)$(MEASUREMENT_INFO)measurement_info.o

measurement_info_cpp := $(SRC)$(MEASUREMENT_INFO)$(CPP)main_measurement_info.cpp
$(BIN)measurement_info: $(measurement_info_cpp) $(measurement_info_dep)
	$(CC) $(LFLAGS) $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)


#================================================================================================
#================================================================================================

#========= data selectioon

$(BUILD)$(DATA_SELECTION)dataSelection.o: $(SRC)$(DATA_SELECTION)$(CPP)dataSelection.cpp
	$(CC) $(DEBUG) $(CFLAGS) $< -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)


#================================================================================================
#================================================================================================

#========= Beta scope analysis

$(BASE_PATH)$(BUILD)$(BETA_SCOPE_ANALYSIS)TimeWindow_Dict.cxx:
	rootcint -f $(BASE_PATH)$(BUILD)$(BETA_SCOPE_ANALYSIS)TimeWindow_Dict.cxx -c $(BASE_PATH)$(SRC)$(BETA_SCOPE_ANALYSIS)$(HPP)TimeWindow.hpp	$(BASE_PATH)$(SRC)$(BETA_SCOPE_ANALYSIS)$(HPP)LinkDef.h

$(BUILD)$(BETA_SCOPE_ANALYSIS)TimeWindow.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)TimeWindow.cxx
	$(CC) $(DEBUG) $(CFLAGS) $< -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)

$(BASE_PATH)$(BUILD)$(BETA_SCOPE_ANALYSIS)TimeWindow_Dict.o: $(BASE_PATH)$(BUILD)$(BETA_SCOPE_ANALYSIS)TimeWindow_Dict.cxx
	$(CC) $(DEBUG) $(CFLAGS) $< -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)

$(BASE_PATH)$(BUILD)$(BETA_SCOPE_ANALYSIS)libTimeWindow_Dict.so: $(BASE_PATH)$(BUILD)$(BETA_SCOPE_ANALYSIS)TimeWindow_Dict.o $(BUILD)$(BETA_SCOPE_ANALYSIS)TimeWindow.o
	$(CC) -shared $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)

$(BUILD)$(BETA_SCOPE_ANALYSIS)betaAnalysis.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)betaAnalysis.cpp
	$(CC) $(DEBUG) $(CFLAGS) $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)

$(BUILD)$(BETA_SCOPE_ANALYSIS)methods.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)methods.cpp
	$(CC) $(DEBUG) $(CFLAGS) $< -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)

$(BUILD)$(BETA_SCOPE_ANALYSIS)setBranch.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)setBranch.cpp
	$(CC) $(DEBUG) $(CFLAGS) $< -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)

$(BUILD)$(BETA_SCOPE_ANALYSIS)fillData.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)fillData.cpp
	$(CC) $(DEBUG) $(CFLAGS) $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)

$(BUILD)$(BETA_SCOPE_ANALYSIS)clearBuffer.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)clearBuffer.cpp
	$(CC) $(DEBUG) $(CFLAGS) $< -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)

$(BUILD)$(BETA_SCOPE_ANALYSIS)saveFile.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)saveFile.cpp
	$(CC) $(DEBUG) $(CFLAGS) $< -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)

$(BUILD)$(BETA_SCOPE_ANALYSIS)betaAnalyzer.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)betaAnalyzer.cpp
	$(CC) $(DEBUG) $(CFLAGS) $< -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE)

betaScope_dep := $(BASE_PATH)$(BUILD)$(BETA_SCOPE_ANALYSIS)libTimeWindow_Dict.so
betaScope_dep += $(BUILD)$(BETA_SCOPE_ANALYSIS)betaAnalysis.o
betaScope_dep += $(BUILD)$(BETA_SCOPE_ANALYSIS)methods.o
betaScope_dep += $(BUILD)$(BETA_SCOPE_ANALYSIS)setBranch.o
betaScope_dep += $(BUILD)$(BETA_SCOPE_ANALYSIS)fillData.o
betaScope_dep += $(BUILD)$(BETA_SCOPE_ANALYSIS)clearBuffer.o
betaScope_dep += $(BUILD)$(BETA_SCOPE_ANALYSIS)saveFile.o
betaScope_dep += $(BUILD)$(BETA_SCOPE_ANALYSIS)betaAnalyzer.o
#betaScope_dep += $(BUILD)$(BETA_SCOPE_ANALYSIS)TimeWindow.o

simpleBetaRun_cpp := $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)main_simpleBetaRun.cpp
$(BIN)simpleBetaRun: $(simpleBetaRun_cpp) $(betaScope_dep) $(EXTRACT_CONTENT_FROM_DIRECTORY_DEP) $(GLIB_CONFIG_DEP)
	$(CC) $(DEBUG) $(LFLAGS) $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(GLIB)




#================================================================================================
#================================================================================================

.PHONY: clean
clean:
	@echo "cleaning .o files in build and binary files in bin."
	@for file in ./*/build/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in ./bin/*; do rm -rf $$file; echo Delete binary file: $$file; done
	@echo "finished"
