SET(REL Memory)
list(APPEND INCLUDE_DIRS ${REL})
list(APPEND SOURCE_CPPS ${REL}/Package.cpp)

SET(INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
SET(SOURCE_CPPS ${SOURCE_CPPS} PARENT_SCOPE)
