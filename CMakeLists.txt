  INCLUDE( ${QT_USE_FILE} )


INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )


QT4_WRAP_UI( UI_SOURCE 
  Display/RenderStateUI.ui
  Display/SceneGraphUI.ui
)

QT4_WRAP_CPP(MOC_SOURCE
#  Display/QtResponder.h
  Display/RenderStateGUI.h
  Display/SceneGraphGUI.h
  )



# Create the extension library
ADD_LIBRARY(Extensions_Qt
  Display/QtEnvironment.h
  Display/QtEnvironment.cpp
  Display/QtFrame.h
  Display/QtFrame.cpp
#  Display/QtResponder.cpp
  Display/RenderStateGUI.cpp
  Display/SceneGraphGUI.cpp
  Utils/FPSGUI.cpp
  ${MOC_SOURCE}
  ${UI_SOURCE}

)

# INCLUDE( ${QT_USE_FILE} )



# INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )


TARGET_LINK_LIBRARIES(Extensions_Qt
  ${QT_LIBRARIES}
)
