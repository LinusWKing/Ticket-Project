#include <iostream>
#include <string>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <vector>

static int ticketCount = 0, empCount = 0;
int main();

void press_any_key()
{
    int c;
    printf("\nPress any key to continue... \n");
    c = getch();
    if (c == 0 || c == 224)
        getch();
}

void clear_input_buffer()
{
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

std::string current_time()
{
    time_t rawtime;
    char now[30];
    tm *curr_tm;
    time(&rawtime);
    curr_tm = localtime(&rawtime);
    strftime(now, 50, "%x %X", curr_tm);

    return now;
}

bool check_chocie()
{
    if (std::cin.fail())
    {
        std::cout << "\nPlease Input Valid Numeric Choice Only" << std::endl;
        press_any_key();
        clear_input_buffer();
        return false;
    }
    else
    {
        return true;
    }
}

class Ticket
{

public:
    std::string ticketID, description, staffID, itstaffID, status, dateRaised, dateClosed;

public:
    void display_ticket()
    {
        std::cout << "\nTicket ID: " << ticketID << std::endl
                  << "Description: " << description << std::endl
                  << "Raised By: " << staffID << std::endl
                  << "Handled By: " << itstaffID << std::endl
                  << "Status: " << status << std::endl
                  << "Date Raised: " << dateRaised << std::endl
                  << "Date Closed: " << dateClosed << std::endl;
    }
    void raise_ticket(std::string staff_id)
    {

        ticketID = "TCKT" + std::to_string(ticketCount + 1);
        staffID = staff_id;
        std::cout << "\nDescription:\n";
        std::getline(std::cin >> std::ws, description);
        status = "Open";
        dateRaised = current_time();
    }

    void address_ticket(std::string itstaff_id)
    {
        itstaffID = itstaff_id;
        status = "Seen";
    }

    void close_ticket()
    {
        status = "Closed";
        dateClosed = current_time();
    }
};
Ticket ticket[20];

class Employee
{
protected:
    std::string employee_id;
    std::string type;
    std::string password;

public:
    static void landing_page(std::string id, int pos);
    static void login();
    // virtual void view_ticket();
    // virtual void view_faq();
};

class Staff : public Employee
{

private:
    void raise_ticket();

public:
    void init_staff()
    {
        employee_id = "ST" + std::to_string(empCount + 1);
        password = "password";
    }

    void view_ticket(std::string id)
    {
    S_TICK:
        int found = 0, choice;
        if (ticketCount == 0)
        {
            std::cout << "\nNo ticket raised\n";
        }
        else
        {
            for (size_t i = 0; i < 20; i++)
            {
                if (ticket[i].staffID.compare(id) == 0)
                {
                    found++;
                    ticket[i].display_ticket();
                    press_any_key();
                }
            }
            if (found == 0)
            {
                std::cout << "\nNo ticket raised\n";
            }
        }

        do
        {

            std::cout << "\n\nPlease select from following options: \n";
            std::cout << "\n1. Raise Ticket \n0. Previous menu";
            std::cout << "\n\nEnter the selected option: ";
            std::cin >> choice;

            if (!check_chocie())
                continue;

            switch (choice)
            {
            case 1:
                ticket[ticketCount].raise_ticket(id);
                ticketCount++;
                goto S_TICK;
            case 0:
                goto END;

            default:
                std::cout
                    << "\nInvalid Choice. Try Again \n";
                press_any_key();
                continue;
            }
        END:
            break;

        } while (true);
    }
    void view_faq() {}
};

Staff staff[5];

class ITStaff : public Employee
{

private:
    void update_faq();

public:
    void init_itstaff()
    {
        employee_id = "IT" + std::to_string(empCount + 1);
        password = "admin";
    }

    void view_ticket(std::string id)
    {
    I_TCKT:
        int choice;
        bool found;
        std::string ticketID;
        if (ticketCount == 0)
        {
            std::cout << "\nNo ticket raised\n";
        }
        else
        {
            for (size_t i = 0; i < 20; i++)
            {
                if (!ticket[i].ticketID.empty())
                {
                    ticket[i].display_ticket();
                }
            }
        }

        do
        {

            std::cout << "\n\nPlease select from following options: \n";
            std::cout << "\n1. Address Ticket \n2. Close ticket \n0. Previous menu";
            std::cout << "\n\nEnter the selected option: ";
            std::cin >> choice;

            if (!check_chocie())
                continue;

            switch (choice)
            {
            case 1:

                std::cout << "\nEnter ticket ID: ";
                std::cin >> ticketID;

                for (size_t i = 0; i < 20; i++)
                {
                    if (ticket[i].ticketID.compare(ticketID) == 0)
                    {
                        ticket[i].address_ticket(id);
                        std::cout << "\nTicket addressed successfully\n";
                        press_any_key();
                        clear_input_buffer();
                        found = true;
                        goto I_TCKT;
                    }
                }
                if (!found)
                {
                    std::cout << "\nTicket not found. Try again\n";
                }

                continue;

            case 2:

                std::cout << "\nEnter ticket ID: ";
                std::cin >> ticketID;

                for (size_t i = 0; i < 20; i++)
                {
                    if (ticket[i].ticketID.compare(ticketID) == 0)
                    {
                        if (ticket[i].status.compare("Seen") == 0 &&
                            ticket[i].itstaffID.compare(id) == 0)
                        {
                            ticket[i].close_ticket();
                            std::cout << "\nTicket closed successfully\n";
                            press_any_key();
                            clear_input_buffer();
                            goto I_TCKT;
                        }
                        else if (ticket[i].status.compare("Seen") != 0)
                        {
                            std::cout << "\nOpen tickets or already closed ticket cannot be closed.\n";
                            press_any_key();
                            clear_input_buffer();
                        }
                        else
                        {

                            std::cout << "\nCannot close other staff tickets\n";
                            press_any_key();
                            clear_input_buffer();
                        }

                        ticket[i].address_ticket(id);
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    std::cout << "\nTicket not found. Try again\n";
                    press_any_key();
                    clear_input_buffer();
                }

                continue;
            case 0:
                goto END;

            default:
                std::cout
                    << "\nInvalid Choice. Try Again \n";
                press_any_key();
                clear_input_buffer();
                continue;
            }
        END:
            break;
        } while (true);
    }
    void view_faq() {}
};
ITStaff itstaff[5];

void Employee ::landing_page(std::string id, int pos)
{
    do
    {
        int choice;

        std::cout << "\n\nPlease select from following options: \n";
        std::cout << "\n1. View Ticket \n2. View FAQ \n3. Logout";
        std::cout << "\n\nEnter the selected option: ";
        std::cin >> choice;

        if (!check_chocie())
            continue;

        switch (choice)
        {
        case 1:
            if (id.substr(0, 2).compare("IT") == 0)
            {
                itstaff[pos].view_ticket(id);
            }
            else
            {
                staff[pos].view_ticket(id);
            }

            break;

        case 2:
            if (id.substr(0, 2).compare("IT") == 0)
            {
                itstaff[pos].view_faq();
            }
            else
            {
                staff[pos].view_faq();
            }
            break;

        case 3:
            login();

        default:
            std::cout
                << "\nInvalid Choice. Try Again \n";
            press_any_key();
            continue;
        }

    } while (true);
}

void Employee ::login()
{
    int currentPosition, choice;
    std::string loginID, password;
    bool found;
    do
    {
        std::cout << "\nPlease select an option\n\n";
        std::cout << "1. Login \n0. Exit";
        std::cout << "\n\nEnter Selected Option: ";
        std::cin >> choice;

        if (!check_chocie())
            continue;

        switch (choice)
        {
        LOGIN:
        case 1:
            std::cout << "\nLogin ID: ";
            std::cin >> loginID;
            std::cout << "\nPassword: ";
            std::cin >> password;

            for (size_t i = 0; i < 5; i++)
            {
                if (staff[i].employee_id == loginID && staff[i].password == password ||
                    itstaff[i].employee_id == loginID && itstaff[i].password == password)
                {
                    currentPosition = i;
                    found = true;
                    std::cout << "\nLogin successful\n";
                    press_any_key();
                    clear_input_buffer();
                    break;
                }
            }

            if (!found)
            {
                std::cout << "\nPlease enter valid username or password!!!!\n";
                press_any_key();
                clear_input_buffer();
                goto LOGIN;
            }

            landing_page(loginID, currentPosition);
            break;

        case 0:
            exit(0);

        default:
            std::cout
                << "\nInvalid Choice. Try Again \n";
            press_any_key();
            continue;
        }
    } while (true);
}

int main()
{

    for (size_t i = 0; i < 5; i++)
    {
        staff[i].init_staff();
        itstaff[i].init_itstaff();
        empCount++;
    }
    Employee::login();
}
