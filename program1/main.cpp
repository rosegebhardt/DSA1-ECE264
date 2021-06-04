using namespace std;

#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator> 
#include <sstream>

/* Program summary: Defines a base class for a singly-linked list called 
"SimpleList" and two derived classes for "Stack" and "Queue". The derived 
classes have their own definitions of push and pop to match the data 
structure. The program takes an input text file and processes it line 
by line. Each time a stack or queue is created it is stored in one of 3
C++ lists of SimpleLists based on data type. Each time push or pop are 
called the SimpleList is adjusted accordingly and the output is printed to
an output text file. */

/* Recieved help from Evan Bubniak and Alexa Jakob */

// --------------- CREATE STACK AND QUEUE CLASSES ---------------

// Defines the abstract base class from which stack and queue are derived

// Creates a singly-linked list that keeps track of the first node, the
// last node, and the name of the list


template <typename T>
class SimpleList
{
    private:
        
        // Defines Node of the linked list - keeps track of entry and next node
        struct Node
        {
            T data;
            Node *next;

            Node(T entry, Node *nextNode)
            {
                data = entry;
                next = nextNode;
            }
        };

        Node *start;
        Node *end;
        string listName;

    protected:

        void insertStart(T entry);
        void insertEnd(T entry);
        T removeStart();

    public:

        // Virtual functions to be defined in Stack and Queue subclasses
        virtual void push(T entry) = 0;
        virtual T pop() = 0;

        // Check if the list is empty to ensure pop function is possible
        bool isEmpty() const
        {
            if (start == nullptr)
                return true;
            else
                return false;
        }

        // Getter for listName data member
        string getlistName()
        {
            return listName;
        }

        // SimpleList constructor
        SimpleList(string name)
        {
            listName = name;
            start = nullptr;
            end = nullptr;
        }
};

// Adds node at start and adjusts pointers to first and last nodes
template <typename T>
void SimpleList<T>::insertStart(T entry)
{
    Node *newStart = new Node(entry, start);
    
    if (start == nullptr)
        end = newStart;
    
    start = newStart;
}

// Adds node at end and adjusts pointers to first and last nodes
template <typename T>
void SimpleList<T>::insertEnd(T entry)
{
    Node *newEnd = new Node(entry, nullptr);

    if (end == nullptr)
        start = newEnd;
    else
        (end->next) = newEnd;
    
    end = newEnd;
}

// Deletes node from start and adjusts pointers to first and last nodes
template <typename T>
T SimpleList<T>::removeStart()
{
    Node *firstNode = start;
    T firstValue = firstNode->data;

    if (start == end)
        end = nullptr;

    start = firstNode->next;

    delete firstNode;
    return firstValue;
}

// Stack class derived from SimpleList
template <typename T>
class Stack:public SimpleList<T>
{
    public:

    // Add node at start - makes list FIFO
    void push(T entry)
    {
        Stack<T>::insertStart(entry);
    }

    // Remove node from start and return value
    T pop()
    {
        return Stack<T>::removeStart();
    }

    // Stack constructor 
    Stack(string stackName):SimpleList<T>(stackName){}
};

// Queue class derived from SimpleList
template <typename T>
class Queue:public SimpleList<T>
{
    public:

    // Add node at end - makes list LIFO
    void push(T entry)
    {
        Queue<T>::insertEnd(entry);
    }

    // Remove node from start and return value
    T pop()
    {
        return Queue<T>::removeStart();
    }

    // Queue constructor
    Queue(string queueName):SimpleList<T>(queueName){}
};

// --------------- READ INPUT FILE AND PROCESS COMMANDS ---------------

// All stacks and queues organized by data type
list<SimpleList<int> *> listSLi;
list<SimpleList<double> *> listSLd;
list<SimpleList<string> *> listSLs;

// Prompts user for input and output files
void openIOFiles(ifstream &input, ofstream &output)
{
    string inputFile;
    string outputFile;

    cout << "Enter input file name: ";
    cin >> inputFile;
    input.open(inputFile);

    cout << "Enter output file name: ";
    cin >> outputFile;
    output.open(outputFile);
}

// Closes files before terminaing program
void closeIOFiles(ifstream &input, ofstream &output)
{
    input.close();
    output.close();
}

// Iterate through C++ list of SimpleLists to get pointer to desired object
template <typename T>
SimpleList<T>* getlistSL(list<SimpleList<T> *> &listSL, string listName)
{
    for (typename list<SimpleList<T> *>::const_iterator it = listSL.begin(); it != listSL.end(); it++)
    {
        if (((*it)->getlistName() == listName))
        {
            return *it;
        }
    }
    return nullptr;
}

// Uses command to run functions and output status to the output file
template <typename T>
void executeCommand(list<SimpleList<T> *> &listSL, string command[], T entry, ofstream &output)
{
    SimpleList<T> *sl = getlistSL(listSL, command[1]);

    if (command[0] == "create")
    {
        if (sl != nullptr)
        {
            output << "ERROR: This name already exists!\n";
        }
        else
        {
            if (command[2] == "stack")
            {
                Stack<T> *newStack;
                newStack = new Stack<T>(command[1]);
                listSL.push_front(newStack);
            }
            else if (command[2] == "queue")
            {
                Queue<T> *newQueue;
                newQueue = new Queue<T>(command[1]);
                listSL.push_front(newQueue);
            }
        }
    }
    else if (command[0] == "push")
    {
        if (sl == nullptr)
        {
            output << "ERROR: This name does not exist!\n";
        }
        else
        {
            sl->push(entry);
        }
    }
    else if (command[0] == "pop")
    {
        if (sl == nullptr)
        {
            output << "ERROR: This name does not exist!\n";
        }
        else if (sl->isEmpty())
        {
            output << "ERROR: This list is empty!\n";
        }
        else
        {
            output << "Value popped: " << sl->pop() << "\n";
        }
    }
}

// Turn input file line into command and execute command based on data type
void processInput(string inputLine, ofstream &output)
{
    output << "PROCESSING COMMAND: " << inputLine << "\n";

    // Read input file line and turns line into a command
    stringstream lineStream(inputLine);
    string* command = new string[3];
    string word;

    int index = 0;
    while (lineStream >> word)
    {
        command[index++] = word;
    }

    // Integer data type switch case
    if ((command[1]).substr(0,1) == "i")
    {
        int entry = 0;
        if (!command[2].empty())
        {
            entry = atoi(command[2].c_str());
        }
        executeCommand(listSLi,command,entry,output);
    }

    // Double data type switch case
    else if ((command[1]).substr(0,1) == "d")
    {
        double entry = 0.0;
        if (!command[2].empty())
        {
            entry = atof(command[2].c_str());
        }        
        executeCommand(listSLd,command,entry,output);
    }

    // String data type switch case
    else if ((command[1]).substr(0,1) == "s")
    {
        string entry = command[2];
        executeCommand(listSLs,command,entry,output);
    }
}

int main()
{
    ifstream inputFile;
    ofstream outputFile;
    string inputLine;

    openIOFiles(inputFile,outputFile);

    while(getline(inputFile,inputLine))
    {
        processInput(inputLine,outputFile);
    }

    closeIOFiles(inputFile,outputFile);

    return 0;
}