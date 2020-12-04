set(TARGET restaurante)
add_executable(${TARGET} ${CMAKE_SOURCE_DIR}/main.c mesas.c pedidos.c pratos.c garcons.c tela.c)
