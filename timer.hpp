#pragma once
#include "template.hpp"
#if defined(LX_LOCAL) && !defined(CPH)
namespace Timer {
    struct Timer {
        clock_t begin;
        Timer(): begin(clock()) {}
        ~Timer() {
            double t = (clock() - begin) * 1000. / CLOCKS_PER_SEC;
            fprintf(stderr, "\033[1;32m");
            if (t >= 60000)
                fprintf(stderr, "%.2lfmin\n", t / 60000.);
            else if (t >= 1000)
                fprintf(stderr, "%.2lfs\n", t / 1000.);
            else
                fprintf(stderr, "%.0lfms\n", t);
            fprintf(stderr, "\033[0m");
        }
    } _;
} // namespace
#endif