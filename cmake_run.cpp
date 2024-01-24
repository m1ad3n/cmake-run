#include <cstring>
#include <ios>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <fstream>
#include <filesystem>
#include <vector>
#include <format>

std::pair<std::string, int> exec(std::string cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    int status = 0;
    return {result, status};
}

std::string find_exec_name() {
    std::fstream file_ptr("CMakeLists.txt", std::ios::in);

    if (!file_ptr)
    {
        std::cout << "Couldn't find the CMakeLists.txt" << std::endl;
        exit(1);
    }

    std::string content;
    std::string exec_name;
    while (std::getline(file_ptr, content))
        if (content.find("add_executable") != std::string::npos)
            break;

    int index = -1;
    for (auto c : content)
    {
        if (c == ')' || c == ' ') break;
        if (index > -1) exec_name.push_back(c);
        if (c == '(') index = 0;
    }

    if (file_ptr)
        file_ptr.close();

    return exec_name;
}

std::string find_build_dir(std::string path)
{
    std::vector<std::string> build_names = {"build", "bin", "out"};
    std::string temp;
    if (path.empty())
        path = std::filesystem::current_path();

    for (const auto& it : std::filesystem::directory_iterator(path))
    {
        temp = it.path().string();
        for (auto name : build_names)
            if (temp.find(name) != std::string::npos)
                return temp;
    }

    return "\0";
}

bool exists_dir(std::string dir_name, std::string path)
{
    for (const auto& it : std::filesystem::directory_iterator(path))
        if (dir_name == it.path().filename().string())
            return true;
    return false;
}

int main(int argc, char *argv[]) {
    std::filesystem::path path(std::filesystem::current_path());
    std::string output_dir;

    if (argc > 1)
    {
        if (std::string(argv[1]).find("..") != std::string::npos)
            path = path.parent_path();
        else if (!exists_dir(std::string(argv[1]), path))
            output_dir = argv[1];
        else
        {
            std::filesystem::current_path(argv[1]);
            path = std::filesystem::current_path();
        }
    }

    std::filesystem::current_path(path);

    std::string executable_name = find_exec_name();
    std::string build_dir = find_build_dir(path.string());
    if (build_dir == "\0")
    {
        if (output_dir.empty())
            output_dir = "build";
        auto cmake_build_buffer = exec(std::format("cmake -S ./ -B ./{}", output_dir));
        build_dir = output_dir;

        if (cmake_build_buffer.second)
        {
            std::cout << "** CMake build failed **\n" << std::endl << cmake_build_buffer.first;
            exit(1);
        }
    }

    std::filesystem::current_path(build_dir);
    auto make_buffer = exec("make");
    auto exec_buffer = exec("./" + executable_name);

    std::cout << exec_buffer.first;
    return 0;
}