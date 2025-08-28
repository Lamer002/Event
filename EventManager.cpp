#include "EventManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

EventManager::EventManager(const std::string &filename) : filename_(filename)
{
    loadFromFile(filename);
}

void EventManager::addEvents(std::chrono::sys_days date, const std::string &description)
{
    events_.emplace_back(date, description);
    std::sort(events_.begin(), events_.end());
    saveToFile();
    std::cout << "Events to save successful!\n";
}

int EventManager::findIndexDate(const std::string& dateStr)
{
    try{
        int findDate = 0;
        auto targetDate = parseDate(dateStr);

        for (size_t i = 0; i < events_.size(); i++)
        {
            if(events_[i].getDate() == targetDate)
            {
                findDate = static_cast<int>(i);
                return static_cast<int>(i);
            }
        }
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "Invalid date format: " << e.what() << std::endl;
        return -1;
    }
}

void EventManager::deleteEvent(const std::string& dateStr)
{
    std::vector<int> RecurrDate;
    int count = 0;
    int targetEvent = findIndexDate(dateStr);
    for (size_t i = 0; i < events_.size(); i++)
    {
        if (dateToString(events_[i].getDate()) == dateStr)
        {
            RecurrDate.push_back(i);
        }
    }

    if (RecurrDate.size() > 1)
    {
        std::cout << "What event you want deleted?\n";
        for (size_t i = 0; i < RecurrDate.size();)
        {
            size_t eventIndex = RecurrDate[i];
            std::cout << ++i << ": " << events_[eventIndex].getDate() << " " << events_[eventIndex].getDescription() << std::endl;
            count++;
        }

        int del;
        std::cin >> del;
        std::cin.ignore();
        if (del > count || del < 0)
        {
            std::cerr << "Invalid number, please try again";
            
        }
        else
        {
            events_.erase(events_.begin() + RecurrDate[del - 1]);
            std::sort(events_.begin(), events_.end());
            saveToFile();
            std::cout << "Events deleted";
        }
    }
    else
    {
        events_.erase(events_.begin() + targetEvent);
            std::sort(events_.begin(), events_.end());
            saveToFile();
            std::cout << "Events deleted";
    }

    
    
}

void EventManager::saveToFile()
{
    std::ofstream file(filename_);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file for writing: " + filename_);
    }

    for (const auto &e : events_)
    {
        file << dateToString(e.getDate()) << " " << e.getDescription() << "\n";
    }
}

void EventManager::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;
    size_t lineNumber = 0;

    while (std::getline(file, line))
    {
        ++lineNumber;

        if (line.size() < 11)
        {
            throw std::runtime_error(
                "Invalid line (too short) at line " + std::to_string(lineNumber) +
                ": " + line);
        }

        std::string dateStr = line.substr(0, 10);  // "DD.MM.YYYY"
        std::string description = line.substr(11); // текст после даты и пробела

        std::chrono::sys_days date = parseDate(dateStr);
        events_.push_back(Event(date, description));
    }
}

std::chrono::sys_days EventManager::parseDate(const std::string &dateStr)
{
    int d, m, y;
    char dot1, dot2;
    std::istringstream ss(dateStr);
    ss >> d >> dot1 >> m >> dot2 >> y;
    if (!ss || dot1 != '.' || dot2 != '.')
    {
        throw std::runtime_error("Invalid date format: " + dateStr);
    }

    std::chrono::year_month_day ymd{std::chrono::year(y) / std::chrono::month(m) / std::chrono::day(d)};
    if (!ymd.ok())
    {
        throw std::runtime_error("Invalid date: " + dateStr);
    }
    return std::chrono::sys_days{ymd};
}

std::vector<Event> EventManager::getUpcomingEvents(std::chrono::sys_days date, size_t count)
{
    std::vector<Event> upcomingEvents;
    for (const Event &e : events_)
    {
        if (e.getDate() >= date)
        {
            upcomingEvents.push_back(e);
        }
    }
    std::sort(upcomingEvents.begin(), upcomingEvents.end());
    if (upcomingEvents.size() > count)
    {
        upcomingEvents.erase(upcomingEvents.begin() + count, upcomingEvents.end());
    }

    return upcomingEvents;
}

std::string EventManager::dateToString(std::chrono::sys_days date) const
{
    auto ymd = std::chrono::year_month_day(date);
    std::ostringstream ss;

    ss << std::setw(2) << std::setfill('0') << static_cast<unsigned>(ymd.day()) << "."
       << std::setw(2) << std::setfill('0') << static_cast<unsigned>(ymd.month()) << "."
       << static_cast<int>(ymd.year());

    return ss.str();
}
