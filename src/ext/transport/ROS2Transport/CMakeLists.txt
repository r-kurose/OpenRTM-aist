cmake_minimum_required(VERSION 3.5)

project (ROS2Transport
	VERSION ${RTM_VERSION}
	LANGUAGES C CXX)


if(NOT CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()



find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)



link_directories(${ORB_LINK_DIR})
include_directories(
		${RTM_INCLUDE_DIR}
		${ORB_INCLUDE_DIR}
		${PROJECT_SOURCE_DIR}/..
		${rclcpp_INCLUDE_DIRS}
		${std_msgs_INCLUDE_DIRS}
		${geometry_msgs_INCLUDE_DIRS}
		${sensor_msgs_INCLUDE_DIRS}
		)

add_definitions(${ORB_C_FLAGS_LIST})

if(WIN32)
	add_definitions(-DNOGDI)
endif()


set(target ROS2Transport)

set(srcs ROS2Transport.cpp ROS2Transport.h ROS2MessageInfo.cpp ROS2MessageInfo.h ROS2Serializer.cpp ROS2Serializer.h)


if(VXWORKS AND NOT RTP)
	set(libs ${RTCSKEL_PROJECT_NAME})

	add_executable(${target} ${srcs})
	target_link_libraries(${target} ${libs})
	
	add_dependencies(${target} ${RTM_PROJECT_NAME})

	install(TARGETS ${target} LIBRARY DESTINATION ${INSTALL_RTM_EXT_DIR}/transport
				ARCHIVE DESTINATION ${INSTALL_RTM_EXT_DIR}/transport
				RUNTIME DESTINATION ${INSTALL_RTM_EXT_DIR}/transport
				COMPONENT ext)
else()
	if(VXWORKS)
		set(libs ${RTM_PROJECT_NAME} ${ORB_LIBRARIES} ${COIL_PROJECT_NAME} ${RTCSKEL_PROJECT_NAME} FastRTPSTransport ${rclcpp_LIBRARIES} ${std_msgs_LIBRARIES} ${geometry_msgs_LIBRARIES}  ${sensor_msgs_LIBRARIES})
	else(VXWORKS)
		set(libs ${RTM_PROJECT_NAME} ${ORB_LIBRARIES} ${COIL_PROJECT_NAME} FastRTPSTransport ${rclcpp_LIBRARIES} ${std_msgs_LIBRARIES}  ${geometry_msgs_LIBRARIES} ${sensor_msgs_LIBRARIES})
	endif(VXWORKS)


	add_library(${target} SHARED ${srcs})
	target_link_libraries(${target} ${libs} ${RTM_LINKER_OPTION})
	add_dependencies(${target} ${RTM_PROJECT_NAME})
	add_dependencies(${target} FastRTPSTransport)
	set_target_properties(${target} PROPERTIES PREFIX "")


	install(TARGETS ${target} LIBRARY DESTINATION ${INSTALL_RTM_EXT_DIR}/transport
				ARCHIVE DESTINATION ${INSTALL_RTM_EXT_DIR}/transport
				RUNTIME DESTINATION ${INSTALL_RTM_EXT_DIR}/transport
				COMPONENT ext)
endif()






if(VXWORKS)
	if(RTP)
	else(RTP)	
		set_target_properties(${target} PROPERTIES SUFFIX ".out")
	endif(RTP)
endif(VXWORKS)





