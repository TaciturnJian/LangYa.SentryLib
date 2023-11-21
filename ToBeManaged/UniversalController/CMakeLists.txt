cmake_minimum_required(VERSION 3.16)

project(UniversalController
	VERSION 1.0.0.1
	DESCRIPTION "Universal Controller For Robots"
	HOMEPAGE_URL ""
	LANGUAGES CXX
)

#<FindPackage>
find_package(Boost COMPONENTS system REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
#</FindPackage>

#<SetCustomVariable>
set(custom_code_dir ${PROJECT})
#</SetCustomVariable>

message("Configuring Library ${libraryName} with codes in ${codeDir}.")

set(sg_code_root_dir "${codeDir}/Code" PARENT_SCOPE)
set(sg_code_helper_dir "${sg_code_root_dir}/Helpers" PARENT_SCOPE)
set(sg_code_script_dir "${sg_code_root_dir}/Scripts" PARENT_SCOPE)
set(sg_code_header_dir "${sg_code_root_dir}/Headers" PARENT_SCOPE)
set(sg_code_source_dir "${sg_code_root_dir}/Sources" PARENT_SCOPE)
set(sg_code_tester_dir "${sg_code_root_dir}/Testers" PARENT_SCOPE)
set(sg_link_libraries )

message("[${libraryName}] sg_code_root_dir -> ${sg_code_root_dir}")
message("[${libraryName}] sg_code_helper_dir -> ${sg_code_helper_dir}")
message("[${libraryName}] sg_code_script_dir -> ${sg_code_script_dir}")
message("[${libraryName}] sg_code_header_dir -> ${sg_code_header_dir}")
message("[${libraryName}] sg_code_source_dir -> ${sg_code_source_dir}")
message("[${libraryName}] sg_code_tester_dir -> ${sg_code_tester_dir}")
