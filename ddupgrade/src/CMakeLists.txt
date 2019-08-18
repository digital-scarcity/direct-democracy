project(ddupgrade)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( ddupgrade ddupgrade ddupgrade.cpp )
target_include_directories( ddupgrade PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( ddupgrade ${CMAKE_SOURCE_DIR}/../ricardian )