#include <iostream>
#include <tuple>
#include <string>

using namespace std;

// Define the message type as a tuple
using Message = tuple<int, int, string>;  // (timestamp, sender_id, message_content)

class Queue {
private:
    struct Node {
        Message message;
        Node* next;
        Node(const Message& msg) : message(msg), next(nullptr) {}
    };

    Node* frontNode;
    Node* rearNode;

public:
    Queue() : frontNode(nullptr), rearNode(nullptr) {}

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    bool isEmpty() const {
        return frontNode == nullptr;
    }

    void enqueue(const Message& msg) {
        Node* newNode = new Node(msg);
        if (isEmpty()) {
            frontNode = newNode;
            rearNode = newNode;
        }
        else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
    }

    void dequeue() {
        if (isEmpty()) {
            return;
        }
        Node* tempNode = frontNode;
        frontNode = frontNode->next;
        if (frontNode == nullptr) {
            rearNode = nullptr;
        }
        delete tempNode;
    }

    bool front(Message& msg) const {
        if (isEmpty()) {
            return false;
        }
        msg = frontNode->message;
        return true;
    }
};

class MessagingSystem {
private:
    Queue messageQueue;

public:
    // Function to send a message
    void sendMessage(int sender_id, const string& message_content) {
        int timestamp = getCurrentTimestamp();
        Message message(timestamp, sender_id, message_content);
        messageQueue.enqueue(message);
    }

    // Function to receive a message
    bool receiveMessage(Message& msg) {
        return messageQueue.front(msg);
    }

    // Function to display all messages in the queue
    void displayMessages() {
        Queue tempQueue = messageQueue;
        while (!tempQueue.isEmpty()) {
            Message message;
            if (tempQueue.front(message)) {
                tempQueue.dequeue();
                cout << "Timestamp: " << get<0>(message) << ", Sender ID: " << get<1>(message)
                    << ", Message: " << get<2>(message) << endl;
            }
        }
    }

private:
    // Function to get the current timestamp
    int getCurrentTimestamp() {
        static int timestamp = 0;
        return ++timestamp;
    }
};

int main() {
    MessagingSystem messagingSystem;
    int choice = 0;
    bool received = false;

    while (choice != 4) {
        cout << "\nMenu:" << endl;
        cout << "1. Send Message" << endl;
        cout << "2. Receive Message" << endl;
        cout << "3. Display Received Messages" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int sender;
            string message;
            cout << "Enter sender ID: ";
            cin >> sender;
            cout << "Enter message: ";
            cin.ignore(); // Ignore the newline character in the input buffer
            getline(cin, message); // Reading the entire line including spaces
            messagingSystem.sendMessage(sender, message); // Send the message
            cout << "Message sent successfully." << endl;
        }
        else if (choice == 2) {
            Message receivedMessage;
            if (messagingSystem.receiveMessage(receivedMessage)) {
                cout << "Received Message: ";
                cout << "Timestamp: " << get<0>(receivedMessage) << ", Sender ID: " << get<1>(receivedMessage)
                    << ", Message: " << get<2>(receivedMessage) << endl;
                received = true;
            }
            else {
                cout << "No messages to receive." << endl;
                received = false;
            }
        }
        else if (choice == 3) {
            messagingSystem.displayMessages();
        }
        else if (choice == 4) {
            cout << "Exiting program." << endl;
        }
        else {
            cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
            break;
        }
    }

    return 0;
}