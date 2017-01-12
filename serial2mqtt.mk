##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=serial2mqtt
ConfigurationName      :=Debug
WorkspacePath          := "/home/lieven/linux"
ProjectPath            := "/home/lieven/linux/serial2mqtt"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Lieven
Date                   :=12/01/17
CodeLitePath           :="/home/lieven/.codelite"
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
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)inc $(IncludeSwitch)../Common/inc $(IncludeSwitch)../paho/src $(IncludeSwitch)../Ebos 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)paho-mqtt3a $(LibrarySwitch)pthread 
ArLibs                 :=  "paho-mqtt3a" "pthread" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)../paho/build/output 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -std=c++11 -Wall -fno-rtti -DLINUX $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall -fno-rtti -DLINUX $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_Serial.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_MqttGtw.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Sys.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ebos_Actor.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ebos_EventBus.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ebos_Router.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ebos_Slip.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ebos_SlipStream.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ebos_static_labels.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Ebos_System.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ebos_Uid.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Base64.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_BipBuffer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_BufferedByteStream.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Bytes.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Cbor.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_CborQueue.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_CircBuf.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_EventSource.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_jsmn.c$(ObjectSuffix) $(IntermediateDirectory)/src_Json.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Log.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Msg.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Msgpack.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Node.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_PIC32_stubs.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_printf.c$(ObjectSuffix) $(IntermediateDirectory)/src_STM32_stubs.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Str.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_Strpack.cpp$(ObjectSuffix) $(IntermediateDirectory)/inc_LinkedList.cpp$(ObjectSuffix) 



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

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:
	@echo Executing Pre Build commands ...
	./keys.sh
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/src_Serial.cpp$(ObjectSuffix): src/Serial.cpp $(IntermediateDirectory)/src_Serial.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/serial2mqtt/src/Serial.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Serial.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Serial.cpp$(DependSuffix): src/Serial.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Serial.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Serial.cpp$(DependSuffix) -MM "src/Serial.cpp"

$(IntermediateDirectory)/src_Serial.cpp$(PreprocessSuffix): src/Serial.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Serial.cpp$(PreprocessSuffix) "src/Serial.cpp"

$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix): src/main.cpp $(IntermediateDirectory)/src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/serial2mqtt/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.cpp$(DependSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main.cpp$(DependSuffix) -MM "src/main.cpp"

$(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix): src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix) "src/main.cpp"

$(IntermediateDirectory)/src_MqttGtw.cpp$(ObjectSuffix): src/MqttGtw.cpp $(IntermediateDirectory)/src_MqttGtw.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/serial2mqtt/src/MqttGtw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_MqttGtw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_MqttGtw.cpp$(DependSuffix): src/MqttGtw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_MqttGtw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_MqttGtw.cpp$(DependSuffix) -MM "src/MqttGtw.cpp"

$(IntermediateDirectory)/src_MqttGtw.cpp$(PreprocessSuffix): src/MqttGtw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_MqttGtw.cpp$(PreprocessSuffix) "src/MqttGtw.cpp"

$(IntermediateDirectory)/src_Sys.cpp$(ObjectSuffix): src/Sys.cpp $(IntermediateDirectory)/src_Sys.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/serial2mqtt/src/Sys.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Sys.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Sys.cpp$(DependSuffix): src/Sys.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Sys.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Sys.cpp$(DependSuffix) -MM "src/Sys.cpp"

$(IntermediateDirectory)/src_Sys.cpp$(PreprocessSuffix): src/Sys.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Sys.cpp$(PreprocessSuffix) "src/Sys.cpp"

$(IntermediateDirectory)/Ebos_Actor.cpp$(ObjectSuffix): ../Ebos/Actor.cpp $(IntermediateDirectory)/Ebos_Actor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Ebos/Actor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ebos_Actor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ebos_Actor.cpp$(DependSuffix): ../Ebos/Actor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ebos_Actor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ebos_Actor.cpp$(DependSuffix) -MM "../Ebos/Actor.cpp"

$(IntermediateDirectory)/Ebos_Actor.cpp$(PreprocessSuffix): ../Ebos/Actor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ebos_Actor.cpp$(PreprocessSuffix) "../Ebos/Actor.cpp"

$(IntermediateDirectory)/Ebos_EventBus.cpp$(ObjectSuffix): ../Ebos/EventBus.cpp $(IntermediateDirectory)/Ebos_EventBus.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Ebos/EventBus.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ebos_EventBus.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ebos_EventBus.cpp$(DependSuffix): ../Ebos/EventBus.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ebos_EventBus.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ebos_EventBus.cpp$(DependSuffix) -MM "../Ebos/EventBus.cpp"

$(IntermediateDirectory)/Ebos_EventBus.cpp$(PreprocessSuffix): ../Ebos/EventBus.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ebos_EventBus.cpp$(PreprocessSuffix) "../Ebos/EventBus.cpp"

$(IntermediateDirectory)/Ebos_Router.cpp$(ObjectSuffix): ../Ebos/Router.cpp $(IntermediateDirectory)/Ebos_Router.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Ebos/Router.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ebos_Router.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ebos_Router.cpp$(DependSuffix): ../Ebos/Router.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ebos_Router.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ebos_Router.cpp$(DependSuffix) -MM "../Ebos/Router.cpp"

$(IntermediateDirectory)/Ebos_Router.cpp$(PreprocessSuffix): ../Ebos/Router.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ebos_Router.cpp$(PreprocessSuffix) "../Ebos/Router.cpp"

$(IntermediateDirectory)/Ebos_Slip.cpp$(ObjectSuffix): ../Ebos/Slip.cpp $(IntermediateDirectory)/Ebos_Slip.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Ebos/Slip.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ebos_Slip.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ebos_Slip.cpp$(DependSuffix): ../Ebos/Slip.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ebos_Slip.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ebos_Slip.cpp$(DependSuffix) -MM "../Ebos/Slip.cpp"

$(IntermediateDirectory)/Ebos_Slip.cpp$(PreprocessSuffix): ../Ebos/Slip.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ebos_Slip.cpp$(PreprocessSuffix) "../Ebos/Slip.cpp"

$(IntermediateDirectory)/Ebos_SlipStream.cpp$(ObjectSuffix): ../Ebos/SlipStream.cpp $(IntermediateDirectory)/Ebos_SlipStream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Ebos/SlipStream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ebos_SlipStream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ebos_SlipStream.cpp$(DependSuffix): ../Ebos/SlipStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ebos_SlipStream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ebos_SlipStream.cpp$(DependSuffix) -MM "../Ebos/SlipStream.cpp"

$(IntermediateDirectory)/Ebos_SlipStream.cpp$(PreprocessSuffix): ../Ebos/SlipStream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ebos_SlipStream.cpp$(PreprocessSuffix) "../Ebos/SlipStream.cpp"

$(IntermediateDirectory)/Ebos_static_labels.cpp$(ObjectSuffix): ../Ebos/static_labels.cpp $(IntermediateDirectory)/Ebos_static_labels.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Ebos/static_labels.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ebos_static_labels.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ebos_static_labels.cpp$(DependSuffix): ../Ebos/static_labels.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ebos_static_labels.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ebos_static_labels.cpp$(DependSuffix) -MM "../Ebos/static_labels.cpp"

$(IntermediateDirectory)/Ebos_static_labels.cpp$(PreprocessSuffix): ../Ebos/static_labels.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ebos_static_labels.cpp$(PreprocessSuffix) "../Ebos/static_labels.cpp"

$(IntermediateDirectory)/Ebos_System.cpp$(ObjectSuffix): ../Ebos/System.cpp $(IntermediateDirectory)/Ebos_System.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Ebos/System.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ebos_System.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ebos_System.cpp$(DependSuffix): ../Ebos/System.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ebos_System.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ebos_System.cpp$(DependSuffix) -MM "../Ebos/System.cpp"

$(IntermediateDirectory)/Ebos_System.cpp$(PreprocessSuffix): ../Ebos/System.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ebos_System.cpp$(PreprocessSuffix) "../Ebos/System.cpp"

$(IntermediateDirectory)/Ebos_Uid.cpp$(ObjectSuffix): ../Ebos/Uid.cpp $(IntermediateDirectory)/Ebos_Uid.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Ebos/Uid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ebos_Uid.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ebos_Uid.cpp$(DependSuffix): ../Ebos/Uid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ebos_Uid.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ebos_Uid.cpp$(DependSuffix) -MM "../Ebos/Uid.cpp"

$(IntermediateDirectory)/Ebos_Uid.cpp$(PreprocessSuffix): ../Ebos/Uid.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ebos_Uid.cpp$(PreprocessSuffix) "../Ebos/Uid.cpp"

$(IntermediateDirectory)/src_Base64.cpp$(ObjectSuffix): ../Common/src/Base64.cpp $(IntermediateDirectory)/src_Base64.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/Base64.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Base64.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Base64.cpp$(DependSuffix): ../Common/src/Base64.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Base64.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Base64.cpp$(DependSuffix) -MM "../Common/src/Base64.cpp"

$(IntermediateDirectory)/src_Base64.cpp$(PreprocessSuffix): ../Common/src/Base64.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Base64.cpp$(PreprocessSuffix) "../Common/src/Base64.cpp"

$(IntermediateDirectory)/src_BipBuffer.cpp$(ObjectSuffix): ../Common/src/BipBuffer.cpp $(IntermediateDirectory)/src_BipBuffer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/BipBuffer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_BipBuffer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_BipBuffer.cpp$(DependSuffix): ../Common/src/BipBuffer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_BipBuffer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_BipBuffer.cpp$(DependSuffix) -MM "../Common/src/BipBuffer.cpp"

$(IntermediateDirectory)/src_BipBuffer.cpp$(PreprocessSuffix): ../Common/src/BipBuffer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_BipBuffer.cpp$(PreprocessSuffix) "../Common/src/BipBuffer.cpp"

$(IntermediateDirectory)/src_BufferedByteStream.cpp$(ObjectSuffix): ../Common/src/BufferedByteStream.cpp $(IntermediateDirectory)/src_BufferedByteStream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/BufferedByteStream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_BufferedByteStream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_BufferedByteStream.cpp$(DependSuffix): ../Common/src/BufferedByteStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_BufferedByteStream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_BufferedByteStream.cpp$(DependSuffix) -MM "../Common/src/BufferedByteStream.cpp"

$(IntermediateDirectory)/src_BufferedByteStream.cpp$(PreprocessSuffix): ../Common/src/BufferedByteStream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_BufferedByteStream.cpp$(PreprocessSuffix) "../Common/src/BufferedByteStream.cpp"

$(IntermediateDirectory)/src_Bytes.cpp$(ObjectSuffix): ../Common/src/Bytes.cpp $(IntermediateDirectory)/src_Bytes.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/Bytes.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Bytes.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Bytes.cpp$(DependSuffix): ../Common/src/Bytes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Bytes.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Bytes.cpp$(DependSuffix) -MM "../Common/src/Bytes.cpp"

$(IntermediateDirectory)/src_Bytes.cpp$(PreprocessSuffix): ../Common/src/Bytes.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Bytes.cpp$(PreprocessSuffix) "../Common/src/Bytes.cpp"

$(IntermediateDirectory)/src_Cbor.cpp$(ObjectSuffix): ../Common/src/Cbor.cpp $(IntermediateDirectory)/src_Cbor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/Cbor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Cbor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Cbor.cpp$(DependSuffix): ../Common/src/Cbor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Cbor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Cbor.cpp$(DependSuffix) -MM "../Common/src/Cbor.cpp"

$(IntermediateDirectory)/src_Cbor.cpp$(PreprocessSuffix): ../Common/src/Cbor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Cbor.cpp$(PreprocessSuffix) "../Common/src/Cbor.cpp"

$(IntermediateDirectory)/src_CborQueue.cpp$(ObjectSuffix): ../Common/src/CborQueue.cpp $(IntermediateDirectory)/src_CborQueue.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/CborQueue.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_CborQueue.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_CborQueue.cpp$(DependSuffix): ../Common/src/CborQueue.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_CborQueue.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_CborQueue.cpp$(DependSuffix) -MM "../Common/src/CborQueue.cpp"

$(IntermediateDirectory)/src_CborQueue.cpp$(PreprocessSuffix): ../Common/src/CborQueue.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_CborQueue.cpp$(PreprocessSuffix) "../Common/src/CborQueue.cpp"

$(IntermediateDirectory)/src_CircBuf.cpp$(ObjectSuffix): ../Common/src/CircBuf.cpp $(IntermediateDirectory)/src_CircBuf.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/CircBuf.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_CircBuf.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_CircBuf.cpp$(DependSuffix): ../Common/src/CircBuf.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_CircBuf.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_CircBuf.cpp$(DependSuffix) -MM "../Common/src/CircBuf.cpp"

$(IntermediateDirectory)/src_CircBuf.cpp$(PreprocessSuffix): ../Common/src/CircBuf.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_CircBuf.cpp$(PreprocessSuffix) "../Common/src/CircBuf.cpp"

$(IntermediateDirectory)/src_EventSource.cpp$(ObjectSuffix): ../Common/src/EventSource.cpp $(IntermediateDirectory)/src_EventSource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/EventSource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_EventSource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_EventSource.cpp$(DependSuffix): ../Common/src/EventSource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_EventSource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_EventSource.cpp$(DependSuffix) -MM "../Common/src/EventSource.cpp"

$(IntermediateDirectory)/src_EventSource.cpp$(PreprocessSuffix): ../Common/src/EventSource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_EventSource.cpp$(PreprocessSuffix) "../Common/src/EventSource.cpp"

$(IntermediateDirectory)/src_jsmn.c$(ObjectSuffix): ../Common/src/jsmn.c $(IntermediateDirectory)/src_jsmn.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/lieven/linux/Common/src/jsmn.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_jsmn.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_jsmn.c$(DependSuffix): ../Common/src/jsmn.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_jsmn.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_jsmn.c$(DependSuffix) -MM "../Common/src/jsmn.c"

$(IntermediateDirectory)/src_jsmn.c$(PreprocessSuffix): ../Common/src/jsmn.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_jsmn.c$(PreprocessSuffix) "../Common/src/jsmn.c"

$(IntermediateDirectory)/src_Json.cpp$(ObjectSuffix): ../Common/src/Json.cpp $(IntermediateDirectory)/src_Json.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/Json.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Json.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Json.cpp$(DependSuffix): ../Common/src/Json.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Json.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Json.cpp$(DependSuffix) -MM "../Common/src/Json.cpp"

$(IntermediateDirectory)/src_Json.cpp$(PreprocessSuffix): ../Common/src/Json.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Json.cpp$(PreprocessSuffix) "../Common/src/Json.cpp"

$(IntermediateDirectory)/src_Log.cpp$(ObjectSuffix): ../Common/src/Log.cpp $(IntermediateDirectory)/src_Log.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/Log.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Log.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Log.cpp$(DependSuffix): ../Common/src/Log.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Log.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Log.cpp$(DependSuffix) -MM "../Common/src/Log.cpp"

$(IntermediateDirectory)/src_Log.cpp$(PreprocessSuffix): ../Common/src/Log.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Log.cpp$(PreprocessSuffix) "../Common/src/Log.cpp"

$(IntermediateDirectory)/src_Msg.cpp$(ObjectSuffix): ../Common/src/Msg.cpp $(IntermediateDirectory)/src_Msg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/Msg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Msg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Msg.cpp$(DependSuffix): ../Common/src/Msg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Msg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Msg.cpp$(DependSuffix) -MM "../Common/src/Msg.cpp"

$(IntermediateDirectory)/src_Msg.cpp$(PreprocessSuffix): ../Common/src/Msg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Msg.cpp$(PreprocessSuffix) "../Common/src/Msg.cpp"

$(IntermediateDirectory)/src_Msgpack.cpp$(ObjectSuffix): ../Common/src/Msgpack.cpp $(IntermediateDirectory)/src_Msgpack.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/Msgpack.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Msgpack.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Msgpack.cpp$(DependSuffix): ../Common/src/Msgpack.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Msgpack.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Msgpack.cpp$(DependSuffix) -MM "../Common/src/Msgpack.cpp"

$(IntermediateDirectory)/src_Msgpack.cpp$(PreprocessSuffix): ../Common/src/Msgpack.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Msgpack.cpp$(PreprocessSuffix) "../Common/src/Msgpack.cpp"

$(IntermediateDirectory)/src_Node.cpp$(ObjectSuffix): ../Common/src/Node.cpp $(IntermediateDirectory)/src_Node.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/Node.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Node.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Node.cpp$(DependSuffix): ../Common/src/Node.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Node.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Node.cpp$(DependSuffix) -MM "../Common/src/Node.cpp"

$(IntermediateDirectory)/src_Node.cpp$(PreprocessSuffix): ../Common/src/Node.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Node.cpp$(PreprocessSuffix) "../Common/src/Node.cpp"

$(IntermediateDirectory)/src_PIC32_stubs.cpp$(ObjectSuffix): ../Common/src/PIC32_stubs.cpp $(IntermediateDirectory)/src_PIC32_stubs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/PIC32_stubs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_PIC32_stubs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_PIC32_stubs.cpp$(DependSuffix): ../Common/src/PIC32_stubs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_PIC32_stubs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_PIC32_stubs.cpp$(DependSuffix) -MM "../Common/src/PIC32_stubs.cpp"

$(IntermediateDirectory)/src_PIC32_stubs.cpp$(PreprocessSuffix): ../Common/src/PIC32_stubs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_PIC32_stubs.cpp$(PreprocessSuffix) "../Common/src/PIC32_stubs.cpp"

$(IntermediateDirectory)/src_printf.c$(ObjectSuffix): ../Common/src/printf.c $(IntermediateDirectory)/src_printf.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/lieven/linux/Common/src/printf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_printf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_printf.c$(DependSuffix): ../Common/src/printf.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_printf.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_printf.c$(DependSuffix) -MM "../Common/src/printf.c"

$(IntermediateDirectory)/src_printf.c$(PreprocessSuffix): ../Common/src/printf.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_printf.c$(PreprocessSuffix) "../Common/src/printf.c"

$(IntermediateDirectory)/src_STM32_stubs.cpp$(ObjectSuffix): ../Common/src/STM32_stubs.cpp $(IntermediateDirectory)/src_STM32_stubs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/STM32_stubs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_STM32_stubs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_STM32_stubs.cpp$(DependSuffix): ../Common/src/STM32_stubs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_STM32_stubs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_STM32_stubs.cpp$(DependSuffix) -MM "../Common/src/STM32_stubs.cpp"

$(IntermediateDirectory)/src_STM32_stubs.cpp$(PreprocessSuffix): ../Common/src/STM32_stubs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_STM32_stubs.cpp$(PreprocessSuffix) "../Common/src/STM32_stubs.cpp"

$(IntermediateDirectory)/src_Str.cpp$(ObjectSuffix): ../Common/src/Str.cpp $(IntermediateDirectory)/src_Str.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/Str.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Str.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Str.cpp$(DependSuffix): ../Common/src/Str.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Str.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Str.cpp$(DependSuffix) -MM "../Common/src/Str.cpp"

$(IntermediateDirectory)/src_Str.cpp$(PreprocessSuffix): ../Common/src/Str.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Str.cpp$(PreprocessSuffix) "../Common/src/Str.cpp"

$(IntermediateDirectory)/src_Strpack.cpp$(ObjectSuffix): ../Common/src/Strpack.cpp $(IntermediateDirectory)/src_Strpack.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/src/Strpack.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Strpack.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Strpack.cpp$(DependSuffix): ../Common/src/Strpack.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Strpack.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Strpack.cpp$(DependSuffix) -MM "../Common/src/Strpack.cpp"

$(IntermediateDirectory)/src_Strpack.cpp$(PreprocessSuffix): ../Common/src/Strpack.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Strpack.cpp$(PreprocessSuffix) "../Common/src/Strpack.cpp"

$(IntermediateDirectory)/inc_LinkedList.cpp$(ObjectSuffix): ../Common/inc/LinkedList.cpp $(IntermediateDirectory)/inc_LinkedList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/inc/LinkedList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/inc_LinkedList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/inc_LinkedList.cpp$(DependSuffix): ../Common/inc/LinkedList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/inc_LinkedList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/inc_LinkedList.cpp$(DependSuffix) -MM "../Common/inc/LinkedList.cpp"

$(IntermediateDirectory)/inc_LinkedList.cpp$(PreprocessSuffix): ../Common/inc/LinkedList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/inc_LinkedList.cpp$(PreprocessSuffix) "../Common/inc/LinkedList.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


