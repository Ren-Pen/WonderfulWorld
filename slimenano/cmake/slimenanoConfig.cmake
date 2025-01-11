
if(NOT TARGET Slimenano::Math)
    include("${CMAKE_CURRENT_LIST_DIR}/SlimenanoMathTargets.cmake")
endif(NOT TARGET Slimenano::Math)

if(NOT TARGET Slimenano::UI)
    include("${CMAKE_CURRENT_LIST_DIR}/SlimenanoUITargets.cmake")
endif(NOT TARGET Slimenano::UI)