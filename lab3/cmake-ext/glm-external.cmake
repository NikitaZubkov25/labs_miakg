# SDL library
URL_OR_FILE(  "${PROJECT_DOWNLOAD_DIR}/glm-${GLM_VERSION}.7z"
  "https://github.com/g-truc/glm/releases/download/${GLM_VERSION}/glm-${GLM_VERSION}.7z"
)

set(GLM_INSTALL_PREFIX      ${CMAKE_BINARY_DIR}/external-libs/installed)
set(GLM_INCLUDE_DIR ${GLM_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})

ExternalProject_Add(glm-external
  URL ${CURL}
  URL_MD5 c8342552801ebeb31497288192c4e793
  PREFIX ${GLM_INSTALL_PREFIX}/source-tree
  DOWNLOAD_DIR ${PROJECT_DOWNLOAD_DIR}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                  <SOURCE_DIR>/glm ${GLM_INSTALL_PREFIX}/glm
  UPDATE_DISCONNECTED ON
)

add_dependencies(external_all glm-external)
