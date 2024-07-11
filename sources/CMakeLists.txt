set(CODEGEN_DIR ${PROJECT_BUILD_DIR}/codegen)

flex_target(scanner
  ${PROJECT_ROOT_DIR}/sources/scanner.ll
  ${CODEGEN_DIR}/scanner.cc
  DEFINES_FILE ${CODEGEN_DIR}/scanner.hh
)

bison_target(parser
  ${PROJECT_ROOT_DIR}/sources/grammar.yy
  ${CODEGEN_DIR}/parser.cc
  DEFINES_FILE ${CODEGEN_DIR}/parser.hh
)

add_flex_bison_dependency(scanner parser)

add_executable(${PROJECT_NAME} 
  Driver.cc
  main.cc
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE ${CODEGEN_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE $ENV{INCLUDE})

