add_rules("mode.debug", "mode.release")

target("particle_system")
    set_languages("c++20")
    set_kind("binary")
    add_includedirs("src")
    add_files("src/*.cpp")
    add_links("m","GL","GLU","glut")