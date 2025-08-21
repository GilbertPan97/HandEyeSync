# Macro: copy_imported_dlls
# Usage: copy_imported_dlls(<target> <json_file>)
#
# This macro reads a JSON file listing DLLs and folders to copy.
# The JSON file should list relative paths from the project source directory.
#
# Example JSON:
# {
#   "debug": {
#     "files": [
#       "3rd-party/opencv/bin/opencv_core454d.dll"
#     ],
#     "folders": [
#       "3rd-party/qt5.12.0-vc15-64/plugins/platforms"
#     ]
#   },
#   "release": {
#     "files": [
#       "3rd-party/opencv/bin/opencv_core454.dll"
#     ],
#     "folders": [
#       "3rd-party/qt5.12.0-vc15-64/plugins/platforms"
#     ]
#   }
# }

macro(copy_imported_dlls target json_file)
    # Check if JSON file exists
    if(NOT EXISTS ${json_file})
        message(FATAL_ERROR "JSON file not found: ${json_file}")
    endif()

    file(READ ${json_file} JSON_CONTENT)

    # Determine current build type
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(JSON_KEY "debug")
    else()
        set(JSON_KEY "release")
    endif()

    # ---- Copy individual files ----
    string(JSON FILE_COUNT LENGTH ${JSON_CONTENT} ${JSON_KEY} files)
    if(FILE_COUNT GREATER 0)
        math(EXPR FILE_LAST_INDEX "${FILE_COUNT}-1")
        foreach(i RANGE 0 ${FILE_LAST_INDEX})
            string(JSON FILE_REL_PATH GET ${JSON_CONTENT} ${JSON_KEY} files ${i})
            # Convert relative path to absolute path
            get_filename_component(FILE_PATH "${CMAKE_SOURCE_DIR}/${FILE_REL_PATH}" ABSOLUTE)

            # Before calling add_custom_command
            if(NOT EXISTS "${FILE_PATH}")
                message(WARNING "File not found: ${FILE_PATH}, skipping copy")
            else()
                add_custom_command(
                    TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${FILE_PATH}" "$<TARGET_FILE_DIR:${target}>"
                    COMMENT "Copying file ${FILE_PATH} to $<TARGET_FILE_DIR:${target}>"
                )
            endif()

        endforeach()
    endif()

    # ---- Copy entire folders ----
    string(JSON FOLDER_COUNT LENGTH ${JSON_CONTENT} ${JSON_KEY} folders)
    if(FOLDER_COUNT GREATER 0)
        math(EXPR FOLDER_LAST_INDEX "${FOLDER_COUNT}-1")
        foreach(i RANGE 0 ${FOLDER_LAST_INDEX})
            string(JSON FOLDER_REL_PATH GET ${JSON_CONTENT} ${JSON_KEY} folders ${i})
            get_filename_component(FOLDER_PATH "${CMAKE_SOURCE_DIR}/${FOLDER_REL_PATH}" ABSOLUTE)

            if(NOT EXISTS "${FOLDER_PATH}")
                message(WARNING "Folder not found: ${FOLDER_PATH}, skipping copy")
            else()
                add_custom_command(
                    TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory "${FOLDER_PATH}" "$<TARGET_FILE_DIR:${target}>"
                    COMMENT "Copying folder ${FOLDER_PATH} to $<TARGET_FILE_DIR:${target}>"
                )
            endif()
        endforeach()
    endif()
endmacro()
