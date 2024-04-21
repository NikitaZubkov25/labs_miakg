cmake_minimum_required( VERSION 3.11 )

cmake_policy(SET CMP0014 NEW)
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

#set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE )

project( sdl2-demo C CXX)

set (PROJECT_DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/external-libs )

include(cmake-ext/StandardProjectSettings.cmake)
include(cmake-ext/CompilerWarnings.cmake)
include(cmake-ext/Sanitizers.cmake)
enable_sanitizers(project_options)
include(cmake-ext/StaticAnalyzers.cmake)
include(cmake-ext/FileOrUrl.cmake)

include(ExternalProject)
add_custom_target(external_all)
set_target_properties(external_all PROPERTIES EXCLUDE_FROM_ALL TRUE)

### SDL2
set(SDL2_VERSION "2.26.5")
include(cmake-ext/SDL2-external.cmake)
if(APPLE) # Apple OS X
  find_library(ObjC Objc)
  find_library(Cocoa Cocoa)
  find_library(AudioToolbox AudioToolbox)
  find_library(CoreAudio CoreAudio)
  find_library(Carbon Carbon)
  find_library(ForceFeedback ForceFeedback)
  find_library(IOKit IOKit)
  find_library(CoreVideo CoreVideo)
  find_library(Metal Metal)
  find_library(GameController GameController)
  find_library(CoreHaptics CoreHaptics)
  set(SDL2_TARGET_LINK_LIBRARIES m iconv
    ${ObjC} ${Cocoa} ${AudioToolbox}
    ${CoreAudio} ${Carbon} ${ForceFeedback}
    ${IOKit} ${CoreVideo} ${Metal} ${GameController} ${CoreHaptics}
    libSDL2.a
    )
elseif(WIN32) #Windows
  if(MSVC)
    set(SDL2_TARGET_LINK_LIBRARIES
        SDL2-static$<$<CONFIG:Debug>:d> SDL2main$<$<CONFIG:Debug>:d>
        user32 gdi32 winmm imm32 ole32 oleaut32 version uuid advapi32 setupapi shell32 dinput8 )
  elseif(MINGW) # pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-cmake
    set(SDL2_TARGET_LINK_LIBRARIES
        mingw32 SDL2main SDL2 m user32 gdi32
		    winmm imm32 ole32 oleaut32 version uuid advapi32
		    setupapi shell32 dinput8)
  endif()
elseif(UNIX AND NOT APPLE) #Linux and etc.
  set(SDL2_TARGET_LINK_LIBRARIES pthread SDL2 dbus-1 dl )
  string(APPEND CMAKE_EXE_LINKER_FLAGS "-pthread -Wl,--no-undefined ")
  string(APPEND CMAKE_EXE_LINKER_FLAGS "-ldl -Wl,--as-needed ")
endif() # OS

### GLM
set(GLM_VERSION  "0.9.9.8")
include(cmake-ext/glm-external.cmake)

include_directories(${SDL2_INCLUDE_DIR} ${GLM_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable( sdl2-demo ${SRC_FILES})
add_dependencies(sdl2-demo external_all)
set_project_warnings( sdl2-demo )
target_link_libraries( sdl2-demo PRIVATE ${SDL2_TARGET_LINK_LIBRARIES} ${glm_TARGET_LINK_LIBRARIES})
