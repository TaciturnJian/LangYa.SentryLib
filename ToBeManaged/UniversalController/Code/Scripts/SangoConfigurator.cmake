function(SANGO_CONFIGURE_LIBRARY libraryName codeDir)
	message("Configuring Library ${libraryName} with codes in ${codeDir}.")

	set(sg_code_root_dir "${codeDir}/Code" PARENT_SCOPE)
	set(sg_code_helper_dir "${sg_code_root_dir}/Helpers" PARENT_SCOPE)
	set(sg_code_script_dir "${sg_code_root_dir}/Scripts" PARENT_SCOPE)
	set(sg_code_header_dir "${sg_code_root_dir}/Headers" PARENT_SCOPE)
	set(sg_code_source_dir "${sg_code_root_dir}/Sources" PARENT_SCOPE)
	set(sg_code_tester_dir "${sg_code_root_dir}/Testers" PARENT_SCOPE)

	message("[${libraryName}] sg_code_root_dir -> ${sg_code_root_dir}")
	message("[${libraryName}] sg_code_helper_dir -> ${sg_code_helper_dir}")
	message("[${libraryName}] sg_code_script_dir -> ${sg_code_script_dir}")
	message("[${libraryName}] sg_code_header_dir -> ${sg_code_header_dir}")
	message("[${libraryName}] sg_code_source_dir -> ${sg_code_source_dir}")
	message("[${libraryName}] sg_code_tester_dir -> ${sg_code_tester_dir}")
endfunction()


