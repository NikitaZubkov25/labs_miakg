# Return first argument of path or url to download library

function(URL_OR_FILE)
  if(EXISTS "${ARGV0}")
    message("File ${ARGV0} exists")
    set(CURL  "${ARGV0}" PARENT_SCOPE)
  else()
    message("File not exists download ${ARGV1}")
    set(CURL  "${ARGV1}" PARENT_SCOPE)
  endif()
endfunction()

