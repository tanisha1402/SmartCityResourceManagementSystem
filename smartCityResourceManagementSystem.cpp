#include <bits/stdc++.h>
using namespace std;

// ===================== STRUCTS =====================
struct Citizen {
    string id;
    string name;
    int age;
    int zone; 
    string profession;
    string phone;
    string email;
};


struct Resource {
    double total;
    double available;
};

struct PowerFaultLogEntry {
    int zone;
    string faultType;   // e.g., "Overload", "Line Failure"
    string timestamp;
    string resolvedStatus; // "Pending" or "Resolved"
};
struct Zone {
    int population = 0;
    double powerDemand = 0;
    double allocatedPower = 0; 
    double waterDemand = 0;
    double wasteLevel = 0;
};
struct EmergencyCase {
    string type;           // Fire, Medical, Crime
    int zone;
    int severity;          // 1-10
    string status;         // "Pending", "Dispatched", "Resolved"
};

vector<Citizen> citizens;
vector<Zone> zones(3);  // Zone 1, 2, 3
vector<double> zoneWaterAllocated(3, 0);  // Stores how much water each zone was allocated
vector<EmergencyCase> emergencies;
int emergencyUnits[3] = {3, 3, 3}; // Number of units in each zone
Resource powerGrid = {100.0, 100.0};     // total and available power
Resource waterSupply = {100.0, 100.0};   // total and available water
vector<PowerFaultLogEntry> powerFaultLogs;
vector<pair<double, string>> powerSourceHistory;
vector<pair<double, string>> waterSourceHistory;

// Pollution thresholds for alerts (example values)
map<string, double> pollutionThresholds = {
    {"CO2", 1000.0},
    {"NOx", 200.0},
    {"PM2.5", 35.0},
    {"O3", 120.0}
};
// ===================== MODULES DECLARATIONS =====================
// ===================== MODULE 1 - CITIZEN MANAGEMENT =====================
void loadCitizensFromFile();
void saveCitizensToFile();
void addCitizen();
void viewZoneStats();
void updateCitizen();
void relocateCitizen();
void removeCitizen();
void searchCitizen();
void citizenManagementMenu();



// ===================== MODULE 2 - POWER GRID CONTROL =====================
void powerGridControlMenu();
void distributePower();
void viewPowerStatus();
void detectOverloadedZones();
void simulateLoadShedding();
void redistributePower();                // Greedy
void addPowerSource();
void generatePowerReport();

// ===================== MODULE 3: WATER DISTRIBUTION & USAGE =====================
void waterDistributionMenu();
void distributeWater();                   // Greedy
void detectWaterStressedZones();
void viewWaterStatus();
void addWaterSource();           // Dynamic Programming
void generateWaterUsageReport();

// ===================== MODULE 5: EMERGENCY SERVICES & SIMULATION =====================
void emergencyServicesMenu();
void loadEmergenciesFromFile();
void reportEmergency();
void viewEmergencies();
void dispatchUnits();
void predictEmergencyTrends();
void generateEmergencyReport();

//====================== MODULE 5 - WASTE MANAGEMENT SYSTEM =====================
void wasteManagementMenu();
void viewWasteLevels();
void scheduleWasteCollection();
void viewCriticalWasteZoneUsingDivideAndConquer();
void predictWasteOverflowDaysDP();
void generateWasteManagementReport();

// ===================== MODULE 6 - CITY ANALYTICS & DASHBOARD =====================
void cityAnalyticsMenu();
void viewOverloadedZones();
void viewCitizenDistribution();
void viewEmergencyLoad();
void predictFutureZoneLoads();
void viewCityHealthSummary();
void generateCityAnalyticsReport();

// ===================== MAIN =====================
void mainMenu() {
    int choice;
    do {
        cout << "\n====== Smart City Resource Management System ======\n";
        cout << "1. Citizen Management\n";
        cout << "2. Power Grid Control\n";
        cout << "3. Water Distribution & Usage\n";
        cout << "4. Emergency Services & Simulation\n";
        cout << "5. Waste Management System\n";
        cout << "6. City Analytics & Dashboard\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: citizenManagementMenu(); break;
            case 2: powerGridControlMenu(); break;
            case 3: waterDistributionMenu(); break;
            case 4: emergencyServicesMenu(); break;
            case 5: wasteManagementMenu(); break;
            case 6: cityAnalyticsMenu(); break;
            case 0: cout << "Exiting system. Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

int main() {
    mainMenu();
    return 0;
}

// ===================== MODULE 1: CITIZEN MANAGEMENT =====================
void citizenManagementMenu() {
    int option;
    do {
        cout << "\n--- Citizen Management ---\n";
        cout << "1. Load Citizens from File\n";
        cout << "2. Add New Citizen\n";
        cout << "3. Update Specific Citizen Info\n";
        cout << "4. Relocate Citizen\n";
        cout << "5. Remove Citizen\n";
        cout << "6. Search Citizen by ID\n";
        cout << "7. View Zone Details\n";
        cout << "8. Save Citizens to File\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> option;
        cin.ignore();

        switch(option) {
            case 1: loadCitizensFromFile(); break;
            case 2: addCitizen(); break;
            case 3: updateCitizen(); break;
            case 4: relocateCitizen(); break;
            case 5: removeCitizen(); break;
            case 6: searchCitizen(); break;
            case 7: viewZoneStats(); break;
            case 8: saveCitizensToFile(); break;
            case 0: cout << "Returning to main menu...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (option != 0);
}
void loadCitizensFromFile() {
    ifstream file("citizens_input.txt");
    ofstream log("citizen_loading_report.txt");

    if (!file) {
        cout << "Error: Could not open file.\n";
        return;
    }

    if (!log) {
        cout << "Error: Could not create report file.\n";
        return;
    }

    citizens.clear(); // Clear previous data
    Citizen c;
    int totalLoaded = 0, invalidZoneCount = 0;

    log << "=================================================================\n";
    log << "                    SMART CITY CITIZEN LOADING REPORT             \n";
    log << "=================================================================\n\n";
    log << "Source File: citizens_input.txt\n\n";

    while (getline(file, c.id) &&
           getline(file, c.name) &&
           file >> c.age >> ws &&
           file >> c.zone >> ws &&
           getline(file, c.profession) &&
           getline(file, c.phone) &&
           getline(file, c.email)) {

        log << "Processing Citizen ID: " << c.id << "\n";
        log << "  Name       : " << c.name << "\n";
        log << "  Age        : " << c.age << "\n";
        log << "  Zone       : " << c.zone << "\n";
        log << "  Profession : " << c.profession << "\n";
        log << "  Phone      : " << c.phone << "\n";
        log << "  Email      : " << c.email << "\n";

        if (c.zone >= 1 && c.zone <= 3) {
            citizens.push_back(c);
            zones[c.zone - 1].population++;
            zones[c.zone - 1].powerDemand += 3;
            zones[c.zone - 1].waterDemand += 3;
            zones[c.zone - 1].wasteLevel += 4;
            totalLoaded++;

            log << "  ✅ Citizen added to Zone " << c.zone << ". Resources updated.\n";
        } else {
            cout << "Invalid zone: " << c.zone << " for citizen ID " << c.id << "\n";
            log << "  ❌ Invalid zone! Citizen skipped.\n";
            invalidZoneCount++;
        }

        log << "--------------------------------------------\n";
    }

    file.close();

    // Final summary

    log << "\n======================= Summary ==========================\n";
    log << "Total Citizens Processed : " << (totalLoaded + invalidZoneCount) << "\n";
    log << "Successfully Loaded       : " << totalLoaded << "\n";
    log << "Invalid Zone Entries      : " << invalidZoneCount << "\n";
    log << "\n\n=============================== Zones Updated ===================================\n";
    for (int i = 0; i < zones.size(); ++i) {
        log << "  Zone " << (i + 1)
            << " -> Population: " << zones[i].population
            << ", Power: " << zones[i].powerDemand
            << ", Water: " << zones[i].waterDemand
            << ", Waste: " << zones[i].wasteLevel << "\n";
    }

    log << "\nNote: This log is auto-generated after reading the citizen input file.\n";
    log.close();

    cout << "Citizens loaded successfully from file.\n";
    cout << "Detailed log saved to 'citizen_loading_report.txt'\n";
}




void addCitizen() {
    Citizen c;
    cout << "Enter ID: "; getline(cin, c.id);
    cout << "Enter Name: "; getline(cin, c.name);
    cout << "Enter Age: "; cin >> c.age; cin.ignore();

    cout << "Choose Zone (Enter 1, 2, or 3):\n";
    cout << "1. Zone 1\n";
    cout << "2. Zone 2\n";
    cout << "3. Zone 3\n";
    cout << "Enter Zone Number: ";
    cin >> c.zone;
    cin.ignore();

    cout << "Enter Profession: "; getline(cin, c.profession);
    cout << "Enter Phone Number: "; getline(cin, c.phone);
    cout << "Enter Email Address: "; getline(cin, c.email);

    citizens.push_back(c);

    ofstream log("citizen_add_log.txt", ios::app); // Append mode
    if (!log) {
        cout << "Error: Could not open citizen_add_log.txt for writing.\n";
        return;
    }

    int zoneIndex = c.zone - 1;
    if (zoneIndex >= 0 && zoneIndex < zones.size()) {
        zones[zoneIndex].population++;
        zones[zoneIndex].powerDemand += 3.0;
        zones[zoneIndex].waterDemand += 3.0;
        zones[zoneIndex].wasteLevel += 4.0;
   
        log << "======= New Citizen Added =======\n";
        log << "ID        : " << c.id << "\n";
        log << "Name      : " << c.name << "\n";
        log << "Age       : " << c.age << "\n";
        log << "Zone      : " << c.zone << "\n";
        log << "Profession: " << c.profession << "\n";
        log << "Phone     : " << c.phone << "\n";
        log << "Email     : " << c.email << "\n";
        log << "-> Zone " << c.zone << " updated:\n";
        log << "   +1 population, +3 kWh power, +3 gallons water, +4 kg waste\n";
        log << "----------------------------------\n";
    } else {
        log << "ERROR: Invalid zone entered for citizen ID " << c.id << "\n";
    }

    log.close();
    cout << "Citizen added successfully.\n";
    cout<< "Citizen data logged to 'citizen_add_log.txt'\n";
}

void updateCitizen() {
    string id;
    cout << "Enter ID of citizen to update: ";
    getline(cin, id);
    
    for (Citizen &c : citizens) {
        if (c.id == id) {
            ofstream logFile("citizen_update_log.txt", ios::app); // Open in append mode
            if (!logFile) {
                cout << "Error opening log file.\n";
                return;
            }
            logFile << "--- Citizen Update Log ---\n";
            logFile << "Citizen ID: " << c.id << "\n";
            logFile << "Previous Info -> Name: " << c.name << ", Age: " << c.age 
                    << ", Zone: " << c.zone << ", Profession: " << c.profession 
                    << ", Phone: " << c.phone << ", Email: " << c.email << "\n";

            int field;
            cout << "What would you like to update?\n";
            cout << "1. Name\n2. Age\n3. Zone\n4. Profession\n5. Phone\n6. Email\n";
            cout << "Enter option number: ";
            cin >> field;
            cin.ignore();

            switch (field) {
                case 1: {
                    string oldName = c.name;
                    cout << "New Name: "; getline(cin, c.name);
                    logFile << "Updated Field: Name | Old: " << oldName << " | New: " << c.name << "\n";
                    break;
                }
                case 2: {
                    int oldAge = c.age;
                    cout << "New Age: "; cin >> c.age; cin.ignore();
                    logFile << "Updated Field: Age | Old: " << oldAge << " | New: " << c.age << "\n";
                    break;
                }
                case 3: {
                    int oldZone = c.zone;
                    cout << "Choose New Zone (1, 2, 3): ";
                    cin >> c.zone;
                    cin.ignore();
                    if (c.zone < 1 || c.zone > 3) {
                        cout << "Invalid zone. Update failed.\n";
                        logFile << "Zone update failed due to invalid input.\n\n";
                        logFile.close();
                        return;
                    }
                    logFile << "Updated Field: Zone | Old: " << oldZone << " | New: " << c.zone << "\n";
                    break;
                }
                case 4: {
                    string oldProf = c.profession;
                    cout << "New Profession: "; getline(cin, c.profession);
                    logFile << "Updated Field: Profession | Old: " << oldProf << " | New: " << c.profession << "\n";
                    break;
                }
                case 5: {
                    string oldPhone = c.phone;
                    cout << "New Phone: "; getline(cin, c.phone);
                    logFile << "Updated Field: Phone | Old: " << oldPhone << " | New: " << c.phone << "\n";
                    break;
                }
                case 6: {
                    string oldEmail = c.email;
                    cout << "New Email: "; getline(cin, c.email);
                    logFile << "Updated Field: Email | Old: " << oldEmail << " | New: " << c.email << "\n";
                    break;
                }
                default:
                    cout << "Invalid choice.\n";
                    logFile << "Update failed due to invalid choice.\n\n";
                    logFile.close();
                    return;
            }

            logFile << "Update Status: SUCCESS\n\n";
            logFile.close();

            cout << "Citizen updated.\n";
            cout << "Update logged to 'citizen_update_log.txt'\n";
            return;
        }
    }

    cout << "Citizen not found.\n";
    
    // Log not found case
    ofstream logFile("citizen_log.txt", ios::app);
    if (logFile) {
        logFile << "--- Citizen Update Attempt ---\n";
        logFile << "Citizen ID: " << id << " not found in records.\n\n";
        logFile.close();
    }
}

void relocateCitizen() {
    string id;
    cout << "Enter ID of citizen to relocate: ";
    getline(cin, id);

    ofstream logFile("citizen_relocation_log.txt", ios::app);
    if (!logFile) {
        cout << "Error opening log file.\n";
        return;
    }

    for (Citizen &c : citizens) {
        if (c.id == id) {
            int oldZone = c.zone;
            cout << "Current Zone: " << c.zone << "\n";
            cout << "Choose New Zone (1, 2, or 3): ";
            cin >> c.zone;
            cin.ignore();

            if (c.zone < 1 || c.zone > 3) {
                cout << "Invalid zone. Relocation cancelled.\n";
                c.zone = oldZone;

                logFile << "[Relocate Citizen] Failed: Invalid zone entered for Citizen ID: " << id << "\n";
                logFile.close();
                return;
            }
            

            // Update population and resource stats for old zone
            zones[oldZone - 1].population--;
            zones[oldZone - 1].powerDemand -= 3;
            zones[oldZone - 1].waterDemand -= 3;
            zones[oldZone - 1].wasteLevel -= 4;

            // Update population and resource stats for new zone
            zones[c.zone - 1].population++;
            zones[c.zone - 1].powerDemand += 3;
            zones[c.zone - 1].waterDemand += 3;
            zones[c.zone - 1].wasteLevel += 4;

            cout << "Citizen relocated successfully.\n";
            cout << "Citizen data logged to 'citizen_relocation_log.txt'\n";
      
            logFile << "[Relocate Citizen] Citizen ID: " << c.id << ", Name: " << c.name
                    << " moved from Zone " << oldZone << " to Zone " << c.zone << ".\n";
            logFile << "Zone " << oldZone << " updated - Population: " << zones[oldZone - 1].population
                    << ", Power: -" << 3 << " kWh, Water: -" << 3 << " gallons, Waste: -" << 4 << " kg\n";
            logFile << "Zone " << c.zone << " updated - Population: " << zones[c.zone - 1].population
                    << ", Power: +" << 3 << " kWh, Water: +" << 3 << " gallons, Waste: +" << 4 << " kg\n\n\n";

            logFile.close();
            return;
        }
    }

    cout << "Citizen not found.\n";
    logFile << "[Relocate Citizen] Failed: Citizen ID not found - " << id << "\n";
    logFile.close();
}


// Utility function to get current timestamp (if not already added globally)
string getCurrentTimestamp() {
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", localtime(&now));
    return string(buf);
}

void removeCitizen() {
    string id;
    cout << "Enter ID of citizen to remove: ";
    getline(cin, id);

    // Attempt to remove the citizen
    auto it = remove_if(citizens.begin(), citizens.end(), [&](Citizen c) {
        return c.id == id;
    });

    // Open log file in append mode
    ofstream log("citizen_remove_log.txt", ios::app);

    if (it != citizens.end()) {
        citizens.erase(it, citizens.end());
        cout << "Citizen removed.\n";
        cout << "Citizen data logged to 'citizen_remove_log.txt'\n";

        // Log successful removal
        log << "[" << getCurrentTimestamp() << "] Citizen with ID " << id << " was removed successfully.\n";
    } else {
        cout << "Citizen not found.\n";

        // Log failed removal
        log << "[" << getCurrentTimestamp() << "] Attempted to remove citizen with ID " << id << " but not found.\n";
    }

    log.close();  // Close the log file
}


void searchCitizen() {
    string id;
    cout << "Enter ID to search: ";
    getline(cin, id);

    // Open log file in append mode
    ofstream log("citizen_search_log.txt", ios::app);
    log << "[" << getCurrentTimestamp() << "] Search attempt for citizen ID: " << id << "\n";

    for (Citizen &c : citizens) {
        if (c.id == id) {
            cout << "Name: " << c.name << "\n";
            cout << "Age: " << c.age << "\n";
            cout << "Zone: " << c.zone << "\n";
            cout << "Profession: " << c.profession << "\n";
            cout << "Phone: " << c.phone << "\n";
            cout << "Email: " << c.email << "\n";

            // Log successful search
            log << "  -> FOUND. Name: " << c.name
                << ", Zone: " << c.zone
                << ", Profession: " << c.profession << "\n";
            log.close();
            cout<< "Citizen details logged to 'citizen_search_log.txt'\n";
            return;
        }
    }

    cout << "Citizen not found.\n";
    log << "  -> NOT FOUND.\n";
    log.close();
}
void viewZoneStats() {
    
    cout << "\n--- Zone Resource Stats ---\n";
    for (int i = 0; i < zones.size(); ++i) {
        cout << "Zone " << (i + 1) << ":\n";
        cout << "  Population: " << zones[i].population << "\n";
        cout << "  Power Demand: " << zones[i].powerDemand << " kWh\n";
        cout << "  Water Demand: " << zones[i].waterDemand << " gallons\n";
        cout << "  Waste Level: " << zones[i].wasteLevel << " kg\n";
        cout << "------------------------------\n";
    }

    // Automatically save the report to 'zone_stats.txt'
    ofstream outFile("zone_stats.txt");
    if (!outFile) {
        cerr << "Error creating file.\n";
        return;
    }

    outFile << "=================================================================\n";
    outFile << "                    SMART CITY ZONE STATS REPORT                 \n";
    outFile << "=================================================================\n\n";
    outFile << "--- Zone Resource Stats ---\n";
    for (int i = 0; i < zones.size(); ++i) {
        outFile << "Zone " << (i + 1) << ":\n";
        outFile << "  Population: " << zones[i].population << "\n";
        outFile << "  Power Demand: " << zones[i].powerDemand << " kWh\n";
        outFile << "  Water Demand: " << zones[i].waterDemand << " gallons\n";
        outFile << "  Waste Level: " << zones[i].wasteLevel << " kg\n";
        outFile << "------------------------------\n";
    }

    outFile.close();
    cout << "\nZone details saved to 'zone_stats.txt'.\n";
}


void saveCitizensToFile() {
    ofstream out("citizens_output.txt");
    if (!out) {
        cout << "Error: Cannot write to citizens_output.txt\n";
        return;
    }

    out << "=====================================================================\n";
    out << "                    SMART CITY CITIZEN RECORDS REPORT                 \n";
    out << "=====================================================================\n\n";
    out << "----- Citizen Records -----\n";
    for (const Citizen& c : citizens) {
        out << "ID: " << c.id << "\n";
        out << "Name: " << c.name << "\n";
        out << "Age: " << c.age << "\n";
        out << "Zone: " << c.zone << "\n";
        out << "Profession: " << c.profession << "\n";
        out << "Phone: " << c.phone << "\n";
        out << "Email: " << c.email << "\n";
        out << "--------------------------\n";
    }
    out.close();
    cout << "Citizen data saved to citizens_output.txt\n";
}

// ===================== STUB MODULES =====================
void powerGridControlMenu() {
    int choice;
    do {
        cout << "\n--- Power Grid Control Module ---\n";
        cout << "1. Distribute Power (Greedy Algorithm)\n";
        cout << "2. View Total Power Availability & Demand\n";
        cout << "3. Detect Overloaded Zones\n";
        cout << "4. Simulate Load Shedding\n";
        cout << "5. Add New Power Source\n";
        cout << "6. Generate Power Usage Report (File Export)\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: distributePower(); break;
            case 2: viewPowerStatus(); break;
            case 3: detectOverloadedZones(); break;
            case 4: simulateLoadShedding(); break;
            case 5: addPowerSource(); break;
            case 6: generatePowerReport(); break;
            case 0: cout << "Returning to main menu...\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);
}
void distributePower() {
    cout << "\n--- Power Distribution (Greedy Allocation) ---\n";
    ofstream logFile("distribute_power_log.txt", ios::app);
    if (!logFile) {
        cerr << "Error opening distribute_power_log.txt for writing.\n";
        return;
    }

    // Timestamp
    time_t now = time(0);
    char* dt = ctime(&now);
    logFile << "\n=== Power Distribution Log ===\n";
    logFile << "Timestamp: " << dt;

    vector<pair<double,int>> demandPriority; // {powerDemand, zoneIndex}
    for (int i = 0; i < zones.size(); i++) {
        demandPriority.push_back({zones[i].powerDemand, i});
    }
    sort(demandPriority.rbegin(), demandPriority.rend()); // highest demand first

    double remainingPower = powerGrid.available;
    vector<double> allocation(zones.size(), 0);

    for (auto& p : demandPriority) {
        int idx = p.second;
        double demand = p.first;
        if (remainingPower >= demand) {
            allocation[idx] = demand;
            remainingPower -= demand;
        } else {
            allocation[idx] = remainingPower;
            remainingPower = 0;
            break;
        }
    }
   
    // Update available power after allocation
    powerGrid.available = remainingPower;

    for (int i = 0; i < zones.size(); i++) {
        cout << "Zone " << i+1 << ": Allocated " << allocation[i] << " / " << zones[i].powerDemand << " kWh\n";
        logFile << "Zone " << i+1 << ": Allocated " << allocation[i] << " / " << zones[i].powerDemand << " kWh\n";
    }

    for (int i = 0; i < zones.size(); i++) {
        zones[i].allocatedPower = allocation[i];
    }
    cout<<"Power distribution saved in distribute_power_log.txt successfully.\n";
    logFile << "Remaining Power in Grid: " << remainingPower << " kWh\n";
    logFile << "-----------------------------\n";
    logFile.close();

}

void viewPowerStatus() {
    cout << "\n--- Power Grid Status ---\n";
    cout << "Total Power Capacity: " << powerGrid.total << " kWh\n";

    double totalDemand = 0, totalAllocated = 0;
    for (const auto& z : zones) {
        totalDemand += z.powerDemand;
        totalAllocated += z.allocatedPower;
    }

    cout << "Total Power Demand: " << totalDemand << " kWh\n";
    cout << "Total Power Allocated: " << totalAllocated << " kWh\n";
    cout << "Available Power: " << powerGrid.available << " kWh\n";

    cout << "\nZone-wise Power Distribution:\n";
    for (int i = 0; i < zones.size(); ++i) {
        cout << "Zone " << i+1 << ": "
             << zones[i].allocatedPower << " allocated / "
             << zones[i].powerDemand << " demanded\n";
    }

    // Logging output to "view_power_status_log.txt"
    ofstream log("view_power_status_log.txt", ios::app);
    if (log.is_open()) {
        time_t now = time(0);
        log << "\n[Timestamp: " << ctime(&now) << "]\n";
        log << "--- Power Grid Status ---\n";
        log << "Total Power Capacity: " << powerGrid.total << " kWh\n";
        log << "Total Power Demand: " << totalDemand << " kWh\n";
        log << "Total Power Allocated: " << totalAllocated << " kWh\n";
        log << "Available Power: " << powerGrid.available << " kWh\n";
        log << "\nZone-wise Power Distribution:\n";
        for (int i = 0; i < zones.size(); ++i) {
            log << "Zone " << i+1 << ": "
                << zones[i].allocatedPower << " allocated / "
                << zones[i].powerDemand << " demanded\n";
        }
        log << "------------------------------------------\n";
        log.close();
    } else {
        cerr << "Error writing to view_power_status_log.txt\n";
    }
    cout << "Power status logged to 'view_power_status_log.txt'.\n";
}

void detectOverloadedZones() {
    cout << "\n--- Overloaded Zones ---\n";
    bool found = false;

    // Logging setup
    ofstream log("detect_overloaded_zones_log.txt", ios::app);
    if (log.is_open()) {
        time_t now = time(0);
        log << "\n[Timestamp: " << ctime(&now) << "]\n";
        log << "--- Overloaded Zones ---\n";

        for (int i = 0; i < zones.size(); ++i) {
            if (zones[i].allocatedPower < zones[i].powerDemand) {
                cout << "Zone " << i + 1 << " is overloaded. "
                     << "Demand: " << zones[i].powerDemand
                     << ", Allocated: " << zones[i].allocatedPower << "\n";

                log << "Zone " << i + 1 << " is overloaded. "
                    << "Demand: " << zones[i].powerDemand
                    << ", Allocated: " << zones[i].allocatedPower << "\n";
                found = true;
            }
        }

        if (!found) {
            cout << "No overloaded zones detected. All zones have sufficient power.\n";
            log << "No overloaded zones detected. All zones have sufficient power.\n";
        }

        log << "------------------------------------------\n";
        log.close();
    } else {
        cerr << "Error writing to detect_overloaded_zones_log.txt\n";
    }
    cout << "Overloaded zones logged to 'detect_overloaded_zones_log.txt'.\n";
}


void simulateLoadShedding() {
    cout << "\n--- Load Shedding Simulation ---\n";

    // Logging setup
    ofstream log("simulate_load_shedding_log.txt", ios::app);
    if (log.is_open()) {
        time_t now = time(0);
        log << "\n[Timestamp: " << ctime(&now) << "]\n";
        log << "--- Load Shedding Simulation ---\n";

        for (int i = 0; i < zones.size(); ++i) {
            if (zones[i].allocatedPower < zones[i].powerDemand) {
                double shortage = zones[i].powerDemand - zones[i].allocatedPower;
                cout << "Zone " << i + 1 << ": Load shedding in effect! "
                     << "Shortage of " << shortage << " kWh.\n";

                log << "Zone " << i + 1 << ": Load shedding in effect! "
                    << "Shortage of " << shortage << " kWh.\n";
            } else {
                cout << "Zone " << i + 1 << ": Power supply is stable.\n";
                log << "Zone " << i + 1 << ": Power supply is stable.\n";
            }
        }
        
        log << "------------------------------------------\n";
        log.close();
    } else {
        cerr << "Error writing to simulate_load_shedding_log.txt\n";
    }
    cout<< "Load shedding simulation logged to 'simulate_load_shedding_log.txt'.\n";
}



void redistributePower() {
    cout << "\n--- Redistributing Power (Greedy to Underpowered Zones) ---\n";

    ofstream log("add_power_source_log.txt", ios::app);
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back(); // remove newline

    if (log.is_open()) {
        log << "--- Redistributing Power (Greedy to Underpowered Zones) ---\n";
    }

    // Step 1: Collect unmet demands only
    vector<pair<double, int>> unmetDemand; // {unmetDemand, zoneIndex}
    for (int i = 0; i < zones.size(); i++) {
        double unmet = zones[i].powerDemand - zones[i].allocatedPower;
        if (unmet > 0) {
            unmetDemand.push_back({unmet, i});
        }
    }

    if (unmetDemand.empty()) {
        cout << "All zones are fully powered. No redistribution needed.\n";
        if (log.is_open()) {
            log << "All zones are fully powered. No redistribution needed.\n";
            log << "----------------------------------------------------\n";
            log.close();
        }
        return;
    }

    // Step 2: Sort by unmet demand (Greedy - largest first)
    sort(unmetDemand.rbegin(), unmetDemand.rend());

    // Step 3: Redistribute remaining power
    double remainingPower = powerGrid.available;

    for (auto& entry : unmetDemand) {
        int idx = entry.second;
        double need = entry.first;

        if (remainingPower >= need) {
            zones[idx].allocatedPower += need;
            remainingPower -= need;
        } else {
            zones[idx].allocatedPower += remainingPower;
            remainingPower = 0;
            break;
        }
    }

    // Step 4: Update available power
    powerGrid.available = remainingPower;

    // Step 5: Report to console and log
    for (int i = 0; i < zones.size(); i++) {
        cout << "Zone " << i + 1 << ": Allocated " << zones[i].allocatedPower
             << " / " << zones[i].powerDemand << " kWh\n";

        if (log.is_open()) {
            log << "Zone " << i + 1 << ": Allocated " << zones[i].allocatedPower
                << " / " << zones[i].powerDemand << " kWh\n";
        }
    }

    cout << "Remaining Power: " << powerGrid.available << " kWh\n";
    if (log.is_open()) {
        log << "Remaining Power: " << powerGrid.available << " kWh\n";
        log << "----------------------------------------------------\n";
        log.close();
    } else {
        cerr << "Error writing to add_power_source_log.txt\n";
    }
}

void addPowerSource() {
    double newPower;
    cout << "Enter power capacity to add: ";
    cin >> newPower;

    ofstream log("add_power_source_log.txt", ios::app);
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back(); // remove trailing newline

    if (newPower <= 0) {
        cout << "Invalid input, must be positive.\n";

        // Log the invalid attempt
        if (log.is_open()) {
            log << "\n[Timestamp: " << timestamp << "]\n";
            log << "Attempted to add invalid power capacity: " << newPower << " kWh (Rejected)\n";
            log << "----------------------------------------------------\n";
            log.close();
        } else {
            cerr << "Error writing to add_power_source_log.txt\n";
        }

        return;
    }

    powerGrid.total += newPower;
    powerGrid.available += newPower;
    cout << "Power capacity increased by " << newPower << " kWh.\n";

    // Format time as YYYY-MM-DD HH:MM:SS
    tm* localtm = localtime(&now);
    stringstream timeStr;
    timeStr << (1900 + localtm->tm_year) << "-"
            << (1 + localtm->tm_mon) << "-"
            << localtm->tm_mday << " "
            << localtm->tm_hour << ":"
            << localtm->tm_min << ":"
            << localtm->tm_sec;

    // Store the added power and timestamp
    powerSourceHistory.push_back({newPower, timeStr.str()});

    // Log the successful addition
    if (log.is_open()) {
        log << "\n[Timestamp: " << timestamp << "]\n";
        log << "Power source added: " << newPower << " kWh\n";
        log << "----------------------------------------------------\n";
        log.close();
    } else {
        cerr << "Error writing to add_power_source_log.txt\n";
    }

    // Redistribute power to underpowered zones automatically
    redistributePower();
    cout << "Power source added and saved to add_power_source_log.txt.\n";
}
void generatePowerReport() {
    std::ofstream out("power_usage_report.txt");
    if (!out) {
        std::cout << "Error opening file for writing.\n";
        return;
    }

    out << std::fixed << std::setprecision(2);

    out << "=================================================================\n";
    out << "                    SMART CITY POWER GRID REPORT                 \n";
    out << "=================================================================\n\n";

    // General summary
    out << "TOTAL POWER GRID SUMMARY\n";
    out << "------------------------\n";
    out << "Total Power Capacity     : " << powerGrid.total << " kWh\n";
    double totalAllocated = 0, totalDemand = 0;
    for (auto& z : zones) {
        totalAllocated += z.allocatedPower;
        totalDemand += z.powerDemand;
    }
    out << "Total Power Allocated    : " << totalAllocated << " kWh\n";
    out << "Total Power Demand       : " << totalDemand << " kWh\n";
    out << "Available Power          : " << powerGrid.available << " kWh\n\n";

    // Power source additions history
    out << "POWER SOURCE ADDITIONS HISTORY\n";
    out << "------------------------------\n";
    if (powerSourceHistory.empty()) {
        out << "No power sources added yet.\n\n";
    } else {
        for (const auto& entry : powerSourceHistory) {
            out << "Added " << entry.first << " kWh on " << entry.second << "\n";
        }
        out << "\n";
    }

    // Zone-wise detailed report
    out << "ZONE-WISE POWER DETAILS\n";
    out << "----------------------\n";
    for (int i = 0; i < zones.size(); i++) {
        out << "Zone " << (i + 1) << ":\n";
        out << "  Population          : " << zones[i].population << "\n";
        out << "  Power Demand        : " << zones[i].powerDemand << " kWh\n";
        out << "  Power Allocated     : " << zones[i].allocatedPower << " kWh\n";

        if (zones[i].allocatedPower < zones[i].powerDemand) {
            double shortage = zones[i].powerDemand - zones[i].allocatedPower;
            out << "  STATUS              : OVERLOADED (Shortage of " << shortage << " kWh)\n";
            out << "  LOAD SHEDDING       : ACTIVE\n";
        } else {
            out << "  STATUS              : Stable\n";
            out << "  LOAD SHEDDING       : None\n";
        }
        out << "\n";
    }

    
    out << "\n";

    // Summary notes
    out << "POWER DISTRIBUTION & REDISTRIBUTION\n";
    out << "-----------------------------------\n";
    out << "Latest power allocation was done using a Greedy algorithm prioritizing zones with highest demand.\n";
    out << "Additional power sources have been added to meet growing demand dynamically.\n";
    out << "Load shedding simulations indicate zones with insufficient power supply.\n\n";

    out << "============================================\n";
    out << "       END OF POWER GRID REPORT             \n";
    out << "============================================\n";

    out.close();
    std::cout << "Power usage report saved to 'power_usage_report.txt'.\n";
}


void waterDistributionMenu() {
    int choice;
    do {
        cout << "\n--- Water Distribution & Usage Module ---\n";
        cout << "1. Distribute Water to Zones (Greedy Allocation)\n";
        cout << "2. View Total and Available Water\n";
        cout << "3. Simulate Water Crisis (Divide & Conquer)\n";
        cout << "4. Add New Water Source (Dynamic Programming)\n";
        cout << "5. Generate Water Usage Report\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: distributeWater(); break;
            case 2: viewWaterStatus(); break;
            case 3: detectWaterStressedZones(); break;  
            case 4: addWaterSource(); break;
            case 5: generateWaterUsageReport(); break;
            case 0: cout << "Returning to main menu...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void distributeWater() {
    cout << "\n--- Water Distribution (Greedy Allocation) ---\n";

    // Prepare for logging
    ofstream log("water_distribution_log.txt", ios::app);
    time_t now = time(0);
    tm* localtm = localtime(&now);
    stringstream timestamp;
    timestamp << "[" << (1900 + localtm->tm_year) << "-"
              << setw(2) << setfill('0') << (1 + localtm->tm_mon) << "-"
              << setw(2) << setfill('0') << localtm->tm_mday << " "
              << setw(2) << setfill('0') << localtm->tm_hour << ":"
              << setw(2) << setfill('0') << localtm->tm_min << ":"
              << setw(2) << setfill('0') << localtm->tm_sec << "]";

    log << "\n" << timestamp.str() << " --- Water Distribution (Greedy Allocation) ---\n";

    vector<pair<double, int>> demandPriority;
    for (int i = 0; i < zones.size(); i++) {
        demandPriority.push_back({zones[i].waterDemand, i});
    }
    sort(demandPriority.rbegin(), demandPriority.rend());

    double remaining = waterSupply.available;
    vector<double> allocation(zones.size(), 0);

    for (auto& p : demandPriority) {
        int idx = p.second;
        double demand = p.first;
        if (remaining >= demand) {
            allocation[idx] = demand;
            remaining -= demand;
        } else {
            allocation[idx] = remaining;
            remaining = 0;
            break;
        }
    }

    // Update allocated water
    for (int i = 0; i < zones.size(); i++) {
        zoneWaterAllocated[i] = allocation[i];
    }

    waterSupply.available = remaining;

    // Output and Log allocation results
    for (int i = 0; i < zones.size(); i++) {
        cout << "Zone " << i + 1 << ": Allocated " << zoneWaterAllocated[i]
             << " / " << zones[i].waterDemand << " gallons\n";

        log << "Zone " << i + 1 << ": Allocated " << zoneWaterAllocated[i]
            << " / " << zones[i].waterDemand << " gallons\n";
    }

    log << "Remaining Water: " << waterSupply.available << " gallons\n";
    log.close();
    cout << "Water distribution logged to 'water_distribution_log.txt'.\n";
}
void viewWaterStatus() {
    cout << "\n--- Water Supply Status ---\n";
    cout << "Total Water Capacity: " << waterSupply.total << " gallons\n";
    cout << "Available Water: " << waterSupply.available << " gallons\n";

    // Prepare logging
    ofstream log("water_status_log.txt", ios::app);
    time_t now = time(0);
    tm* localtm = localtime(&now);
    stringstream timestamp;
    timestamp << "[" << (1900 + localtm->tm_year) << "-"
              << setw(2) << setfill('0') << (1 + localtm->tm_mon) << "-"
              << setw(2) << setfill('0') << localtm->tm_mday << " "
              << setw(2) << setfill('0') << localtm->tm_hour << ":"
              << setw(2) << setfill('0') << localtm->tm_min << ":"
              << setw(2) << setfill('0') << localtm->tm_sec << "]";

    log << "\n" << timestamp.str() << " --- Water Supply Status ---\n";
    log << "Total Water Capacity: " << waterSupply.total << " gallons\n";
    log << "Available Water: " << waterSupply.available << " gallons\n\n";

    cout << "\nZone-wise Water Details:\n";
    log << "Zone-wise Water Details:\n";

    for (int i = 0; i < zones.size(); ++i) {
        double unmet = max(0.0, zones[i].waterDemand - zoneWaterAllocated[i]);

        cout << "Zone " << (i + 1) << ":\n";
        cout << "  Water Demand: " << zones[i].waterDemand << " gallons\n";
        cout << "  Water Allocated: " << zoneWaterAllocated[i] << " gallons\n";
        cout << "  Unmet Demand: " << unmet << " gallons\n";
        cout << "------------------------------\n";

        log << "Zone " << (i + 1) << ":\n";
        log << "  Water Demand: " << zones[i].waterDemand << " gallons\n";
        log << "  Water Allocated: " << zoneWaterAllocated[i] << " gallons\n";
        log << "  Unmet Demand: " << unmet << " gallons\n";
        log << "------------------------------\n";
    }

    log.close();
    cout<< "Water status logged to 'water_status_log.txt'.\n";
}
void detectWaterStressedZones() {
    cout << "\n--- Water-Stressed Zones ---\n";

    // Open log file in append mode
    ofstream log("water_stressed_zones_log.txt", ios::app);

    // Generate timestamp
    time_t now = time(0);
    tm* localtm = localtime(&now);
    stringstream timestamp;
    timestamp << "[" << (1900 + localtm->tm_year) << "-"
              << setw(2) << setfill('0') << (1 + localtm->tm_mon) << "-"
              << setw(2) << setfill('0') << localtm->tm_mday << " "
              << setw(2) << setfill('0') << localtm->tm_hour << ":"
              << setw(2) << setfill('0') << localtm->tm_min << ":"
              << setw(2) << setfill('0') << localtm->tm_sec << "]";

    log << "\n" << timestamp.str() << " --- Water-Stressed Zones ---\n";

    bool found = false;
    for (int i = 0; i < zones.size(); ++i) {
        double unmet = zones[i].waterDemand - zoneWaterAllocated[i];
        if (unmet > 0.0) {
            cout << "Zone " << (i + 1) << " is stressed! Unmet Demand: " << unmet << " gallons\n";
            log << "Zone " << (i + 1) << " is stressed! Unmet Demand: " << unmet << " gallons\n";
            found = true;
        }
    }

    if (!found) {
        cout << "All zones are adequately supplied with water.\n";
        log << "All zones are adequately supplied with water.\n";
    }

    log.close();
    cout << "Water-stressed zones logged to 'water_stressed_zones_log.txt'.\n";
}
void redistributeWater() {
    cout << "\n--- Redistributing Water (Greedy to Under-supplied Zones) ---\n";

    // Open log file in append mode
    ofstream log("add_water_source_log.txt", ios::app);

   

   
    vector<pair<double, int>> unmet;
    for (int i = 0; i < zones.size(); i++) {
        double unmetDemand = zones[i].waterDemand - zoneWaterAllocated[i];
        if (unmetDemand > 0)
            unmet.push_back({unmetDemand, i});
    }

    if (unmet.empty()) {
        cout << "All zones are fully supplied. No redistribution needed.\n";
        log << "All zones are fully supplied. No redistribution needed.\n";
        log.close();
        return;
    }

    // Sort by highest unmet demand first (Greedy)
    sort(unmet.rbegin(), unmet.rend());

    double remaining = waterSupply.available;

    for (auto& p : unmet) {
        int idx = p.second;
        double need = p.first;
        double allocated = 0.0;

        if (remaining >= need) {
            allocated = need;
            zoneWaterAllocated[idx] += need;
            remaining -= need;
        } else {
            allocated = remaining;
            zoneWaterAllocated[idx] += remaining;
            remaining = 0;
        }

        log << "Zone " << (idx + 1) << ": Allocated " << allocated
            << " gallons (Need: " << need << " gallons)\n";

        if (remaining <= 0)
            break;
    }

    waterSupply.available = remaining;

    cout << "\nZone-wise Water Allocation After Redistribution:\n";
    for (int i = 0; i < zones.size(); i++) {
        cout << "Zone " << i + 1 << ": Allocated " << zoneWaterAllocated[i]
             << " / " << zones[i].waterDemand << " gallons\n";
    }

    cout << "Remaining Water Supply: " << waterSupply.available << " gallons\n";
    log << "Final Remaining Water: " << waterSupply.available << " gallons\n";

    log.close();
}

void addWaterSource() {
    double newWater;
    cout << "Enter water capacity to add: ";
    cin >> newWater;

    ofstream log("add_water_source_log.txt", ios::app);
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back(); // remove trailing newline

    if (newWater <= 0) {
        cout << "Invalid input. Must be positive.\n";

        // Log the invalid input
        if (log.is_open()) {
            log << "Attempted to add invalid water capacity: " << newWater << " gallons (Rejected)\n";
            log << "-------------------------------------------------------------\n";
            log.close();
        } else {
            cerr << "Error writing to add_water_source_log.txt\n";
        }

        return;
    }

    // Update water supply
    waterSupply.total += newWater;
    waterSupply.available += newWater;
    cout << "Water supply increased by " << newWater << " gallons.\n";

    // Format timestamp: YYYY-MM-DD HH:MM:SS
    tm* localtm = localtime(&now);
    stringstream ss;
    ss << (1900 + localtm->tm_year) << "-"
       << (1 + localtm->tm_mon) << "-"
       << localtm->tm_mday << " "
       << localtm->tm_hour << ":"
       << localtm->tm_min << ":"
       << localtm->tm_sec;

    // Record to history
    waterSourceHistory.push_back({newWater, ss.str()});

    // Log valid addition
    if (log.is_open()) {
        log << "\n[Timestamp: " << timestamp << "]\n";
        log << "Water source added: " << newWater << " gallons\n";
        log.close();
    } else {
        cerr << "Error writing to add_water_source_log.txt\n";
    }

    // Auto-redistribute
    redistributeWater();
    cout << "Water source added and saved to 'add_water_source_log.txt'.\n";
}


void generateWaterUsageReport() {
    std::ofstream out("water_usage_report.txt");
    if (!out) {
        std::cout << "Error writing report file.\n";
        return;
    }

    out << std::fixed << std::setprecision(2);

    out << "=================================================================\n";
    out << "                    SMART CITY WATER USAGE REPORT                \n";
    out << "=================================================================\n\n";

    // Overall summary
    out << "TOTAL WATER SUPPLY SUMMARY\n";
    out << "--------------------------\n";
    out << "Total Water Capacity     : " << waterSupply.total << " gallons\n";

    double totalAllocated = 0, totalDemand = 0;
    for (int i = 0; i < zones.size(); ++i) {
        totalAllocated += zoneWaterAllocated[i];
        totalDemand += zones[i].waterDemand;
    }

    out << "Total Water Allocated    : " << totalAllocated << " gallons\n";
    out << "Total Water Demand       : " << totalDemand << " gallons\n";
    out << "Available Water Remaining: " << waterSupply.available << " gallons\n\n";

    // Water source additions history
    out << "WATER SOURCE ADDITIONS HISTORY\n";
    out << "------------------------------\n";
    if (waterSourceHistory.empty()) {
        out << "No water sources added yet.\n\n";
    } else {
        for (const auto& entry : waterSourceHistory) {
            out << "Added " << entry.first << " gallons on " << entry.second << "\n";
        }
        out << "\n";
    }

    // Zone-wise distribution
    out << "ZONE-WISE WATER DETAILS\n";
    out << "------------------------\n";
    for (int i = 0; i < zones.size(); ++i) {
        out << "Zone " << (i + 1) << ":\n";
        out << "  Population         : " << zones[i].population << "\n";
        out << "  Water Demand       : " << zones[i].waterDemand << " gallons\n";
        out << "  Water Allocated    : " << zoneWaterAllocated[i] << " gallons\n";

        if (zoneWaterAllocated[i] < zones[i].waterDemand) {
            double shortage = zones[i].waterDemand - zoneWaterAllocated[i];
            out << "  STATUS             : WATER STRESSED (Shortage of " << shortage << " gallons)\n";
        } else {
            out << "  STATUS             : Stable\n";
        }
        out << "\n";
    }
    out << "\n";

    // Summary notes
    out << "WATER DISTRIBUTION STRATEGY\n";
    out << "----------------------------\n";
    out << "Water distribution uses a Greedy approach prioritizing high-demand zones.\n";
    out << "Zone-wise stress status helps optimize future supply planning.\n\n";

    out << "============================================\n";
    out << "       END OF WATER USAGE REPORT            \n";
    out << "============================================\n";

    out.close();
    std::cout << "Water usage report saved to 'water_usage_report.txt'.\n";
}
void emergencyServicesMenu() {
    int option;
    do {
        cout << "\n--- Emergency Services & Simulation ---\n";
        cout << "1. Load Emergencies from File\n";
        cout << "2. Report an Emergency\n";
        cout << "3. View Active Emergency Cases\n";
        cout << "4. Dispatch Emergency Units (Greedy)\n";
        cout << "5. Predict Emergency Trends (Dynamic Programming)\n";
        cout << "6. Emergency Services Report\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> option;

        switch (option) {
            case 1: loadEmergenciesFromFile(); break;
            case 2: reportEmergency(); break;
            case 3: viewEmergencies(); break;
            case 4: dispatchUnits(); break;
            case 5: predictEmergencyTrends(); break;
            case 6: generateEmergencyReport(); break;
            case 0: cout << "Returning to main menu...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (option != 0);
}void loadEmergenciesFromFile() {
    ifstream file("emergencies_input.txt");
    ofstream log("load_emergencies_log.txt", ios::app);

    // Get timestamp
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back(); // remove trailing newline

    if (!file) {
        cout << "Error: Could not open emergencies_input.txt\n";

        // Log the error
        if (log.is_open()) {
            log << "\n[Timestamp: " << timestamp << "]\n";
            log << "ERROR: Could not open emergencies_input.txt for reading.\n";
            log << "-------------------------------------------------------------\n";
            log.close();
        }
        return;
    }

    emergencies.clear();
    string type;
    int zone, severity;

    if (log.is_open()) {
        log << "\n[Timestamp: " << timestamp << "]\n";
        log << "Loading emergencies from 'emergencies_input.txt'...\n";
    }

    while (getline(file, type) && file >> zone >> severity) {
        file.ignore(); // Skip newline after severity
        if (zone < 1 || zone > 3 || severity < 1 || severity > 10) {
            cout << "Invalid data: Type=" << type << ", Zone=" << zone << ", Severity=" << severity << "\n";

            // Log invalid entry
            if (log.is_open()) {
                log << "INVALID ENTRY: Type=" << type << ", Zone=" << zone 
                    << ", Severity=" << severity << " (Rejected)\n";
            }
            continue;
        }

        EmergencyCase e = {type, zone, severity, "Pending"};
        emergencies.push_back(e);

        // Log successful load
        if (log.is_open()) {
            log << "LOADED: Type=" << type << ", Zone=" << zone 
                << ", Severity=" << severity << ", Status=Pending\n\n";
        }
    }

    file.close();
    cout << "Emergencies loaded from file successfully.\n";

    // Finish log
    if (log.is_open()) {
        log << "Total Emergencies Loaded: " << emergencies.size() << "\n";
        log << "-------------------------------------------------------------\n";
        log.close();
    }
    cout<< "Emergency cases logged to 'load_emergencies_log.txt'.\n";
}

void reportEmergency() {
    ofstream log("report_emergency_log.txt", ios::app);

    // Get timestamp
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back(); // remove trailing newline

    EmergencyCase e;
    cout << "Enter Emergency Type (Fire/Medical/Crime): ";
    cin >> ws;
    getline(cin, e.type);
    cout << "Enter Zone (1-3): ";
    cin >> e.zone;
    cout << "Enter Severity (1-10): ";
    cin >> e.severity;
    e.status = "Pending";

    // Start logging
    if (log.is_open()) {
        log << "\n[Timestamp: " << timestamp << "]\n";
        log << "Attempting to report emergency...\n";
    }

    // Validate input
    if (e.zone < 1 || e.zone > 3 || e.severity < 1 || e.severity > 10) {
        cout << "Invalid zone or severity. Emergency not recorded.\n";

        if (log.is_open()) {
            log << "ERROR: Invalid input. Zone=" << e.zone 
                << ", Severity=" << e.severity << " (Rejected)\n";
            log << "-------------------------------------------------------------\n";
            log.close();
        }
        return;
    }

    emergencies.push_back(e);
    cout << "Emergency reported.\n";

    if (log.is_open()) {
        log << "REPORTED: Type=" << e.type 
            << ", Zone=" << e.zone 
            << ", Severity=" << e.severity 
            << ", Status=Pending\n";
        log << "-------------------------------------------------------------\n";
        log.close();
    }
    cout << "Emergency reported and logged to 'report_emergency_log.txt'.\n";
}
void viewEmergencies() {
    cout << "\n--- Active Emergencies ---\n";

    ofstream log("view_emergencies_log.txt", ios::app);
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back(); // remove newline

    if (log.is_open()) {
        log << "\n[Timestamp: " << timestamp << "]\n";
        log << "--- Viewing Active Emergencies ---\n";
    }

    if (emergencies.empty()) {
        cout << "No active emergencies.\n";
        if (log.is_open()) {
            log << "No active emergencies.\n";
            log << "-------------------------------------------------------------\n";
            log.close();
        }
        return;
    }

    for (const EmergencyCase& e : emergencies) {
        cout << "Type: " << e.type << ", Zone: " << e.zone
             << ", Severity: " << e.severity << ", Status: " << e.status << "\n";

        if (log.is_open()) {
            log << "Type: " << e.type << ", Zone: " << e.zone
                << ", Severity: " << e.severity << ", Status: " << e.status << "\n\n";
        }
    }

    if (log.is_open()) {
        log << "-------------------------------------------------------------\n";
        log.close();
    }
    cout << "Active emergencies logged to 'view_emergencies_log.txt'.\n";
}
void dispatchUnits() {
    cout << "\n--- Dispatching Emergency Units ---\n";

    ofstream log("dispatch_units_log.txt", ios::app);
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back(); // remove trailing newline

    if (log.is_open()) {
        log << "\n[Timestamp: " << timestamp << "]\n";
        log << "--- Dispatching Emergency Units ---\n";
    }

    // Sort emergencies by severity (descending)
    sort(emergencies.begin(), emergencies.end(), [](EmergencyCase a, EmergencyCase b) {
        return a.severity > b.severity;  // Greedy: highest severity first
    });

    bool anyDispatched = false;
    for (EmergencyCase& e : emergencies) {
        if (e.status == "Pending" && emergencyUnits[e.zone - 1] > 0) {
            emergencyUnits[e.zone - 1]--;
            e.status = "Dispatched";
            cout << "Unit dispatched to Zone " << e.zone << " for " << e.type << "\n";

            if (log.is_open()) {
                log << "Dispatched unit to Zone " << e.zone << " for " << e.type << " (Severity: " << e.severity << ")\n\n";
            }
            anyDispatched = true;
        }
    }

    if (!anyDispatched) {
        cout << "No units available to dispatch or no pending emergencies.\n";
        if (log.is_open()) {
            log << "No units available to dispatch or no pending emergencies.\n";
        }
    }

    if (log.is_open()) {
        log << "-------------------------------------------------------------\n";
        log.close();
    }
    cout << "Emergency dispatch logged to 'dispatch_units_log.txt'.\n";
}

void predictEmergencyTrends() {
    cout << "\n--- Predicting Emergency Trends (Dynamic Programming) ---\n";

    // Step 1: Create a 2D DP table: dp[zone][type]
    map<string, int> typeIndex = {{"Fire", 0}, {"Medical", 1}, {"Crime", 2}};
    int dp[4][3] = {};  // dp[zone][type]: zones 1-3 and 3 types

    // Step 2: Read emergency history from file (3 lines per entry)
    ifstream file("emergencies_input.txt");
    if (!file) {
        cout << "Error: Could not open emergencies_input.txt\n";
        return;
    }

    string type;
    int zone, severity;

    while (getline(file, type)) {
        string zoneStr, severityStr;
        getline(file, zoneStr);
        getline(file, severityStr);

        zone = stoi(zoneStr);
        severity = stoi(severityStr);

        if (zone < 1 || zone > 3 || typeIndex.find(type) == typeIndex.end()) continue;
        dp[zone][typeIndex[type]]++;
    }
    file.close();

    // Step 3: Display prediction summary
    string emergencyTypes[3] = {"Fire", "Medical", "Crime"};
    for (int z = 1; z <= 3; ++z) {
        cout << "Zone " << z << " Emergency History:\n";
        for (int t = 0; t < 3; ++t) {
            cout << "  " << emergencyTypes[t] << ": " << dp[z][t] << " cases\n";
        }
    }

    // Step 4: Predict future trend: zone with highest total emergencies
    int maxEmergencies = 0, riskyZone = -1;
    for (int z = 1; z <= 3; ++z) {
        int total = dp[z][0] + dp[z][1] + dp[z][2];
        if (total > maxEmergencies) {
            maxEmergencies = total;
            riskyZone = z;
        }
    }

    if (riskyZone != -1) {
        cout << "\nPrediction: Zone " << riskyZone << " is at highest risk based on historical data.\n";
    } else {
        cout << "\nNo sufficient data available for prediction.\n";
    }
ofstream report("predict_emergency_report.txt");


    report << "=================================================================\n";
    report << "                    SMART CITY EMERGENCY TREND PREDICTION REPORT                 \n";
    report << "=================================================================\n\n";

report << "--- EMERGENCY HISTORY SUMMARY ---\n\n";

for (int z = 1; z <= 3; ++z) {
    report << "Zone " << z << " Emergency History:\n";
    for (int t = 0; t < 3; ++t) {
        report << "  " << emergencyTypes[t] << ": " << dp[z][t] << " cases\n";
    }
    report << "\n";
}

report << "--- Prediction Analysis ---\n\n";
int totals[4] = {};
for (int z = 1; z <= 3; ++z) {
    totals[z] = dp[z][0] + dp[z][1] + dp[z][2];
    report << "  Zone " << z << ": " << totals[z] << " emergencies\n";
}

if (totals[1] == totals[2] && totals[2] == totals[3]) {
    report << "\nAll zones have equal emergency occurrence. No single zone is at higher risk.\n";
} else {
    int maxEmergencies = 0, riskyZone = -1;
    for (int z = 1; z <= 3; ++z) {
        if (totals[z] > maxEmergencies) {
            maxEmergencies = totals[z];
            riskyZone = z;
        }
    }
    report << "\nPrediction: Zone " << riskyZone << " is at highest risk based on historical data.\n";
    cout << "Report saved to 'predict_emergency_report.txt'\n";
}

report << "\nConclusion:\nBased on historical data from 'emergencies_input.txt', the emergency distribution is ";
if (totals[1] == totals[2] && totals[2] == totals[3])
    report << "balanced across all zones. No urgent red flags detected at this stage. Continue monitoring for new trends.\n";
else
    report << "skewed toward Zone " << riskyZone << ", requiring immediate resource attention.\n";

report << "\n===================================================\n";
report << "GENERATED BY SMART CITY RESOURCE MANAGEMENT SYSTEM\n";
report << "(Algorithm Used: Dynamic Programming for historical pattern analysis)\n";
report.close();

}

void generateEmergencyReport() {
    ofstream file("emergency_report.txt");
    if (!file) {
        cout << "Error writing report.\n";
        return;
    }

    // Header
   
    file << "========================================================================\n";
    file << "                  SMART CITY EMERGENCY SERVICES REPORT                 \n";
    file << "========================================================================\n\n";

    file << "Report Generated: " << __DATE__ << " at " << __TIME__ << "\n";
    file << "Module: Emergency Services & Simulation\n";
    file << "Algorithms Used: Greedy (Dispatch), Dynamic Programming (Prediction)\n";
    file << "------------------------------------------------------------------\n\n";

    // Section 1: Emergency Case Log
    file << "[1] Complete Emergency Case Log:\n";
    int totalEmergencies = 0;
    map<string, int> typeCounts;
    map<string, int> resolvedCounts;
    map<int, int> zoneCounts;
    map<string, int> severityLevels;

    for (const EmergencyCase& e : emergencies) {
        file << "  • Type: " << e.type
             << ", Zone: " << e.zone
             << ", Severity: " << e.severity
             << ", Status: " << e.status << "\n";

        totalEmergencies++;
        typeCounts[e.type]++;
        severityLevels[e.type] += e.severity;
        zoneCounts[e.zone]++;
        if (e.status == "Dispatched") resolvedCounts[e.type]++;
    }

    if (totalEmergencies == 0) {
        file << "  (No emergencies recorded yet.)\n";
    }

    file << "\n------------------------------------------------------------------\n";

    // Section 2: Summary Statistics
    file << "[2] Summary Statistics:\n";
    file << "  → Total Emergencies: " << totalEmergencies << "\n";
    file << "  → Emergencies by Type:\n";
    for (auto& pair : typeCounts) {
        file << "     - " << pair.first << ": " << pair.second
             << " (Resolved: " << resolvedCounts[pair.first]
             << ", Avg. Severity: " << (pair.second ? (severityLevels[pair.first] / pair.second) : 0) << ")\n";
    }

    file << "  → Zone-wise Emergency Distribution:\n";
    for (auto& z : zoneCounts) {
        file << "     - Zone " << z.first << ": " << z.second << " cases\n";
    }

    file << "\n------------------------------------------------------------------\n";

    // Section 3: Emergency Trend Prediction (from DP)
    file << "[3] Emergency Trend Prediction (using Dynamic Programming):\n";
    int dp[4][3] = {};  // zones 1-3, types: Fire, Medical, Crime
    map<string, int> typeIndex = {{"Fire", 0}, {"Medical", 1}, {"Crime", 2}};
    for (const EmergencyCase& e : emergencies) {
        if (e.zone >= 1 && e.zone <= 3 && typeIndex.count(e.type)) {
            dp[e.zone][typeIndex[e.type]]++;
        }
    }

    for (int z = 1; z <= 3; ++z) {
        file << "  Zone " << z << " Emergency History:\n";
        file << "     - Fire: " << dp[z][0] << " cases\n";
        file << "     - Medical: " << dp[z][1] << " cases\n";
        file << "     - Crime: " << dp[z][2] << " cases\n";
    }

    int maxZone = 1, maxCases = dp[1][0] + dp[1][1] + dp[1][2];
    for (int z = 2; z <= 3; ++z) {
        int total = dp[z][0] + dp[z][1] + dp[z][2];
        if (total > maxCases) {
            maxCases = total;
            maxZone = z;
        }
    }

    file << "\n  → Prediction: Zone " << maxZone << " is at highest risk.\n";

    file << "\n------------------------------------------------------------------\n";

    // Section 4: Dispatch Effectiveness (Greedy)
    file << "[4] Dispatch Effectiveness (using Greedy Algorithm):\n";
    int dispatched = 0, pending = 0;
    for (const EmergencyCase& e : emergencies) {
        if (e.status == "Dispatched") dispatched++;
        else pending++;
    }
    file << "  → Total Dispatched Cases: " << dispatched << "\n";
    file << "  → Pending / Unresolved Cases: " << pending << "\n";
    file << "  → Dispatch Success Rate: "
         << (totalEmergencies > 0 ? (dispatched * 100 / totalEmergencies) : 0) << "%\n";

    file << "\n------------------------------------------------------------------\n";

    // Section 5: Zone-wise Load Distribution
    file << "[5] Zone-wise Emergency Load Analysis:\n";
    for (int z = 1; z <= 3; ++z) {
        int count = zoneCounts[z];
        string status = (count > 5) ? "⚠️ Overloaded" : "✅ Normal Load";
        file << "  - Zone " << z << ": " << count << " emergencies → " << status << "\n";
    }

    file << "\n------------------------------------------------------------------\n";

    // Section 6: Critical Observations & Suggestions
    file << "[6] Observations & Actionable Insights:\n";

    if (pending > 0) {
        file << "  • Some emergencies remain unresolved. Investigate reasons and increase response units.\n";
    }
    if (maxCases >= 5) {
        file << "  • High emergency volume in Zone " << maxZone << ". Recommend resource reinforcement.\n";
    } else {
        file << "  • Emergency volume is within manageable limits.\n";
    }

    file << "  • Ensure continuous data collection to improve predictions.\n";
    file << "  • Consider dynamic allocation of emergency vehicles based on recent trends.\n";

    file << "\n------------------------------------------------------------------\n";

    // Footer
    file << "✔ Report generated successfully by Smart City Emergency Management Module.\n";
    file << "===============================================================\n";

    file.close();
    cout << "Detailed Emergency Report saved to 'emergency_report.txt'\n";
}
void cityAnalyticsMenu() {
    int option;
    do {
        cout << "\n--- City Analytics & Dashboard ---\n";
        cout << "1. View Overloaded Resource Zones\n";
        cout << "2. View Citizen Distribution\n";
        cout << "3. View Emergency Load per Zone\n";
        cout << "4. View Overall City Health Summary\n";
        cout << "5. Generate & Save Analytics Report\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> option;

        switch (option) {
            case 1: viewOverloadedZones(); break;
            case 2: viewCitizenDistribution(); break;
            case 3: viewEmergencyLoad(); break;
            case 4: viewCityHealthSummary(); break;
            case 5: generateCityAnalyticsReport(); break;
            case 0: break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (option != 0);
}
void viewOverloadedZones() {
    cout << "\n--- Overloaded Resource Zones ---\n";

    ofstream log("overloaded_zones_log.txt", ios::app);
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back(); // remove trailing newline

    if (log.is_open()) {
        log << "\n[Timestamp: " << timestamp << "]\n";
        log << "--- Overloaded Resource Zones ---\n";
    }

    bool found = false;
    for (int i = 0; i < zones.size(); ++i) {
        bool overloaded = false;
        stringstream zoneLog;

        if (zones[i].powerDemand > 20) {
            overloaded = true;
            zoneLog << "[Power Demand: " << zones[i].powerDemand << "] ";
        }
        if (zones[i].waterDemand > 20) {
            overloaded = true;
            zoneLog << "[Water Demand: " << zones[i].waterDemand << "] ";
        }
        if (zones[i].wasteLevel > 30) {
            overloaded = true;
            zoneLog << "[Waste Level: " << zones[i].wasteLevel << "] ";
        }

        if (overloaded) {
            cout << "Zone " << (i + 1) << " - " << zoneLog.str() << "\n";
            if (log.is_open()) {
                log << "Zone " << (i + 1) << " - " << zoneLog.str() << "\n";
            }
            found = true;
        }
    }

    if (!found) {
        cout << " All zones operating within safe resource levels.\n";
        if (log.is_open()) {
            log << "All zones operating within safe resource levels.\n";
        }
    }

    if (log.is_open()) {
        log << "----------------------------------------------------\n";
        log.close();
    } else {
        cerr << "Error writing to overloaded_zones_log.txt\n";
    }
    cout << "Overloaded zones logged to 'overloaded_zones_log.txt'.\n";
}

void viewCitizenDistribution() {
    cout << "\n--- Citizen Distribution by Zone ---\n";

    ofstream log("citizen_distribution_log.txt", ios::app);
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back();  // Remove trailing newline

    if (log.is_open()) {
        log << "\n[Timestamp: " << timestamp << "]\n";
        log << "--- Citizen Distribution by Zone ---\n";
    }

    int total = 0;
    for (const auto& z : zones) total += z.population;

    if (total == 0) {
        cout << "No citizens registered yet.\n";
        if (log.is_open()) {
            log << "No citizens registered yet.\n";
            log << "----------------------------------------------------\n";
            log.close();
        } else {
            cerr << "Error writing to citizen_distribution_log.txt\n";
        }
        return;
    }

    for (int i = 0; i < zones.size(); ++i) {
        double percent = (zones[i].population * 100.0) / total;
        cout << "Zone " << (i + 1) << ": " << zones[i].population << " citizens ("
             << fixed << setprecision(2) << percent << "%)\n";

        if (log.is_open()) {
            log << "Zone " << (i + 1) << ": " << zones[i].population << " citizens ("
                << fixed << setprecision(2) << percent << "%)\n";
        }
    }

    cout << "Total Citizens: " << total << "\n";
    if (log.is_open()) {
        log << "Total Citizens: " << total << "\n";
        log << "----------------------------------------------------\n";
        log.close();
    } else {
        cerr << "Error writing to citizen_distribution_log.txt\n";
    }
    cout << "Citizen distribution logged to 'citizen_distribution_log.txt'.\n";
}
void viewEmergencyLoad() {
    cout << "\n--- Emergency Load by Zone ---\n";

    ofstream log("emergency_load_log.txt", ios::app);
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back();  // Remove trailing newline

    if (log.is_open()) {
        log << "\n[Timestamp: " << timestamp << "]\n";
        log << "--- Emergency Load by Zone ---\n";
    }

    vector<int> fireCount(4, 0), medicalCount(4, 0), crimeCount(4, 0);

    for (const auto& e : emergencies) {
        if (e.type == "Fire") fireCount[e.zone]++;
        else if (e.type == "Medical") medicalCount[e.zone]++;
        else if (e.type == "Crime") crimeCount[e.zone]++;
    }

    for (int i = 1; i <= 3; ++i) {
        cout << "Zone " << i << ":\n";
        cout << "  Fire Emergencies   : " << fireCount[i] << "\n";
        cout << "  Medical Emergencies: " << medicalCount[i] << "\n";
        cout << "  Crime Emergencies  : " << crimeCount[i] << "\n";

        if (log.is_open()) {
            log << "Zone " << i << ":\n";
            log << "  Fire Emergencies   : " << fireCount[i] << "\n";
            log << "  Medical Emergencies: " << medicalCount[i] << "\n";
            log << "  Crime Emergencies  : " << crimeCount[i] << "\n";
        }
    }

    if (log.is_open()) {
        log << "----------------------------------------------------\n";
        log.close();
    } else {
        cerr << "Error writing to emergency_load_log.txt\n";
    }
    cout << "Emergency load logged to 'emergency_load_log.txt'.\n";
}


// Main Waste Management Menu
void wasteManagementMenu() {
    int choice;
    do {
        cout << "\n--- Waste Management Module ---\n";
        cout << "1. View Current Waste Levels\n";
        cout << "2. Schedule Waste Collection (Greedy)\n";
        cout << "3. View Max Waste Zone (Divide & Conquer)\n";
        cout << "4. Predict Overflow Days (DP)\n";
        cout << "5. Generate Waste Summary Report (to txt)\n";
        cout << "0. Exit Waste Management Module\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewWasteLevels(); break;
            case 2: scheduleWasteCollection(); break;
            case 3: viewCriticalWasteZoneUsingDivideAndConquer(); break;
            case 4: predictWasteOverflowDaysDP(); break;
            case 5: generateWasteManagementReport(); break;
            case 0: cout << "Exiting Waste Management Module...\n"; break;
            default: cout << "Invalid option. Try again.\n";
        }
    } while (choice != 0);
}
// 1. View Current Waste Levels
void viewWasteLevels() {
    cout << "\n--- Current Waste Levels in All Zones ---\n";

    // Open log file in append mode
    ofstream log("waste_level_log.txt", ios::app);

    // Get current timestamp
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back(); // Remove newline

    if (log.is_open()) {
        log << "\n[Timestamp: " << timestamp << "]\n";
        log << "--- Viewing Waste Levels ---\n";
    }

    for (int i = 0; i < zones.size(); ++i) {
        cout << "Zone " << i + 1 << ": " << zones[i].wasteLevel << "% waste level\n";

        if (log.is_open()) {
            log << "Zone " << i + 1 << ": " << zones[i].wasteLevel << "% waste level\n";
        }
    }

    if (log.is_open()) {
        log << "--------------------------------------------------\n";
        log.close();
    }
    cout << "Waste levels logged to 'waste_level_log.txt'.\n";
}
void scheduleWasteCollection() {
    vector<pair<double, int>> wasteZones;
    for (int i = 0; i < zones.size(); ++i) {
        if (zones[i].wasteLevel > 50) {
            wasteZones.push_back({zones[i].wasteLevel, i});
        }
    }

    // Sort zones by highest waste level first (Greedy)
    sort(wasteZones.rbegin(), wasteZones.rend());

    cout << "\n--- Scheduled Waste Collection (Greedy Priority) ---\n";

    // Open log file
    ofstream log("waste_collection_log.txt", ios::app);
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back(); // remove trailing newline

    if (log.is_open()) {
        log << "\n[Timestamp: " << timestamp << "]\n";
        log << "--- Scheduled Waste Collection (Greedy Priority) ---\n";
    }

    for (auto &zone : wasteZones) {
        int i = zone.second;
        cout << "Zone " << i + 1 << " with Waste Level: " << zones[i].wasteLevel
             << "% --> Scheduled for collection.\n";

        if (log.is_open()) {
            log << "Zone " << i + 1 << " with Waste Level: " << zones[i].wasteLevel
                << "% --> Scheduled for collection.\n";
        }
    }

    if (wasteZones.empty()) {
        cout << "No zones exceed the 50% waste level threshold.\n";
        if (log.is_open()) {
            log << "No zones exceed the 50% waste level threshold.\n";
        }
    }

    if (log.is_open()) {
        log << "------------------------------------------------------\n";
        log.close();
    }
    cout<< "Waste collection schedule logged to 'waste_collection_log.txt'.\n";
}

int findMaxWasteZone(int left, int right) {
    if (left == right)
        return left;

    int mid = (left + right) / 2;
    int leftMax = findMaxWasteZone(left, mid);
    int rightMax = findMaxWasteZone(mid + 1, right);

    return (zones[leftMax].wasteLevel > zones[rightMax].wasteLevel) ? leftMax : rightMax;
}

void viewCriticalWasteZoneUsingDivideAndConquer() {
    if (zones.empty()) {
        cout << "\nNo zones available to analyze.\n";
        return;
    }

    int idx = findMaxWasteZone(0, zones.size() - 1);

    cout << "\n--- Zone with Highest Waste Level (Divide & Conquer) ---\n";
    cout << "Zone " << idx + 1 << " --> " << zones[idx].wasteLevel << "% Waste\n";

    // Logging
    ofstream log("critical_waste_log.txt", ios::app);
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back(); // Remove newline

    if (log.is_open()) {
        log << "\n[Timestamp: " << timestamp << "]\n";
        log << "--- Zone with Highest Waste Level (Divide & Conquer) ---\n";
        log << "Zone " << idx + 1 << " --> " << zones[idx].wasteLevel << "% Waste\n";
        log << "--------------------------------------------------------\n";
        log.close();
    }
    cout << "Critical waste zone logged to 'critical_waste_log.txt'.\n";
}

// 4. Dynamic Programming: Predict Overflow Days
void predictWasteOverflowDaysDP() {
    vector<int> days(zones.size(), INT_MAX);
    const double dailyIncrease = 5.0;
    const double maxLimit = 100.0;

    for (int i = 0; i < zones.size(); ++i) {
        double current = zones[i].wasteLevel;
        int d = 0;
        while (current < maxLimit) {
            d++;
            current += dailyIncrease;
        }
        days[i] = d;
    }

    cout << "\n--- Days Before Waste Overflows in Each Zone (DP Prediction) ---\n";
    
    // Logging
    ofstream log("waste_overflow_prediction_log.txt", ios::app);
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back(); // Remove newline

    if (log.is_open()) {
        log << "\n[Timestamp: " << timestamp << "]\n";
        log << "--- Waste Overflow Prediction (Dynamic Programming) ---\n";
    }

    for (int i = 0; i < days.size(); ++i) {
        cout << "Zone " << i + 1 << ": " << days[i] << " day(s) until overflow\n";
        if (log.is_open()) {
            log << "Zone " << i + 1 << ": " << days[i] << " day(s) until overflow\n";
        }
    }

    if (log.is_open()) {
        log << "------------------------------------------------------\n";
        log.close();
    }
    cout << "Waste overflow prediction logged to 'waste_overflow_prediction_log.txt'.\n";
}

// 5. Summary Report (Printable)
void generateWasteManagementReport() {
    ofstream report("waste_report.txt");
    if (!report) {
        cout << "Error writing to waste_report.txt\n";
        return;
    }

    report << "--- Waste Management Summary Report ---\n\n";
    const double maxCapacity = 100.0;
    const double dailyWasteIncrease = 5.0;
    const double collectionThreshold = 60.0;

    for (int i = 0; i < zones.size(); ++i) {
        double currentWaste = zones[i].wasteLevel;

        // Predict days until overflow
        int overflowDays = 0;
        double temp = currentWaste;
        while (temp < maxCapacity) {
            temp += dailyWasteIncrease;
            overflowDays++;
        }

        // Determine waste truck size recommendation
        string truckSize;
        if (currentWaste < 40) truckSize = "Small";
        else if (currentWaste <= 80) truckSize = "Medium";
        else truckSize = "Large";

        // Risk status
        string risk = (currentWaste >= 80) ? "HIGH" :
                      (currentWaste >= 60) ? "MODERATE" : "LOW";

        // Collection frequency suggestion
        string frequency;
        if (currentWaste >= 90)
            frequency = "Daily";
        else if (currentWaste >= 70)
            frequency = "Every 2 Days";
        else
            frequency = "Twice a Week";

        report << "Zone " << i+1 << ":\n";
        report << "  Waste Level: " << currentWaste << "%\n";
        report << "  Overflow Risk: " << risk << "\n";
        report << "  Estimated Days Before Overflow: " << overflowDays << " day(s)\n";
        report << "  Recommended Truck Size: " << truckSize << "\n";
        report << "  Suggested Collection Frequency: " << frequency << "\n";
        report << "--------------------------------------------------\n";
    }

    report << "\nNote: Overflow prediction based on current trends and 5% daily waste rise.\n";
    report.close();

    cout << "\n Report generated: waste_report.txt\n";
}


void viewCityHealthSummary() {
    cout << "\n--- Overall City Health Summary ---\n";

    // Open log file
    ofstream log("city_health_summary_log.txt", ios::app);

    // Timestamp
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back(); // Remove trailing newline

    if (log.is_open()) {
        log << "\n[Timestamp: " << timestamp << "]\n";
        log << "--- Overall City Health Summary ---\n";
    }

    int overloaded = 0;
    int totalPopulation = 0;

    for (int i = 0; i < zones.size(); ++i) {
        totalPopulation += zones[i].population;
        if (zones[i].powerDemand > 20 || zones[i].waterDemand > 20 || zones[i].wasteLevel > 30)
            ++overloaded;
    }

    int normal = zones.size() - overloaded;
    if (normal < 0) normal = 0;

    cout << "Total Zones             : " << zones.size() << "\n";
    cout << "Total Population        : " << totalPopulation << "\n";
    cout << "Overloaded Zones        : " << overloaded << "\n";
    cout << "Normal Operating Zones  : " << normal << "\n";

    if (log.is_open()) {
        log << "Total Zones             : " << zones.size() << "\n";
        log << "Total Population        : " << totalPopulation << "\n";
        log << "Overloaded Zones        : " << overloaded << "\n";
        log << "Normal Operating Zones  : " << normal << "\n";
        log << "--------------------------------------------------\n";
        log.close();
    }
    cout << "City health summary logged to 'city_health_summary_log.txt'.\n";
}

void generateCityAnalyticsReport() {
    ofstream report("city_analytics_report.txt");
    if (!report) {
        cout << "Error writing city analytics report file.\n";
        return;
    }

    report << "========== City Analytics Summary Report ==========\n\n";

    int totalPopulation = 0, overloadedZones = 0;
    double totalPower = 0.0, totalWater = 0.0, totalWaste = 0.0;
    double maxPower = 0.0, maxWater = 0.0, maxWaste = 0.0;

    // Emergency stats per zone
    vector<int> fireCount(zones.size(), 0);
    vector<int> medicalCount(zones.size(), 0);
    vector<int> crimeCount(zones.size(), 0);

    for (const EmergencyCase& e : emergencies) {
        if (e.zone >= 1 && e.zone <= zones.size()) {
            int idx = e.zone - 1;
            if (e.type == "Fire") fireCount[idx]++;
            else if (e.type == "Medical") medicalCount[idx]++;
            else if (e.type == "Crime") crimeCount[idx]++;
        }
    }

    for (int i = 0; i < zones.size(); ++i) {
        totalPopulation += zones[i].population;
        totalPower += zones[i].powerDemand;
        totalWater += zones[i].waterDemand;
        totalWaste += zones[i].wasteLevel;

        maxPower = max(maxPower, zones[i].powerDemand);
        maxWater = max(maxWater, zones[i].waterDemand);
        maxWaste = max(maxWaste, zones[i].wasteLevel);

        report << "Zone " << (i + 1) << ":\n";
        report << "  Population         : " << zones[i].population << "\n";
        report << "  Power Demand       : " << zones[i].powerDemand << " MW\n";
        report << "  Water Demand       : " << zones[i].waterDemand << " kL/day\n";
        report << "  Waste Level        : " << zones[i].wasteLevel << " tons/day\n";
        report << "  Emergencies        :\n";
        report << "    - Fire           : " << fireCount[i] << " cases\n";
        report << "    - Medical        : " << medicalCount[i] << " cases\n";
        report << "    - Crime          : " << crimeCount[i] << " cases\n";

        if (zones[i].powerDemand > 20 || zones[i].waterDemand > 20 || zones[i].wasteLevel > 30) {
            report << "  [ALERT] Status     : OVERLOADED RESOURCES ⚠️\n";
            overloadedZones++;
        } else {
            report << "  Status             : Normal ✅\n";
        }

        report << "---------------------------------------------------\n";
    }

    int normalZones = zones.size() - overloadedZones;
    if (normalZones < 0) normalZones = 0;

    // Overall Emergency Totals
    int totalFires = accumulate(fireCount.begin(), fireCount.end(), 0);
    int totalMedical = accumulate(medicalCount.begin(), medicalCount.end(), 0);
    int totalCrimes = accumulate(crimeCount.begin(), crimeCount.end(), 0);

    report << "\n------------ City Summary ------------\n";
    report << "Total Zones              : " << zones.size() << "\n";
    report << "Total Population         : " << totalPopulation << "\n";
    report << "Average Power Demand     : " << (zones.empty() ? 0 : totalPower / zones.size()) << " MW\n";
    report << "Average Water Demand     : " << (zones.empty() ? 0 : totalWater / zones.size()) << " kL/day\n";
    report << "Average Waste Generation : " << (zones.empty() ? 0 : totalWaste / zones.size()) << " tons/day\n";
    report << "Max Power Demand         : " << maxPower << " MW\n";
    report << "Max Water Demand         : " << maxWater << " kL/day\n";
    report << "Max Waste Level          : " << maxWaste << " tons/day\n";
    report << "Overloaded Zones         : " << overloadedZones << "\n";
    report << "Normal Zones             : " << normalZones << "\n";

    report << "\n------------ Emergency Summary ------------\n";
    report << "Total Fire Cases         : " << totalFires << "\n";
    report << "Total Medical Cases      : " << totalMedical << "\n";
    report << "Total Crime Cases        : " << totalCrimes << "\n";
    report << "Total Emergency Reports  : " << emergencies.size() << "\n";

    report << "\nNote: This report aggregates power, water, waste, and emergency statistics across all zones.\n";
    report << "It is intended to assist with urban planning, resource allocation, and public safety response optimization.\n";

    report.close();
    cout << "Report saved to 'city_analytics_report.txt'\n";
}
