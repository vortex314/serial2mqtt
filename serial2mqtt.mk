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
Date                   :=15/01/17
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
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)inc $(IncludeSwitch)../Common $(IncludeSwitch)../paho/src $(IncludeSwitch)../Ebos 
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
CXXFLAGS :=  -g -O0 -std=c++11 -Wall -fno-rtti -DLINUX -fPIC $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall -DLINUX -fPIC $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_Serial.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_MqttGtw.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Sys.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ebos_Actor.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ebos_EventBus.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ebos_Slip.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ebos_SlipStream.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ebos_static_labels.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ebos_System.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Ebos_Uid.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_Base64.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_BipBuffer.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_BufferedByteStream.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_Bytes.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_Cbor.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_CborQueue.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_CircBuf.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_EventSource.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_jsmn.c$(ObjectSuffix) \
	$(IntermediateDirectory)/Common_Json.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_LinkedList.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_Log.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_Msg.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_Msgpack.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_Node.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_PIC32_stubs.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_printf.c$(ObjectSuffix) $(IntermediateDirectory)/Common_STM32_stubs.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common_Str.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Common_Strpack.cpp$(ObjectSuffix) 



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
	./keys.sh main
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

$(IntermediateDirectory)/Common_Base64.cpp$(ObjectSuffix): ../Common/Base64.cpp $(IntermediateDirectory)/Common_Base64.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/Base64.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_Base64.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_Base64.cpp$(DependSuffix): ../Common/Base64.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_Base64.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_Base64.cpp$(DependSuffix) -MM "../Common/Base64.cpp"

$(IntermediateDirectory)/Common_Base64.cpp$(PreprocessSuffix): ../Common/Base64.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_Base64.cpp$(PreprocessSuffix) "../Common/Base64.cpp"

$(IntermediateDirectory)/Common_BipBuffer.cpp$(ObjectSuffix): ../Common/BipBuffer.cpp $(IntermediateDirectory)/Common_BipBuffer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/BipBuffer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_BipBuffer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_BipBuffer.cpp$(DependSuffix): ../Common/BipBuffer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_BipBuffer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_BipBuffer.cpp$(DependSuffix) -MM "../Common/BipBuffer.cpp"

$(IntermediateDirectory)/Common_BipBuffer.cpp$(PreprocessSuffix): ../Common/BipBuffer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_BipBuffer.cpp$(PreprocessSuffix) "../Common/BipBuffer.cpp"

$(IntermediateDirectory)/Common_BufferedByteStream.cpp$(ObjectSuffix): ../Common/BufferedByteStream.cpp $(IntermediateDirectory)/Common_BufferedByteStream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/BufferedByteStream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_BufferedByteStream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_BufferedByteStream.cpp$(DependSuffix): ../Common/BufferedByteStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_BufferedByteStream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_BufferedByteStream.cpp$(DependSuffix) -MM "../Common/BufferedByteStream.cpp"

$(IntermediateDirectory)/Common_BufferedByteStream.cpp$(PreprocessSuffix): ../Common/BufferedByteStream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_BufferedByteStream.cpp$(PreprocessSuffix) "../Common/BufferedByteStream.cpp"

$(IntermediateDirectory)/Common_Bytes.cpp$(ObjectSuffix): ../Common/Bytes.cpp $(IntermediateDirectory)/Common_Bytes.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/Bytes.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_Bytes.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_Bytes.cpp$(DependSuffix): ../Common/Bytes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_Bytes.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_Bytes.cpp$(DependSuffix) -MM "../Common/Bytes.cpp"

$(IntermediateDirectory)/Common_Bytes.cpp$(PreprocessSuffix): ../Common/Bytes.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_Bytes.cpp$(PreprocessSuffix) "../Common/Bytes.cpp"

$(IntermediateDirectory)/Common_Cbor.cpp$(ObjectSuffix): ../Common/Cbor.cpp $(IntermediateDirectory)/Common_Cbor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/Cbor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_Cbor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_Cbor.cpp$(DependSuffix): ../Common/Cbor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_Cbor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_Cbor.cpp$(DependSuffix) -MM "../Common/Cbor.cpp"

$(IntermediateDirectory)/Common_Cbor.cpp$(PreprocessSuffix): ../Common/Cbor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_Cbor.cpp$(PreprocessSuffix) "../Common/Cbor.cpp"

$(IntermediateDirectory)/Common_CborQueue.cpp$(ObjectSuffix): ../Common/CborQueue.cpp $(IntermediateDirectory)/Common_CborQueue.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/CborQueue.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_CborQueue.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_CborQueue.cpp$(DependSuffix): ../Common/CborQueue.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_CborQueue.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_CborQueue.cpp$(DependSuffix) -MM "../Common/CborQueue.cpp"

$(IntermediateDirectory)/Common_CborQueue.cpp$(PreprocessSuffix): ../Common/CborQueue.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_CborQueue.cpp$(PreprocessSuffix) "../Common/CborQueue.cpp"

$(IntermediateDirectory)/Common_CircBuf.cpp$(ObjectSuffix): ../Common/CircBuf.cpp $(IntermediateDirectory)/Common_CircBuf.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/CircBuf.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_CircBuf.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_CircBuf.cpp$(DependSuffix): ../Common/CircBuf.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_CircBuf.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_CircBuf.cpp$(DependSuffix) -MM "../Common/CircBuf.cpp"

$(IntermediateDirectory)/Common_CircBuf.cpp$(PreprocessSuffix): ../Common/CircBuf.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_CircBuf.cpp$(PreprocessSuffix) "../Common/CircBuf.cpp"

$(IntermediateDirectory)/Common_EventSource.cpp$(ObjectSuffix): ../Common/EventSource.cpp $(IntermediateDirectory)/Common_EventSource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/EventSource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_EventSource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_EventSource.cpp$(DependSuffix): ../Common/EventSource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_EventSource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_EventSource.cpp$(DependSuffix) -MM "../Common/EventSource.cpp"

$(IntermediateDirectory)/Common_EventSource.cpp$(PreprocessSuffix): ../Common/EventSource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_EventSource.cpp$(PreprocessSuffix) "../Common/EventSource.cpp"

$(IntermediateDirectory)/Common_jsmn.c$(ObjectSuffix): ../Common/jsmn.c $(IntermediateDirectory)/Common_jsmn.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/lieven/linux/Common/jsmn.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_jsmn.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_jsmn.c$(DependSuffix): ../Common/jsmn.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_jsmn.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_jsmn.c$(DependSuffix) -MM "../Common/jsmn.c"

$(IntermediateDirectory)/Common_jsmn.c$(PreprocessSuffix): ../Common/jsmn.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_jsmn.c$(PreprocessSuffix) "../Common/jsmn.c"

$(IntermediateDirectory)/Common_Json.cpp$(ObjectSuffix): ../Common/Json.cpp $(IntermediateDirectory)/Common_Json.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/Json.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_Json.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_Json.cpp$(DependSuffix): ../Common/Json.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_Json.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_Json.cpp$(DependSuffix) -MM "../Common/Json.cpp"

$(IntermediateDirectory)/Common_Json.cpp$(PreprocessSuffix): ../Common/Json.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_Json.cpp$(PreprocessSuffix) "../Common/Json.cpp"

$(IntermediateDirectory)/Common_LinkedList.cpp$(ObjectSuffix): ../Common/LinkedList.cpp $(IntermediateDirectory)/Common_LinkedList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/LinkedList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_LinkedList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_LinkedList.cpp$(DependSuffix): ../Common/LinkedList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_LinkedList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_LinkedList.cpp$(DependSuffix) -MM "../Common/LinkedList.cpp"

$(IntermediateDirectory)/Common_LinkedList.cpp$(PreprocessSuffix): ../Common/LinkedList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_LinkedList.cpp$(PreprocessSuffix) "../Common/LinkedList.cpp"

$(IntermediateDirectory)/Common_Log.cpp$(ObjectSuffix): ../Common/Log.cpp $(IntermediateDirectory)/Common_Log.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/Log.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_Log.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_Log.cpp$(DependSuffix): ../Common/Log.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_Log.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_Log.cpp$(DependSuffix) -MM "../Common/Log.cpp"

$(IntermediateDirectory)/Common_Log.cpp$(PreprocessSuffix): ../Common/Log.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_Log.cpp$(PreprocessSuffix) "../Common/Log.cpp"

$(IntermediateDirectory)/Common_Msg.cpp$(ObjectSuffix): ../Common/Msg.cpp $(IntermediateDirectory)/Common_Msg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/Msg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_Msg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_Msg.cpp$(DependSuffix): ../Common/Msg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_Msg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_Msg.cpp$(DependSuffix) -MM "../Common/Msg.cpp"

$(IntermediateDirectory)/Common_Msg.cpp$(PreprocessSuffix): ../Common/Msg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_Msg.cpp$(PreprocessSuffix) "../Common/Msg.cpp"

$(IntermediateDirectory)/Common_Msgpack.cpp$(ObjectSuffix): ../Common/Msgpack.cpp $(IntermediateDirectory)/Common_Msgpack.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/Msgpack.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_Msgpack.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_Msgpack.cpp$(DependSuffix): ../Common/Msgpack.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_Msgpack.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_Msgpack.cpp$(DependSuffix) -MM "../Common/Msgpack.cpp"

$(IntermediateDirectory)/Common_Msgpack.cpp$(PreprocessSuffix): ../Common/Msgpack.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_Msgpack.cpp$(PreprocessSuffix) "../Common/Msgpack.cpp"

$(IntermediateDirectory)/Common_Node.cpp$(ObjectSuffix): ../Common/Node.cpp $(IntermediateDirectory)/Common_Node.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/Node.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_Node.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_Node.cpp$(DependSuffix): ../Common/Node.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_Node.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_Node.cpp$(DependSuffix) -MM "../Common/Node.cpp"

$(IntermediateDirectory)/Common_Node.cpp$(PreprocessSuffix): ../Common/Node.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_Node.cpp$(PreprocessSuffix) "../Common/Node.cpp"

$(IntermediateDirectory)/Common_PIC32_stubs.cpp$(ObjectSuffix): ../Common/PIC32_stubs.cpp $(IntermediateDirectory)/Common_PIC32_stubs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/PIC32_stubs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_PIC32_stubs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_PIC32_stubs.cpp$(DependSuffix): ../Common/PIC32_stubs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_PIC32_stubs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_PIC32_stubs.cpp$(DependSuffix) -MM "../Common/PIC32_stubs.cpp"

$(IntermediateDirectory)/Common_PIC32_stubs.cpp$(PreprocessSuffix): ../Common/PIC32_stubs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_PIC32_stubs.cpp$(PreprocessSuffix) "../Common/PIC32_stubs.cpp"

$(IntermediateDirectory)/Common_printf.c$(ObjectSuffix): ../Common/printf.c $(IntermediateDirectory)/Common_printf.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/lieven/linux/Common/printf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_printf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_printf.c$(DependSuffix): ../Common/printf.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_printf.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_printf.c$(DependSuffix) -MM "../Common/printf.c"

$(IntermediateDirectory)/Common_printf.c$(PreprocessSuffix): ../Common/printf.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_printf.c$(PreprocessSuffix) "../Common/printf.c"

$(IntermediateDirectory)/Common_STM32_stubs.cpp$(ObjectSuffix): ../Common/STM32_stubs.cpp $(IntermediateDirectory)/Common_STM32_stubs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/STM32_stubs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_STM32_stubs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_STM32_stubs.cpp$(DependSuffix): ../Common/STM32_stubs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_STM32_stubs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_STM32_stubs.cpp$(DependSuffix) -MM "../Common/STM32_stubs.cpp"

$(IntermediateDirectory)/Common_STM32_stubs.cpp$(PreprocessSuffix): ../Common/STM32_stubs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_STM32_stubs.cpp$(PreprocessSuffix) "../Common/STM32_stubs.cpp"

$(IntermediateDirectory)/Common_Str.cpp$(ObjectSuffix): ../Common/Str.cpp $(IntermediateDirectory)/Common_Str.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/Str.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_Str.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_Str.cpp$(DependSuffix): ../Common/Str.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_Str.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_Str.cpp$(DependSuffix) -MM "../Common/Str.cpp"

$(IntermediateDirectory)/Common_Str.cpp$(PreprocessSuffix): ../Common/Str.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_Str.cpp$(PreprocessSuffix) "../Common/Str.cpp"

$(IntermediateDirectory)/Common_Strpack.cpp$(ObjectSuffix): ../Common/Strpack.cpp $(IntermediateDirectory)/Common_Strpack.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/linux/Common/Strpack.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common_Strpack.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common_Strpack.cpp$(DependSuffix): ../Common/Strpack.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common_Strpack.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common_Strpack.cpp$(DependSuffix) -MM "../Common/Strpack.cpp"

$(IntermediateDirectory)/Common_Strpack.cpp$(PreprocessSuffix): ../Common/Strpack.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common_Strpack.cpp$(PreprocessSuffix) "../Common/Strpack.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


