#include <iostream>
#include <vector>
#include <string>
#include <numeric> // for std::accumulate

class Student {
private:
    std::string name;
    int score;

public:
    Student(std::string name, int score) : name(name), score(score) {}

    std::string getName() const {
        return name;
    }

    int getScore() const {
        return score;
    }
};

double calculateAverage(const std::vector<Student>& students) {
    int totalScore = std::accumulate(students.begin(), students.end(), 0, [](int sum, const Student& student) {
        return sum + student.getScore();
        });
    return static_cast<double>(totalScore) / students.size();
}

void printAboveBelowAverage(const std::vector<Student>& students, double average) {
    std::cout << "=== ��� �̻��� �л��� ===\n";
    for (const auto& student : students) {
        if (student.getScore() >= average) {
            std::cout << student.getName() << " (" << student.getScore() << "��)\n";
        }
    }

    std::cout << "=== ��� ������ �л��� ===\n";
    for (const auto& student : students) {
        if (student.getScore() < average) {
            std::cout << student.getName() << " (" << student.getScore() << "��)\n";
        }
    }
}

int main() {
    std::vector<Student> students;
    int numStudents;

    std::cout << "�л� ���� �Է��ϼ���: ";
    std::cin >> numStudents;

    for (int i = 0; i < numStudents; ++i) {
        std::string name;
        int score;

        std::cout << i + 1 << "��° �л��� �̸��� �Է��ϼ���: ";
        std::cin >> name;
        std::cout << name << "�� ������ �Է��ϼ���: ";
        std::cin >> score;

        students.emplace_back(name, score);
    }

    double average = calculateAverage(students);
    std::cout << "\n��� ����: " << average << "��\n";

    printAboveBelowAverage(students, average);

    return 0;
}
