# Install script for directory: /home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/build/lib/NGT/libngt.so.1.6.2"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/build/lib/NGT/libngt.so.1"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libngt.so.1.6.2"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libngt.so.1"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/build/lib/NGT/libngt.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libngt.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libngt.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libngt.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/build/lib/NGT/libngt.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/NGT" TYPE FILE FILES
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/ArrayFile.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/Capi.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/Clustering.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/Command.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/Common.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/Graph.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/GraphReconstructor.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/HashBasedBooleanSet.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/Index.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/MmapManager.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/MmapManagerDefs.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/MmapManagerException.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/MmapManagerImpl.hpp"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/Node.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/ObjectRepository.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/ObjectSpace.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/ObjectSpaceRepository.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/Optimizer.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/PrimitiveComparator.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/SharedMemoryAllocator.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/Thread.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/Tree.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/build/lib/NGT/defines.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/NGT/NGTQ" TYPE FILE FILES
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/NGTQ/Command.h"
    "/home/cjz18001/DeployedProjects/tmp/pycharm_project_431/lib/NGT/NGTQ/Quantizer.h"
    )
endif()

