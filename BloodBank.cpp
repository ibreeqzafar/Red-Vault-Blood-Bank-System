#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
using namespace std;

struct Node { //Linked List Node
    string bloodGroup;
    int units;
    int year;
    int month;
    int day;
    Node *next;
};

struct QueueNode {
    string name ;     // patient name
    string blood ;    // blood type
    int age ;         // patient age
    QueueNode* next ;   // pointer to next request in queue
};

struct BSTNode {
    string name; string bloodGroup;
    int id; int age;
    float weight;
    bool hasDisease;
    double cnic;
    int monthsSinceLastDonation;
    BSTNode* left;
    BSTNode* right;
};

struct StackNode {
    string patientName;
    string bloodType;
    int units;
    int year;
    int month;
    int day;
    StackNode* next;
};

class BloodBank {
public:

    Node* head;
    QueueNode* front;
    QueueNode* rear;
    BSTNode* root;
    StackNode* top;
    int nextId;

    BloodBank() { //constructor
        head = nullptr;
        front = rear = nullptr;
        root = nullptr;
        top = nullptr;
        nextId = 1;
    }

    //-------------------------LINKED LIST--------------------------------
    
    void addBloodUnit() { //add blood unit function
        cout << "----- ADD BLOOD UNIT -----" << endl;
        cout << "--------------------------" << endl;
        int y, m, d;
        string bG;
        int qty;

        cout << "Enter Collection Year: ";
        cin >> y;
        cout << "Enter Collection Month: ";
        cin >> m;
        cout << "Enter Collection Day: ";
        cin >> d;

        time_t now = time(0); //current time
        tm *ltm = localtime(&now); //convert to local time

        int cday = ltm->tm_mday;
        int cmonth = 1 + ltm->tm_mon; //we added 1 because tm_mon stores january as 0, and so on
        int cyear = 1900 + ltm->tm_year; //tm_year stores years passed since 1900, (it stores 126 for the year 2026), so we added 1900

        //basic range check
        if (m < 1 || m > 12 || d < 1 || d > 31) {
            cout << "Please enter a valid date." << endl;
            return;
        }

        //future date check
        if (y > cyear ||
        (y == cyear && m > cmonth) ||
        (y == cyear && m == cmonth && d > cday)) {

            cout << "Collection date cannot be in the future." << endl;
            return;
        }

        cout << "Enter Blood Group: ";
        cin >> bG;
        cout << "Enter Quantity: ";
        cin >> qty;
        cout << "NOTE: All Blood Groups get expired after 1 month.";

        Node *newNode = new Node();
        newNode->bloodGroup = bG;
        newNode->units = qty;
        newNode->year = y;
        newNode->month = m;
        newNode->day = d;
        newNode->next = nullptr;

        //if inventory is empty
        if (head == nullptr) {
            head = newNode; //newNode becomes head
        }
        else {
            //make temp pointer
            Node *temp = head;         //temp points to head
            while (temp->next != nullptr) //traverse the list until reaches nullptr
            {
                temp = temp->next; //move to next
            }
            temp->next = newNode;
        }
        cout << "Blood unit added successfully in the inventory!" << endl;
    }

    void viewBloodInventory() { //display blood inventory fucntion
        cout << "All expired blood is first remove before checking it.";
        deleteExpiredBlood();
        cout << "----- BLOOD INVENTORY -----" << endl;
        cout << "---------------------------" << endl;

        if (head == nullptr) {
            cout << "The inventory is empty." << endl;
            return;
        }
        else {
            Node *temp = head;   //temp points to head
            while (temp != nullptr) { //traverse until reaches nullptr
                cout << "Blood Group: " << left << setw(10) << temp->bloodGroup << "Units: " << temp->units << "     Date:" << temp->day << "/" << temp->month << "/" << temp->year << endl;
                temp = temp->next; //move to next
            }
        }
    }

    Node* searchBloodGroup() { //search blood group function
        cout<<"All expired blood is first remove before checking it.";
        deleteExpiredBlood();
        string bg;
        cout << "Enter blood group to search: ";
        cin >> bg;

        if (head == nullptr) {
            cout << "Inventory is empty!" << endl;
            return nullptr;
        }
        Node *temp = head;
        Node *firstMatch = nullptr;
        bool found = false;

        while (temp != nullptr) { //traverse until reaches nullptr5

            if (temp->bloodGroup == bg && temp->units > 0) {

                if (firstMatch == nullptr)
                    firstMatch = temp;   //store first match

                cout << "Blood Group: " << temp->bloodGroup
                     << " | Units: " << temp->units
                     << " | Date: " << temp->day << "/"
                     << temp->month << "/" << temp->year << endl;
                found = true;
            }

            temp = temp->next;
        }

        if (!found) {
            cout << "BLOOD GROUP NOT FOUND." << endl;
            return nullptr;
        }

        return firstMatch;
    }

    void checkLowStock() { //check low stock fucntion
        cout<<"All expired blood is first remove before checking it.";
        deleteExpiredBlood();
        if (head == nullptr) { //when inventory is empty
            cout << "Inventory is empty." << endl;
            return;
        }

        bool found = false;
        Node *temp = head;

        while (temp != nullptr) { //traverse until reaches nullptr
            if (temp->units < 3) {
                cout << "BLOOD GROUP: " << temp->bloodGroup << " UNITS: " << temp->units << " - Stock is LOW!" << endl;
                found = true;
            }
            temp = temp->next; //move to next
        }

        if (!found) //if blood group not found
            cout << "No Blood Group is low in stock!" << endl;
    }

    void deleteExpiredBlood() { //delete expire blood function
        if (head == nullptr) {
            cout << "Inventory is empty." << endl;
            return;
        }

        time_t now = time(0); //current time
        tm *ltm = localtime(&now); //convert to local time

        int cday = ltm->tm_mday;
        int cmonth = 1 + ltm->tm_mon;
        int cyear = 1900 + ltm->tm_year;

        Node *temp = head;
        Node *prev = nullptr;
        bool deleted = false;
        int count=0;

        while (temp != nullptr) {
            bool isexpired = false;
            int currentDays = cyear * 365 + cmonth * 30 + cday; //total days in current year
            int bloodDays   = temp->year * 365 + temp->month * 30 + temp->day; //total days since this blood unit was received
            if (currentDays - bloodDays > 30) //condition for expiry (assuming blood expires in 30 days)
                isexpired = true;

            if (isexpired) {
                Node *todelete = temp;

                
                if (temp == head) { //if expired node is head
                    head = head->next;
                    temp = head;
                }
                else {
                    //if expired node is middle or last
                    prev->next = temp->next;
                    temp = temp->next;
                }
                delete todelete; //removes the node from memory
                deleted = true;
                count++;
            }
            else {
                //move to next node if not expired
                prev = temp;
                temp = temp->next;
            }
        }
        if (deleted)
            cout << count << " expired blood units are deleted successfully!" << endl;
        else
            cout << "No expired blood units found." << endl;
    }
    
    //--------------------------END OF LINKED LIST------------------------

    //--------------------------QUEUE-------------------------------------

    void enqueuePatientRequest() { //function to add a new patient request in the queue
        QueueNode* newNode = new QueueNode(); //create a new node for patient request

        cout<< "\n==== Blood Request Form ====\n";
        cout<< "Enter Patient Name: ";
        cin.ignore();
        getline(cin, newNode->name);
        cout<< "Enter Required Blood Type: ";
        cin >> newNode->blood;
        cout<< "Enter Patient Age: ";
        cin >> newNode->age;

        newNode->next = nullptr; //new node will be last node so next is null

        if(rear == nullptr) { //if queue is empty
            front = rear = newNode;  //front and rear both point to new node
        }
        else {
            rear->next = newNode;  //link new node after rear
            rear = newNode;        //update rear
        }
        cout<< "\nPatient request added successfully.\n";
    }

    void dequeuePatientRequest() { //function to Dequeue patient request
        if (front == nullptr) { //if queue is empty
            cout << "\n No pending requests.\n";
            return;
        }

        QueueNode* temp = front; //store front node in temp
        front = front->next; //move front to next request

        if (front == nullptr) { //if queue becomes empty after removal
            rear = nullptr;
        }

        string name = temp->name; //store data in variables
        string blood = temp->blood;
        int age = temp->age;
        delete temp; //free memory

        cout << "\n--- Processing Request ---\n"; //display patient info
        cout << "Name       : " << name  << endl;
        cout << "Blood Type : " << blood << endl;
        cout << "Age        : " << age   << endl;
        cout << "--------------------------\n";

        Node* found = searchBloodGroup(); //search blood type in inventory
        

        if (found != nullptr) { //if blood found in inventory
            found->units--;  //inventory se 1 unit minus karo

            int units = found->units;
            int year = found->year;
            int month = found->month;
            int day = found->day;

            cout << "Status: Request fulfilled.\n";
            cout << "Remaining units of " << blood << " : " << found->units << endl;
            pushIssuedBlood(name, blood, units, year, month, day);
        }
        else {
            cout << "Status: No match in inventory, try using a compatible type (if any) or check other blood banks if urgent.\n";
        }
    }

    void cancelRequest() { //function to cancel the next patient request
        if(front == nullptr){ //if queue is empty
            cout<<"No request to cancel\n";
            return;
        }

        QueueNode* temp = front; //temporary pointer to store first node
        front = front->next; //move front to next request

        if (front == nullptr){ //if queue becomes empty after deletion
            rear = nullptr;
        }
        cout<<"Next patient request cancelled.\nName:"<<temp->name<<" , Blood type:"<<temp->blood<<endl;
        delete temp; //delete the cancelled request
    }

    void viewPendingRequests() { //function to view pending requests
        if (front == nullptr) { //if queue is empty
            cout << "\n No pending requests.\n";
            return;
        }

        QueueNode* temp = front; //start from front of queue
        int count = 1;

        while (temp != nullptr) { //traverse until end of queue
            string name = temp->name; //store current node data in variables
            string blood = temp->blood;
            int age = temp->age;

            //display current request
            cout << count << ". Name: " << name << " | Blood Type: " << blood << " | Age: " << age << endl;
            count++; //increment
            temp = temp->next; //move to next request
        }
    }

    //--------------------------END OF QUEUE------------------------------

    //-----------------------------BST------------------------------------

    void addDonor() {
        BSTNode* newNode = new BSTNode;

        cout << "\n------DONOR APPLICATION FORM------\n" << endl;
        cout << "Enter Donor Name: ";
        cin.ignore();
        getline(cin, newNode->name);

        cout << "Enter Blood Type: ";
        cin >> newNode->bloodGroup;

        cout << "Enter Cnic: ";
        cin >> newNode->cnic;

        cout << "Enter Age: ";
        cin >> newNode->age;

        cout << "Enter Weight: ";
        cin >> newNode->weight;

        cout << "Months since last donation (If it is your first time then enter 12): ";
        cin >> newNode->monthsSinceLastDonation;

        cout << "\nMajor Diseases List:\n";
        cout << "\t1. Hepatitis\n";
        cout << "\t2. HIV/AIDS\n";
        cout << "\t3. Cancer\n";
        cout << "\t4. Tuberculosis\n";
        cout << "\t5. Heart Disease\n";
        cout << "\t6. Other serious disease\n";
        cout << "Have you ever had any major disease from the above list? (1 = Yes / 0 = No): ";
        cin >> newNode->hasDisease;
        cout << endl;

        //now eligibility check/ health screening
        if (newNode->age < 18) {
            cout << "Rejected: Age must be 18+\n";
            return;
        }

        if (newNode->weight < 50) {
            cout << "Rejected: Weight must be >= 50\n";
            return;
        }

        if (newNode->monthsSinceLastDonation < 3) {
            cout << "Rejected: Wait 3 months\n";
            return;
        }

        if (newNode->hasDisease) {
            cout << "Rejected due to disease.\n";
            return;
        }

        newNode->id = nextId++;
        newNode->left = nullptr;
        newNode->right = nullptr;

        //insert into BST
        if (root == nullptr) {
            root = newNode;
        }

        else {

        BSTNode* temp = root;
        BSTNode* parent = nullptr;

        while (temp != nullptr) {

            parent = temp;

            if (newNode->cnic < temp->cnic)
                temp = temp->left;
            else
                temp = temp->right;
        }

        if (newNode->cnic < parent->cnic)
            parent->left = newNode;
        else
            parent->right = newNode;
    }
        cout << "Donor added successfully. ID = " << newNode->id << endl;
    }

    BSTNode* findMin(BSTNode* node) {

        while (node->left != nullptr) {
            node = node->left;
        }

        return node;
    }

    double findCnicById(BSTNode* node, int id) {

        if(node == nullptr)
            return -1;

        if(node->id == id)
            return node->cnic;

        double left = findCnicById(node->left, id);
        if(left != -1)
            return left;

        return findCnicById(node->right, id);
    }

    BSTNode* deleteDonorHelper(BSTNode* node, double cnic) {

        if (node == nullptr) {
            cout << "Donor not found.\n";
            return node;
        }

        if (cnic < node->cnic)
            node->left = deleteDonorHelper(node->left, cnic);

        else if (cnic > node->cnic)
            node->right = deleteDonorHelper(node->right, cnic);

        else {

            cout << "Donor deleted: " << node->name << endl;

            //no child
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }

            //one child
            else if (node->left == nullptr) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            }

            else if (node->right == nullptr) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }

            //two children
            BSTNode* temp = findMin(node->right);

            node->cnic = temp->cnic;
            node->name = temp->name;
            node->bloodGroup = temp->bloodGroup;
            node->age = temp->age;
            node->weight = temp->weight;
            node->hasDisease = temp->hasDisease;
            node->monthsSinceLastDonation = temp->monthsSinceLastDonation;

            node->right = deleteDonorHelper(node->right, temp->cnic);
        }

        return node;
    }

    void deleteDonor() {

        if (root == nullptr) {
            cout << "No donors available.\n";
            return;
        }

        int id;
        cout << "Enter Donor ID to delete: ";
        cin >> id;
        double cnic= findCnicById(root, id);
        // Check if we have any id like this or it is wrong id.
        if (cnic == -1) {
            cout << "Donor not found with the id: "<<id<<endl;
            return;
        }

        root = deleteDonorHelper(root, cnic);
    }

    void searchById() {
        int id;
        cout << "Enter ID to search: ";
        cin >> id;
        double cnic= findCnicById(root, id);
        searchById_helper(root,cnic);
    }

    void searchById_helper(BSTNode* root,double cnic) {


        if (root == nullptr) {
            cout << "Not found!" << endl;
            return;
        }

        if (cnic == root->cnic) {
            cout << "Found: ID " << root->id << ", Name: " << root->name << ", Blood Group: " << root->bloodGroup << endl;
            return;
        }
        if (cnic < root->cnic)
            searchById_helper(root->left,cnic);
        else
            searchById_helper(root->right,cnic);
          
    }

    void displayDonors() {
        if (root == nullptr) {
            cout << "Empty!" << endl;
            return;
        }

        cout << "\n------Donors List------\n" << endl;

        displayDonorsHelper(root);
    }

    void displayDonorsHelper(BSTNode* node) {

            if (node == nullptr)
                return;

            displayDonorsHelper(node->left);

            cout << "ID          : " << node->id << endl;
            cout << "Name        : " << node->name << endl;
            cout << "Blood Group : " << node->bloodGroup << endl;
            cout << "-----------------------------\n";

            displayDonorsHelper(node->right);

    }

    //--------------------------END OF BST--------------------------------

    //-----------------------------STACK----------------------------------

    void pushIssuedBlood(string name, string blood, int units, int year, int month, int day) {
        /*This function will be called from Queue when they remove an issued
        blood record from inventory so they pass that record into stack(this function)*/

        StackNode* newNode = new StackNode; //create new node
        newNode->patientName = name;
        newNode->bloodType = blood;
        newNode->units = units;
        newNode->year = year;
        newNode->month = month;
        newNode->day = day;

        newNode->next = top; //stack insertion
        top = newNode;
        cout << "Blood issue recorded successfully."<<endl;
    }

    void popLastIssuedBlood() {
        //This function is used when a mistake occur in entering record so to delete the most recent record in stack

        if(top == nullptr) { //if there was no record
            cout << "No issue records available.\n";
            return;
        }

        StackNode* temp = top;
        cout << "Removing last issued record of "<< temp->patientName << endl;
        top = top->next; //updating list's top
        delete temp; //delete most recent record
    }

    void viewRecentIssues() { //display function
        if(top == nullptr) { //if no records available
            cout << "No issue records available.\n";
            return;
        }

        StackNode* temp = top;
        cout << "\n------Recent Blood Issues------\n";

        while(temp != nullptr) { //loop to display all records
            cout << "Patient        : " << temp->patientName << endl;
            cout << "Blood Type     : " << temp->bloodType << endl;
            cout << "Unit Used      : 1 "<< endl ;
            cout << "Remaining Unit : "<<temp->units<< "\nof "<<temp->day << "/" << temp->month << "/" << temp->year << endl;
            cout << "Date           : " << temp->day << "/" << temp->month << "/" << temp->year << endl;
            cout << "-----------------------------\n";

            temp = temp->next;
        }
    }

    void countRecentIssues() { //to count records in history
        int count = 0;
        StackNode* temp = top;

        while(temp != nullptr) {
            count++;
            temp = temp->next;
        }
        cout << "Total issued blood records: " << count << endl;
    }

    //--------------------------END OF STACK------------------------------
};

int main() {
    BloodBank bb;
    int choice;
    do {
        cout << "\n-----BLOOD BANK MANAGEMENT SYSTEM-----\n";
        cout << "\n\t1. Add Donor\n\t2. Delete Donor\n\t3. Search Donor by ID\n\t4. Display all Donors" << endl;
        cout << "\n\t5. Add Patient Request\n\t6. Process Patient Request\n\t7. View Pending Requests\n\t8. Cancel Request" << endl;
        cout << "\n\t9. Add Blood Unit\n\t10. View Blood Inventory\n\t11. Search for a Blood Group\n\t12. Check Low Stock\n\t13. Remove Expired Blood" << endl;
        cout << "\n\t14. Remove last record\n\t15. View blood issues history\n\t16. Count total blood issues" << endl;
        cout << "\n\t17. Exit\n\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: bb.addDonor(); break;
            case 2: bb.deleteDonor(); break;
            case 3: bb.searchById(); break;
            case 4: bb.displayDonors(); break;
            case 5: bb.enqueuePatientRequest(); break;
            case 6: bb.dequeuePatientRequest(); break;
            case 7: bb.viewPendingRequests(); break;
            case 8: bb.cancelRequest(); break;
            case 9: bb.addBloodUnit(); break;
            case 10: bb.viewBloodInventory(); break;
            case 11: bb.searchBloodGroup(); break;
            case 12: bb.checkLowStock(); break;
            case 13: bb.deleteExpiredBlood(); break;
            case 14: bb.popLastIssuedBlood(); break;
            case 15: bb.viewRecentIssues(); break;
            case 16: bb.countRecentIssues(); break;
            case 17: cout << "Exiting..."; break;
            default: cout << "Invalid choice!" << endl;
        }
    } while (choice != 17);
    cout << "Goodbye!" << endl;
    return 0;
} 
