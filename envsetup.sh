# #############################################################################
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
# 
# Copyright (C) 2025 Ragab R. Hassan (r.elkattawy@gmail.com)
#
# #############################################################################

# Directories
ABS_ROOT_WD=$(realpath .)
ROOT_WD=$(pwd)
DEPENDENCIES_DIR=${ABS_ROOT_WD}/deps
CURRENT_PROJECT=


# Build Configs
ENABLE_SIGNAL_HANDLING=1
DEP_INSTALL_PATH=${ABS_ROOT_WD}/install_folder
DEP_LIBRARY_PATH=${DEP_INSTALL_PATH}/lib/
COMMONAPI_CONFIG="commonapi4someip.ini"
USE_INSTALLED_COMMONAPI=ON

################################ General Utils ################################  

# it wraps a function call while preserving the working directory
function _callWrapper() {
    TEMP_CWD=$(pwd)
    "$@"
    cd ${TEMP_CWD}
}  

# this function will check the existence of a directory relative to 
# the project root directory and cd to that directory
# Parameters:
# 1: path of interest
# 2: (optional) n -> don't create path
#
# if n is passed and the directory does't exist, cwd = root directory
#
# to avoid path corruption use it inside a wrapped function only
function _checkPath() {
    # go to root
    cd ${ABS_ROOT_WD}
    
    ls $1 2> /dev/null > /dev/null
    # path existing
    if [[ $? -eq 0 ]]; then 
        cd $1
    else #path doesn't exist
        if [[ $2 = "n" ]] then 
            return 1
        fi
        mkdir -p $1 && cd $1
    fi
}

# same as _checkPath but works with cwd
# feel free to use it in unwrapped functions
function _checkRelPath() {
    ls $1 2> /dev/null > /dev/null
    if [[ $? -eq 0 ]]; then 
        cd $(realpath $1)
    else 
        if [[ $2 = "n" ]] then 
            return 1
        fi
        mkdir $1 && cd $(realpath $1)
    fi
}

# go to the root directory
function goToRoot() {
    cd ${ABS_ROOT_WD}
}

# go to the current project if set
function goToProject() {
    if [[ -n ${CURRENT_PROJECT} ]]; then
        cd ${CURRENT_PROJECT}
    fi
}

function _wrapProjectFunction() {
    if [[ -n ${CURRENT_PROJECT} ]]; then
        _callWrapper "$@"
    else
        echo "Couldn't find project, please run workOn <projectName>"
    fi
}

function nowWhat() {
    echo "
Are you stuck? here's what you can do

General:
    workOn <project>        Setup the project work directory     
    goToRoot                Go to the environment root directory
    goToProject             Jump to the project directory

Project:
    genCoreApiFiles         Generate CommonApi files, fidl and fdepl files are expected to be under projectName/fidl
    buildProject            Build the current project
    runApp <target> cfg.ini Runs the target (Client or Service) app with the specified cfg.ini, 
                            if cfg.ini not specified projectName/commonapi4someip.ini will be used

Dependencies:
    cloneDeps               Clone dependencies under deps
    buildDeps               Build all dependencies
    buildCapi-core          Build only capi-core-runtime lib and install it to install_folder
    buildCapi-someip        Build only capi-someip-runtime lib and install it to install_folder
    buildVsomeip            Build only vsomeip libs and install it to install_folder

Code Generators:
    getGenTools             Download and extract the code generators
"
}

########################## Dependencies Handling ##############################  
# Libraries versions/tags
CAPI_CORE_REPO_NAME="capicxx-core-runtime"
CAPI_CORE_VER="3.2.4"
CAPI_CORE_PATCHES="../patches/core"

CAPI_VSOMEIP_REPO_NAME="capicxx-someip-runtime"
CAPI_VSOMEIP_VER="3.2.4"

VSOMEIP_REPO_NAME="vsomeip"
VSOMEIP_VER="3.5.5"

# used by the user to clone dependencies
function cloneDeps() {
    _callWrapper _cloneDeps
}

# Implementation of cloning a dependency
# takes two arguments
# 1: repo name -> should match the name in the url
# 2: repo version -> the version tag of a repo
# 3: (optional) patch directories
function _cloneDep() {
    local REPO_NAME=$1
    local REPO_VERSION=$2
    echo "============================================================="
    echo "Cloning ${REPO_NAME}..."
    echo "============================================================="

    _checkPath ${DEPENDENCIES_DIR}
    _checkRelPath ${REPO_NAME} n
    if [[ $? -eq 1 ]]; then
        git clone https://github.com/COVESA/${REPO_NAME}.git ${REPO_NAME}
        cd ${REPO_NAME}
        echo "Checking out Version ${REPO_VERSION}..."
        git checkout ${REPO_VERSION}
        echo "Cloning Succeeded!"
        if [[ -n $3 ]]; then 
            if [[ $? -eq 0 ]]; then 
                echo "applying patches..."
                git apply $3/*.patch
            fi
        fi
    else
        echo "Error: /deps/${REPO_NAME} already existing"
        echo "Cloning Failed!"
    fi
    echo " "
}


#Clone the needed dependencies Dependencies
function _cloneDeps() {
 
    _callWrapper _cloneDep ${CAPI_CORE_REPO_NAME} ${CAPI_CORE_VER} ${CAPI_CORE_PATCHES}
    
    _callWrapper _cloneDep ${CAPI_VSOMEIP_REPO_NAME} ${CAPI_VSOMEIP_VER}

    _callWrapper _cloneDep ${VSOMEIP_REPO_NAME} ${VSOMEIP_VER} 
    
}

function buildCapi-core() {
    _callWrapper _buildCapi-core
}

# build the CommonAPI-core 
function _buildCapi-core() {
    cd ${DEPENDENCIES_DIR}
    _checkRelPath ${CAPI_CORE_REPO_NAME} n
    if [[ $? -eq 1 ]]; then
        echo "ERROR: ${CAPI_CORE_REPO_NAME} not found, please clone"
        return 1
    fi
    cmake -Bbuild -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_PATH} .
    cmake --build build/ --target install -j$(nproc)
}

function buildVsomeip() {
    _callWrapper _buildVsomeip
}

# build the CommonAPI-someip 
function _buildVsomeip() {
    cd ${DEPENDENCIES_DIR}
    _checkRelPath ${VSOMEIP_REPO_NAME}

    cmake -Bbuild -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_PATH} -DENABLE_SIGNAL_HANDLING=${ENABLE_SIGNAL_HANDLING} -DDIAGNOSIS_ADDRESS=0x10 .
    cmake --build build/ --target install -j$(nproc)
}


function buildCapi-someip() {
    _callWrapper _buildCapi-someip
}
# build vsomeip
function _buildCapi-someip() {
    cd ${DEPENDENCIES_DIR}
    _checkRelPath ${CAPI_VSOMEIP_REPO_NAME}
    cmake -Bbuild -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_PATH} -DCMAKE_PREFIX_PATH=${DEP_INSTALL_PATH} -DUSE_INSTALLED_COMMONAPI=${USE_INSTALLED_COMMONAPI} .
    cmake --build build/ --target install -j$(nproc)
}

# build all dependencies
function buildDeps() {
    echo "Building ${CAPI_CORE_REPO_NAME}..."
    buildCapi-core
    echo "Building ${CAPI_VSOMEIP_REPO_NAME}..."
    buildCapi-someip
    echo "Building ${VSOMEIP_REPO_NAME}..."
    buildVsomeip
}

######################## Generation Tools Handling ############################ 
# Code generators
export CORE_CGEN=${ABS_ROOT_WD}/cgen/commonapi_core_generator/commonapi-core-generator-linux-x86_64
export SOMEIP_CGEN=${ABS_ROOT_WD}/cgen/commonapi_someip_generator/commonapi-someip-generator-linux-x86_64
CORE_GEN_VERSION="3.2.15"
CORE_GEN_ID="3.2.15"
SOMEIP_GEN_VERSION="3.2.15"
SOMEIP_GEN_ID="3.2.15"
SRC_GEN_DIR=src-gen

function getGenTools() {
    _callWrapper _getGenTools
}

function _getGenTools() {
    
    _checkPath cgen

    echo "Downloading CommonAPI-core-tools.."
    wget https://github.com/COVESA/capicxx-core-tools/releases/download/3.2.15/commonapi_core_generator.zip
    unzip commonapi_core_generator.zip -d commonapi_core_generator/

    echo "CommonAPI-someip-tools.."
    wget https://github.com/COVESA/capicxx-someip-tools/releases/download/3.2.15/commonapi_someip_generator.zip
    unzip commonapi_someip_generator.zip -d commonapi_someip_generator/
}

############################ Project Specifics ################################ 
PROJECT_NAME=
# specify the project to work on
function workOn() {
    _callWrapper _workOn "$@" 
}

function _workOn() {
    _checkPath $1 n
    if [[ $? -eq 1 ]]; then 
        echo "ERROR: Project $1 not found"
        return 1
    fi
    PROJECT_NAME=$1
    CURRENT_PROJECT=$(realpath $(pwd))
}

# 1: fidl file
# 2: fdepl file
# currently supporting single interface files
function genCoreApiFiles() {
    _wrapProjectFunction _genCoreApiFiles "$@"
}


function _genCoreApiFiles() {
    cd ${CURRENT_PROJECT}
    $CORE_CGEN --skel --dest-common ./src-gen/core/common --dest-proxy ./src-gen/core/proxy --dest-stub ./src-gen/core/stub --dest-skel ./src-gen/core/skel -sp ./fidl
    $SOMEIP_CGEN --dest-common ./src-gen/someip/common --dest-proxy ./src-gen/someip/proxy --dest-stub ./src-gen/someip/stub -sp ./fidl
}

function buildProject() {
    _wrapProjectFunction _buildProject
}

function _buildProject() {
    cd ${CURRENT_PROJECT}
    cmake -Bbuild -DCMAKE_PREFIX_PATH=${DEP_INSTALL_PATH} -DPKG_CONFIG_USE_CMAKE_PREFIX_PATH=ON .
    cmake --build build
}

function runApp() {
    _wrapProjectFunction _runApp "$@"
}


function _runApp() {
    cd ${CURRENT_PROJECT}
    pwd
    export LD_LIBRARY_PATH=${DEP_LIBRARY_PATH}:$PWD/build/ 
    local TARGET=$1
    if [[ -n $2 ]]; then 
        export COMMONAPI_CONFIG=$2
    else 
        export COMMONAPI_CONFIG="commonapi4someip.ini"
    fi
    local APP_NAME="${PROJECT_NAME}${TARGET}"
    build/${APP_NAME}
}
