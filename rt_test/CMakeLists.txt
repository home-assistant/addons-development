cmake_minimum_required(VERSION 3.5)
project(MyMQTTApp VERSION 1.0.0 LANGUAGES C)

find_package(eclipse-paho-mqtt-c REQUIRED)

add_executable(mqtt-rt-test mqtt-rt-test.c)
target_link_libraries(mqtt-rt-test eclipse-paho-mqtt-c::paho-mqtt3a)
