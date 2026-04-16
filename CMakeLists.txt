cmake_minimum_required(VERSION 3.19)

project(HaramBallFC LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

set(BOX2D_SOURCES
    Box2D/Collision/Shapes/b2ChainShape.cpp
    Box2D/Collision/Shapes/b2CircleShape.cpp
    Box2D/Collision/Shapes/b2EdgeShape.cpp
    Box2D/Collision/Shapes/b2PolygonShape.cpp

    Box2D/Collision/b2BroadPhase.cpp
    Box2D/Collision/b2CollideCircle.cpp
    Box2D/Collision/b2CollideEdge.cpp
    Box2D/Collision/b2CollidePolygon.cpp
    Box2D/Collision/b2Collision.cpp
    Box2D/Collision/b2Distance.cpp
    Box2D/Collision/b2DynamicTree.cpp
    Box2D/Collision/b2TimeOfImpact.cpp

    Box2D/Common/b2BlockAllocator.cpp
    Box2D/Common/b2Draw.cpp
    Box2D/Common/b2Math.cpp
    Box2D/Common/b2Settings.cpp
    Box2D/Common/b2StackAllocator.cpp
    Box2D/Common/b2Timer.cpp

    Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp
    Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp
    Box2D/Dynamics/Contacts/b2CircleContact.cpp
    Box2D/Dynamics/Contacts/b2Contact.cpp
    Box2D/Dynamics/Contacts/b2ContactSolver.cpp
    Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp
    Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp
    Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp
    Box2D/Dynamics/Contacts/b2PolygonContact.cpp

    Box2D/Dynamics/Joints/b2DistanceJoint.cpp
    Box2D/Dynamics/Joints/b2FrictionJoint.cpp
    Box2D/Dynamics/Joints/b2GearJoint.cpp
    Box2D/Dynamics/Joints/b2Joint.cpp
    Box2D/Dynamics/Joints/b2MotorJoint.cpp
    Box2D/Dynamics/Joints/b2MouseJoint.cpp
    Box2D/Dynamics/Joints/b2PrismaticJoint.cpp
    Box2D/Dynamics/Joints/b2PulleyJoint.cpp
    Box2D/Dynamics/Joints/b2RevoluteJoint.cpp
    Box2D/Dynamics/Joints/b2RopeJoint.cpp
    Box2D/Dynamics/Joints/b2WeldJoint.cpp
    Box2D/Dynamics/Joints/b2WheelJoint.cpp

    Box2D/Dynamics/b2Body.cpp
    Box2D/Dynamics/b2ContactManager.cpp
    Box2D/Dynamics/b2Fixture.cpp
    Box2D/Dynamics/b2Island.cpp
    Box2D/Dynamics/b2World.cpp
    Box2D/Dynamics/b2WorldCallbacks.cpp

    Box2D/Rope/b2Rope.cpp
)

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

    include/models/QuizQuestion.h
    include/models/QuizRepository.h
    src/models/QuizRepository.cpp

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

    include/pages/BracketPage.h
    src/pages/BracketPage.cpp

    include/pages/QuizPage.h
    src/pages/QuizPage.cpp

    include/pages/PenaltyGame.h
    src/pages/PenaltyGame.cpp

    resources/resources.qrc

    ${BOX2D_SOURCES}

    include/pages/JugglingGame.h
    src/pages/JugglingGame.cpp
)

target_include_directories(HaramBallFC
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(HaramBallFC
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
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
