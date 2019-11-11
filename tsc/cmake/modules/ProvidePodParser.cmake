if (USE_SYSTEM_PODPARSER)
  find_path(PodParser_INCLUDE_DIRS pod.hpp)
  find_library(PodParser_LIBRARIES pod-cpp)
  message("-- Found pod-cpp at ${PodParser_LIBRARIES}")
else()
  message("-- Building pod-cpp statically")
  add_library(podparser STATIC
    "${TSC_SOURCE_DIR}/../pod-parser/pod.cpp"
    "${TSC_SOURCE_DIR}/../pod-parser/pod.hpp")

  set(PodParser_LIBRARIES podparser)
  set(PodParser_INCLUDE_DIRS "${TSC_SOURCE_DIR}/../pod-parser")
endif()
