set(LLL_SOURCES 
LLL.c
LLL_functions.c
)

set(LLL_HEADERS
LLL_conf.h
LLL.h
LLL_functions.h
)
add_library(LLL ${LLL_SOURCES} ${LLL_HEADERS})
