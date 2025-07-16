function Install_dependencies()
    local hasher = darwin.dtw.newHasher()
    hasher.digest_folder_by_content("dependencies")
    local TARGET_HASH = "b576c55b53ff050ffcb5a85507ebbe8b6b77cd53e0d7b28e06798c7f88418902"
    if hasher.get_value() == TARGET_HASH then
        return
    end
    local comands = {
        "mkdir -p dependencies",
        "curl -L https://github.com/OUIsolutions/DoTheWorld/releases/download/10.2.0/doTheWorldOne.c -o dependencies/doTheWorldOne.c",
        "curl -L https://github.com/OUIsolutions/C-argv-parser/releases/download/0.0.2/CArgvParseOne.c -o dependencies/CArgvParseOne.c",
    }
    for _, command in ipairs(comands) do
        os.execute(command)
    end
    local new_hasher = darwin.dtw.newHasher()
    new_hasher.digest_folder_by_content("dependencies")
    print("new hasher is: ", new_hasher.get_value())
end