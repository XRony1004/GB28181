add_rules("mode.debug", "mode.release")

add_requires("exosip", "spdlog", "fmt", "jrtplib")

target("gbserver")
    set_kind("binary")

    add_packages("exosip", "jrtplib", "spdlog", "fmt")

    add_files("src/*.cpp")
    add_files("src/GB28181Server/*.cpp")
    add_files("src/RtpReciever/*.cpp")
    add_files("src/MD5/*.cpp")
    add_files("src/Device/*.cpp")
    add_files("src/Mutex/*.cpp")


    add_includedirs("src")
    add_includedirs("src/GB28181Server")
    add_includedirs("src/RtpReciever/")
    add_includedirs("src/MD5/")
    add_includedirs("src/Device/")
    add_includedirs("src/Mutex/")
    add_includedirs("src/EventHandle/")


    add_links("mxml")
    add_links("avformat", "avcodec", "avutil", "swresample", "swscale")

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

