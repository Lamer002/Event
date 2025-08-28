#pragma once

#include "Event.h"
#include <vector>
#include <string>

class EventManager
{
public:
    EventManager(const std::string &filename);

    std::vector<Event> getUpcomingEvents(std::chrono::sys_days date, size_t count = 7);
    std::chrono::sys_days parseDate(const std::string &dateStr);

    void addEvents(std::chrono::sys_days date, const std::string &description);
    void saveToFile();

private:
    std::vector<Event> events_;
    std::string filename_;

    void loadFromFile(const std::string &filename);
    std::string dateToString(std::chrono::sys_days date) const;
};
