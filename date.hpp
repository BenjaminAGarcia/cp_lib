#pragma once
#include "template.hpp"

struct Date {
    static constexpr int month_days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int year, month, day;
    Date(int y, int m, int d): year(y), month(m), day(d) {}
    Date(int x) {
        year = x * 400 / 146097 + 1;
        day = x - Date(year, 1, 1).to_int();
        month = 1;
        while (day >= 28) {
            int k = month_days[month] + (month == 2 && is_leap_year(year) ? 1 : 0);
            if (day < k)
                break;
            month++;
            day -= k;
        }
        if (month == 13) {
            year++;
            month = 1;
        }
        day++;
    }
    static bool is_leap_year(int y) { return (y % 4 == 0 && y % 100) || y % 400 == 0; }
    Date& operator++() {
        int lim = month_days[month];
        if (is_leap_year(year) && month == 2)
            lim = 29;
        if (++day > lim) {
            day = 1;
            if (++month == 13) {
                year++;
                month = 1;
            }
        }
        return *this;
    }
    Date operator++(int) {
        Date tmp = *this;
        ++*this;
        return tmp;
    }
    auto operator<=>(const Date& rhs) const = default;
    [[nodiscard]] str to_string(const str& sep = "-") const {
        str y = std::to_string(year);
        str m = std::to_string(month);
        str d = std::to_string(day);
        while (len(y) < 4)
            y.insert(y.begin(), '0');
        while (len(m) < 2)
            m.insert(m.begin(), '0');
        while (len(d) < 2)
            d.insert(d.begin(), '0');
        return y + sep + m + sep + d;
    }
    // {1, 1, 1} = 0
    [[nodiscard]] int to_int() const {
        int y = (month <= 2 ? year - 1 : year);
        int m = (month <= 2 ? month + 12 : month);
        int d = day;
        return 365 * y + y / 4 - y / 100 + y / 400 + 306 * (m + 1) / 10 + d - 429;
    }
    [[nodiscard]] int weekday() const { return (to_int() + 1) % 7; }
};