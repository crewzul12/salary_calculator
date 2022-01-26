cmake_minimum_required(VERSION 3.0.0)
project(salary_calc_proj VERSION 0.1.0)

include(CTest)
enable_testing()

include(/dev/vcpkg/scripts/buildsystems/vcpkg.cmake)

find_package(CURL CONFIG REQUIRED)



add_executable(salary_calc_proj main.c)
target_link_libraries(salary_calc_proj PRIVATE CURL::libcurl)


set(CPACK_PROJECT_NAME salary_calc_proj)
set(CPACK_PROJECT_VERSION VERSION 0.1.0)
include(CPack)
