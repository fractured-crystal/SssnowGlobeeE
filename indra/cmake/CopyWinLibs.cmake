# -*- cmake -*-

# The copy_win_libs folder contains file lists and a script used to 
# copy dlls, exes and such needed to run the SecondLife from within 
# VisualStudio. 

include(CMakeCopyIfDifferent)

set(vivox_src_dir "${CMAKE_SOURCE_DIR}/newview/vivox-runtime/i686-win32")
set(vivox_files
    SLVoice.exe
    alut.dll
    vivoxsdk.dll
    ortp.dll
    wrap_oal.dll
    )
copy_if_different(
    ${vivox_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/Debug"
    out_targets
    ${vivox_files}
    )
set(all_targets ${all_targets} ${out_targets})


set(debug_src_dir "${CMAKE_SOURCE_DIR}/../libraries/i686-win32/lib/debug")
set(debug_files
    libhunspell.dll
    libapr-1.dll
    libaprutil-1.dll
    libapriconv-1.dll
    )

copy_if_different(
    ${debug_src_dir} 
    "${CMAKE_CURRENT_BINARY_DIR}/Debug"
    out_targets 
    ${debug_files}
    )
set(all_targets ${all_targets} ${out_targets})

set(debug_src_dir "${CMAKE_SOURCE_DIR}/../fmodapi375win/api")
set(debug_files
    fmod.dll
    )
    
copy_if_different(
    ${debug_src_dir} 
    "${CMAKE_CURRENT_BINARY_DIR}/Debug"
    out_targets 
    ${debug_files}
    )
set(all_targets ${all_targets} ${out_targets})

# Debug config runtime files required for the plugin test mule
set(plugintest_debug_src_dir "${CMAKE_SOURCE_DIR}/../libraries/i686-win32/lib/debug")
set(plugintest_debug_files
    libeay32.dll
    libglib-2.0-0.dll
    libgmodule-2.0-0.dll
    libgobject-2.0-0.dll
    libgthread-2.0-0.dll
    qtcored4.dll
    qtguid4.dll
    qtnetworkd4.dll
    qtopengld4.dll
    qtwebkitd4.dll
    ssleay32.dll
    )
copy_if_different(
    ${plugintest_debug_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/../test_apps/llplugintest/Debug"
    out_targets
    ${plugintest_debug_files}
    )
set(all_targets ${all_targets} ${out_targets})

# Debug config runtime files required for the plugin test mule (Qt image format plugins)
set(plugintest_debug_src_dir "${CMAKE_SOURCE_DIR}/../libraries/i686-win32/lib/debug/imageformats")
set(plugintest_debug_files
    qgifd4.dll
    qicod4.dll
    qjpegd4.dll
    qmngd4.dll
    qsvgd4.dll
    qtiffd4.dll
    )
copy_if_different(
    ${plugintest_debug_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/../test_apps/llplugintest/Debug/imageformats"
    out_targets
    ${plugintest_debug_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${plugintest_debug_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/llplugin/imageformats"
    out_targets
    ${plugintest_debug_files}
    )
set(all_targets ${all_targets} ${out_targets})

# Release & ReleaseDebInfo config runtime files required for the plugin test mule
set(plugintest_release_src_dir "${CMAKE_SOURCE_DIR}/../libraries/i686-win32/lib/release")
set(plugintest_release_files
    libeay32.dll
    libglib-2.0-0.dll
    libgmodule-2.0-0.dll
    libgobject-2.0-0.dll
    libgthread-2.0-0.dll
#    llkdu.dll        (not required for plugin test)
    qtcore4.dll
    qtgui4.dll
    qtnetwork4.dll
    qtopengl4.dll
    qtwebkit4.dll
    ssleay32.dll
    )
copy_if_different(
    ${plugintest_release_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/../test_apps/llplugintest/Release"
    out_targets
    ${plugintest_release_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${plugintest_release_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/../test_apps/llplugintest/ReleaseSSE2"
    out_targets
    ${plugintest_release_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${plugintest_release_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/../test_apps/llplugintest/RelWithDebInfo"
    out_targets
    ${plugintest_release_files}
    )
set(all_targets ${all_targets} ${out_targets})

# Release & ReleaseDebInfo config runtime files required for the plugin test mule (Qt image format plugins)
set(plugintest_release_src_dir "${CMAKE_SOURCE_DIR}/../libraries/i686-win32/lib/release/imageformats")
set(plugintest_release_files
    qgif4.dll
    qico4.dll
    qjpeg4.dll
    qmng4.dll
    qsvg4.dll
    qtiff4.dll
    )
copy_if_different(
    ${plugintest_release_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/../test_apps/llplugintest/Release/imageformats"
    out_targets
    ${plugintest_release_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${plugintest_release_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/../test_apps/llplugintest/ReleaseSSE2/imageformats"
    out_targets
    ${plugintest_release_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${plugintest_release_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/../test_apps/llplugintest/RelWithDebInfo/imageformats"
    out_targets
    ${plugintest_release_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${plugintest_release_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/Release/llplugin/imageformats"
    out_targets
    ${plugintest_release_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${plugintest_release_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/ReleaseSSE2/llplugin/imageformats"
    out_targets
    ${plugintest_release_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${plugintest_release_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo/llplugin/imageformats"
    out_targets
    ${plugintest_release_files}
    )
set(all_targets ${all_targets} ${out_targets})

# Debug config runtime files required for the plugins
set(plugins_debug_src_dir "${CMAKE_SOURCE_DIR}/../libraries/i686-win32/lib/debug")
set(plugins_debug_files
    libeay32.dll
    qtcored4.dll
    qtguid4.dll
    qtnetworkd4.dll
    qtopengld4.dll
    qtwebkitd4.dll
    ssleay32.dll
    )
copy_if_different(
    ${plugins_debug_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/Debug/llplugin"
    out_targets
    ${plugins_debug_files}
    )
set(all_targets ${all_targets} ${out_targets})

# Release & ReleaseDebInfo config runtime files required for the plugins
set(plugins_release_src_dir "${CMAKE_SOURCE_DIR}/../libraries/i686-win32/lib/release")
set(plugins_release_files
    libeay32.dll
    qtcore4.dll
    qtgui4.dll
    qtnetwork4.dll
    qtopengl4.dll
    qtwebkit4.dll
    ssleay32.dll
    )
copy_if_different(
    ${plugins_release_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/Release/llplugin"
    out_targets
    ${plugins_release_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${plugins_release_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/ReleaseSSE2/llplugin"
    out_targets
    ${plugins_release_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${plugins_release_src_dir}
    "${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo/llplugin"
    out_targets
    ${plugins_release_files}
    )
set(all_targets ${all_targets} ${out_targets})

set(release_src_dir "${CMAKE_SOURCE_DIR}/../libraries/i686-win32/lib/release")
set(release_files
    libtcmalloc_minimal.dll
    libhunspell.dll
    libapr-1.dll
    libaprutil-1.dll
    libapriconv-1.dll
    )
    
copy_if_different(
    ${release_src_dir} 
    "${CMAKE_CURRENT_BINARY_DIR}/Release"
    out_targets 
    ${release_files}
    )
set(all_targets ${all_targets} ${out_targets})

set(release_src_dir "${CMAKE_SOURCE_DIR}/../fmodapi375win/api")
set(release_files
    fmod.dll
    )
    
copy_if_different(
    ${release_src_dir} 
    "${CMAKE_CURRENT_BINARY_DIR}/Release"
    out_targets 
    ${release_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${vivox_src_dir} 
    "${CMAKE_CURRENT_BINARY_DIR}/Release"
    out_targets 
    ${vivox_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${release_src_dir} 
    "${CMAKE_CURRENT_BINARY_DIR}/ReleaseSSE2"
    out_targets 
    ${release_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${vivox_src_dir} 
    "${CMAKE_CURRENT_BINARY_DIR}/ReleaseSSE2"
    out_targets 
    ${vivox_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${release_src_dir} 
    "${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo"
    out_targets 
    ${release_files}
    )
set(all_targets ${all_targets} ${out_targets})

copy_if_different(
    ${vivox_src_dir} 
    "${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo"
    out_targets 
    ${vivox_files}
    )
set(all_targets ${all_targets} ${out_targets})

if(MSVC10)
    FIND_PATH(release_msvc10_redist_path msvcr100.dll
        PATHS
        ${MSVC_REDIST_PATH}
	if ARCH_TYPE=x64		
		 [HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\10.0\\Setup\\VC;ProductDir]/redist/x86/Microsoft.VC100.CRT
	endif ARCH_TYPE=x64
	if ARCH_TYPE=x86
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\10.0\\Setup\\VC;ProductDir]/redist/x86/Microsoft.VC100.CRT
	endif ARCH_TYPE=x86
        NO_DEFAULT_PATH
        NO_DEFAULT_PATH
        )
    if(EXISTS ${release_msvc10_redist_path})
        set(release_msvc10_files
            msvcr100.dll
            msvcp100.dll
            )

        copy_if_different(
            ${release_msvc10_redist_path} 
            "${CMAKE_CURRENT_BINARY_DIR}/Release"
            out_targets 
            ${release_msvc10_files}
            )
        set(all_targets ${all_targets} ${out_targets})

        copy_if_different(
            ${release_msvc10_redist_path} 
            "${CMAKE_CURRENT_BINARY_DIR}/ReleaseSSE2"
            out_targets 
            ${release_msvc10_files}
            )
        set(all_targets ${all_targets} ${out_targets})
	endif(EXISTS ${release_msvc10_redist_path})
endif(MSVC10)

# Copy MS C runtime dlls, required for packaging.
# *TODO - Adapt this to support VC9
if (MSVC80)
    FIND_PATH(debug_msvc8_redist_path msvcr80d.dll
        PATHS
         [HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\8.0\\Setup\\VC;ProductDir]/redist/Debug_NonRedist/x86/Microsoft.VC80.DebugCRT
        NO_DEFAULT_PATH
        NO_DEFAULT_PATH
        )

    if(EXISTS ${debug_msvc8_redist_path})
        set(debug_msvc8_files
            msvcr80d.dll
            msvcp80d.dll
            Microsoft.VC80.DebugCRT.manifest
            )

        copy_if_different(
            ${debug_msvc8_redist_path} 
            "${CMAKE_CURRENT_BINARY_DIR}/Debug"
            out_targets 
            ${debug_msvc8_files}
            )
        set(all_targets ${all_targets} ${out_targets})

        set(debug_appconfig_file ${CMAKE_CURRENT_BINARY_DIR}/Debug/${VIEWER_BINARY_NAME}.exe.config)
        add_custom_command(
            OUTPUT ${debug_appconfig_file}
            COMMAND ${PYTHON_EXECUTABLE}
            ARGS
              ${CMAKE_CURRENT_SOURCE_DIR}/build_win32_appConfig.py
              ${CMAKE_CURRENT_BINARY_DIR}/Debug/Microsoft.VC80.DebugCRT.manifest
              ${CMAKE_CURRENT_SOURCE_DIR}/SecondLifeDebug.exe.config
              ${debug_appconfig_file}
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Debug/Microsoft.VC80.DebugCRT.manifest
            COMMENT "Creating debug app config file"
            )

    endif (EXISTS ${debug_msvc8_redist_path})

    FIND_PATH(release_msvc8_redist_path msvcr80.dll
        PATHS
         [HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\8.0\\Setup\\VC;ProductDir]/redist/x86/Microsoft.VC80.CRT
        NO_DEFAULT_PATH
        NO_DEFAULT_PATH
        )

    if(EXISTS ${release_msvc8_redist_path})
        set(release_msvc8_files
            msvcr80.dll
            msvcp80.dll
            Microsoft.VC80.CRT.manifest
            )

        copy_if_different(
            ${release_msvc8_redist_path} 
            "${CMAKE_CURRENT_BINARY_DIR}/Release"
            out_targets 
            ${release_msvc8_files}
            )
        set(all_targets ${all_targets} ${out_targets})

        copy_if_different(
            ${release_msvc8_redist_path} 
            "${CMAKE_CURRENT_BINARY_DIR}/ReleaseSSE2"
            out_targets 
            ${release_msvc8_files}
            )
        set(all_targets ${all_targets} ${out_targets})

        copy_if_different(
            ${release_msvc8_redist_path} 
            "${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo"
            out_targets 
            ${release_msvc8_files}
            )
        set(all_targets ${all_targets} ${out_targets})

        set(release_appconfig_file ${CMAKE_CURRENT_BINARY_DIR}/Release/${VIEWER_BINARY_NAME}.exe.config)
        add_custom_command(
            OUTPUT ${release_appconfig_file}
            COMMAND ${PYTHON_EXECUTABLE}
            ARGS
              ${CMAKE_CURRENT_SOURCE_DIR}/build_win32_appConfig.py
              ${CMAKE_CURRENT_BINARY_DIR}/Release/Microsoft.VC80.CRT.manifest
              ${CMAKE_CURRENT_SOURCE_DIR}/SecondLife.exe.config
              ${release_appconfig_file}
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Release/Microsoft.VC80.CRT.manifest
            COMMENT "Creating release app config file"
            )

        set(releasesse2_appconfig_file ${CMAKE_CURRENT_BINARY_DIR}/ReleaseSSE2/${VIEWER_BINARY_NAME}.exe.config)
        add_custom_command(
            OUTPUT ${releasesse2_appconfig_file}
            COMMAND ${PYTHON_EXECUTABLE}
            ARGS
              ${CMAKE_CURRENT_SOURCE_DIR}/build_win32_appConfig.py
              ${CMAKE_CURRENT_BINARY_DIR}/ReleaseSSE2/Microsoft.VC80.CRT.manifest
              ${CMAKE_CURRENT_SOURCE_DIR}/SecondLife.exe.config
              ${releasesse2_appconfig_file}
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ReleaseSSE2/Microsoft.VC80.CRT.manifest
            COMMENT "Creating release-sse2 app config file"
            )
            
        set(relwithdebinfo_appconfig_file ${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo/${VIEWER_BINARY_NAME}.exe.config)
        add_custom_command(
            OUTPUT ${relwithdebinfo_appconfig_file}
            COMMAND ${PYTHON_EXECUTABLE}
            ARGS
              ${CMAKE_CURRENT_SOURCE_DIR}/build_win32_appConfig.py
              ${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo/Microsoft.VC80.CRT.manifest
              ${CMAKE_CURRENT_SOURCE_DIR}/SecondLife.exe.config
              ${relwithdebinfo_appconfig_file}
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo/Microsoft.VC80.CRT.manifest
            COMMENT "Creating relwithdebinfo app config file"
            )
          
    endif (EXISTS ${release_msvc8_redist_path})
endif (MSVC80)

add_custom_target(copy_win_libs ALL
  DEPENDS 
    ${all_targets}
    ${release_appconfig_file} 
    ${releasesse2_appconfig_file} 
    ${relwithdebinfo_appconfig_file} 
    ${debug_appconfig_file}
  )
add_dependencies(copy_win_libs prepare)

if(EXISTS ${internal_llkdu_path})
    add_dependencies(copy_win_libs llkdu)
endif(EXISTS ${internal_llkdu_path})
