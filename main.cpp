#include "EventManager.h"
#include <iostream>
#include <chrono>
#include <format>

int main()
{
    try
    {

        EventManager manager("events.txt");

        int choice = 0;

        do
        {

            std::cout << "1. Add to events\n";
            std::cout << "2. Delete to events\n";
            std::cout << "3. Print to events\n";
            std::cout << "4. Save and exit\n";

            std::cin >> choice;
            std::cin.ignore();

            switch (choice)
            {
            case 1:
            {
                std::string dateStr, description;

                std::cout << "Print to date DD.MM.YYYY\n";
                std::getline(std::cin, dateStr);

                std::cout << "Print to description\n";
                std::getline(std::cin, description);
                try
                {

                    auto date = manager.parseDate(dateStr);
                    manager.addEvents(date, description);
                    std::cout << "Event to add successful!\n";
                    std::cout << std::endl;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 2:
            {
                std::cout << "print date for deleted\n";
                std::string delDate;
                std::cin >> delDate;
                std::cout << std::endl;
                manager.deleteEvent(delDate);
                std::cout << "Events to deleted\n";
                break;
            }
            case 3:
            {

                try
                {
                    auto today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
                    int HowLine;
                    std::cin >> HowLine;
                    auto upcoming = manager.getUpcomingEvents(today, HowLine);

                    if (upcoming.empty())
                    {
                        std::cout << "No upcoming events." << std::endl;
                    }
                    else
                    {
                        for (const auto &e : upcoming)
                        {
                            auto ymd = std::chrono::year_month_day(e.getDate());
                            std::cout << std::format("{:02}.{:02}.{:04} ",
                                                     unsigned(ymd.day()),
                                                     unsigned(ymd.month()),
                                                     int(ymd.year()))
                                      << e.getDescription() << "\n";
                        }
                    }
                }
                catch (const std::exception &ex)
                {
                    std::cerr << "Error: " << ex.what() << std::endl;
                    return 1;
                }
                std::cout << std::endl;
                break;
            }
            case 4:
            {

                manager.saveToFile();
                std::cout << "Data save to file!";
                std::cout << std::endl;
                break;
            }
            default:
            {
                std::cout << "Pleas try again";
                std::cout << std::endl;
            }
            }

        } while (choice != 4);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Critical Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}