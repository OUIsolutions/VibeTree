---@class Darwin
darwin = darwin


function main()

    Install_dependencies()
    local build_funcs = create_build_funcs()


    darwin.dtw.remove_any("release")
    darwin.dtw.remove_any(".cache")
    local i = 1
    for name, buld_lambda in pairs(build_funcs) do
        if darwin.argv.one_of_args_exist(name) then
            print("\n=========================================================================================")
            print("\tstarted of build:", i, "\n")
            buld_lambda()
            print("\n\tA build [" .. i .. "] finished")
            print("=========================================================================================\n\n")
            i = i + 1
        end
    end

    darwin.dtw.move_any_overwriting("release/alpine_static_bin.out","release/VibeTree.out")
    darwin.dtw.move_any_overwriting("release/windows64.exe","release/VibeTree64.exe")
    darwin.dtw.move_any_overwriting("release/windowsi32.exe","release/VibeTreei32.exe")
    darwin.dtw.move_any_overwriting("release/debian_static.deb","release/VibeTree.deb")
    darwin.dtw.move_any_overwriting("release/rpm_static_build.rpm","release/VibeTree.rpm")

end
