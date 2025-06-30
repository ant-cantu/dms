#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//Prototype
double matrixMarkUp(double cost);
bool inventoryCheck(const string& searchNumber);
void load_config();
void addPart(string& partNumber, string& bin, string& descp, double& cost);
void updateConfig(char type, int newInvoiceNumber);
bool fileExists(const string& filename);
double warrantyMarkUp(int type, double cost);

//Global Variables
string config = "config/config.txt";
string filename;
string filename_path;
string billed_path;
string quoted_path;
int invoice_number;
int quote_number;


string number, binLocation, description, priceStr;
double tax_rate = 0.08375;

int main()
{
    load_config();

    double rev[100];

    string partNumber[100];
    int qty[100];

    string descp[100];
    string bin[100];

    int count = 0;
    string RO;

    double cost = 0.00;
    double total = 0.00;

    ofstream outFile;

    string mode;

    int emp;
    string empName;
    bool validEmp = false;

    char answer;

    string priceType;
    int warrantyType;

    do
    {
        cout << "Welcome parts personel." << endl;
        cout << "Please enter your employee number: ";
        cin >> emp;

        switch (emp)
        {
        case 774:
            empName = "Anthony";
            validEmp = true;
            break;
        case 616:
            empName = "Brandon";
            validEmp = true;
            break;
        case 932:
            empName = "Greg";
            validEmp = true;
            break;
        case 1074:
            empName = "Kevin";
            validEmp = true;
            break;
        case 1058:
            empName = "Jimmy";
            validEmp = true;
            break;
        case 906:
            validEmp = true;
            empName = "Eric";
            break;
        default:
            system("CLS");
            cout << "Invalid employee number, try again." << endl;
            validEmp = false;
            break;
        }

        cout << endl;

        if (validEmp)
        {
            system("CLS");
            cout << "Welcome " << empName << "." << endl;
            system("PAUSE");
            system("CLS");
        }
    } while (!validEmp);

    do
    {
        cout << "MM    MM  BBBB   FFFFFF  FFFFFF          DDDD   M   M  SSSS" << endl;
        cout << "M M  M M  B   B  F       F               D   D  MM MM  S   " << endl;
        cout << "M  MM  M  BBBB   FFFF    FFFF     ****   D   D  M M M  SSSS" << endl;
        cout << "M      M  B   B  F       F               D   D  M   M     S" << endl;
        cout << "M      M  BBBB   F       F               DDDD   M   M  SSSS" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << "Enter \"done\" as the part number when you are done with the mode you are using." << endl;
        cout << "------------------------------------------------------------" << endl;

        cout << "(Q)uote | (R)epair Order | (I)nvoice | (S)tock Check | (E)xit: ";
        cin >> mode;

        cout << endl;

        if (mode == "E" || mode == "e")
            return 0;
        else if (mode == "R" || mode == "r") //Repair Order
        {
            cout << "Enter the repair order number: ";
            cin >> RO;

            //if (fileExists(billed_path + "\\RO" + RO + ".txt"))
            //{
            //    cout << "Repair order already exists." << endl
            //        << "Enter repair order again but include an additonal character." << endl
            //        << "Example: 1234A or 1234B" << endl;
            //}

            outFile.open(billed_path + "\\RO" + RO + ".txt");

            cout << endl;

            cout << "Enter price type (W)arranty or (M)atrix: ";
            cin >> priceType;

            if (priceType == "W" || priceType == "w")
            {
                cout << "What type of warranty is this?" << endl
                    << "1. Maintenance" << endl
                    << "2. Repair" << endl
                    << "Enter type (1) or (2): ";
                cin >> warrantyType;
            }

            cout << endl;

            do
            {
                cout << "Enter part number: ";
                cin >> partNumber[count];

                if (partNumber[count] == "done" || partNumber[count] == "DONE")
                    break;
                
                if (inventoryCheck(partNumber[count]))
                {
                    descp[count] = description;
                    bin[count] = binLocation;
                    if (priceType == "M" || priceType == "m")
                        rev[count] = matrixMarkUp(stod(priceStr));
                    else if (priceType == "W" || priceType == "w")
                        rev[count] = warrantyMarkUp(warrantyType, stod(priceStr));

                    cout << descp[count] << " " << bin[count] << " " << rev[count] << endl;

                    cout << "Enter the quantity: ";
                    cin >> qty[count];
                }
                else
                {
                    //Manual Entry
                    cin.ignore();

                    cout << "Enter Description: ";
                    getline(cin, descp[count]);

                    cout << "Enter Bin Location: ";
                    cin >> bin[count];

                    cout << "Enter the quantity: ";
                    cin >> qty[count];

                    cout << "Enter the cost: ";
                    cin >> cost;

                    //Add part to database
                    cout << "Add part to database? (y/n): ";
                    cin >> answer;

                    if (answer == 'y' || answer == 'Y')
                        addPart(partNumber[count], bin[count], descp[count], cost);

                    if (priceType == "M" || priceType == "m")
                        rev[count] = matrixMarkUp(cost);
                    else if (priceType == "W" || priceType == "w")
                        rev[count] = warrantyMarkUp(warrantyType, cost);
                }

                cout << endl;

                count++;

            } while (partNumber[count] != "done" || partNumber[count] != "DONE");

            cout << endl << "------------------------------------------------------------" << endl;

            cout << left << setw(25) << "Part Number"
                << setw(25) << "Description"
                << setw(25) << "Bin Location"
                << setw(25) << "Quantity"
                << setw(25) << "Cost" << endl;

            cout << setfill(' ');

            // Data rows
            double total = 0.0;
            for (int i = 0; i < count; i++)
            {
                cout << left << setw(25) << partNumber[i]
                    << left << setw(25) << descp[i]
                    << left << setw(25) << bin[i]
                    << left << setw(25) << qty[i]
                    << left << setw(25) << fixed << setprecision(2) << (rev[i] * qty[i]) << endl;

                total += (rev[i] * qty[i]);
            }

            if (outFile.is_open())
            {
                outFile << "Parts Employee: " << emp << endl;
                outFile << "Repair Order: " << RO << endl << endl;

                outFile << left << setw(25) << "Part Number"
                    << setw(25) << "Description"
                    << setw(25) << "Bin Location"
                    << setw(25) << "Quantity"
                    << setw(25) << "Cost" << endl;

                outFile << setfill(' ');
                for (int i = 0; i < count; i++)
                {
                    outFile << left << setw(25) << partNumber[i]
                        << left << setw(25) << descp[i]
                        << left << setw(25) << bin[i]
                        << left << setw(25) << qty[i]
                        << left << setw(25) << fixed << setprecision(2) << (rev[i] * qty[i]) << endl;
                }

                outFile << endl;

                outFile << "Subtotal: $" << total << endl;
                outFile << "Tax @ " << setprecision(3) << (tax_rate * 100) << "%: $" << setprecision(2) << (total * tax_rate) << endl;
                outFile << "Total Price With Tax: $" << total * (1 + tax_rate) << endl << endl;

                outFile.close();
            }
            else
            {
                cout << "Error Encountered: Unable to create .txt file" << endl;
            }

            cout << endl;

            cout << "Subtotal: $" << total << endl;
            cout << "Tax @ " << setprecision(3) << (tax_rate * 100) << "%: $" << setprecision(2) << (total * tax_rate) << endl;
            cout << "Total Price With Tax: $" << total * (1 + tax_rate) << endl << endl;

            system("PAUSE");
            system("CLS");

            //Reset variables
            partNumber[count] = "";

            for (int x = 0; x < count; x++)
            {
                partNumber[x] = "";
                descp[x] = "";
                bin[x] = "";
                qty[x] = 0;
                rev[x] = 0.00;
            }
            cost = 0.00;
            total = 0.00;
            count = 0;
            RO = "";
        }
        else if (mode == "S" || mode == "s") //Stock Check Only
        {
            do
            {
                cout << "Enter the part number: ";
                cin >> partNumber[count];

                if (partNumber[count] == "done" || partNumber[count] == "DONE")
                    break;

                if (inventoryCheck(partNumber[count]))
                {
                    cout << "Description: " << description << endl;
                    cout << "Bin Location: " << binLocation << endl;
                    cout << "Dealer Cost: " << priceStr << endl;
                    cout << "Matrix: " << matrixMarkUp(stod(priceStr)) << endl;
                }
                else
                {
                    //Add part to database
                    cout << "Add part to database? (y/n): ";
                    cin >> answer;

                    if (answer == 'y' || answer == 'Y')
                    {
                        cin.ignore();

                        cout << "Enter Description: ";
                        getline(cin, descp[count]);

                        cout << "Enter Bin Location: ";
                        cin >> bin[count];

                        cout << "Enter the cost: ";
                        cin >> cost;

                        addPart(partNumber[count], bin[count], descp[count], cost);
                    }
                }

                cout << endl;

                count++;

            } while (partNumber[count] != "done" || partNumber[count] != "DONE");

            cout << endl;

            system("PAUSE");
            system("CLS");

            //Reset Variables
            partNumber[count] = "";

            for (int i = 0; i < count; i++)
                partNumber[i] = "";

            count = 0;
        }
        else if (mode == "i" || mode == "I") //Create Invoice
        {
            cout << "Invoice: " << invoice_number << endl;

            outFile.open(billed_path + "\\INV" + to_string(invoice_number) + ".txt");

            //Update Invoice Number
            updateConfig('i', invoice_number + 1);
            load_config(); //Reload Config

            cout << endl;

            do
            {
                cout << "Enter part number: ";
                cin >> partNumber[count];

                if (partNumber[count] == "done" || partNumber[count] == "DONE")
                    break;

                if (inventoryCheck(partNumber[count]))
                {
                    descp[count] = description;
                    bin[count] = binLocation;
                    rev[count] = matrixMarkUp(stod(priceStr));

                    cout << descp[count] << " " << bin[count] << " " << rev[count] << endl;

                    cout << "Enter the quantity: ";
                    cin >> qty[count];
                }
                else
                {
                    //Manual Entry
                    cin.ignore();

                    cout << "Enter Description: ";
                    getline(cin, descp[count]);

                    cout << "Enter Bin Location: ";
                    cin >> bin[count];

                    cout << "Enter the quantity: ";
                    cin >> qty[count];

                    cout << "Enter the cost: ";
                    cin >> cost;

                    //Add part to database
                    cout << "Add part to database? (y/n): ";
                    cin >> answer;

                    if (answer == 'y' || answer == 'Y')
                        addPart(partNumber[count], bin[count], descp[count], cost);

                    rev[count] = matrixMarkUp(cost);
                }

                cout << endl;

                count++;

            } while (partNumber[count] != "done" || partNumber[count] != "DONE");

            cout << endl << "------------------------------------------------------------" << endl;

            cout << left << setw(25) << "Part Number"
                << setw(25) << "Description"
                << setw(25) << "Bin Location"
                << setw(25) << "Quantity"
                << setw(25) << "Cost" << endl;

            cout << setfill(' ');

            // Data rows
            double total = 0.0;
            for (int i = 0; i < count; i++)
            {
                cout << left << setw(25) << partNumber[i]
                    << left << setw(25) << descp[i]
                    << left << setw(25) << bin[i]
                    << left << setw(25) << qty[i]
                    << left << setw(25) << fixed << setprecision(2) << (rev[i] * qty[i]) << endl;

                total += (rev[i] * qty[i]);
            }

            if (outFile.is_open())
            {
                outFile << "Parts Employee: " << emp << endl;
                outFile << "Invoice Number: " << invoice_number - 1 << endl << endl;

                outFile << left << setw(25) << "Part Number"
                    << setw(25) << "Description"
                    << setw(25) << "Bin Location"
                    << setw(25) << "Quantity"
                    << setw(25) << "Cost" << endl;

                outFile << setfill(' ');
                for (int i = 0; i < count; i++)
                {
                    outFile << left << setw(25) << partNumber[i]
                        << left << setw(25) << descp[i]
                        << left << setw(25) << bin[i]
                        << left << setw(25) << qty[i]
                        << left << setw(25) << fixed << setprecision(2) << (rev[i] * qty[i]) << endl;
                }

                outFile << endl;

                outFile << "Subtotal: $" << total << endl;
                outFile << "Tax @ " << setprecision(3) << (tax_rate * 100) << "%: $" << setprecision(2) << (total * tax_rate) << endl;
                outFile << "Total Price With Tax: $" << total * (1 + tax_rate) << endl << endl;

                outFile.close();
            }
            else
            {
                cout << "Error Encountered: Unable to create .txt file" << endl;
            }

            cout << endl;

            cout << "Subtotal: $" << total << endl;
            cout << "Tax @ " << setprecision(3) << (tax_rate * 100) << "%: $" << setprecision(2) << (total * tax_rate) << endl;
            cout << "Total Price With Tax: $" << total * (1 + tax_rate) << endl << endl;

            system("PAUSE");
            system("CLS");

            //Reset variables
            partNumber[count] = "";

            for (int x = 0; x < count; x++)
            {
                partNumber[x] = "";
                descp[x] = "";
                bin[x] = "";
                qty[x] = 0;
                rev[x] = 0.00;
            }
            cost = 0.00;
            total = 0.00;
            count = 0;
        }
        else if (mode == "Q" || mode == "q") //Create Quote
        {
            string q_num;
            cout << "Enter quote number or \"new\" for a new quote: ";
            cin >> q_num;

            if (fileExists(quoted_path + "\\" + q_num + ".txt"))
            {
                ifstream prev_quote(quoted_path + "\\" + q_num + ".txt");
                string line;

                if (!prev_quote.is_open())
                {
                    cout << "Error: Unable to open previous quote." << endl;
                    system("PAUSE");
                    return 1;
                }

                system("CLS");

                while (getline(prev_quote, line))
                {
                    cout << line << endl;
                }
                prev_quote.close();

                cout << endl;
                cout << "------------------------------------------------------" << endl;
                cout << "Do you want to roll this over to billed? (y/n): ";
                char roll_answer;
                cin >> roll_answer;

                if (roll_answer == 'n' || roll_answer == 'N')
                {
                    cout << endl;
                    system("PAUSE");
                    system("CLS");
                }
                else if (roll_answer == 'y' || roll_answer == 'Y')
                {
                    cout << "Enter repair order number: ";
                    cin >> RO;

                    ofstream ro_roll(billed_path + "\\" + RO + ".txt");
                    if (!ro_roll.is_open())
                    {
                        cout << "Error: Unable to create new repair order file." << endl;
                        system("PAUSE");
                        return 1;
                    }

                    ifstream quoted_file(quoted_path + "\\" + q_num + ".txt");
                    if (!quoted_file.is_open())
                    {
                        cout << "Error: Unable to open prevois quote." << endl;
                        ro_roll.close();
                        system("PAUSE");
                        return 1;
                    }

                    string new_ro_line;
                    while (getline(quoted_file, new_ro_line))
                    {
                        ro_roll << new_ro_line << endl;
                    }

                    prev_quote.close();
                    ro_roll.close();

                    cout << endl;
                    system("PAUSE");
                    system("CLS");
                }
            }
            else
            {
                cout << "Quote: " << quote_number << endl;

                outFile.open(quoted_path + "\\Q" + to_string(quote_number) + ".txt");

                updateConfig('q', quote_number + 1);
                load_config();

                cout << endl;

                do
                {
                    cout << "Enter part number: ";
                    cin >> partNumber[count];

                    if (partNumber[count] == "done" || partNumber[count] == "DONE")
                        break;

                    if (inventoryCheck(partNumber[count]))
                    {
                        descp[count] = description;
                        bin[count] = binLocation;
                        rev[count] = matrixMarkUp(stod(priceStr));

                        cout << descp[count] << " " << bin[count] << " " << rev[count] << endl;

                        cout << "Enter the quantity: ";
                        cin >> qty[count];
                    }
                    else
                    {
                        //Manual Entry
                        cin.ignore();

                        cout << "Enter Description: ";
                        getline(cin, descp[count]);

                        cout << "Enter Bin Location: ";
                        cin >> bin[count];

                        cout << "Enter the quantity: ";
                        cin >> qty[count];

                        cout << "Enter the cost: ";
                        cin >> cost;

                        //Add part to database
                        cout << "Add part to database? (y/n): ";
                        cin >> answer;

                        if (answer == 'y' || answer == 'Y')
                            addPart(partNumber[count], bin[count], descp[count], cost);

                        rev[count] = matrixMarkUp(cost);
                    }

                    cout << endl;

                    count++;

                } while (partNumber[count] != "done" || partNumber[count] != "DONE");

                cout << endl << "------------------------------------------------------------" << endl;

                cout << left << setw(25) << "Part Number"
                    << setw(25) << "Description"
                    << setw(25) << "Bin Location"
                    << setw(25) << "Quantity"
                    << setw(25) << "Cost" << endl;

                cout << setfill(' ');

                // Data rows
                double total = 0.0;
                for (int i = 0; i < count; i++)
                {
                    cout << left << setw(25) << partNumber[i]
                        << left << setw(25) << descp[i]
                        << left << setw(25) << bin[i]
                        << left << setw(25) << qty[i]
                        << left << setw(25) << fixed << setprecision(2) << (rev[i] * qty[i]) << endl;

                    total += (rev[i] * qty[i]);
                }

                if (outFile.is_open())
                {
                    outFile << "Parts Employee: " << emp << endl;
                    outFile << "Quote Number: " << quote_number - 1 << endl << endl;

                    outFile << left << setw(25) << "Part Number"
                        << setw(25) << "Description"
                        << setw(25) << "Bin Location"
                        << setw(25) << "Quantity"
                        << setw(25) << "Cost" << endl;

                    outFile << setfill(' ');
                    for (int i = 0; i < count; i++)
                    {
                        outFile << left << setw(25) << partNumber[i]
                            << left << setw(25) << descp[i]
                            << left << setw(25) << bin[i]
                            << left << setw(25) << qty[i]
                            << left << setw(25) << fixed << setprecision(2) << (rev[i] * qty[i]) << endl;
                    }

                    outFile << endl;

                    outFile << "Subtotal: $" << total << endl;
                    outFile << "Tax @ " << setprecision(3) << (tax_rate * 100) << "%: $" << setprecision(2) << (total * tax_rate) << endl;
                    outFile << "Total Price With Tax: $" << total * (1 + tax_rate) << endl << endl;

                    outFile.close();
                }
                else
                {
                    cout << "Error Encountered: Unable to create .txt file" << endl;
                }

                cout << endl;

                cout << "Subtotal: $" << total << endl;
                cout << "Tax @ " << setprecision(3) << (tax_rate * 100) << "%: $" << setprecision(2) << (total * tax_rate) << endl;
                cout << "Total Price With Tax: $" << total * (1 + tax_rate) << endl << endl;

                system("PAUSE");
                system("CLS");

                //Reset variables
                partNumber[count] = "";

                for (int x = 0; x < count; x++)
                {
                    partNumber[x] = "";
                    descp[x] = "";
                    bin[x] = "";
                    qty[x] = 0;
                    rev[x] = 0.00;
                }
                cost = 0.00;
                total = 0.00;
                count = 0;
            }
        }

    } while (mode != "e" || mode != "E");

    return 0;
}

double warrantyMarkUp(int type, double cost)
{
    //type 1 = warranty maintenance
    //type 2 = warranty repair

    switch (type)
    {
    case 1:
        return cost * 1.67;
        break;
    case 2:
        return cost * 1.816;
        break;
    }
}

double matrixMarkUp(double cost)
{
    double matrix = 0.00;
    double profit = 0.00;
    double rev = 0.00;

    if (cost >= 0.01 && cost <= 0.60)
        matrix = 3.70;
    else if (cost >= 0.61 && cost <= 1.20)
        matrix = 2.45;
    else if (cost >= 1.21 && cost <= 1.80)
        matrix = 2.40;
    else if (cost >= 1.81 && cost <= 2.99)
        matrix = 2.35;
    else if (cost >= 3.00 && cost <= 4.79)
        matrix = 2.30;
    else if (cost >= 4.80 && cost <= 5.99)
        matrix = 2.10;
    else if (cost >= 6.00 && cost <= 8.98)
        matrix = 1.70;
    else if (cost >= 8.99 && cost <= 11.98)
        matrix = 1.60;
    else if (cost >= 11.99 && cost <= 29.94)
        matrix = 1.40;
    else if (cost >= 29.95 && cost <= 44.91)
        matrix = 1.35;
    else if (cost >= 44.92 && cost <= 59.98)
        matrix = 1.30;
    else if (cost >= 59.99 && cost <= 74.85)
        matrix = 1.20;
    else if (cost >= 74.86 && cost <= 89.82)
        matrix = 1.16;
    else if (cost >= 89.83 && cost <= 119.76)
        matrix = 1.15;
    else if (cost >= 119.77 && cost <= 149.70)
        matrix = 1.12;
    else if (cost >= 149.71 && cost <= 179.64)
        matrix = 1.10;
    else if (cost >= 179.65 && cost <= 299.40)
        matrix = 1.00;
    else if (cost >= 299.41 && cost <= 598.80)
        matrix = 0.91;
    else if (cost >= 598.81 && cost <= 898.80)
        matrix = 0.90;
    else if (cost >= 898.81)
        matrix = 0.83;

    profit = cost * matrix;
    rev = cost + profit;

    return rev;
}

// Function to find and parse the line with the specified number
bool inventoryCheck(const string& searchNumber)
{
    ifstream file(filename_path + "\\" + filename);

    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return false;
    }

    string line;
    while (getline(file, line))
    {
        // Split the line into components separated by tabs
        istringstream iss(line);

        // Read the components
        if (getline(iss, number, '\t') && getline(iss, binLocation, '\t') &&
            getline(iss, description, '\t') && getline(iss, priceStr, '\t'))
        {

            // Remove quotes from the parsed strings
            number = number.substr(1, number.size() - 2);
            binLocation = binLocation.substr(1, binLocation.size() - 2);
            description = description.substr(1, description.size() - 2);
            description = description.substr(0, min(description.size(), size_t(17)));
            priceStr = priceStr.substr(1, priceStr.size() - 2);

            // Convert priceStr to a double
            double price;
            try
            {
                price = stod(priceStr);
            }
            catch (const std::invalid_argument& e) {
                cerr << "Invalid price format: " << priceStr << endl;
                continue; // Skip this line if price conversion fails
            }
            catch (const std::out_of_range& e) {
                cerr << "Price out of range: " << priceStr << endl;
                continue; // Skip this line if price conversion fails
            }

            // Check if the number matches the search number
            if (number == searchNumber)
            {
                file.close(); // Close the file
                return true; // Found and parsed successfully
            }
        }
    }

    cout << "Part number not found in file." << endl;
    file.close(); // Close the file
    return false; // Number not found
}

void load_config()
{
    ifstream config("config/config.txt");

    if (!config.is_open())
    {
        cout << "Error: Unable to open config file." << endl
            << "Please check with Anthony and try again." << endl;
        system("PAUSE");
        exit(1);
    }

    string line;

    while (getline(config, line))
    {
        if (!line.empty() && line[0] == '#')
        {
            size_t first_quote = line.find('"');
            size_t second_quote = line.find('"', first_quote + 1);

            if (first_quote != string::npos && second_quote != string::npos)
            {
                string value = line.substr(first_quote + 1, second_quote - first_quote - 1);

                //Store the value based on the key
                if (line.find("#filename ") != string::npos)
                    filename = value;
                else if (line.find("#filename_path") != string::npos)
                    filename_path = value;
                else if (line.find("#billed_path") != string::npos)
                    billed_path = value;
                else if (line.find("#quoted_path") != string::npos)
                    quoted_path = value;
                else if (line.find("#invoice_number") != string::npos)
                    invoice_number = stoi(value);
                else if (line.find("#quote_number") != string::npos)
                    quote_number = stoi(value);
            }
        }
    }
    config.close();
}

void addPart(string& partNumber, string& bin, string& descp, double& cost)
{
    ofstream inventory_data(filename_path + "\\" + filename, ios::app);
    if (inventory_data.is_open())
    {
        inventory_data << endl;

        inventory_data << "\"" << partNumber << "\""
            << "\t\"" << bin << "\""
            << "\t\"" << descp << "\""
            << "\t\"" << cost << "\"";
        inventory_data.close();
        cout << "Part added to database." << endl;
    }
}

void updateConfig(char type, int newInvoiceNumber)
{
    ifstream infile(config);
    if (!infile.is_open())
    {
        cout << "Error: Unable to open config file." << endl
            << "Please see Anthony." << endl;
        system("PAUSE");
        exit(1);
    }

    ofstream outfile(config + ".tmp");
    if (!outfile.is_open())
    {
        cout << "Error: Unable to open temp file." << endl
            << "Please see Anthony." << endl;
        infile.close();
        system("PAUSE");
        exit(1);
    }

    string line;
    string newInvoiceNumberStr = to_string(newInvoiceNumber);

    while (getline(infile, line))
    {
        if (line.find("#invoice_number") != string::npos && type == 'i')
        {
            size_t first_quote = line.find('"');
            size_t second_quote = line.find('"', first_quote + 1);

            if (first_quote != string::npos && second_quote != string::npos)
            {
                line = line.substr(0, first_quote + 1) + newInvoiceNumberStr +
                    line.substr(second_quote);
            }
        }
        else if (line.find("#quote_number") != string::npos && type == 'q')
        {
            size_t first_quote = line.find('"');
            size_t second_quote = line.find('"', first_quote + 1);

            if (first_quote != string::npos && second_quote != string::npos)
            {
                line = line.substr(0, first_quote + 1) + newInvoiceNumberStr +
                    line.substr(second_quote);
            }
        }
        outfile << line << endl;
    }
    infile.close();
    outfile.close();

    //Replace the original file with the temporary file
    if (remove(config.c_str()) != 0)
    {
        cout << "Error: Unable to delete original file: " << config << endl;
    }
    else if (rename((config + ".tmp").c_str(), config.c_str()) != 0)
    {
        cout << "Error: Unable to rename temporary file." << endl;
    }
}

bool fileExists(const string& filename)
{
    ifstream file(filename);
    return file.is_open();
}