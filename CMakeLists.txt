
# Create the extension library
ADD_LIBRARY(Extensions_Qt
  Display/QtEnvironment.h
  Display/QtEnvironment.cpp
  Display/QtFrame.h
  Display/QtFrame.cpp
)

TARGET_LINK_LIBRARIES(Extensions_Qt
  ${QT_LIBRARIES}
)
