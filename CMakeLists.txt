INCLUDE( ${QT_USE_FILE} )
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )

QT4_WRAP_UI( UI_SOURCE 
  Display/RenderStateUI.ui
  Display/SceneGraphUI.ui
  Display/TransformationNodeGUI.ui
  Display/RenderStateNodeGUI.ui
)

QT4_WRAP_CPP(MOC_SOURCE
#  Display/QtResponder.h
  Display/RenderStateGUI.h
  Display/SceneGraphGUI.h
  Display/TransformationNodeGUI.h
  Display/RenderStateNodeGUI.h
  )

QT4_ADD_RESOURCES(QRC_SOURCE
  icons/icons.qrc
)

MESSAGE(${QRC_SOURCE})

# Create the extension library
ADD_LIBRARY(Extensions_Qt
  Display/QtEnvironment.h
  Display/QtEnvironment.cpp
  Display/QtFrame.h
  Display/QtFrame.cpp
#  Display/QtResponder.cpp
  Display/RenderStateGUI.cpp
  Display/SceneGraphGUI.cpp
  Display/TransformationNodeGUI.cpp
  Display/RenderStateNodeGUI.cpp
  Utils/FPSGUI.cpp
  Display/SceneNodeGUI.h
  Display/SceneNodeGUI.cpp
  ${MOC_SOURCE}
  ${UI_SOURCE}
  ${QRC_SOURCE}
)

TARGET_LINK_LIBRARIES(Extensions_Qt
  ${QT_LIBRARIES}
  OpenEngine_Scene
)
