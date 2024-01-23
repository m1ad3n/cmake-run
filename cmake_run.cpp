#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <fstream>
#include <filesystem>
#include <vector>

std::string exec(std::string cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string find_exec_name() {
    std::fstream file_ptr("CMakeLists.txt");

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
        temp = it.path();
        for (auto name : build_names)
            if (temp.find(name) != std::string::npos)
                return temp;
    }

    return nullptr;
}

std::string get_path(std::string given_path)
{
    std::string path = std::filesystem::current_path();
    if (given_path == "..")
    {
        std::cout << "ss";
        for (int i = path.length() - 1; i != '/'; i--)
            path.pop_back();
    }
    return path;
}

int main(int argc, char *argv[]) {
    std::filesystem::path path(std::filesystem::current_path());
    std::string str_path;

    if (argc > 1)
    {
        if (std::string(argv[1]).find("..") != std::string::npos)
        {
            path = path.parent_path();
        }
    }

    std::filesystem::current_path(path);

    std::string executable_name = find_exec_name();
    std::string build_dir = find_build_dir(path.string());

    std::filesystem::current_path(build_dir);

    std::string make_buffer = exec("make");
    std::string exec_buffer = exec("./" + executable_name);

    std::cout << exec_buffer;
    return 0;
}