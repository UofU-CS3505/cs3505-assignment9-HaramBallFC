cmake_minimum_required(VERSION 3.19)

project(HaramBallFC LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(HaramBallFC
    WIN32
    MACOSX_BUNDLE

    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    include/models/Lesson.h
    include/models/LessonRepository.h
    src/models/LessonRepository.cpp

    include/pages/HomePage.h
    src/pages/HomePage.cpp

    include/pages/ModeSelectionPage.h
    src/pages/ModeSelectionPage.cpp

    include/pages/FanModePage.h
    src/pages/FanModePage.cpp

    include/pages/PlayerModePage.h
    src/pages/PlayerModePage.cpp

    include/pages/LessonMenuPage.h
    src/pages/LessonMenuPage.cpp

    include/pages/LessonViewerPage.h
    src/pages/LessonViewerPage.cpp
)

target_include_directories(HaramBallFC
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(HaramBallFC
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS HaramBallFC
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET HaramBallFC
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
