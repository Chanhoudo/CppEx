#include <iostream>
#include <string>
using namespace std;

// ���ڿ��� ȸ������ Ȯ���ϴ� �Լ�
bool isPalindrome(const string& str, int start, int end) {
    // �⺻ ���� ����: ���ڿ��� ���۰� ���� �����ų� �����ϸ� ȸ��
    if (start >= end)
        return true;

    // ���۰� �� ���ڰ� �ٸ��� ȸ���� �ƴ�
    if (str[start] != str[end])
        return false;

    // ��������� ���� ���� Ȯ��
    return isPalindrome(str, start + 1, end - 1);
}

// ��� �Լ��� ���ڿ��� ������ �Լ�
string reverseString(const string& str, int index = 0) {
    static string reversed = ""; // ������ ���ڿ� ����
    if (index == str.size()) // ��� ���ڸ� ó�������� ��� ��ȯ
        return reversed;

    reversed = str[index] + reversed; // ���� ���ڸ� �տ� �߰�
    return reverseString(str, index + 1); // ���� ���� ó��
}

int main() {
    string input;
    cout << "���ڿ��� �Է��ϼ���: ";
    cin >> input;

    // ȸ������ Ȯ��
    if (isPalindrome(input, 0, input.size() - 1)) {
        cout << "�Է��� ���ڿ��� ȸ���Դϴ�!" << endl;
    }
    else {
        cout << "�Է��� ���ڿ��� ȸ���� �ƴմϴ�." << endl;
    }

    // ���ڿ� ������
    string reversed = reverseString(input);
    cout << "������ ���ڿ�: " << reversed << endl;

    return 0;
}
