# CMakeList.txt : CMake project for CWAlpha_Randomizer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("CWAlpha_Randomizer")

add_subdirectory(cwsdk)
add_subdirectory(APCpp)
# Add source to this project's executable.
add_library (CWAlpha_Randomizer SHARED "dllmain.cpp")
target_link_libraries (CWAlpha_Randomizer LINK_PUBLIC cwsdk)
target_link_libraries (CWAlpha_Randomizer LINK_PUBLIC APCpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CWAlpha_Randomizer PROPERTY CXX_STANDARD 20)
endif()

