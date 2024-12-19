#include "InstallerFirst.h"
#include "utils.h"
#include "Option.h"
#include <iostream>
#include <vector>

int main() {
    std::vector<OptionMenu> options;
    options.push_back(OptionMenu("Test1", 0));
    options.push_back(OptionMenu("Test2", 1));
    options.push_back(OptionMenu("Test3", 2));
    selectMenu(options, "Test", "Test desc");
    InstallerFirst installer;
    installer.install();
    return 0;
}