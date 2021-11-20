
#ifdef WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include <iostream>
#include <string>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <vector>

static int ticketCount = 0, empCount = 0, faqCount = 0; // Defined global variables
int main();                                             // Prototyping main function

// Suppress terminal output
void SetStdinEcho(bool enable = true)
{
#ifdef WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if (!enable)
        mode &= ~ENABLE_ECHO_INPUT;
    else
        mode |= ENABLE_ECHO_INPUT;

    SetConsoleMode(hStdin, mode);
#else
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (!enable)
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

//Pause output
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

//Function to get cuurent time
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

//Check that input is numeric
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
    // Print created tickets
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

    // Create a ticket
    void raise_ticket(std::string staff_id)
    {

        ticketID = "TCKT" + std::to_string(ticketCount + 1);
        staffID = staff_id;
        std::cout << "\nDescription:\n";
        std::getline(std::cin >> std::ws, description);
        status = "Open";
        dateRaised = current_time();
    }

    // Address ticket and set staffID
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

class FAQ
{

public:
    std::string entryID, title, description;

public:
    // Print created entries
    void display_entry()
    {
        std::cout << "\nEntry ID: " << entryID << std::endl
                  << "Title: " << title << std::endl;
    }

    // View created entries
    void read_entry()
    {
        std::cout << "\nEntry ID: " << entryID << std::endl
                  << "Title: " << title << std::endl
                  << "Description: " << description << std::endl;
    }

    // Create entry
    void add_entry()
    {

        entryID = "FAQ" + std::to_string(faqCount + 1);
        std::cout << "\nTitle:\n";
        std::getline(std::cin >> std::ws, title);
        std::cout << "\nDescription:\n";
        std::getline(std::cin >> std::ws, description);
        faqCount++;
    }

    void init_faq()
    {
        entryID = "FAQ1";
        title = "Universal fix";
        description = "Power it off and on again.";
        faqCount++;
    }
};
FAQ faq[20];

// An abstract class with virtual methods
class Employee
{
protected:
    std::string employee_id;
    std::string type;
    std::string password;

public:
    static void landing_page(std::string id, int pos);
    static void login();
    virtual void view_ticket(std::string id) {}
    virtual void view_faq() {}
};

// Class inheriting from Employee
class Staff : public Employee
{

public:
    void init_staff() // Create dummy Staff. With password=password and ID=ST{1...5}
    {
        employee_id = "ST" + std::to_string(empCount + 1);
        password = "password";
    }

    void view_ticket(std::string id)
    {
    S_TICK:
        int found = 0, choice;
        if (ticketCount == 0) // Check if any tickets exist in the system
        {
            std::cout << "\nNo ticket raised\n";
        }
        else
        {
            // Check if current staff has any tickets and display them
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
    void view_faq()
    {

        int choice;
        bool found = false;
        std::string entryID;
        if (faqCount == 0) // Check if any entries exist in the system
        {
            std::cout << "\nNo entries\n";
        }
        else
        {
            // Check if there are any entries and display them
            for (size_t i = 0; i < 20; i++)
            {
                if (!faq[i].entryID.empty())
                {
                    faq[i].display_entry();
                }
            }
        }

        do
        {

            std::cout << "\n\nPlease select from following options: \n";
            std::cout << "\n1. Read Entry \n0. Previous menu";
            std::cout << "\n\nEnter the selected option: ";
            std::cin >> choice;

            if (!check_chocie())
                continue;

            switch (choice)
            {
            case 1:
                std::cout << "\nEnter ticket ID: ";
                std::cin >> entryID;

                for (size_t i = 0; i < 20; i++)
                {
                    if (faq[i].entryID == entryID) // Checkif entry ID is valid
                    {
                        faq[i].read_entry();
                        press_any_key();
                        clear_input_buffer();
                    }
                }
                if (!found)
                {
                    std::cout << "\nTicket not found. Try again\n";
                }
                continue;

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
};

Staff staff[5];

class ITStaff : public Employee
{

public:
    void init_itstaff() // Create Dummy IT Staff. With password=admin and ID=IT{1...5}
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
                // Check if tickets exist and display them
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
                    if (ticket[i].ticketID.compare(ticketID) == 0) // Checkif ticket ID is valid
                    {
                        if (ticket[i].status == "Open") // Check ticket status. Only open tickets addressed
                        {
                            ticket[i].address_ticket(id);
                            std::cout << "\nTicket addressed successfully\n";
                            press_any_key();
                            clear_input_buffer();
                            found = true;
                            goto I_TCKT;
                        }
                        else if (ticket[i].status == "Seen")
                        {
                            std::cout << "\nTicket already addressed\n";
                            press_any_key();
                            clear_input_buffer();
                        }
                        else
                        {
                            std::cout << "\nTicket already closed\n";
                            press_any_key();
                            clear_input_buffer();
                        }
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
                    if (ticket[i].ticketID.compare(ticketID) == 0) // Checkif ticket ID is valid
                    {
                        if (ticket[i].status.compare("Seen") == 0 &&
                            ticket[i].itstaffID.compare(id) == 0) // Check if staff attempting to close ticket is same as staff who raised it
                        {
                            ticket[i].close_ticket();
                            std::cout << "\nTicket closed successfully\n";
                            press_any_key();
                            clear_input_buffer();
                            goto I_TCKT;
                        }
                        else if (ticket[i].status.compare("Seen") != 0) // Check that only 'Seen' status tickets can be closed
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
    void view_faq()
    {
    V_FAQ:
        int choice;
        bool found;
        std::string entryID;
        if (faqCount == 0)
        {
            std::cout << "\nNo entry\n";
        }
        else
        {
            for (size_t i = 0; i < 20; i++)
            {
                // Check if entries exist and display them
                if (!faq[i].entryID.empty())
                {
                    faq[i].display_entry();
                }
            }
        }

        do
        {

            std::cout << "\n\nPlease select from following options: \n";
            std::cout << "\n1. View entry \n2. Add entry \n0. Previous menu";
            std::cout << "\n\nEnter the selected option: ";
            std::cin >> choice;

            if (!check_chocie())
                continue;

            switch (choice)
            {
            case 1:
                std::cout << "\nEnter entry ID: ";
                std::cin >> entryID;

                for (size_t i = 0; i < 20; i++)
                {
                    if (faq[i].entryID == entryID) // Checkif entry ID is valid
                    {
                        found = true;
                        faq[i].read_entry();
                        press_any_key();
                        clear_input_buffer();
                    }
                }
                if (!found)
                {
                    std::cout << "\nEntry not found. Try again\n";
                }
                continue;

            case 2:
                faq[faqCount].add_entry();
                goto V_FAQ;
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
            if (id.substr(0, 2).compare("IT") == 0) // Check if ID contains 'IT'
            {
                itstaff[pos].view_ticket(id);
            }
            else
            {
                staff[pos].view_ticket(id);
            }

            break;

        case 2:
            if (id.substr(0, 2).compare("IT") == 0) // Check if ID contains 'IT'
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
        int counter = 3;
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
            SetStdinEcho(false);
            std::cin >> password;
            SetStdinEcho(true);

            for (size_t i = 0; i < 5; i++)
            {
                // Check for corresponding ID and passwords. If found, login.
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

            if (counter == 0)
            {
                std::cout << "\nAttempts exhausted! Redirecting...";
                press_any_key();
                clear_input_buffer();
                login();
            }

            // If no matching credentials
            if (!found)
            {
                std::cout << "\nPlease enter valid username or password!!!!\n";
                std::cout << '\n'
                          << counter << " Attempt(s) remaining";
                counter--;
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

    //Initialise dummy employees
    for (size_t i = 0; i < 5; i++)
    {
        staff[i].init_staff();
        itstaff[i].init_itstaff();
        empCount++;
    }

    //Add dummy entry
    faq[0].init_faq();

    // Call to login page
    Employee::login();
}
