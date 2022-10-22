file(GLOB PO_FILES *.po)

add_custom_command(
        OUTPUT ${CMAKE_PROJECT_NAME}.pot
        COMMAND intltool-update -g ${CMAKE_PROJECT_NAME} -p
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Updating default translation file"
    )

add_custom_target(update-translation DEPENDS ${CMAKE_PROJECT_NAME}.pot)

foreach (LANG ${PO_FILES})
    get_filename_component(LANG ${LANG} NAME)
    string(REGEX REPLACE "\\.po$" "" LANG ${LANG})
    set(MSGFMT_INPUT_FILE ${LANG}.po)
    set(MSGFMT_OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/${GETTEXT_PACKAGE}-${LANG}.mo)

    add_custom_command(
            OUTPUT ${MSGFMT_OUTPUT_FILE}
            COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --output-file=${MSGFMT_OUTPUT_FILE} ${MSGFMT_INPUT_FILE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${MSGFMT_INPUT_FILE}
            COMMENT "Compiling ${LANG} translation"
        )

    list(APPEND ${CMAKE_PROJECT_NAME}_MO_FILES ${MSGFMT_OUTPUT_FILE})

    install(FILES ${MSGFMT_OUTPUT_FILE} DESTINATION ${CMAKE_INSTALL_LOCALEDIR}/${LANG}/LC_MESSAGES/ RENAME ${GETTEXT_PACKAGE}.mo)
endforeach()

if (${CMAKE_PROJECT_NAME}_MO_FILES)
    add_custom_target(compile-translations DEPENDS ${${CMAKE_PROJECT_NAME}_MO_FILES})
    add_dependencies(${CMAKE_PROJECT_NAME} compile-translations)
endif()
