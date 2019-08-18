project(directdemocr)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( directdemocr directdemocr directdemocr.cpp )
target_include_directories( directdemocr PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( directdemocr ${CMAKE_SOURCE_DIR}/../ricardian )