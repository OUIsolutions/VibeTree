function Install_dependencies()
    local hasher = darwin.dtw.newHasher()
    hasher.digest_folder_by_content("dependencies")
    local TARGET_HASH = "90291b5144697288b1221f0baa348e36f810ff626445cda5ff95ba3f3a173789"
    if hasher.get_value() == TARGET_HASH then
        return
    end
    local comands = {
        "mkdir -p dependencies",
        "curl -L https://github.com/OUIsolutions/DoTheWorld/releases/download/10.1.1/doTheWorldOne.c -o dependencies/doTheWorldOne.c",
        "curl -L https://github.com/OUIsolutions/C-argv-parser/releases/download/0.0.2/CArgvParseOne.c -o dependencies/CArgvParseOne.c",
    }
    for _, command in ipairs(comands) do
        os.execute(command)
    end
    local new_hasher = darwin.dtw.newHasher()
    new_hasher.digest_folder_by_content("dependencies")
    print("new hasher is: ", new_hasher.get_value())
end