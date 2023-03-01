add_rules("mode.debug", "mode.release")

target("raytracer")
    set_kind("binary")
    add_includedirs("src")
    add_files("src/*.cpp")
    add_links("m","GL","GLU","glut")