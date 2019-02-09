##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=serial2mqtt
ConfigurationName      :=Debug
WorkspacePath          :=/home/lieven/workspace
ProjectPath            :=/home/lieven/workspace/serial2mqtt
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Lieven
Date                   :=09/02/19
CodeLitePath           :=/home/lieven/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="serial2mqtt.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -static
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../Common $(IncludeSwitch)../json/single_include/nlohmann $(IncludeSwitch)../paho.mqtt.c/src $(IncludeSwitch)../inja/src 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)Common $(LibrarySwitch)paho-mqtt3c $(LibrarySwitch)pthread 
ArLibs                 :=  "Common" "paho-mqtt3c" "pthread" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)../Common/Debug $(LibraryPathSwitch)../paho.mqtt.c/build/output 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall -std=c++0x $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/Config.cpp$(ObjectSuffix) $(IntermediateDirectory)/Serial2Mqtt.cpp$(ObjectSuffix) $(IntermediateDirectory)/Timer.cpp$(ObjectSuffix) $(IntermediateDirectory)/Sys.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	cp Debug/serial2mqtt Debug/serial2mqtt.`arch`
	zip -r Debug/serial2mqtt.`arch`.zip Debug/serial2mqtt.`arch`
	
	@echo Done

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/serial2mqtt/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/Config.cpp$(ObjectSuffix): Config.cpp $(IntermediateDirectory)/Config.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/serial2mqtt/Config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Config.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Config.cpp$(DependSuffix): Config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Config.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Config.cpp$(DependSuffix) -MM Config.cpp

$(IntermediateDirectory)/Config.cpp$(PreprocessSuffix): Config.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Config.cpp$(PreprocessSuffix) Config.cpp

$(IntermediateDirectory)/Serial2Mqtt.cpp$(ObjectSuffix): Serial2Mqtt.cpp $(IntermediateDirectory)/Serial2Mqtt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/serial2mqtt/Serial2Mqtt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Serial2Mqtt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Serial2Mqtt.cpp$(DependSuffix): Serial2Mqtt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Serial2Mqtt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Serial2Mqtt.cpp$(DependSuffix) -MM Serial2Mqtt.cpp

$(IntermediateDirectory)/Serial2Mqtt.cpp$(PreprocessSuffix): Serial2Mqtt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Serial2Mqtt.cpp$(PreprocessSuffix) Serial2Mqtt.cpp

$(IntermediateDirectory)/Timer.cpp$(ObjectSuffix): Timer.cpp $(IntermediateDirectory)/Timer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/serial2mqtt/Timer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Timer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Timer.cpp$(DependSuffix): Timer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Timer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Timer.cpp$(DependSuffix) -MM Timer.cpp

$(IntermediateDirectory)/Timer.cpp$(PreprocessSuffix): Timer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Timer.cpp$(PreprocessSuffix) Timer.cpp

$(IntermediateDirectory)/Sys.cpp$(ObjectSuffix): Sys.cpp $(IntermediateDirectory)/Sys.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/serial2mqtt/Sys.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Sys.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Sys.cpp$(DependSuffix): Sys.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Sys.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Sys.cpp$(DependSuffix) -MM Sys.cpp

$(IntermediateDirectory)/Sys.cpp$(PreprocessSuffix): Sys.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Sys.cpp$(PreprocessSuffix) Sys.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


