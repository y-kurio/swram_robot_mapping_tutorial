# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "swram_robot_mapping_tutorial: 1 messages, 0 services")

set(MSG_I_FLAGS "-Iswram_robot_mapping_tutorial:/home/ubuntu/catkin_ws/src/swram_robot_mapping_tutorial/msg;-Isensor_msgs:/opt/ros/melodic/share/sensor_msgs/cmake/../msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/melodic/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(swram_robot_mapping_tutorial_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/ubuntu/catkin_ws/src/swram_robot_mapping_tutorial/msg/cluster_data.msg" NAME_WE)
add_custom_target(_swram_robot_mapping_tutorial_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "swram_robot_mapping_tutorial" "/home/ubuntu/catkin_ws/src/swram_robot_mapping_tutorial/msg/cluster_data.msg" "geometry_msgs/Point32:geometry_msgs/PolygonStamped:geometry_msgs/Point:geometry_msgs/Polygon:std_msgs/Header"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(swram_robot_mapping_tutorial
  "/home/ubuntu/catkin_ws/src/swram_robot_mapping_tutorial/msg/cluster_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PolygonStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/swram_robot_mapping_tutorial
)

### Generating Services

### Generating Module File
_generate_module_cpp(swram_robot_mapping_tutorial
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/swram_robot_mapping_tutorial
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(swram_robot_mapping_tutorial_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(swram_robot_mapping_tutorial_generate_messages swram_robot_mapping_tutorial_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ubuntu/catkin_ws/src/swram_robot_mapping_tutorial/msg/cluster_data.msg" NAME_WE)
add_dependencies(swram_robot_mapping_tutorial_generate_messages_cpp _swram_robot_mapping_tutorial_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(swram_robot_mapping_tutorial_gencpp)
add_dependencies(swram_robot_mapping_tutorial_gencpp swram_robot_mapping_tutorial_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS swram_robot_mapping_tutorial_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(swram_robot_mapping_tutorial
  "/home/ubuntu/catkin_ws/src/swram_robot_mapping_tutorial/msg/cluster_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PolygonStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/swram_robot_mapping_tutorial
)

### Generating Services

### Generating Module File
_generate_module_eus(swram_robot_mapping_tutorial
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/swram_robot_mapping_tutorial
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(swram_robot_mapping_tutorial_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(swram_robot_mapping_tutorial_generate_messages swram_robot_mapping_tutorial_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ubuntu/catkin_ws/src/swram_robot_mapping_tutorial/msg/cluster_data.msg" NAME_WE)
add_dependencies(swram_robot_mapping_tutorial_generate_messages_eus _swram_robot_mapping_tutorial_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(swram_robot_mapping_tutorial_geneus)
add_dependencies(swram_robot_mapping_tutorial_geneus swram_robot_mapping_tutorial_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS swram_robot_mapping_tutorial_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(swram_robot_mapping_tutorial
  "/home/ubuntu/catkin_ws/src/swram_robot_mapping_tutorial/msg/cluster_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PolygonStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/swram_robot_mapping_tutorial
)

### Generating Services

### Generating Module File
_generate_module_lisp(swram_robot_mapping_tutorial
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/swram_robot_mapping_tutorial
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(swram_robot_mapping_tutorial_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(swram_robot_mapping_tutorial_generate_messages swram_robot_mapping_tutorial_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ubuntu/catkin_ws/src/swram_robot_mapping_tutorial/msg/cluster_data.msg" NAME_WE)
add_dependencies(swram_robot_mapping_tutorial_generate_messages_lisp _swram_robot_mapping_tutorial_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(swram_robot_mapping_tutorial_genlisp)
add_dependencies(swram_robot_mapping_tutorial_genlisp swram_robot_mapping_tutorial_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS swram_robot_mapping_tutorial_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(swram_robot_mapping_tutorial
  "/home/ubuntu/catkin_ws/src/swram_robot_mapping_tutorial/msg/cluster_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PolygonStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/swram_robot_mapping_tutorial
)

### Generating Services

### Generating Module File
_generate_module_nodejs(swram_robot_mapping_tutorial
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/swram_robot_mapping_tutorial
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(swram_robot_mapping_tutorial_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(swram_robot_mapping_tutorial_generate_messages swram_robot_mapping_tutorial_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ubuntu/catkin_ws/src/swram_robot_mapping_tutorial/msg/cluster_data.msg" NAME_WE)
add_dependencies(swram_robot_mapping_tutorial_generate_messages_nodejs _swram_robot_mapping_tutorial_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(swram_robot_mapping_tutorial_gennodejs)
add_dependencies(swram_robot_mapping_tutorial_gennodejs swram_robot_mapping_tutorial_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS swram_robot_mapping_tutorial_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(swram_robot_mapping_tutorial
  "/home/ubuntu/catkin_ws/src/swram_robot_mapping_tutorial/msg/cluster_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PolygonStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/swram_robot_mapping_tutorial
)

### Generating Services

### Generating Module File
_generate_module_py(swram_robot_mapping_tutorial
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/swram_robot_mapping_tutorial
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(swram_robot_mapping_tutorial_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(swram_robot_mapping_tutorial_generate_messages swram_robot_mapping_tutorial_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ubuntu/catkin_ws/src/swram_robot_mapping_tutorial/msg/cluster_data.msg" NAME_WE)
add_dependencies(swram_robot_mapping_tutorial_generate_messages_py _swram_robot_mapping_tutorial_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(swram_robot_mapping_tutorial_genpy)
add_dependencies(swram_robot_mapping_tutorial_genpy swram_robot_mapping_tutorial_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS swram_robot_mapping_tutorial_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/swram_robot_mapping_tutorial)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/swram_robot_mapping_tutorial
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET sensor_msgs_generate_messages_cpp)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_cpp sensor_msgs_generate_messages_cpp)
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/swram_robot_mapping_tutorial)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/swram_robot_mapping_tutorial
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET sensor_msgs_generate_messages_eus)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_eus sensor_msgs_generate_messages_eus)
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/swram_robot_mapping_tutorial)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/swram_robot_mapping_tutorial
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET sensor_msgs_generate_messages_lisp)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_lisp sensor_msgs_generate_messages_lisp)
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/swram_robot_mapping_tutorial)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/swram_robot_mapping_tutorial
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET sensor_msgs_generate_messages_nodejs)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_nodejs sensor_msgs_generate_messages_nodejs)
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/swram_robot_mapping_tutorial)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/swram_robot_mapping_tutorial\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/swram_robot_mapping_tutorial
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET sensor_msgs_generate_messages_py)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_py sensor_msgs_generate_messages_py)
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(swram_robot_mapping_tutorial_generate_messages_py geometry_msgs_generate_messages_py)
endif()
