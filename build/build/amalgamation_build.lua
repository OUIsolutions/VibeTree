local alreay_amalamated_done = false
function amalgamation_build()
    if alreay_amalamated_done then
        return
    end
    alreay_amalamated_done = true

    local prev = [[
        #ifndef PATH_MAX
    #define PATH_MAX 4096
    #endif
    ]]
    local runtime = prev.. darwin.camalgamator.generate_amalgamation("src/main.c")
    darwin.dtw.write_file("release/amalgamation.c", runtime)

end
