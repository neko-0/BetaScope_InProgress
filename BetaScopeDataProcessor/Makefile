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
BOOST_LIB    = -L/usr/include/boost/property_tree -L/usr/include/boost/ -lboost_thread -lboost_system

################################################################################
# top level folder path
################################################################################
BASE_PATH = /home/kuro/Codes/BetaScopeDataProcessor
MAIN_DIR = ./main
BIN_DIR = ./bin


################################################################################
# Data output format Dir & Src
################################################################################
DATA_OUTPUT_FORMAT_INCLUDE_DIR = ./DataOutputFormat/include
DATA_OUTPUT_FORMAT_SRC_DIR = ./DataOutputFormat/src
DATA_OUTPUT_FORMAT_BUILD_DIR = ./DataOutputFormat/build
DATA_OUTPUT_FORMAT_SRC_FILES = $(wildcard $(DATA_OUTPUT_FORMAT_SRC_DIR)/*.cpp)
DATA_OUTPUT_FORMAT_BUILD_FILES = $(patsubst $(DATA_OUTPUT_FORMAT_SRC_DIR)/%.cpp, $(DATA_OUTPUT_FORMAT_BUILD_DIR)/%.o, $(DATA_OUTPUT_FORMAT_SRC_FILES))

################################################################################
# Data processing configuration Dir & Src
################################################################################
DATA_PROCESSING_CONFIG_INCLUDE_DIR = ./DataProcessingConfig/include
DATA_PROCESSING_CONFIG_SRC_DIR = ./DataProcessingConfig/src
DATA_PROCESSING_CONFIG_BUILD_DIR = ./DataProcessingConfig/build
DATA_PROCESSING_CONFIG_SRC_FILES = $(wildcard $(DATA_PROCESSING_CONFIG_SRC_DIR)/*.cpp)
DATA_PROCESSING_CONFIG_BUILD_FILES = $(patsubst $(DATA_PROCESSING_CONFIG_SRC_DIR)/%.cpp, $(DATA_PROCESSING_CONFIG_BUILD_DIR)/%.o, $(DATA_PROCESSING_CONFIG_SRC_FILES))

################################################################################
# Data selection Dir & Src
################################################################################
DATA_SELECTION_INCLUDE_DIR = ./DataSelection/include
DATA_SELECTION_SRC_DIR = ./DataSelection/src
DATA_SELECTION_BUILD_DIR = ./DataSelection/build
DATA_SELECTION_SRC_FILES = $(wildcard $(DATA_SELECTION_SRC_DIR)/*.cpp)
DATA_SELECTION_BUILD_FILES = $(patsubst $(DATA_SELECTION_SRC_DIR)/%.cpp, $(DATA_SELECTION_BUILD_DIR)/%.o, $(DATA_SELECTION_SRC_FILES))


################################################################################
# Histogram processor Dir & Src
################################################################################
HISTOGRAM_PROCESSOR_INCLUDE_DIR = ./HistogramProcessor/include
HISTOGRAM_PROCESSOR_SRC_DIR = ./HistogramProcessor/src
HISTOGRAM_PROCESSOR_BUILD_DIR = ./HistogramProcessor/build
HISTOGRAM_PROCESSOR_SRC_FILES = $(wildcard $(HISTOGRAM_PROCESSOR_SRC_DIR)/*.cpp)
HISTOGRAM_PROCESSOR_BUILD_FILES = $(patsubst $(HISTOGRAM_PROCESSOR_SRC_DIR)/%.cpp, $(HISTOGRAM_PROCESSOR_BUILD_DIR)/%.o, $(HISTOGRAM_PROCESSOR_SRC_FILES))

################################################################################
# TimeRes Dir & Src
################################################################################
TIME_RES_INCLUDE_DIR = ./TimeRes/include
TIME_RES_SRC_DIR = ./TimeRes/src
TIME_RES_BUILD_DIR = ./TimeRes/build
TIME_RES_SRC_FILES = $(wildcard $(TIME_RES_SRC_DIR)/*.cpp)
TIME_RES_BUILD_FILES = $(patsubst $(TIME_RES_SRC_DIR)/%.cpp, $(TIME_RES_BUILD_DIR)/%.o, $(TIME_RES_SRC_FILES))

################################################################################
# Avearge waveform
################################################################################
AVERAGE_WAVE_INCLUDE_DIR = ./averageWave/include
AVERAGE_WAVE_SRC_DIR = ./averageWave/src
AVERAGE_WAVE_BUILD_DIR = ./averageWave/build
AVERAGE_WAVE_SRC_FILES = $(wildcard $(AVERAGE_WAVE_SRC_DIR)/*.cpp)
AVERAGE_WAVE_BUILD_FILES = $(patsubst $(AVERAGE_WAVE_SRC_DIR)/%.cpp, $(AVERAGE_WAVE_BUILD_DIR)/%.o, $(AVERAGE_WAVE_SRC_FILES))


#*******************************************************************************
# Build Waveform Analsyis object files.
#*******************************************************************************
$(DATA_OUTPUT_FORMAT_BUILD_DIR)/%.o: $(DATA_OUTPUT_FORMAT_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(ROOFIT)


#*******************************************************************************
# Build Waveform Analsyis object files.
#*******************************************************************************
$(DATA_PROCESSING_CONFIG_BUILD_DIR)/%.o: $(DATA_PROCESSING_CONFIG_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(ROOFIT)

#*******************************************************************************
# Build Waveform Analsyis object files.
#*******************************************************************************
$(DATA_SELECTION_BUILD_DIR)/%.o: $(DATA_SELECTION_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(ROOFIT)

#*******************************************************************************
# Build Waveform Analsyis object files.
#*******************************************************************************
$(HISTOGRAM_PROCESSOR_BUILD_DIR)/%.o: $(HISTOGRAM_PROCESSOR_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(ROOFIT)

#*******************************************************************************
# Build TimeRes object files.
#*******************************************************************************
$(TIME_RES_BUILD_DIR)/%.o: $(TIME_RES_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(ROOFIT)

#*******************************************************************************
# Build Average_Wave object files.
#*******************************************************************************
$(AVERAGE_WAVE_BUILD_DIR)/%.o: $(AVERAGE_WAVE_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(ROOFIT)



#===============================================================================
# Make exe

all:$(BIN_DIR)/GetResults.exe \
		$(BIN_DIR)/GenerateDataProcessorConfig.exe \
		$(BIN_DIR)/CFDScan.exe \
		$(BIN_DIR)/dataExtractor.exe \
		$(BIN_DIR)/GetAverageWave.exe


$(BIN_DIR)/GetResults.exe: $(MAIN_DIR)/main_GetResults.cpp $(HISTOGRAM_PROCESSOR_BUILD_FILES) $(DATA_SELECTION_BUILD_FILES) $(DATA_PROCESSING_CONFIG_BUILD_FILES) $(DATA_OUTPUT_FORMAT_BUILD_FILES)
	$(CC) $(DEBUG) $(LFLAGS) $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOFIT)

$(BIN_DIR)/GenerateDataProcessorConfig.exe: $(MAIN_DIR)/main_measurement_info.cpp $(DATA_PROCESSING_CONFIG_BUILD_FILES)
	$(CC) $(DEBUG) $(LFLAGS) $^ -o $@

$(BIN_DIR)/CFDScan.exe: $(MAIN_DIR)/main_CFDScan.cpp $(TIME_RES_BUILD_FILES) $(DATA_PROCESSING_CONFIG_BUILD_FILES) $(DATA_SELECTION_BUILD_FILES)
	$(CC) $(DEBUG) $(LFLAGS) $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOFIT)

$(BIN_DIR)/dataExtractor.exe: $(MAIN_DIR)/main_dataExtractor.cpp $(TIME_RES_BUILD_FILES) $(DATA_PROCESSING_CONFIG_BUILD_FILES) $(DATA_SELECTION_BUILD_FILES)
	$(CC) $(DEBUG) $(LFLAGS) $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOFIT) $(BOOST_LIB)

$(BIN_DIR)/GetAverageWave.exe: $(MAIN_DIR)/main_GetAverageWave.cpp $(AVERAGE_WAVE_BUILD_FILES) $(DATA_PROCESSING_CONFIG_BUILD_FILES)
	$(CC) $(DEBUG) $(LFLAGS) $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOFIT) $(BOOST_LIB)

.PHONY: clean
clean:
	@echo "cleaning .o files in build and binary files in bin."
	@for file in ./*/build/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in ./bin/*; do rm -rf $$file; echo Delete binary file: $$file; done
	@echo "finished"
