#include <iostream>
#include <chrono>
#include <omp.h>

#include "joiner.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    Joiner joiner;

    // Read join relations
    std::string line;
    while (getline(std::cin, line)) {
        if (line == "Done") break;
        joiner.addRelation(line.c_str());
    }

    // Preparation phase (not timed)
    // Build histograms, indexes,...

    QueryInfo i;
    while (getline(std::cin, line)) {
        if (line == "F") continue; // End of a batch
        auto start = std::chrono::high_resolution_clock::now();
        i.parseQuery(line);
        std::cout << joiner.join(i);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        // 输出结果
        std::cout << "代码执行时间: " << duration.count() << " 毫秒" << std::endl;
    }

    return 0;
}
