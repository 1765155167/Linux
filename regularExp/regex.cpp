#include <iostream>
#include <sys/types.h>
#include <regex.h>
int main(int argc, char* argv[])
{
    if(argc != 3) {
        std::cout << "Please input regex and text" << std::endl;
        return 0;
    }
    regex_t preg;
    char*  p_regex_str = argv[1];
    char* p_text = argv[2];
    int ret = regcomp(&preg, p_regex_str, REG_EXTENDED|REG_NOSUB);
    if(ret == 0) {
        ret = regexec(&preg, p_text, 0, NULL, 0);
        if(ret == 0) {
            std::cout << "匹配成功" << std::endl;
            regfree(&preg);
            return 0;
        }
    }
    char emsg[1024];
    size_t emsg_len = 0;
    emsg_len = regerror(ret, &preg, emsg, sizeof(emsg));
    if(emsg_len < sizeof(emsg)) {
        emsg[emsg_len] = '\0';
    }else{
        emsg[1023] = '\0';
    }
    std::cout << "匹配失败，原因：" << emsg << std::endl;
    regfree(&preg);
    return 0;
}

