#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
using namespace std;

struct Question {
    string text;
    string options[4];
    char correctAnswer;
};

void setColor(int colorCode) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

void createQuizFile(const string& filename) {
    int numQuestions;
    cout << "How many questions do you want to add? ";
    cin >> numQuestions;
    cin.ignore();

    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file for writing.\n";
        return;
    }

    for (int i = 0; i < numQuestions; i++) {
        string question, option;
        char correct;

        cout << "\nQuestion " << i + 1 << ":\n";
        cout << "Enter question text: ";
        getline(cin, question);
        file << question << "\n";

        for (char ch = 'a'; ch <= 'd'; ch++) {
            cout << "Option " << ch << ": ";
            getline(cin, option);
            file << ch << ") " << option << "\n";
        }

        do {
            cout << "Enter correct answer (a/b/c/d): ";
            cin >> correct;
            correct = tolower(correct);
        } while (correct < 'a' || correct > 'd');
        file << correct << "\n\n";
        cin.ignore();
    }

    file.close();
    cout << "\nQuestions saved successfully to file.\n";
}

bool loadQuestionsFromFile(const string& filename, vector<Question>& questions) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << "\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        Question q;
        q.text = line;
        for (int i = 0; i < 4; i++) {
            getline(file, q.options[i]);
        }
        getline(file, line);
        q.correctAnswer = (line.length() == 1) ? tolower(line[0]) : 'x';
        questions.push_back(q);
        getline(file, line); // skip empty line
    }

    file.close();
    return true;
}

void runQuiz(const vector<Question>& quiz) {
    cout << "\nStarting the quiz!\n";
    cout << "Answer using letters (a/b/c/d).\n\n";

    int score = 0;
    char answer;

    for (size_t i = 0; i < quiz.size(); i++) {
        cout << "Question " << i + 1 << ": " << quiz[i].text << "\n";
        for (int j = 0; j < 4; j++) {
            cout << quiz[i].options[j] << "\n";
        }

        do {
            cout << "Your answer: ";
            cin >> answer;
            answer = tolower(answer);
        } while (answer < 'a' || answer > 'd');

        if (answer == quiz[i].correctAnswer) {
            setColor(10); // Green
            cout << "Correct!\n\n";
            score++;
        } else {
            setColor(12); // Red
            cout << "Incorrect. Correct answer: " << quiz[i].correctAnswer << "\n\n";
        }

        setColor(7); // Reset to default
    }

    cout << "Quiz finished. Your score: " << score << " out of " << quiz.size() << ".\n";
    cout << "Thanks for playing!\n";
}

int main() {
    string filename = "questions.txt";
    int choice;

    cout << "C++ Quiz Application\n";
    cout << "1. Create a new quiz and save to file\n";
    cout << "2. Run quiz from existing file\n";
    cout << "Choose (1 or 2): ";
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        createQuizFile(filename);
    } else if (choice == 2) {
        vector<Question> quiz;
        if (loadQuestionsFromFile(filename, quiz)) {
            runQuiz(quiz);
        }
    } else {
        cout << "Invalid choice.\n";
    }

    return 0;
}
