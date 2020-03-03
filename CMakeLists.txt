cmake_minimum_required(VERSION 3.15)
project(connector)

set(CMAKE_CXX_STANDARD 14)

add_executable(connector main.cpp)
target_link_libraries(connector stdc++fs)
install(TARGETS connector DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
#TODO: Fix symlinking to VPNmanip
install(CODE "execute_process( \
            COMMAND ${CMAKE_COMMAND} -E create_symlink \
            connector \
            ${CMAKE_INSTALL_PREFIX}/bin/vpnmanip \
            )"
        )
install(FILES connector.1 DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man1")
install(FILES disconnector DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
install(FILES disconnector.1 DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man1")

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "cam-o")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "openvpn")
include(CPack)