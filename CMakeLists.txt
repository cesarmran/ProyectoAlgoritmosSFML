cmake_minimum_required(VERSION 3.28)
project(RetoSFML)
set(SFML_DIR "C:/Users/cmora/Documents/SFML-2.6.1/lib/cmake/SFML")  # Cambia a la ruta correcta
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
set(CMAKE_CXX_STANDARD 17)

add_executable(RetoSFML main.cpp
        BinarySearch.cpp
        BinarySearch.h
        LinearSearch.cpp
        LinearSearch.h
        Utils.cpp
        Utils.h
        BubbleSort.cpp
        BubbleSort.h
        QuickSort.cpp
        QuickSort.h
        SelectionSort.cpp
        SelectionSort.h
        InsertionSort.cpp
        InsertionSort.h
        MergeSort.cpp
        MergeSort.h
        stacks.cpp
        stacks.h
        queues.cpp
        queues.h
        LinkedLists.cpp
        LinkedLists.h
        AVL.cpp
        AVL.h
        Dijkstra.cpp
        Dijkstra.h
        DFS.cpp
        DFS.h
)
target_link_libraries(RetoSFML sfml-graphics sfml-window sfml-system)