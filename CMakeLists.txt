project(LLL LANGUAGES C VERSION 2.0)

set(LLL_FILES
                    LLL_conf.h
LLL.c               LLL.h
LLL_functions.c     LLL_functions.h
LLL_processing.c    LLL_processing.h
)

add_library(lll ${LLL_FILES})
