cmake_minimum_required(VERSION 3.5)

project(Memorable)

SET(INCLUDE_DIRS)
SET(PRIV_INCLUDE_DIRS)
SET(SOURCE_CPPS)

#Add subdirectories

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fconcepts -std=c++2a")
add_subdirectory(Memory)
#add_subdirectory(Core)

include_directories(${INCLUDE_DIRS})


#make targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_CPPS})

#INSTALL
include(GNUInstallDirs)


#install(#TARGETS ${PROJECT_NAME}
        #EXPORT ${PROJECT_NAME}Targets
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}
#        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
#)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
				 
list(LENGTH INCLUDE_DIRS TOTAL_INCLUDES)
MATH(EXPR TOTAL_INCLUDES "${TOTAL_INCLUDES}-1")
foreach(i RANGE ${TOTAL_INCLUDES})
	list(GET INCLUDE_DIRS ${i} dir)
	install(DIRECTORY ${dir}/ 
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
		FILES_MATCHING PATTERN "*.h"
	)
endforeach()

#TEST    
#add_executable(${PROJECT_NAME}Test graphicsdemo.cc)
#target_link_libraries(${PROJECT_NAME}Test ${PROJECT_NAME} CUDA_LIB)
include(CTest)
enable_testing()
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING))
	SET(TEST_FILES )
	add_subdirectory(tests) #get the tests
	list(LENGTH TEST_FILES NUM_TESTS)
	MATH(EXPR NUM_TESTS "${NUM_TESTS}-1")
	foreach(i RANGE ${NUM_TESTS})
		list(GET TEST_FILES ${i} test)
		#add_executable(${test} tests/${test})
		#target_link_libraries(${test} ${PROJECT_NAME})
		
		#set_target_properties(${test} PROPERTIES OUTPUT_NAME ${test})
		add_test(NAME ${test}
			 COMMAND ${test})
	endforeach()
endif()
#



