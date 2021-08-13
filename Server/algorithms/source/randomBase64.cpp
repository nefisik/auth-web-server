#include "algorithms/include/randomBase64.h"

std::string gen_random(const int len) {
    
    std::string tmp_s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    srand( (unsigned) time(NULL) * getpid());

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) 
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    
    
    return tmp_s;
    
}

// int main(int argc, char *argv[]) {
    
//     std::cout << gen_random(12) << std::endl;
    
//     return 0;
// }