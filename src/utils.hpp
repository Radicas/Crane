#ifndef REDEDA_UTILS_H
#define REDEDA_UTILS_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

namespace SUtil
{
    const int LINE_LENGTH = 1024;

    std::string execCmd(const char *cmd, const char *mode)
    {
        std::string text;
        FILE *f = popen(cmd, mode);
        char line[LINE_LENGTH];
        if (!f)
        {
            throw("failed to open pipe file");
        }
        while (fgets(line, LINE_LENGTH, f))
        {
            text.append(line);
        }
        pclose(f);
        f = nullptr;
        return std::move(text);
    }
} // namespace SUtil

#endif // REDEDA_UTILS_H