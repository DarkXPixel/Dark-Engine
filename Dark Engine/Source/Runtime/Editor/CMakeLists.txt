add_library(Editor)

find_library(EditorLibs Core Engine)

target_compile_features(Editor PUBLIC cxx_std_20)
target_link_libraries(Editor PRIVATE Core Engine)
#target_link_libraries(Editor PUBLIC ${EditorLibs})

set(Editor_Sources ${Editor_Sources} "private/DEditor.cpp")

#GroupSourcesByFolder(Editor "${Editor_Sources}")


target_sources(Editor PRIVATE "private/DEditor.cpp")
target_include_directories(Editor PUBLIC "public")
SettingProject(Editor)
#install(TARGETS Editor)


